#include	<cstdlib>
#include	<cmath>
#include	<fstream>
#include	<sstream>
#include	<iostream>
#include	<iomanip>
#include	<string>
#include	<vector>
#include	<algorithm>
#include	<exception>
#include	<sys/time.h>
#include	"modules/htmTree.h"
#include	"modules/kdTree.h"
#include        "misc.h"
#include        "feat.h"
#include        "structs.h"
#include        "collision.h"
#include        "global.h"

int main(int argc, char **argv) {
  //// Initializations ---------------------------------------------
  srand48(1234); // Make sure we have reproducibility
  check_args(argc);
  Time t, time; // t for global, time for local
  init_time(t);
  Feat F;
  
  // Read parameters file //
  F.readInputFile(argv[1]);
  printFile(argv[1]);
  // Read galaxies
  
  
  MTL M=read_MTLfile(F);
  F.Ngal = M.size();
  assign_priority_class(M);
  //find available SS and SF galaxies on each petal
  
  std::vector <int> count_class(M.priority_list.size(),0);
  
  printf("Number in each priority class.  The last two are SF and SS.\n");
  for(int i;i<M.size();++i){
    count_class[M[i].priority_class]+=1;
  }
  for(int i;i<M.priority_list.size();++i){
    printf("  class  %d  number  %d\n",i,count_class[i]);
  }
  
  printf(" number of MTL galaxies  %d\n",(int)(M.size()));
  
  PP pp;
  pp.read_fiber_positions(F); 
  F.Nfiber = pp.fp.size()/2; 
  F.Npetal = max(pp.spectrom)+1;
  F.Nfbp = (int) (F.Nfiber/F.Npetal);// fibers per petal = 500
  pp.get_neighbors(F); pp.compute_fibsofsp(F);
  Plates P = read_plate_centers(F);
  F.Nplate=P.size();
  printf("# Read %s plate centers from %s and %d fibers from %s\n",f(F.Nplate).c_str(),F.tileFile.c_str(),F.Nfiber,F.fibFile.c_str());
  
  // Computes geometries of cb and fh: pieces of positioner - used to determine possible collisions
  F.cb = create_cb(); // cb=central body
  F.fh = create_fh(); // fh=fiber holder
  
  //// Collect available galaxies <-> tilefibers --------------------
  // HTM Tree of galaxies
  const double MinTreeSize = 0.01;
  init_time_at(time,"# Start building HTM tree",t);
  htmTree<struct target> T(M,MinTreeSize);
  print_time(time,"# ... took :");//T.stats();
  
  // For plates/fibers, collect available galaxies; done in parallel  P[plate j].av_gal[k]=[g1,g2,..]
  collect_galaxies_for_all(M,T,P,pp,F);
  
  // For each galaxy, computes available tilefibers  G[i].av_tfs = [(j1,k1),(j2,k2),..]
  collect_available_tilefibers(M,P,F);
  
  
  //// Assignment |||||||||||||||||||||||||||||||||||||||||||||||||||
  Assignment A(M,F);
  print_time(t,"# Start assignment at : ");
  
  printf(" Nplate %d  Ngal %d   Nfiber %d \n", F.Nplate, F.Ngal, F.Nfiber);
  
  simple_assign(M,P,pp,F,A);
  
  //diagnostic for skeleton
  std::vector<int> countj(F.Nplate,0);
  int count_total=0;
  for (int j=0;j<F.Nplate;++j){
    int nj=0;
    for(int k=0;k<F.Nfiber;++k){
      if (A.TF[j][k]!=-1){
	nj++;
	count_total++;
      }
    }
    if(nj>0) printf(" j = %d tileid %d number assigned= %d\n",j, P[j].tileid, nj);
  }
  printf(" total assigned = %d\n",count_total);
  
  
  // Results -------------------------------------------------------*/
  
  if (F.PrintFits){
    for (int j=0; j<F.Nplate; j++){
      fa_write(j,F.outDir,M,P,pp,F,A); // Write output
    }
  }
  
  print_time(t,"# Finished !... in");
  
  return(0);    
}

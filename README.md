# Fiber assignment code for DESI

## Building

By default, options for a generic Linux machine are used.  All 
dependencies (e.g. libcfitsio) are assumed to be installed in
the default compiler and linker search paths.  To build the 
software, go to the top source directory:

    $> cd fiberassign
    $> make
    $> make install

This will put the "assign" executable in the top source directory.
To clean up the build, do:

    $> make clean

You can customize the build options by using one of the 
configurations in the platforms directory.  This is done by 
exporting an environment variable and setting it to the name of 
the file in the platforms directory you want to use.  For example, 
when building at NERSC with HPCPorts on the Cray systems, you would do:

    $> hpcports shared_gnu
    $> module load cfitsio-hpcp
    $> export PLATFORM=hpcports
    $> make install

Similarly, on OSX with the Clang compiler, you would do:

    $> export PLATFORM=osx
    $> make install

To install the "assign" executable to a different location,
you can set the INSTALL environment variable to the path you
wish to use.

## Running

From the top directory (for example), you can run with:

    $> ./assign features.txt

On edison.nersc.gov, you can use the PBS script in the "scripts"
directory:

    $> qsub script/run

This uses the file features.txt to define the input files, the priorities of the various galaxy types, etc.

An analogous system is in place for a prototype bright time survey:

   $> qsub script/run_bright_time

which uses bright_time_features.txt.  This uses as one of the input
files, mocks provided by Jeremy Tinker.  At present, there is no
provision here for standard stars or sky fibers.



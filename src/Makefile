# Makefile for building fiber assignment tools
# variables for compiler flags, etc are propagated
# from the top-level Makefile.  DO NOT use this
# Makefile directly (build from the top level).

HEADERS = \
collision.h \
feat.h \
global.h \
misc.h \
structs.h

OBJS = \
collision.o \
feat.o \
global.o \
pipeline.o \
misc.o \
structs.o

COMPILE = $(CXXFLAGS) $(OMPFLAGS) -I. $(CFITSIO_CPPFLAGS)
LINK = $(OMPFLAGS) $(CFITSIO)


all: assign

install : all
	cp assign $(INSTALL)

assign : $(OBJS)
	$(CXX) -o $@ $^ $(LINK)


%.o : %.cpp $(HEADERS)
	$(CXX) $(COMPILE) -o $@ -c $<


clean :
	rm -f assign  *.o *~

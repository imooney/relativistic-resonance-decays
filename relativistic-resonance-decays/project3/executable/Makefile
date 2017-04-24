###################################

CXX	= g++
LD	= g++

###################################

os = $(shell uname -s)

ifeq ($(os),Linux)
CXXFLAGS	= -I$(shell root-config --incdir) -std=c++11 -fPIC
else
CXXFLAGS	= -O -std=c++11 -fPIC -pipe -Wall -dynamiclib -I$(shell root-config --incdir)
endif

ifeq ($(os),Linux)
LDFLAGS		= -g
LDFLAGSS	= -g --shared
else
LDFLAGS		= -O -Xlinker -bind_at_load -flat_namespace
LDFLAGSS  	= -dynamiclib -flat_namespace -undefined suppress
LDFLAGSSS 	= -bundle
endif

ROOTLIBS	= $(shell root-config --libs)
LIBS		= $(ROOTLIBS)

INSTALL_DIR	= /usr/local
GSL		= $(INSTALL_DIR)

CXXFLAGSGSL 	= $(CXXFLAGS)
CXXFLAGSGSL 	+= -I$(GSL)/include
CXXFLAGSROOT 	= -I$(shell root-config --incdir) -std=c++11

LIBSROOT       	= $(ROOTLIBS)
LIBSGSL	       	= $(LIBS)
LIBSGSL       	+= -L$(GSL)/lib -lgsl -lgslcblas


HDRS		= Event.h LinkDef.h
SRCS		= Event.C dictEvent.C

OBJS		= $(SRCS:.C=.o)

SLIB		= libEvent.so

$(SLIB):	$(OBJS)
		@echo "Linking $(PROGRAM) for "$(os)
		@echo $(LD) $(LDFLAGSS) $(OBJS) $(LIBS) -o $(SLIB)
		@$(LD) $(LDFLAGSS) $(OBJS) $(LIBS) -o $(SLIB)
		@echo "done"

#######################################

Event.o:	Event.h

dictEvent.C:	$(HDRS)
		@echo "Generating dictionary ..."
		@$(ROOTSYS)/bin/rootcint -f dictEvent.C -c $(HDRS)

.C.o:
		$(CXX) $(CXXFLAGS) -c $<

treeRead:
		g++ $(CXXFLAGSGSL) $(CXXFLAGSROOT) -c src/treeRead.C
		g++ $(LDFLAGS) treeRead.o $(LIBSGSL) $(LIBSROOT) -L./ -lEvent -o bin/treeRead
		rm treeRead.o

writerrd:
		g++ $(CXXFLAGSGSL) $(CXXFLAGSROOT) -c src/writerrd.C
		g++ $(LDFLAGS) writerrd.o $(LIBSGSL) $(LIBSROOT) -L./ -lEvent -o bin/writerrd
		rm writerrd.o

clean :
		rm -f *.o
		rm -f *.o dict*.* $(SLIB)
		rm bin/*
		rm Resonance.root	
		@echo "All cleaned-up :-)"

#all : bin/treeRead

# DO NOT DELETE

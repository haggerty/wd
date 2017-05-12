PROG = wd409

MARTIN=/direct/phenix+u/purschke/install

all: $(PROG)

OBJ = wd409.o wd409_main.o
INC = -I. -I/usr/local/include -I$(OFFLINE_MAIN)/include -I$(MARTIN)/include/Event
LIB = -pthread -L$(MARTIN)/lib -L$(OFFLINE_MAIN)/lib -lNoRootEvent 

CXX = g++
CFLAGS = -Wall -g `root-config --cflags`
LDFLAGS = -g `root-config --glibs` -lMathMore -Wl,-rpath,$(MARTIN)/lib:$(OFFLINE_MAIN)/lib:$(ROOTSYS)/lib 

$(PROG): $(OBJ)
	$(CXX) -o $(PROG) $(LDFLAGS) $(OBJ) $(LIB) 

clean:
	rm -f $(PROG) *.o *.so *.d *~ core*

.cc.o:
	$(CXX) -c $(CFLAGS) $(INC) $<

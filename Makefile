## sspXCSCFC
# AB Siddique
#

OUTPUTFILE = sspXSCCFC
LINKMATH = -lm

CC = g++
CC_OPTS = -O3 -ffloat-store -Wall -m32

all: $(OUTPUTFILE)

$(OUTPUTFILE):	 xcsharness.o xcssystem.o xcsclassifierharness.o xcscodefragmentharness.o xcsconfig.o xcsenvironment.o xcstime.o xcsdefs.o
	$(CC) $(CC_OPTS) -o $(OUTPUTFILE).out $(LINKMATH) xcsharness.o xcssystem.o xcsclassifierharness.o xcscodefragmentharness.o xcsconfig.o xcsenvironment.o xcstime.o xcsdefs.o

xcsharness.o:	xcssystem.hpp
	$(CC) -c $(CC_OPTS) xcsharness.cpp

xcssystem.o:	xcssystem.cpp xcssystem.hpp 
	$(CC) -c $(CC_OPTS) xcssystem.cpp

xcsclassifierharness.o: xcsclassifierharness.cpp xcsclassifierharness.hpp
	$(CC) -c $(CC_OPTS) xcsclassifierharness.cpp

xcscodefragmentharness.o: xcscodefragmentharness.cpp xcscodefragmentharness.hpp
	$(CC) -c $(CC_OPTS) xcscodefragmentharness.cpp

xcsconfig.o: xcsconfig.cpp xcsconfig.hpp
	$(CC) -c $(CC_OPTS) xcsconfig.cpp

xcsenvironment.o: xcsenvironment.cpp xcsenvironment.hpp xcsconfig.hpp
	$(CC) -c $(CC_OPTS) xcsenvironment.cpp

xcstime.o: xcstime.cpp xcstime.hpp
	$(CC) -c $(CC_OPTS) xcstime.cpp

xcsdefs.o: xcsdefs.cpp xcsdefs.hpp
	$(CC) -c $(CC_OPTS) xcsdefs.cpp

clean:
	-rm -f *.o *.out *~

NAME    = testDifAlgebra

#you must set BFDISTRIB and BFCURRENT for your enviroment to
#use this simplfied makefile

.SUFFIXES:
.SUFFIXES: .o .cc .h
.SUFFIXES: .o .f .inc
CFILES  =
HFILES  =  
CFLAGS  = -g -DDISPLAY 
CFILES = $(wildcard *.cc)

LDFLAGS =
LOOK =

# define LIBS for the xgks libraries.
LIBS    = -L $(BFDISTRIB)releases/$(BFCURRENT)/lib/AIX3 -lCLHEP -lf2c \
	 -L/usr/local/lib -lxgks -lX11 -lm            
#  define LIBS for the graPHIGS/GKS-CO libraries (IBM supplied).

# Rules...

FC = f77
CC = g++ -Wall
SRCFILES = $(CFILES)
OBJFILES = $(CFILES:.cc=.o)


$(NAME): $(OBJFILES)
	 $(CC) -o $@ $(CFLAGS) $(OBJFILES) $(LIBS) $(LDFLAGS) 
.cc.o:
	$(CC) \
$(CFLAGS) -c -I.. \
-I $(BFDISTRIB)releases/$(BFCURRENT)/CLHEP/ \
$*.cc \
-I/usr/local/lib.shared/include

# DO NOT DELETE THIS LINE -- make depend depends on it.

DifHelixZ.o: DifHelixZ.hh /usr/include/assert.h /usr/include/standards.h
DifHelixZ.o: /usr/include/stdlib.h DifNumber.hh /usr/include/math.h
DifHelixZ.o: DifNumber.icc DifVector.hh DifVector.icc DifPoint.hh
DifHelixZ.o: DifPoint.icc DifRay.hh DifPlane.hh DifPlane.icc Code.hh
DifHelixZ.o: DifRay.icc DifHelixZ.icc
DifNumber.o: DifNumber.hh /usr/include/math.h /usr/include/assert.h
DifNumber.o: /usr/include/standards.h DifNumber.icc
DifPoint.o: DifPoint.hh /usr/include/assert.h /usr/include/standards.h
DifPoint.o: DifVector.hh DifNumber.hh /usr/include/math.h DifNumber.icc
DifPoint.o: DifVector.icc DifPoint.icc
DifRay.o: DifRay.hh /usr/include/assert.h /usr/include/standards.h
DifRay.o: /usr/include/stdlib.h DifNumber.hh /usr/include/math.h
DifRay.o: DifNumber.icc DifVector.hh DifVector.icc DifPoint.hh DifPoint.icc
DifRay.o: DifPlane.hh DifPlane.icc Code.hh DifRay.icc
DifRotation.o: DifRotation.hh DifVector.hh DifNumber.hh /usr/include/math.h
DifRotation.o: /usr/include/assert.h /usr/include/standards.h DifNumber.icc
DifRotation.o: DifVector.icc
DifVector.o: DifVector.hh DifNumber.hh /usr/include/math.h
DifVector.o: /usr/include/assert.h /usr/include/standards.h DifNumber.icc
DifVector.o: DifVector.icc DifRotation.hh
testDifAlgebra.o: /usr/include/math.h DifNumber.hh /usr/include/assert.h
testDifAlgebra.o: /usr/include/standards.h DifNumber.icc DifVector.hh
testDifAlgebra.o: DifVector.icc DifPoint.hh DifPoint.icc DifRotation.hh
testDifAlgebra.o: DifRay.hh /usr/include/stdlib.h DifPlane.hh DifPlane.icc
testDifAlgebra.o: Code.hh DifRay.icc DifHelixZ.hh DifHelixZ.icc

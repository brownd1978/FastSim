*
* $Id: locf.F 643 2010-04-28 10:03:25Z stroili $
*
* $Log: locf.F,v $
* Revision 1.1.1.1  1996/02/15 17:50:37  mclareni
* Kernlib
*
*
#include "CrnLib/pilot.h"
#if defined(CERNLIB_QMMPW)
      DIMENSION AM(*)

      LOCF= %LOC (AM(1))
      LOCF= ISHFT(LOCF, -2)
      END

#else
      FUNCTION LOCF (IVAR)
C
C CERN PROGLIB# N100    LOCF            .VERSION KERNFOR  4.34  930114
C
C-    This is a default which works on several machines
C
      DIMENSION    IVAR(9)
*    Number of ADdress Units Per Word for Fortran
C                         and its logarithm base 2
      PARAMETER    (NADUPW=4, LADUPW=2)

      J = LOC(IVAR)
      LOCF = ISHFT (J, -LADUPW)
      END
#endif

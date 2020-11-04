*
* $Id: vfill.F 643 2010-04-28 10:03:25Z stroili $
*
* $Log: vfill.F,v $
* Revision 1.1.1.1  1996/02/15 17:50:17  mclareni
* Kernlib
*
*
#include "CrnLib/pilot.h"
      SUBROUTINE VFILL (A,N,STUFF)
C
C CERN PROGLIB# F121    VFILL           .VERSION KERNFOR  4.40  940929
C ORIG. 01/07/71
C
#if defined(CERNLIB_QINTCOPY)
      INTEGER   A(*), STUFF
#endif
#if !defined(CERNLIB_QINTCOPY)
      DIMENSION A(*)
#endif
C
      IF (N.LE.0)  RETURN
      DO 9 I= 1,N
    9 A(I)= STUFF
C
      RETURN
      END

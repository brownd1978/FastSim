*
* $Id: ucopy.F 643 2010-04-28 10:03:25Z stroili $
*
* $Log: ucopy.F,v $
* Revision 1.1.1.1  1996/02/15 17:50:19  mclareni
* Kernlib
*
*
#include "CrnLib/pilot.h"
      SUBROUTINE UCOPY (A,B,N)
C
C CERN PROGLIB# V301    UCOPY           .VERSION KERNFOR  4.40  940929
C ORIG. 01/01/65 JZ
C
#if defined(CERNLIB_QINTCOPY)
      INTEGER   A(*),B(*)
#endif
#if !defined(CERNLIB_QINTCOPY)
      DIMENSION A(*),B(*)
#endif
C
C--                NO OVERLAP  OR  BEGINNING OF A ON END OF B
      IF (N.EQ.0) RETURN
         DO 21 I=1,N
   21 B(I)=A(I)
      RETURN
      END

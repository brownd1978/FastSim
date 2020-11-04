*
* $Id: lenocc.F 609 2010-03-31 10:16:45Z stroili $
*
* $Log: lenocc.F,v $
* Revision 1.1.1.1  1996/02/15 17:49:49  mclareni
* Kernlib
*
*
#include "CrnLib/pilot.h"
      FUNCTION LENOCC (CHV)
C
C CERN PROGLIB# M507    LENOCC          .VERSION KERNFOR  4.21  890323
C ORIG. March 85, A.Petrilli, re-write 21/02/89, JZ
C
C-    Find last non-blank character in CHV

      CHARACTER    CHV*(*)

      N = LEN(CHV)

      DO 17  JJ= N,1,-1
      IF (CHV(JJ:JJ).NE.' ') GO TO 99
   17 CONTINUE
      JJ = 0

   99 LENOCC = JJ
      RETURN
      END

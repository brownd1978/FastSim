*
* $Id: dzero.F 609 2010-03-31 10:16:45Z stroili $
*
* $Log: dzero.F,v $
* Revision 1.1.1.1  1996/02/15 17:49:07  mclareni
* Kernlib
*
*
#include "CrnLib/pilot.h"
#include "CrnLib/CERNLIB_machine.h"
#if defined(CERNLIB_NUMLOPRE)
      SUBROUTINE DZERO(A,B,X0,R,EPS,MXF,F)
      IMPLICIT DOUBLE PRECISION (A-H,O-Z)
#include "CrnLib/c205body.inc"
  100 FORMAT(1X,'***** CERN C205 DZERO ... F(A) AND F(B)',
     1          ' HAVE THE SAME SIGN')
  101 FORMAT(1X,'***** CERN C205 DZERO ... TOO MANY FUNCTION CALLS')
      END
#endif

*
* $Id: ludat1.inc 662 2010-05-11 10:37:41Z stroili $
*
* $Log: ludat1.inc,v $
* Revision 1.1.1.1  1996/03/08 17:32:19  mclareni
* jetset74
*
*
#ifndef CERNLIB_JETSET74_LUDAT1_INC
#define CERNLIB_JETSET74_LUDAT1_INC
*
*
* ludat1.inc
*
      INTEGER MSTU,MSTJ
      REAL PARU,PARJ
      COMMON/LUDAT1/MSTU(200),PARU(200),MSTJ(200),PARJ(200)
      SAVE /LUDAT1/

#endif

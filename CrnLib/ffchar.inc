*
* $Id: ffchar.inc 643 2010-04-28 10:03:25Z stroili $
*
* $Log: ffchar.inc,v $
* Revision 1.1.1.1  1996/03/08 11:50:41  mclareni
* Ffread
*
*
#ifndef CERNLIB_CrnLib_FFCHAR_INC
#define CERNLIB_CrnLib_FFCHAR_INC
*
*
* ffchar.inc
*
      COMMON /FFCHAR/ BUFFER
      CHARACTER*(LENMAX) BUFFER
#if defined(CERNLIB_SAVE)
#include "CrnLib/ffcharsv.inc"
#endif
C
C     FFCHAR is a character string which contains the input data card
C

#endif

*
* $Id: wordsize.inc 643 2010-04-28 10:03:25Z stroili $
*
* $Log: wordsize.inc,v $
* Revision 1.1.1.1  1996/02/15 17:49:18  mclareni
* Kernlib
*
*
*
* wordsize.inc
*
*    # of bits/word
*    # of characters/word
#if defined(CERNLIB_QMIRTD)
      PARAMETER   (NBITPW=64, NCHAPW=8, NBITPC=8)
#else
      PARAMETER   (NBITPW=32)
      PARAMETER   (NCHAPW=4)
#endif

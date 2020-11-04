
      subroutine STDRISND(lok)

C... read end run information for Isajet
C    contains cross sections and luminosity
C
C       NBUF = buffer size
C
C       lok = 0 if no problems were encountered

#include "stdhep/final.inc"
#include "stdhep/stdlun.inc"

      integer lok

      READ(LNHRD,ERR=900) NKINF,SIGF,ALUM,ACCEPT

      return
 900  lok = 1
      write (LNHOUT,1005)
      return
 1005 format(/5X,'STDRISND: read error')
      end

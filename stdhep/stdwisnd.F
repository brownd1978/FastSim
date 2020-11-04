
      subroutine STDWISND(ilbl,lok)

C... write end run information for Isajet
C    contains cross sections and luminosity
C
C     lok = 0 if no problems were encountered

#include "stdhep/final.inc"
#include "stdhep/stdlun.inc"

      integer ilbl,lok,NBUF

      NBUF = 4
      write(LNHWRT,ERR=900) ilbl,NBUF
      write(LNHWRT,ERR=900) NKINF,SIGF,ALUM,ACCEPT

      return
  900 lok = 1
      write (LNHOUT,1005)
      return
 1005 format(/5X,'STDWISND: write error')
      end

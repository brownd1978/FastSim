
      subroutine STDWRT(ilbl,lok)

C...Purpose: to write an event from the standard common block.
C
C     lok = 0 if no problems were encountered

#include "stdhep/stdhep.inc"
#include "stdhep/stdcnt.inc"
#include "stdhep/stdlun.inc"

      integer I,K,ilbl,lok,NBUF

      lok=0
      if(LNHWRT.LE.0)then
      elseif(NHEP.LE.0)then
        write(LNHOUT,101) NEVHEP
      else
        NBUF = NHEP*15 + 1
        write(LNHWRT,ERR=900) ilbl,NBUF
        write(LNHWRT,ERR=910) NEVHEP,(ISTHEP(I),I=1,NHEP),
     1        (IDHEP(I),I=1,NHEP),((JMOHEP(K,I),K=1,2),I=1,NHEP),
     2        ((JDAHEP(K,I),K=1,2),I=1,NHEP),
     3        ((PHEP(K,I),K=1,5),I=1,NHEP),
     4        ((VHEP(K,I),K=1,4),I=1,NHEP)
        nstdwrt = nstdwrt + 1
      endif
      return
  900 write (LNHOUT,901) NEVHEP
      lok=1
      return
  910 write (LNHOUT,911) NEVHEP
      lok=1
      return
  101 format(/5X,'STDWRT: no particles to write in event',I8)
  901 format(/5X,'STDWRT: write error on event label',I8)
  911 format(/5X,'STDWRT: write error on event contents',I8)
      end

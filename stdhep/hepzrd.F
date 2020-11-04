
      subroutine HEPZRD(lok)
C
C...read an event from a zebra FZ file
C
C     lok = 0 if no problems were encountered

      IMPLICIT NONE
#include "stdhep/stdhep.inc"
#include "stdhep/stdlun.inc"
#include "stdhep/hepzbr.inc"

      integer NUH,IUH,lok
      logical lfirst
      data lfirst/.TRUE./
      save lfirst

C...print version number if this is the first call
      if(lfirst)then
        call stdversn
        lfirst=.FALSE.
      endif

      lok = 0
      if(LNHRD.LE.0)then
        write(LNHOUT,102) LNHRD
        lok=1
      else
C...read the event 
        call MZWIPE(HPXEVT)
        call FZIN(LNHRD,HPXEVT,LE1,1,' ',NUH,IUH)
C...unpack and fill HEPEVT
        call HPZFLC
        if(NHEP.LE.0)then
          write(LNHOUT,101) NEVHEP,IUH
          lok=2
        endif
      endif
      return
  102 format(/5X,'HEPZRD: illegal output unit number:',I8)
  101 format(/5X,'HEPZRD: no data for event',I8,' header:',I10)
      end

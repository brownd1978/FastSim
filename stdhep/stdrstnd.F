
      subroutine STDRSTND(lok)

C  read stdhep end run information
C
C       NB = buffer size
C
C       lok = 0 if no problems were encountered

#include "stdhep/stdcm1.inc"
#include "stdhep/stdlun.inc"

      integer lok

      READ(LNHRD,ERR=900) STDECOM,STDXSEC,STDSEED1,STDSEED2,
     1              NEVTREQ,NEVTGEN,NEVTWRT
      return

  900 lok = 1
      write (LNHOUT,1005)
 1005 format(/5X,'STDRSTND: read error')
      return
      end

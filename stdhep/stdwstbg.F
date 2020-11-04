
      subroutine STDWSTBG(ilbl,lok)

C  write stdhep begin run information
C
C     lok = 0 if no problems were encountered

#include "stdhep/stdcm1.inc"
#include "stdhep/stdlun.inc"

      integer ilbl,lok,NB

      NB = 9
      write(LNHWRT,ERR=900) ilbl,NB
      write(LNHWRT,ERR=900) STDECOM,STDXSEC,STDSEED1,STDSEED2,
     1              NEVTREQ,NEVTGEN,NEVTWRT
      return
 900  lok = 1
      return
      end

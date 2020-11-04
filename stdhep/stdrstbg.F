
      subroutine STDRSTBG(lok)

C  read begin run stdhep information
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
      return
      end

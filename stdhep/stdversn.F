      subroutine stdversn
C
C...print STDHEP version number
C
#include "stdhep/stdver.inc"
#include "stdhep/stdlun.inc"
      logical lfirst
      data lfirst/.TRUE./
      save lfirst

      if(lfirst)then
        lfirst = .FALSE.
        stdhep_ver  = '3.00'
        stdhep_date = 'June 21, 1995'
        write(LNHOUT,1001) stdhep_ver,stdhep_date
      endif
1001  format(//
     1 10X,'********************************************************'/
     2 10X,'*       STDHEP version ',a5,' -  ',a20,'   *'/
     3 10X,'********************************************************'//)
      return
      end


      subroutine STDFLISXSEC

C   fill stdhep common block 1 with Isajet information
C
C   STDECOM  - center-of-mass energy
C   STDXSEC  - cross-section
C   STDSEED1 - random number seed
C   STDSEED2 - random number seed
C   NEVTREQ  - number of events to be generated
C   NEVTGEN  - number of events actually generated
C   NEVTWRT  - number of events written to output file
C
      IMPLICIT NONE

#include "stdhep/stdcm1.inc"
#include "stdhep/stdcnt.inc"
#include "stdhep/primar.inc"
#include "stdhep/seed.inc"
#include "stdhep/idrun.inc"
#include "stdhep/final.inc"

      READ(XSEED,'(E24.15)') STDSEED1
      STDSEED2 = 0.
      STDECOM = ECM
      STDXSEC = SIGF
      NEVTREQ = NEVENT
      NEVTGEN = IEVT
      NEVTWRT = nstdwrt
      return
      end

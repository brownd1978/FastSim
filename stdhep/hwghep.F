
      subroutine HWGHEP(mconv)

C...Purpose: to convert HERWIG event record contents to or from
C...the standard event record common block.
C...convert (mconv=1) from HERWIG numbering scheme to PDG numbering scheme
C...     or (mconv=2) from PDG numbering scheme to HERWIG numbering scheme

C...Transfer between single (STDHEP) and double (HERWIG) precision
C...Only IDHEP needs to be changed

#include "stdhep/herwig.inc"

#include "stdhep/hepevts.inc"

      integer ITMP,hwtran,mconv,I,J

      logical lfirst
      data lfirst/.TRUE./
      save lfirst

C...print version number if this is the first call
      if(lfirst)then
        call stdversn
        lfirst=.FALSE.
      endif
C...Conversion from HERWIG to standard.
      if(mconv.EQ.1) then
        NHEPS = NHEP
        NEVHEPS = NEVHEP
        do 140 I=1,NHEPS
          ISTHEPS(I) = ISTHEP(I)
          if(IDHEP(I).NE.0)then
            ITMP=IDHEP(I)
            IDHEPS(I) = hwtran(ITMP,1)
          elseif(IDHW(I).EQ.20)then
C...????
            IDHEPS(I)=89
          elseif(IDHW(I).LE.18)then
C...the oddball intermediate states
            IDHEPS(I)=IDHW(I)+67
          else
C...undefined
            IDHEPS(I)=0
          endif
          do 110 J=1,2
            JMOHEPS(J,I) = JMOHEP(J,I)
 110        JDAHEPS(J,I) = JDAHEP(J,I)
          do 120 J=1,5
 120        PHEPS(J,I) = PHEP(J,I)
          do 130 J=1,4
 130        VHEPS(J,I) = VHEP(J,I)
 140    CONTINUE
C...Conversion from standard to HERWIG.
      elseif(mconv.EQ.2)then
        NHEP = NHEPS
        NEVHEP = NEVHEPS
        do 180 I=1,NHEP
          ISTHEP(I) = ISTHEPS(I)
          ITMP=IDHEPS(I)
          IDHEP(I) = hwtran(ITMP,2)
          do 150 J=1,2
            JMOHEP(J,I) = JMOHEPS(J,I)
 150        JDAHEP(J,I) = JDAHEPS(J,I)
          do 160 J=1,5
 160        PHEP(J,I) = PHEPS(J,I)
          do 170 J=1,4
 170        VHEP(J,I) = VHEPS(J,I)
 180    CONTINUE
      endif
      return
      end

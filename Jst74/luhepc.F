*CMZ :          10/02/95  14.23.09  by  Unknown
*-- Author :
C*********************************************************************
 
      SUBROUTINE LUHEPC(MCONV)
 
C...Purpose: to convert JETSET event record contents to or from
C...the standard event record commonblock.
CX      PARAMETER (NMXHEP=4000)
CX      COMMON/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
CX     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)

C  Update to the format in stdhep 3.0 and later.  (12/09/98 PCK)
C  Incidently, LUHEPC is not used in Babar software.  LUNHEP in
C  STDHEP library is used to fill the HETEVT common block.
C  Fixed energies for low mass particles when copying back to double precision

#include "Jst74/hepevt.inc"
#include "Jst74/lujets.inc"
#include "Jst74/ludat1.inc"
#include "Jst74/ludat2.inc"

C      integer NMXHEP
C      parameter (NMXHEP=4000)
C      common/HEPEVT/NEVHEP,NHEP,ISTHEP(NMXHEP),IDHEP(NMXHEP),
C     &JMOHEP(2,NMXHEP),JDAHEP(2,NMXHEP),PHEP(5,NMXHEP),VHEP(4,NMXHEP)
C      integer NEVHEP,NHEP,ISTHEP,IDHEP,JMOHEP,JDAHEP
C      double precision PHEP,VHEP

C      SAVE /HEPEVT/

      inew=0

C...Conversion from JETSET to standard, the easy part.
      IF(MCONV.EQ.1) THEN
        NEVHEP=0
        IF(N.GT.NMXHEP) CALL LUERRM(8,
     &  '(LUHEPC:) no more space in /HEPEVT/')
        NHEP=MIN(N,NMXHEP)
        DO 140 I=1,NHEP
        ISTHEP(I)=0
        IF(K(I,1).GE.1.AND.K(I,1).LE.10) ISTHEP(I)=1
        IF(K(I,1).GE.11.AND.K(I,1).LE.20) ISTHEP(I)=2
        IF(K(I,1).GE.21.AND.K(I,1).LE.30) ISTHEP(I)=3
        IF(K(I,1).GE.31.AND.K(I,1).LE.100) ISTHEP(I)=K(I,1)
        IDHEP(I)=K(I,2)
        JMOHEP(1,I)=K(I,3)
        JMOHEP(2,I)=0
        IF(K(I,1).NE.3.AND.K(I,1).NE.13.AND.K(I,1).NE.14) THEN
          JDAHEP(1,I)=K(I,4)
          JDAHEP(2,I)=K(I,5)
        ELSE
          JDAHEP(1,I)=0
          JDAHEP(2,I)=0
        ENDIF
        DO 100 J=1,5
        PHEP(J,I)=P(I,J)
  100   CONTINUE
        DO 110 J=1,4
        VHEP(J,I)=V(I,J)
  110   CONTINUE

c fix numerical issues for light particles - I know this is not pretty...
        IF ( IDHEP(I).EQ.11.or.IDHEP(I).EQ.-11.or.
     &       IDHEP(I).EQ.12.or.IDHEP(I).EQ.-12.or.
     &       IDHEP(I).EQ.14.or.IDHEP(I).EQ.-14.or.
     &       IDHEP(I).EQ.16.or.IDHEP(I).EQ.-16     ) THEN
           PHEP(4,I)=SQRT(PHEP(1,I)*PHEP(1,I)+PHEP(2,I)*PHEP(2,I)+
     &                    PHEP(3,I)*PHEP(3,I)+PHEP(5,I)*PHEP(5,I))
        ENDIF

 
C...Check if new event (from pileup).
        IF(I.EQ.1) THEN
          INEW=1
        ELSE
          IF(K(I,1).EQ.21.AND.K(I-1,1).NE.21) INEW=I
        ENDIF
 
C...Fill in missing mother information.
        IF(I.GE.INEW+2.AND.K(I,1).EQ.21.AND.K(I,3).EQ.0) THEN
          IMO1=I-2
          IF(I.GE.INEW+3.AND.K(I-1,1).EQ.21.AND.K(I-1,3).EQ.0)
     &    IMO1=IMO1-1
          JMOHEP(1,I)=IMO1
          JMOHEP(2,I)=IMO1+1
        ELSEIF(K(I,2).GE.91.AND.K(I,2).LE.93) THEN
          I1=K(I,3)-1
  120     I1=I1+1
          IF(I1.GE.I) then
c abi 17 Apr 03: Changing to a more informative message,
c     and going to 125 to prevent 2nd mother from being
c     set to the particle I itself:
c             write(MSTU(11),*) 'LUHEPC: 2nd mother of particle of type ' 
c     &            , K(I,2), ' not found. Setting to 0.'
c             write(MSTU(11),*) 'LUHEPC: This happens with some EvtGen '
c     &            , 'events and is safe.'
             goto 125  ! no second mother, probably from EvtGen, Ignore it
c end of abi 17 Apr 03
          endif
          IF(I1.LT.I.AND.K(I1,1).NE.1.AND.K(I1,1).NE.11) GOTO 120
          KC=LUCOMP(K(I1,2))
          IF(I1.LT.I.AND.KC.EQ.0) GOTO 120
          IF(I1.LT.I.AND.KCHG(KC,2).EQ.0) GOTO 120
          JMOHEP(2,I)=I1
        ELSEIF(K(I,2).EQ.94) THEN
          NJET=2
          IF(NHEP.GE.I+3.AND.K(I+3,3).LE.I) NJET=3
          IF(NHEP.GE.I+4.AND.K(I+4,3).LE.I) NJET=4
          JMOHEP(2,I)=MOD(K(I+NJET,4)/MSTU(5),MSTU(5))
          IF(JMOHEP(2,I).EQ.JMOHEP(1,I)) JMOHEP(2,I)=
     &    MOD(K(I+1,4)/MSTU(5),MSTU(5))
        ENDIF
 125    continue  !abi 17 Apr 03

C...Fill in missing daughter information.
        IF(K(I,2).EQ.94.AND.MSTU(16).NE.2) THEN
          DO 130 I1=JDAHEP(1,I),JDAHEP(2,I)
          I2=MOD(K(I1,4)/MSTU(5),MSTU(5))
          JDAHEP(1,I2)=I
  130     CONTINUE
        ENDIF
        IF(K(I,2).GE.91.AND.K(I,2).LE.94) GOTO 140
        I1=JMOHEP(1,I)
        IF(I1.LE.0.OR.I1.GT.NHEP) GOTO 140
        IF(K(I1,1).NE.13.AND.K(I1,1).NE.14) GOTO 140
        IF(JDAHEP(1,I1).EQ.0) THEN
          JDAHEP(1,I1)=I
        ELSE
          JDAHEP(2,I1)=I
        ENDIF
  140   CONTINUE
        DO 150 I=1,NHEP
        IF(K(I,1).NE.13.AND.K(I,1).NE.14) GOTO 150
        IF(JDAHEP(2,I).EQ.0) JDAHEP(2,I)=JDAHEP(1,I)
  150   CONTINUE
 
C...Conversion from standard to JETSET, the easy part.
      ELSE
        IF(NHEP.GT.MSTU(4)) CALL LUERRM(8,
     &  '(LUHEPC:) no more space in /LUJETS/')
        N=MIN(NHEP,MSTU(4))
        NKQ=0
        KQSUM=0
        DO 180 I=1,N
        K(I,1)=0
        IF(ISTHEP(I).EQ.1) K(I,1)=1
        IF(ISTHEP(I).EQ.2) K(I,1)=11
        IF(ISTHEP(I).EQ.3) K(I,1)=21
        K(I,2)=IDHEP(I)
        K(I,3)=JMOHEP(1,I)
        K(I,4)=JDAHEP(1,I)
        K(I,5)=JDAHEP(2,I)
        DO 160 J=1,5
        P(I,J)=PHEP(J,I)
  160   CONTINUE
        DO 170 J=1,4
        V(I,J)=VHEP(J,I)
  170   CONTINUE
        V(I,5)=0.
        IF(ISTHEP(I).EQ.2.AND.PHEP(4,I).GT.PHEP(5,I)) THEN
          I1=JDAHEP(1,I)
          IF(I1.GT.0.AND.I1.LE.NHEP) V(I,5)=(VHEP(4,I1)-VHEP(4,I))*
     &    PHEP(5,I)/PHEP(4,I)
        ENDIF
 
C...Fill in missing information on colour connection in jet systems.
        IF(ISTHEP(I).EQ.1) THEN
          KC=LUCOMP(K(I,2))
          KQ=0
          IF(KC.NE.0) KQ=KCHG(KC,2)*ISIGN(1,K(I,2))
          IF(KQ.NE.0) NKQ=NKQ+1
          IF(KQ.NE.2) KQSUM=KQSUM+KQ
          IF(KQ.NE.0.AND.KQSUM.NE.0) THEN
            K(I,1)=2
          ELSEIF(KQ.EQ.2.AND.I.LT.N) THEN
            IF(K(I+1,2).EQ.21) K(I,1)=2
          ENDIF
        ENDIF
  180   CONTINUE
        IF(NKQ.EQ.1.OR.KQSUM.NE.0) CALL LUERRM(8,
     &  '(LUHEPC:) input parton configuration not colour singlet')
      ENDIF
 
      END

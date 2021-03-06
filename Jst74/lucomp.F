      FUNCTION LUCOMP(KF)
      implicit none
*****-*****************************************************************-*******
C...Purpose: to compress the standard KF codes for use in mass and decay
C...arrays; also to check whether a given code actually is defined.
C.. History:
C
C     12-Aug-1997 - Lockman : implicit none added; save KFTAB, KCTAB 
C... modified R.Waldi/92-07.v7.4:97-06 beget conv./stdhep, 97/11 evtgen
C     11-Sep-2000 - Mark Ian Williams added X_su/d/s for BtoXsgamma model
*****-*****************************************************************-*******
#include "Jst74/ludat2.inc"
      integer kf
      integer lucomp, lucomp_beg
      integer kchg
      real*4 pmas, parf, vckm
*     DIMENSION KFTAB(25),KCTAB(25)
      integer KFTAB(25),KCTAB(25)
      save KFTAB, KCTAB
      integer kfa, ikf, kfla, kflb, kflc, kfls, kflr
      DATA KFTAB/211,111,221,311,321,130,310,213,113,223,
     &313,323,2112,2212,210,2110,2210,110,220,330,440,30443,30553,0,0/
      DATA KCTAB/101,111,112,102,103,221,222,121,131,132,
     &122,123,332,333,281,282,283,284,285,286,287,231,235,0,0/
 
C...Starting values.
      LUCOMP=lucomp_beg(KF)
      IF (LUCOMP .NE. 0)  RETURN

      KFA=IABS(KF)
 
C...Subdivide KF code into constituent pieces.
      
      KFLR=MOD(KFA/10000,10)
      KFLA=MOD(KFA/1000,10)
      KFLB=MOD(KFA/100,10)
      KFLC=MOD(KFA/10,10)
      KFLS=MOD(KFA,10)

C...Hardwire the return code for -42 since EvtJetSet updates the particles
C   too late for the Xu- decays to be recognized
      IF (KF.EQ.-42) THEN
        LUCOMP=KFA
        RETURN
      ENDIF

C...Add double charm baryons
      IF (KFLA.EQ.4.AND.KFLB.EQ.4) THEN
C.....We only have the ground states
         IF (KFLC.LE.3) THEN
            LUCOMP=490+KFLC
            IF (KFLS.EQ.4) LUCOMP=LUCOMP+3
         ELSE 
            LUCOMP=0
         ENDIF
         RETURN
      ENDIF

C...Add exited charm baryons
      IF (KFLA.EQ.4.AND.KFLR.GE.1) THEN
         IF (KFLR.EQ.1) THEN
            LUCOMP=460+(KFLB-1)*3+KFLC+9*(KFLS-2)/2
            RETURN
         ENDIF
c...More Lambda_c* baryons         
         IF (KFLB.EQ.1.AND.KFLC.EQ.2) THEN
            LUCOMP=480+(KFLS-2)/2+(KFLR-2)*2
            RETURN
         ENDIF
      ENDIF
         
C...Allow for massive sbar-u, sbar-d, sbar-s systems
      IF (KFA.EQ.30343.OR.KFA.EQ.30353.OR.KFA.EQ.30363) THEN
        LUCOMP=451+KFLC
        RETURN
      ENDIF
                                                                                                                
C...Allow for massive dbar-u, dbar-d systems
      IF (KFA.EQ.30373.OR.KFA.EQ.30383) THEN
        LUCOMP=451+KFLC
        RETURN
      ENDIF

C...Add eta_c(2S)
      IF (KFA.EQ.20441) THEN
         LUCOMP=460
         RETURN
      ENDIF
             
C...Add sbottom
      IF (KFA.EQ.500) THEN
         LUCOMP=500
         RETURN
      ENDIF

C...Simple cases: direct translation or table.
      IF(KFA.EQ.0.OR.KFA.GE.100000) THEN
        RETURN
      ELSEIF(KFA.LE.100) THEN
        LUCOMP=KFA
        IF(KF.LT.0.AND.KCHG(KFA,3).EQ.0) LUCOMP=0
        RETURN
      ELSE
        DO 100 IKF=1,23
        IF(KFA.EQ.KFTAB(IKF)) THEN
          LUCOMP=KCTAB(IKF)
          IF(KF.LT.0.AND.KCHG(LUCOMP,3).EQ.0) LUCOMP=0
          RETURN
        ENDIF
  100   CONTINUE
      ENDIF

C...Mesons.
      IF(KFA-10000*KFLR.LT.1000) THEN
        IF(KFLB.EQ.0.OR.KFLB.EQ.9.OR.KFLC.EQ.0.OR.KFLC.EQ.9) THEN
        ELSEIF(KFLB.LT.KFLC) THEN
        ELSEIF(KF.LT.0.AND.KFLB.EQ.KFLC) THEN
        ELSEIF(KFLB.EQ.KFLC) THEN
          IF(KFLR.EQ.0.AND.KFLS.EQ.1) THEN
            LUCOMP=110+KFLB
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.3) THEN
            LUCOMP=130+KFLB
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.3) THEN
            LUCOMP=150+KFLB
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.1) THEN
            LUCOMP=170+KFLB
          ELSEIF(KFLR.EQ.2.AND.KFLS.EQ.3) THEN
            LUCOMP=190+KFLB
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.5) THEN
            LUCOMP=210+KFLB
          ENDIF
        ELSEIF(KFLB.LE.5) THEN
          IF(KFLR.EQ.0.AND.KFLS.EQ.1) THEN
            LUCOMP=100+((KFLB-1)*(KFLB-2))/2+KFLC
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.3) THEN
            LUCOMP=120+((KFLB-1)*(KFLB-2))/2+KFLC
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.3) THEN
            LUCOMP=140+((KFLB-1)*(KFLB-2))/2+KFLC
          ELSEIF(KFLR.EQ.1.AND.KFLS.EQ.1) THEN
            LUCOMP=160+((KFLB-1)*(KFLB-2))/2+KFLC
          ELSEIF(KFLR.EQ.2.AND.KFLS.EQ.3) THEN
            LUCOMP=180+((KFLB-1)*(KFLB-2))/2+KFLC
          ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.5) THEN
            LUCOMP=200+((KFLB-1)*(KFLB-2))/2+KFLC
          ENDIF
        ELSEIF((KFLS.EQ.1.AND.KFLR.LE.1).OR.(KFLS.EQ.3.AND.KFLR.LE.2)
     &  .OR.(KFLS.EQ.5.AND.KFLR.EQ.0)) THEN
          LUCOMP=80+KFLB
        ENDIF
 
C...Diquarks.
      ELSEIF((KFLR.EQ.0.OR.KFLR.EQ.1).AND.KFLC.EQ.0) THEN
        IF(KFLS.NE.1.AND.KFLS.NE.3) THEN
        ELSEIF(KFLA.EQ.9.OR.KFLB.EQ.0.OR.KFLB.EQ.9) THEN
        ELSEIF(KFLA.LT.KFLB) THEN
        ELSEIF(KFLS.EQ.1.AND.KFLA.EQ.KFLB) THEN
        ELSE
          LUCOMP=90
        ENDIF
 
C...Spin 1/2 baryons.
      ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.2) THEN
        IF(KFLA.EQ.9.OR.KFLB.EQ.0.OR.KFLB.EQ.9.OR.KFLC.EQ.9) THEN
        ELSEIF(KFLA.LE.KFLC.OR.KFLA.LT.KFLB) THEN
        ELSEIF(KFLA.GE.6.OR.KFLB.GE.4.OR.KFLC.GE.4) THEN
          LUCOMP=80+KFLA
        ELSEIF(KFLB.LT.KFLC) THEN
          LUCOMP=300+((KFLA+1)*KFLA*(KFLA-1))/6+(KFLC*(KFLC-1))/2+KFLB
        ELSE
          LUCOMP=330+((KFLA+1)*KFLA*(KFLA-1))/6+(KFLB*(KFLB-1))/2+KFLC
        ENDIF
 
C...Spin 3/2 baryons.
      ELSEIF(KFLR.EQ.0.AND.KFLS.EQ.4) THEN
        IF(KFLA.EQ.9.OR.KFLB.EQ.0.OR.KFLB.EQ.9.OR.KFLC.EQ.9) THEN
        ELSEIF(KFLA.LT.KFLB.OR.KFLB.LT.KFLC) THEN
        ELSEIF(KFLA.GE.6.OR.KFLB.GE.4) THEN
          LUCOMP=80+KFLA
        ELSE
          LUCOMP=360+((KFLA+1)*KFLA*(KFLA-1))/6+(KFLB*(KFLB-1))/2+KFLC
        ENDIF
      ENDIF
 
      RETURN
      END

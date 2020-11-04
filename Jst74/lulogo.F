*CMZ :          10/02/95  14.23.04  by  Unknown
*-- Author :
C*********************************************************************
 
      SUBROUTINE LULOGO
 
C...Purpose: to write logo for JETSET and PYTHIA programs.
#include "Jst74/ludat1.inc"
#include "Jst74/pypars.inc"

      CHARACTER MONTH(12)*3, LOGO(48)*32, REFER(16)*36, LINE*79,
     &VERS*1, SUBV*3, DATE*2, YEAR*4
 
C...Data on months, logo, titles, and references.
      DATA MONTH/'Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep',
     &'Oct','Nov','Dec'/
      DATA (LOGO(J),J=1,10)/
     &'PPP  Y   Y TTTTT H   H III   A  ',
     &'P  P  Y Y    T   H   H  I   A A ',
     &'PPP    Y     T   HHHHH  I  AAAAA',
     &'P      Y     T   H   H  I  A   A',
     &'P      Y     T   H   H III A   A',
     &'JJJJ EEEE TTTTT  SSS  EEEE TTTTT',
     &'   J E      T   S     E      T  ',
     &'   J EEE    T    SSS  EEE    T  ',
     &'J  J E      T       S E      T  ',
     &' JJ  EEEE   T    SSS  EEEE   T  '/
      DATA (LOGO(J),J=11,29)/
     &'            *......*            ',
     &'       *:::!!:::::::::::*       ',
     &'    *::::::!!::::::::::::::*    ',
     &'  *::::::::!!::::::::::::::::*  ',
     &' *:::::::::!!:::::::::::::::::* ',
     &' *:::::::::!!:::::::::::::::::* ',
     &'  *::::::::!!::::::::::::::::*! ',
     &'    *::::::!!::::::::::::::* !! ',
     &'    !! *:::!!:::::::::::*    !! ',
     &'    !!     !* -><- *         !! ',
     &'    !!     !!                !! ',
     &'    !!     !!                !! ',
     &'    !!                       !! ',
     &'    !!        ep             !! ',
     &'    !!                       !! ',
     &'    !!                 pp    !! ',
     &'    !!   e+e-                !! ',
     &'    !!                       !! ',
     &'    !!                          '/
      DATA (LOGO(J),J=30,48)/
     &'Welcome to the Lund Monte Carlo!',
     &'                                ',
     &'  This is PYTHIA version x.xxx  ',
     &'Last date of change: xx xxx 199x',
     &'                                ',
     &'  This is JETSET version x.xxx  ',
     &'Last date of change: xx xxx 199x',
     &'                                ',
     &'                                ',
     &'          Main author:          ',
     &'       Torbjorn Sjostrand       ',
     &'     Theory Division, CERN,     ',
     &'       CH-1211 Geneva 23,       ',
     &'          Switzerland           ',
     &'   phone +41 - 22 - 767 28 20   ',
     &'  E-mail TORSJO@CERNVM.CERN.CH  ',
     &'                                ',
     &'  Copyright Torbjorn Sjostrand  ',
     &'     and CERN, Geneva 1993      '/
      DATA REFER/
     &'When you cite these programs, priori',
     &'ty should always be given to the    ',
     &'latest published description. Curren',
     &'tly this is                         ',
     &'T. Sjostrand, Computer Physics Commu',
     &'n. 82 (1994) 74.                    ',
     &'The most recent long description (un',
     &'published) is                       ',
     &'T. Sjostrand, CERN-TH.7112/93 (1993,',
     &' revised August 1994).              ',
     &'Also remember that the programs, to ',
     &'a large extent, represent original  ',
     &'physics research. Other publications',
     &' of special relevance to your       ',
     &'studies may therefore deserve separa',
     &'te mention.                         '/
 
C...Check if PYTHIA linked.
      IF(MSTP(183)/10.NE.199) THEN
        LOGO(32)=' Warning: PYTHIA is not loaded! '
        LOGO(33)='Did you remember to link PYDATA?'
      ELSE
        WRITE(VERS,'(I1)') MSTP(181)
        LOGO(32)(26:26)=VERS
        WRITE(SUBV,'(I3)') MSTP(182)
        LOGO(32)(28:30)=SUBV
        WRITE(DATE,'(I2)') MSTP(185)
        LOGO(33)(22:23)=DATE
        LOGO(33)(25:27)=MONTH(MSTP(184))
        WRITE(YEAR,'(I4)') MSTP(183)
        LOGO(33)(29:32)=YEAR
      ENDIF
 
C...Check if JETSET linked.
      IF(MSTU(183)/10.NE.199) THEN
        LOGO(35)='  Error: JETSET is not loaded!  '
        LOGO(36)='Did you remember to link LUDATA?'
      ELSE
        WRITE(VERS,'(I1)') MSTU(181)
        LOGO(35)(26:26)=VERS
        WRITE(SUBV,'(I3)') MSTU(182)
        LOGO(35)(28:30)=SUBV
        WRITE(DATE,'(I2)') MSTU(185)
        LOGO(36)(22:23)=DATE
        LOGO(36)(25:27)=MONTH(MSTU(184))
        WRITE(YEAR,'(I4)') MSTU(183)
        LOGO(36)(29:32)=YEAR
      ENDIF
 
C...Loop over lines in header. Define page feed and side borders.
      DO 100 ILIN=1,45
      LINE=' '
      IF(ILIN.EQ.1) THEN
        LINE(1:1)='1'
      ELSE
        LINE(2:3)='**'
        LINE(78:79)='**'
      ENDIF
 
C...Separator lines and logos.
      IF(ILIN.EQ.2.OR.ILIN.EQ.3.OR.ILIN.EQ.44.OR.ILIN.EQ.45) THEN
        LINE(4:77)='***********************************************'//
     &  '***************************'
      ELSEIF(ILIN.GE.6.AND.ILIN.LE.10) THEN
        LINE(6:37)=LOGO(ILIN-5)
        LINE(44:75)=LOGO(ILIN)
      ELSEIF(ILIN.GE.13.AND.ILIN.LE.31) THEN
        LINE(6:37)=LOGO(ILIN-2)
        LINE(44:75)=LOGO(ILIN+17)
      ELSEIF(ILIN.GE.34.AND.ILIN.LE.41) THEN
        LINE(5:40)=REFER(2*ILIN-67)
        LINE(41:76)=REFER(2*ILIN-66)
      ENDIF
 
C...Write lines to appropriate unit.
      IF(MSTU(183)/10.EQ.199) THEN
        WRITE(MSTU(11),'(A79)') LINE
      ELSE
        WRITE(*,'(A79)') LINE
      ENDIF
  100 CONTINUE
 
C...Check that matching subversions are linked.
      IF(MSTU(183)/10.EQ.199.AND.MSTP(183)/10.EQ.199) THEN
        IF(MSTU(182).LT.MSTP(186)) WRITE(MSTU(11),
     &  '(/'' Warning: JETSET subversion too old for PYTHIA''/)')
        IF(MSTP(182).LT.MSTU(186)) WRITE(MSTU(11),
     &  '(/'' Warning: PYTHIA subversion too old for JETSET''/)')
      ENDIF
 
      RETURN
      END

*CMZ :          10/02/95  14.23.02  by  Unknown
*-- Author :
C*********************************************************************
 
      SUBROUTINE LUERRM(MERR,CHMESS)
 
C...Purpose: to inform user of errors in program execution.
#include "Jst74/lujets.inc"
#include "Jst74/ludat1.inc"

      CHARACTER CHMESS*(*)
 
C...Write first few warnings, then be silent.
      IF(MERR.LE.10) THEN
        MSTU(27)=MSTU(27)+1
        MSTU(28)=MERR
        IF(MSTU(25).EQ.1.AND.MSTU(27).LE.MSTU(26)) WRITE(MSTU(11),5000)
     &  MERR,MSTU(31),CHMESS
 
C...Write first few errors, then be silent or stop program.
      ELSEIF(MERR.LE.20) THEN
        MSTU(23)=MSTU(23)+1
        MSTU(24)=MERR-10
        IF(MSTU(21).GE.1.AND.MSTU(23).LE.MSTU(22)) WRITE(MSTU(11),5100)
     &  MERR-10,MSTU(31),CHMESS
        IF(MSTU(21).GE.2.AND.MSTU(23).GT.MSTU(22)) THEN
          WRITE(MSTU(11),5100) MERR-10,MSTU(31),CHMESS
          WRITE(MSTU(11),5200)
          IF(MERR.NE.17) CALL LULIST(2)
          STOP
        ENDIF
 
C...Stop program in case of irreparable error.
      ELSE
        WRITE(MSTU(11),5300) MERR-20,MSTU(31),CHMESS
        STOP
      ENDIF
 
C...Formats for output.
 5000 FORMAT(/5X,'Advisory warning type',I2,' given after',I6,
     &' LUEXEC calls:'/5X,A)
 5100 FORMAT(/5X,'Error type',I2,' has occured after',I6,
     &' LUEXEC calls:'/5X,A)
 5200 FORMAT(5X,'Execution will be stopped after listing of last ',
     &'event!')
 5300 FORMAT(/5X,'Fatal error type',I2,' has occured after',I6,
     &' LUEXEC calls:'/5X,A/5X,'Execution will now be stopped!')
 
      RETURN
      END

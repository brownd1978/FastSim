C
      integer   MXJSET,JPACK
      parameter (MXJSET=400,JPACK=1000)
      common/JETSET/NJSET,PJSET(5,MXJSET),JORIG(MXJSET),JTYPE(MXJSET),
     $JDCAY(MXJSET)
      save /JETSET/
      integer   NJSET,JORIG,JTYPE,JDCAY
      real      PJSET
C

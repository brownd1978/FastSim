C
      integer   MXPTCL,IPACK
      parameter (MXPTCL=4000,IPACK=10000)
      common/PARTCL/NPTCL,PPTCL(5,MXPTCL),IORIG(MXPTCL),IDENT(MXPTCL)
     1,IDCAY(MXPTCL)
      save /PARTCL/
      integer   NPTCL,IORIG,IDENT,IDCAY
      real      PPTCL
C

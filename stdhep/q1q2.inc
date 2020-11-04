C
      integer MXGOQ
      parameter (MXGOQ=85)
      common/Q1Q2/GOQ(MXGOQ,3),GOALL(3),GODY(4),STDDY,GOWW(25,2),
     $ALLWW(2),GOWMOD(25,3)
      save /Q1Q2/
      logical GOQ,GOALL,GODY,STDDY,GOWW,ALLWW,GOWMOD
C

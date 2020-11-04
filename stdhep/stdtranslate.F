c------------------------------------------------------------------------
c PURPOSE: 
c translate the vertex points of all particles in the common block by 
c the vector (x, y, z).
c
c AUTHOR: 
c Abi Soffer
c------------------------------------------------------------------------

      subroutine stdTranslate(x, y, z, t)
      implicit none

#include "stdhep/stdhep.inc"
      
      double precision x, y, z, t   ! translation vector
      integer trk                   ! dummy indices

      do trk = 1, nhep              ! on tracks 
         vhep(1, trk) = vhep(1, trk) + x
         vhep(2, trk) = vhep(2, trk) + y
         vhep(3, trk) = vhep(3, trk) + z
         vhep(4, trk) = vhep(4, trk) + t
      enddo
      return
      end


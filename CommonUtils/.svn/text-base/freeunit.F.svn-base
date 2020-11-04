      subroutine freeunit(Iunit)
*
* find an unused fortran file unit number
* returns Iunit =0 if there are none left
*
* Doug Wright, Mar 1994
* T. Wenaus 14/07/1994: Start high; stay away from low luns.
*
      implicit none
      integer iunit
      logical used

      do iunit=99,1,-1
         inquire(unit=iunit,opened=used)
         if( .not. used ) goto 999
      enddo
      print *,'FREEUNIT: NO FREE UNIT NUMBERS'
      iunit = 0 ! NO FREE UNITS!
 999  continue
      end

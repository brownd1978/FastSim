c FILE:
c   StdHepUtil.F
c---------------------------------------------------------------
c PURPOSE:
c   Fortran interface to stdhep common blocks to perform tasks common
c   to several generators. See stdhep/include/stdhep.inc.
c   utilities needed both by beget and GenFwkInt
c---------------------------------------------------------------
c Author:
c   Abi Soffer
c---------------------------------------------------------------

c===================================================================
c Add incoming particles to the common block:

      subroutine std_insert_incoming(p1, id1, p2, id2, v)
      
      implicit none

#include "stdhep/stdhep.inc"
*
      double precision p1(4), p2(4), v(4)
      integer id1, id2
      integer axis, first, second, particle
      integer firstDaughter, lastDaughter, next, i
      double precision me

      integer INCOMING
      parameter(INCOMING = 3)
      data me / 0.00051099906 /
c Make space for 2 particles at the beginning of all arrays:

      do particle = NHEP, 1, -1
c     Increment non-0 mother and daughter pointers:
         do i = 1,2
            if (JMOHEP(i, particle) .gt. 0) then
               JMOHEP(i, particle) = JMOHEP(i, particle) + 2 
            endif

            if (JDAHEP(i, particle) .gt. 0) then
               JDAHEP(i, particle) = JDAHEP(i, particle) + 2
            endif
         enddo

c     Shift arrays:
         next = particle + 2
         ISTHEP(next) = ISTHEP(particle)
         IDHEP(next) = IDHEP(particle)
         
         do i = 1,4
            PHEP(i, next) = PHEP(i, particle)
            VHEP(i, next) = VHEP(i, particle)
         enddo
         
         PHEP(5, next) = PHEP(5, particle)
         
         do i = 1,2
            JMOHEP(i, next) = JMOHEP(i, particle)
            JDAHEP(i, next) = JDAHEP(i, particle)
         enddo
      enddo                     ! loop on all entries

c Set indices:
      first = 1
      second = 2
      NHEP = NHEP + 2

c Set the argument data:
      do axis = 1,4
         PHEP(axis, first) = p1(axis)
         VHEP(axis, first) = v(axis)
         PHEP(axis, second) = p2(axis)
         VHEP(axis, second) = v(axis)
      enddo
      
      if (abs(id1).eq.11) then
         PHEP(5, first) = me
      else
         print *, 'unknown beam particle code:',id1
         PHEP(5, first) = 0
      end if
      if (abs(id2).eq.11) then
         PHEP(5, second) = me
      else
         print *, 'unknown beam particle code:',id2
         PHEP(5, second) = 0
      end if

      IDHEP(first) = id1
      IDHEP(second) = id2

      JMOHEP(1, first) = 0
      JMOHEP(1, second) = 0
      JMOHEP(2, first) = 0
      JMOHEP(2, second) = 0

      ISTHEP(first) = INCOMING
      ISTHEP(second) = INCOMING

c Make mother-daughter links. First, find the 1st and last daughters of the 
c incoming particles:
      firstDaughter = 0
      lastDaughter = 0

      do particle = 3, NHEP
         if (JMOHEP(1, particle) .eq. 0) then
            if (firstDaughter .eq. 0) then
               firstDaughter = particle
            endif
            lastDaughter = particle
         endif
      enddo

      JDAHEP(1, first) = firstDaughter
      JDAHEP(2, first) = lastDaughter
      JDAHEP(1, second) = firstDaughter
      JDAHEP(2, second) = lastDaughter

      do particle = firstDaughter, lastDaughter
         if (particle .gt. 0 .and. particle .le. nhep) then
            JMOHEP(1, particle) = first
            JMOHEP(2, particle) = second
         endif
      enddo

      return
      end

c===================================================================
c Remove incoming particles (isthep = 3) from the common block, and
c undo their daughter links to them:

      subroutine std_remove_incoming()

      implicit none

#include "stdhep/stdhep.inc"      

      integer inc, trk, previous, i
      integer INCOMING
      parameter (INCOMING = 3)   ! stdhep flag for an incoming particle

c------------

      inc = 1        ! Test if the first entry is an incoming particle
      do while (.true.)
         if (ISTHEP(inc) .eq. INCOMING) then                  
            do trk = 1, NHEP    ! on all entries
               if (trk .ne. inc) then ! do nothing to inc
c     Decrement mother and daughter pointers that point beyond inc,
c     and zero the ones that point at inc:
                  do i = 1,2
                     if (JMOHEP(i, trk) .gt. inc) then
                        JMOHEP(i, trk) = JMOHEP(i, trk) - 1
                     elseif (JMOHEP(i, trk) .eq. inc) then
                        JMOHEP(i, trk) = 0
                     endif

                     if (JDAHEP(i, trk) .gt. inc) then
                        JDAHEP(i, trk) = JDAHEP(i, trk) - 1
                     elseif (JDAHEP(i, trk) .eq. inc) then
                        JDAHEP(i, trk) = 0
                     endif
                  enddo

c     Shift arrays of particles beyond inc down one position:
                  previous = trk - 1
                  ISTHEP(previous) = ISTHEP(trk)
                  IDHEP(previous) = IDHEP(trk)
                  
                  do i = 1,4
                     PHEP(i, previous) = PHEP(i, trk)
                     VHEP(i, previous) = VHEP(i, trk)
                  enddo
                  
                  PHEP(5, previous) = PHEP(5, trk)
                  
                  do i = 1,2
                     JMOHEP(i, previous) = JMOHEP(i, trk)
                     JDAHEP(i, previous) = JDAHEP(i, trk)
                  enddo
               endif  ! if trk .ne. inc
            enddo  ! loop on all entries
            
c     Decrement the # of entries:
            NHEP = NHEP - 1
         else  ! if inc is not an incoming particle
c     Increment the test entry only if the common block wasn't changed
            inc = inc + 1       
            if (inc .gt. NHEP) then
               goto 100         ! break the while(.true.) loop 
            endif
         endif
      enddo  ! while(.true.)
 100  continue

      return
      end

c====================================================================
c Set the stdhep common block variable nhep to 0 at the start of an event, 
c and increments the event counter

      subroutine std_new_event()

      implicit none
      
#include "stdhep/stdhep.inc"

      NHEP = 0
      NEVHEP = NEVHEP + 1
      return
      end

c====================================================================
c Put the event number in the event into the common block.

      subroutine std_set_evt_num(l_nevhep)
      implicit none
      integer l_nevhep
#include "stdhep/stdhep.inc"

      nevhep = l_nevhep
      return
      end

c====================================================================
c Put number of entries in the event into the common block.

      subroutine std_set_num_entries(l_nhep)
      implicit none
      integer l_nhep
#include "stdhep/stdhep.inc"

      nhep = l_nhep
      return
      end


c======================================================================
c Change the p4 of an entry: 

      subroutine std_set_p4(index, p4) 
      implicit none
      integer index
      double precision p4(4)
      integer i
#include "stdhep/stdhep.inc"

      do i = 1,4
         phep(i, index) = p4(i)         
      enddo

      phep(5, index) = p4(4)**2 - p4(3)**2 - p4(2)**2 - p4(1)**2
      return
      end


c======================================================================
c Put entries into the common block. 

      subroutine std_set_entry(l_entry, l_isthep, l_idhep, 
     &     l_jmohep, l_jdahep, l_phep, l_vhep)

      implicit none
      integer l_entry, l_isthep, l_idhep, l_jmohep(2), l_jdahep(2)
      double precision l_phep(5), l_vhep(4)
      integer i
#include "stdhep/stdhep.inc"

      isthep(l_entry) = l_isthep
      idhep(l_entry) = l_idhep

      do i = 1,2
         jmohep(i, l_entry) = l_jmohep(i)
         jdahep(i, l_entry) = l_jdahep(i)
      enddo

      do i = 1,5
         phep(i, l_entry) = l_phep(i)
      enddo

      do i = 1,4
         vhep(i, l_entry) = l_vhep(i)
      enddo

      return
      end
      
c=====================================================================
c Return the first and last daughters of an entry:

      subroutine std_get_daughters(mother, first, last)

      implicit none

#include "stdhep/stdhep.inc"

      integer mother, first, last

      first = JDAHEP(1, mother)
      last = JDAHEP(2, mother)
C 8    last = JDAHEP(2, mother)
      return
      end

c=====================================================================
c Give the entry index and 4-momentum of primary particles, ie., particles 
c which have no mothers or whose mother's isthep = 3.
c The search starts from particle startIndex + 1. Returns 0 if no more primary 
c particles were found. isIncoming is returned as 1 if the particle whose
c index is returned is incoming, 0 otherwise.

      integer function std_primary(startIndex, mom, isIncoming)
      
      implicit none

#include "stdhep/stdhep.inc"

      integer startIndex, isIncoming
      double precision mom(4)
      integer particle, axis

      integer INCOMING
      parameter(INCOMING = 3)  ! stdhep code for an incoming particle

      std_primary = 0   ! return 0 by default
      isIncoming = 0

      do particle = startIndex + 1, NHEP
         if (JMOHEP(1, particle) .eq. 0) then

            do axis = 1,4
               mom(axis) = PHEP(axis, particle)
            enddo

            if (ISTHEP(particle) .eq. INCOMING) then
               isIncoming = 1
            endif

            std_primary = particle
            return
         elseif (ISTHEP(JMOHEP(1, particle)) .eq. INCOMING) then
            do axis = 1,4
               mom(axis) = PHEP(axis, particle)
            enddo

            if (ISTHEP(particle) .eq. INCOMING) then
               isIncoming = 1
            endif

            std_primary = particle
            return
         endif
      enddo            
      
      return
      end

c==================================================================
c This subroutine increases the isthep of particle entry by a given number
c to indicate that the particle (or one of its ancestors) is long lived and 
c the simulation should regard its decay mode as a suggestion only.

      subroutine std_flag_long_lived(entry)

      implicit none

      integer entry
#include "stdhep/stdhep.inc"

      ISTHEP(entry) = ISTHEP(entry) + 200  ! must match STDHEP_LONG_LIVED_FLAG
      return
      end


c==================================================================
c This subroutine returns nhep and an array of idheps:

      subroutine std_get_particle_ids(nParticles, particleIds)
      implicit none
      integer nParticles, particleIds(*), i
#include "stdhep/stdhep.inc"

      nParticles = NHEP
      do i = 1, NHEP
         particleIDs(i) = IDHEP(i)
      enddo

      return
      end

      



#ifndef MEMCLASSALLOCATOR_HH
#define MEMCLASSALLOCATOR_HH

/**
 * @file MemUtil/MemClassAllocator.h
 * 
 * Declaration of class MemClassAllocator and related functions
 *
 * @author Andrei A. Salnikov
 * @date   May 25, 2001
 *
 * Copyright (C) BINP, 2001 
 * 
 * $Id: MemClassAllocator.hh 517 2010-01-15 08:24:18Z stroili $
 */

// C++ includes
#include <cstdlib>
#include <iostream>

#include "MemUtil/MemUnsafePool.hh"
#include "MemUtil/MemAllocator.hh"

/**
 *  Per-class memory allocator. Works for fixed block size.
 */
template< typename Class >
class MemClassAllocator {

public:

  /// Constructor
  MemClassAllocator() ;

  /// Destructor 
  ~MemClassAllocator() ;

  /// allocate some memory
  void* alloc ( std::size_t size ) {
    if ( size == sizeof(Class) ) {
      void* p = pool_.get() ;
      if ( ! p ) p = ::operator new ( size ) ;
      return p ;
    } else {
      return MemAllocator::instance().alloc( size ) ;
    }
  }

  /// deallocate memory
  void free ( void* p, std::size_t size ) {
    if ( size == sizeof(Class) ) {
      pool_.take( p ) ;
    } else {
      MemAllocator::instance().free( p, size ) ;
    }
  }

protected:

private:

  MemUnsafePool pool_ ;

  /// Assigment operator, avoid default implementation
  MemClassAllocator< Class >& operator=(const MemClassAllocator< Class >&);

  /// Copy constructor, avoid default implementation
  MemClassAllocator(const MemClassAllocator< Class >&);

};

/// Output for class MemClassAllocator in "standard" form 
template< typename Class >
inline 
std::ostream& 
operator<<(std::ostream& s, const MemClassAllocator< Class >& t) 
{ 
  return t.print(s);
}

#ifdef    BABAR_COMP_INST
#include "MemUtil/MemClassAllocator.icc"
#endif  // BABAR_COMP_INST

#endif // MEMCLASSALLOCATOR_HH

// Local Variables: ***
// mode: c++ ***
// End: ***

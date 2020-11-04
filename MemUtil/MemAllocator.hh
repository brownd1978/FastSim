#ifndef MEMALLOCATOR_HH
#define MEMALLOCATOR_HH

/**
 * @file MemUtil/MemAllocator.h
 * 
 * Declaration of class MemAllocator and related functions
 *
 * @author Andrei A. Salnikov
 * @date   May 24, 2001
 *
 * Copyright (C) BINP, 2001 
 * 
 * $Id: MemAllocator.hh 517 2010-01-15 08:24:18Z stroili $
 */

// iostream forward declarations

#include "MemUtil/MemUnsafePool.hh"

/**
 *  This is a memory allocator which has a list of memory pools
 *  which each pool serving its block size.
 */
class MemAllocator {

  enum { N_POOLS = 129 } ;

public:

  /// "Constructor"
  static MemAllocator& instance() { return instance_ ; }

  /// Destructor 
  ~MemAllocator();

  /// allocate some memory
  void* alloc ( std::size_t size )
  {
    void* p = 0 ;
    if ( size < N_POOLS ) p = pools_[size].get() ;
    if ( ! p ) p = ::operator new ( size ) ;
    return p ;
  }

  /// deallocate memory
  void free ( void* p, std::size_t size )
  {
    if ( size >= N_POOLS ) {
      ::operator delete ( p ) ;
    } else {
      pools_[size].take( p ) ;
    }
  }

protected:

  /// Constructor
  MemAllocator();

private:

  MemUnsafePool pools_[ N_POOLS ] ;

  static MemAllocator instance_ ;

  /// Assigment operator, avoid default implementation
  MemAllocator& operator=(const MemAllocator&);

  /// Copy constructor, avoid default implementation
  MemAllocator(const MemAllocator&);

};

#endif // MEMALLOCATOR_HH

// Local Variables: ***
// mode: c++ ***
// End: ***

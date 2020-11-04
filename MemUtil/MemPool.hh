#ifndef MEMPOOL_HH
#define MEMPOOL_HH

/**
 * @file MemUtil/MemPool.h
 * 
 * Declaration of class MemPool and related functions
 *
 * @author Andrei A. Salnikov
 * @date   May 24, 2001
 *
 * Copyright (C) BINP, 2001 
 * 
 * $Id: MemPool.hh 517 2010-01-15 08:24:18Z stroili $
 */

// iostream forward declarations
#include <cstdlib>

/**
 *  This class is a memory pool of the blocks of the same size.
 */
class MemPool {

public:

  /// Constructor
  MemPool( std::size_t block_size );

  /// Destructor 
  ~MemPool();

  /// get one block. 
  bool empty() const {
    return _size == 0 ;
  }

  /// get one block. 
  void* get( std::size_t size ) {
    if ( size == _block_size && _size > 0 ) {
      return _blocks[--_size] ;
    }
    return ::operator new ( size ) ;
  }

  /// take one block
  void take ( void* p, std::size_t size ) {
    if ( size != _block_size ) {
      ::operator delete ( p ) ;
    } else {
      if ( _size >= _allocated ) this->realloc() ;
      _blocks[ _size++ ] = p ;
    }
  }

protected:

private:

  std::size_t _block_size ;
  std::size_t _allocated ;
  std::size_t _size ;
  void** _blocks ;

  void realloc ( ) ;

  /// Assigment operator, avoid default implementation
  MemPool& operator=(const MemPool&);

  /// Copy constructor, avoid default implementation
  MemPool(const MemPool&);

};

#endif // MEMPOOL_HH

// Local Variables: ***
// mode: c++ ***
// End: ***

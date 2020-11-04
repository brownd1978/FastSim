#ifndef MEMUNSAFEPOOL_HH
#define MEMUNSAFEPOOL_HH

/**
 * @file MemUtil/MemUnsafePool.h
 * 
 * Declaration of class MemUnsafePool and related functions
 *
 * @author Andrei A. Salnikov
 * @date   May 24, 2001
 *
 * Copyright (C) BINP, 2001 
 * 
 * $Id: MemUnsafePool.hh 517 2010-01-15 08:24:18Z stroili $
 */


#include <cstdlib>

/**
 *  This class is a memory pool of the blocks of the same size.
 *  It is unsafe in the sense that it assumes that all requests are 
 *  correct size (it does not actually care about correct sie, calling
 *  code will do)
 */
class MemUnsafePool {

public:

  /// Constructor
  MemUnsafePool();

  /// Destructor 
  ~MemUnsafePool();

  /// check emtpyness
  bool empty() const {
    return _size == 0 ;
  }

  /// get one block unchecked
  void* unchecked_get() {
    return _blocks[--_size] ;
  }

  /// get one block. 
  void* get() {
    return empty() ? 0 : unchecked_get() ;
  }

  /// take one block
  void take ( void* p ) {
    if ( _size >= _allocated ) {
      this->realloc() ;
    }
    _blocks[ _size++ ] = p ;
  }

  /// get the number of block
  std::size_t size() const {
    return _size ;
  }

protected:

private:

  std::size_t _allocated ;
  std::size_t _size ;
  void** _blocks ;

  void realloc ( ) ;

  /// Assigment operator, avoid default implementation
  MemUnsafePool& operator=(const MemUnsafePool&);

  /// Copy constructor, avoid default implementation
  MemUnsafePool(const MemUnsafePool&);

};

#endif // MEMUNSAFEPOOL_HH

// Local Variables: ***
// mode: c++ ***
// End: ***

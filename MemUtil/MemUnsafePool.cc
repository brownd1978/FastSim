/**
 * @file MemUtil/MemUnsafePool.cc
 *
 * Implementation of class MemUnsafePool.
 * 
 * @see MemUtil/MemUnsafePool.h
 * @author Andrei A. Salnikov
 * @date   May 24, 2001
 * Copyright (C) BINP, 2001 
 *
 * $Id: MemUnsafePool.cc 517 2010-01-15 08:24:18Z stroili $
 */

// ----- includes ------

// Experiment.h must be in any implementation file
#include "BaBar/BaBar.hh"

// standard C headers
#include <cstring>

// standard C++ headers

// this class header
#include "MemUtil/MemUnsafePool.hh"

// other packages headers
// #include "LogUtils/LogStream.h"

// ----- methods definition ------

// default constructor
MemUnsafePool::MemUnsafePool()
  : _allocated(0)
  , _size(0)
  , _blocks(0)
{
}

// destructor
MemUnsafePool::~MemUnsafePool()
{
  for ( std::size_t i = 0 ; i < _size ; ++ i ) {
    ::operator delete ( _blocks[i] ) ;
  }
  delete [] _blocks ;
}

void 
MemUnsafePool::realloc ( ) 
{
  std::size_t new_size = _size ? _size*2 : 32 ;
  void** blocks = new void*[ new_size ] ;
  std::memcpy ( blocks, _blocks, _size*sizeof(void*) ) ;
  delete [] _blocks ;
  _blocks = blocks ;
  _allocated = new_size ;
}

// Local Variables: ***
// mode: c++ ***
// End: ***

#ifndef MEMREFCOUNTED_HH
#define MEMREFCOUNTED_HH

/**
 * @file SmrPtr/MemRefCounted.h
 * 
 * Declaration of class MemRefCounted and related functions
 *
 * @author Andrei A. Salnikov
 * @date   Jun 13, 2001
 *
 * Copyright (C) BINP, 2001 
 * 
 * $Id: MemRefCounted.hh 517 2010-01-15 08:24:18Z stroili $
 */

// C++ includes
#include <stdlib.h>

/**
 *  This is a base class for the intrusively reference counted classes.
 *  Subclasses to be counted should inherit from this class and grant 
 *  friendship to the class SmrRCIPtr<T>.
 */
class MemRefCounted {

protected:

  /// Constructor
  MemRefCounted() : counter_ ( 0 ) {}

  /// Destructor 
  ~MemRefCounted() {}

  /// increment
  size_t mem_rc_increment() { return ++ counter_ ; }

  /// decrement
  size_t mem_rc_decrement() { return -- counter_ ; }

  /// get counter
  size_t mem_rc_count() const { return counter_ ; }

protected:

private:

  size_t counter_ ;

  /// Assigment operator, avoid default implementation
  MemRefCounted& operator=(const MemRefCounted&);

  /// Copy constructor, avoid default implementation
  MemRefCounted(const MemRefCounted&);

};

#endif // MEMREFCOUNTED_HH

// Local Variables: ***
// mode: c++ ***
// End: ***

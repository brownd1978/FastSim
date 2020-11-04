// -*- C++ -*-
// CLASSDOC OFF
// $Id: AIteratorBase.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// This file is a part of what might beome the CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the HepAListIteratorBase class. It is the base 
// class used by the template classes HepAListIterator and 
// HepConstAListIterator 
//
// .SS See Also
//
// AList.h, AIterator.h, ConstAList.h, ConstAIterator.h, AListBase.h
//
// .SS History
// HepConstAListIterator was developed from an original (non-template) list
// iterator class written by Dag Bruck.
//
// Author: Leif Lonnblad
//

#ifndef _AITERATORBASE_H_
#define _AITERATORBASE_H_

#ifdef GNUPRAGMA
#pragma interface
#endif

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Alist/AListBase.h"

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define inline
#endif

class HepAListIteratorBase {

protected:

  inline HepAListIteratorBase(const HepAListBase &);
  // Constructor taking a corresponding list as argument. Starting at the 
  // first element.

  inline HepAListIteratorBase(const HepAListIteratorBase &);
  // Copy constructor.

  inline ~HepAListIteratorBase();

  inline void * next();
  // Returns a pointer to the current object in the associated list, moving
  // forward to the next. Returns 0 if all objects are done.

  inline void * prev();
  // Moves backward one step in the list and returns the object found there.
  // If current object is the first in the list, no stepping is done and 0 is
  // returned.

  inline void * current() const;
  // Returns a pointer to the current object in the associated list,
  // without incrementing the index.

public:

  inline int index() const;

  inline void rewind();
  // Rewinds the iterator to the first element of the list.

  void skip(int);
  // Move iterator forward or backward.

  inline void skipAll();
  // Move iterator past last element.

private:

  const HepAListBase * l;
  // Pointer to the associated list.

  int i;
  // Current position in the associated list.

};

#ifdef HEP_NO_INLINE_IN_DECLARATION
#undef inline
#endif

#ifndef HEP_DEBUG_INLINE
#include "CLHEP/Alist/AIteratorBase.icc"
#endif

#endif

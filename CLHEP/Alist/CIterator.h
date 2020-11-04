// -*- C++ -*-
// CLASSDOC OFF
// $Id: CIterator.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// This file is a part of what might beome the CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the HepCListIterator class. It is used
// in the same way as the HepConstAListIterator class but can also be used
// with HepCList.
//
// .SS
// AList.h, CList.h, ConstAList.h, AIterator.h, ConstAIterator.h,
// AListBase.h, AIteratorBase.h
//
// Author: Leif Lonnblad
//

#ifndef _CITERATOR_H_
#define _CITERATOR_H_

#include "CLHEP/config/CLHEP.h"

#include "CLHEP/Alist/AIteratorBase.h"
#include "CLHEP/Alist/CList.h"

#ifdef HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#define inline
#endif

template <class T>
class HepCListIterator : public HepAListIteratorBase {

public:

  inline HepCListIterator(const HepCList<T> &);
  inline HepCListIterator(const HepAList<T> &);
  // Constructor taking a corresponding list as argument. Starting at the 
  // first element.

  inline HepCListIterator(const HepCListIterator<T> &);
  // Copy constructor.

  inline ~HepCListIterator();

  inline T * operator () ();
  inline T * next();
  // Returns a pointer to the current object in the associated list, moving
  // forward to the next. Returns 0 if all objects are done.

  inline T * prev();
  // Moves backward one step in the list and returns the object found there.
  // If current object is the first in the list, no stepping is done and 0 is
  // returned.

  inline T * current() const;
  // Returns a pointer to the current object in the associated list,
  // without incrementing the index.
  
};

#ifdef HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#undef inline
#endif

#ifdef HEP_SHORT_NAMES
#define CIterator HepCListIterator
#endif

#include "CLHEP/Alist/CIterator.icc"

#endif

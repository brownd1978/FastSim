// -*- C++ -*-
// CLASSDOC OFF
// $Id: AIterator.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// This file is a part of what might beome the CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the HepAListIterator class.
//
// HepAListIterator is a template based list iterator class to be used with
// the HepAList list class. An iterator will sequentially go through each 
// element of an associated list, starting with the first element. It is
// also possible to explicitly rewind the iterator, or to move forward or
// backward in the list.
//
// .SS Usage
// An iterator of a list of eg. integers  defined as:
//
// .ft B
//     HepAList<int> intlist;
//
// should be declared as:
//
// .ft B
//     HepAListIterator<int> it(intlist);
//
// and used like this:
//
// .ft B
//     int * p;
// .fi
//     while ( p = it() ) {
// .fi
//       /* something */
// .fi
//     }
//
//
// .SS Caveat
//
// Do not insert or remove elements in a list that is being iterated. This
// result in elements being skipped or iterated several times.
//
// .SS See Also
//
// AList.h, ConstAList.h, ConstAIterator.h, AListBase.h, AIteratorBase.h
//
// .SS History
// HepAListIterator was developed from an original (non-template) list iterator
// class written by Dag Bruck.
//
// Author: Leif Lonnblad
//

#ifndef _AITERATOR_H_
#define _AITERATOR_H_

#include "CLHEP/config/CLHEP.h"

#include "CLHEP/Alist/AIteratorBase.h"
#include "CLHEP/Alist/AList.h"

#ifdef HEP_NO_INLINE_IN_TEMPLATE_DECLARATION
#define inline
#endif

template <class T>
class HepAListIterator : public HepAListIteratorBase {

public:

  inline HepAListIterator(const HepAList<T> &);
  // Constructor taking a corresponding list as argument. Starting at the 
  // first element.

  inline HepAListIterator(const HepAListIterator<T> &);
  // Copy constructor.

  inline ~HepAListIterator();

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
#define AIterator HepAListIterator
#endif

#include "CLHEP/Alist/AIterator.icc"

#endif

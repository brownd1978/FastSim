// -*- C++ -*-
// CLASSDOC OFF
// $Id: AListBase.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// This file is a part of what might become CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the definition of the HepAListBase class which is the base class
// used in the template classes HepAList and HepConstAList.
//
// .SS See Also
//
// AList.h, AIterator.h, ConstAList.h, ConstAIterator.h, AIteratorBase.h
//
// .SS History
// HepAListBase was developed from an original (non-template) list class
// written by Dag Bruck.
//
// Author: Leif Lonnblad
//

#ifndef _ALISTBASE_H_
#define _ALISTBASE_H_

#ifdef GNUPRAGMA
#pragma interface
#endif

#include "CLHEP/config/CLHEP.h"

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define inline
#endif

#ifdef HEP_QSORT_NEEDS_FUNCTION_WITH_EXTERN_C_LINKAGE
extern "C" {
#endif
typedef int compare_function(const void*, const void*);

#ifdef HEP_QSORT_NEEDS_FUNCTION_WITH_EXTERN_C_LINKAGE
}
#endif

class  HepAListBase {

friend class HepAListIteratorBase;

protected:

  inline HepAListBase();
  // Constructs a list with no objects.

  HepAListBase(const HepAListBase &);
  // Copy constructor

  inline ~HepAListBase();
  // Destroys the list. The objects in the list are not destroyed.
  // Use HepAListDeleteAll(HepAList<T> &) to destroy all objects in the list.

  inline void insert(void *);
  // Inserts an object first in the list.

  inline void insert(void * e, void * r);
  // Inserts the object e just before the first occurence of
  // object r in the list. 

  inline void insert(void * e, unsigned pos);
  // Inserts the object e at the position pos in the list. If pos is outside
  // the list, the object will be appended.

  inline void append(void *);
  // Appends an object in the end of the list

  void append(void *, void *);
  // Appends the object e just after the last occurrence of object r
  // in the list

  void append(const HepAListBase &);
  // Appends all objects of list l to the end of this list.

  void remove(const void *);
  // Remove all occurencies of the object from the list.

  void remove(const HepAListBase & l);
  // Remove all occurencies of the objects in list l from this list.

  inline bool hasMember(const void *) const;
  // Returns true if the object is a member of the list.

  void replace(void *, void *);
  // Replace all occurencies of object eo with object en.

  inline void * operator[] (unsigned) const;
  // return a pointer to the object in position i (the first element has i=0).

  int index(const void *) const;
  // Returns the index of the last occurrence of the object.
  // NOTE! the objects are numbered 0 to n-1.

  int fIndex(const void *) const;
  // Returns the index of the first occurence of the object.
  // NOTE! the objects are numbered 0 to n-1.

  inline void * first() const;
  inline void * last() const;
  // Returns a pointer to the first and last object in the list.

  void operator = (const HepAListBase &);
  // Assignment.
  
public:

  inline void remove(unsigned);
  // Remove an object from the list.

  inline void removeAll();
  // Remove all objects from the list.

  void purge();
  // Remove all duplicate objects in the list.

  void reverse();
  // Reverse the order in the list.

  void swap(unsigned i1, unsigned i2);
  // Swap the position of objects number i1 and i2.

  inline int length() const;
  // Returns the number of objects in the list

  inline bool empty() const;
  inline bool isEmpty() const;
  // Returns true if the list is empty.

  void allocate(int buffersize);
  // preallocate array of pointers
  void realloc();
  // make size of the array of pointers to be equal to number of elements

  inline void sort(compare_function*);
  // sort the list. compare_function* is a pointer to a function
  // used by standard C qsort();
  
protected:

  void ** p;
  // Array of pointers to actual member objects.

  int n;
  // Number of objects in the list.

  int s;
  // Size of array

  void copyArray( void ** dst,  void ** src,  int n);
  // Internal function for fast copying of arrays.

  void checksize();
  // Allocate more space if not enough

  void * & newInsert( int);
  // Allocate space for a new object before entry number ir

  void * & newAppend( int);
  // Allocate space for a new object after entry number ir

  void removeEntry(int);
  // Remove one entry in the list.

};

#ifdef HEP_NO_INLINE_IN_DECLARATION
#undef inline
#endif

#ifndef HEP_DEBUG_INLINE
#include "CLHEP/Alist/AListBase.icc"
#endif

#endif


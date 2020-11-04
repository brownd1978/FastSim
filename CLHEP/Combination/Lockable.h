// -*- C++ -*- 
// CLASSDOC OFF
// $Id: Lockable.h 627 2010-09-16 12:58:44Z stroili $
// CLASSDOC ON
//
// Lockable.h - handle locks for combinatorics
//
// Copyright (C)  1993  The Board of Trustees of The Leland Stanford
//                      Junior University.  All Rights Reserved.
//
// Author: Paul Rensing (rensing@slac.stanford.edu)
//
// Usage:
//  HepLockable is intended to be a (multiply inherited) base class for objects
//  which will be handled by a combinatoric engine. It maintains an integer,
//  which is used bit-by-bit for locks. When asked to lock itself, HepLockable
//  sets the appropriate bit and then asks all its children to lock
//  themselves with the same bit. When asked to unlock, it does a similar
//  process. A HepLockable is locked if any of its bits are set, or if any of
//  its children are locked. 
//
//  HepLockable also has static methods to allocate and de-allocate bits for
//  use by HepLock objects. In this way, Locks can be turned on and off
//  independent of each other.
//
//  Anyone using HepLockable should remember to implement the methods:
//     virtual HepLockable *lockableChild(int i);
//     virtual int nLockChildren() const;
//  if they are appropriate. If there is multiple inheritance in the
//  inheritance hierarchy, be certain to return correct pointer.

#ifndef _LOCKABLE_H_
#define _LOCKABLE_H_

#ifdef GNUPRAGMA
#pragma interface
#endif

#include "CLHEP/config/CLHEP.h"

typedef void* ClassId;

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define inline
#endif

class HepLockable {

public:

  inline HepLockable();
  // Default constructor;
  virtual ~HepLockable() {;}

  inline virtual int isA(ClassId id);
  static ClassId ClassID();
  // These methods must be re-implemented by sub-classes to allow for
  // dynamic cast (will become redundant with new ANSI/ISO standard)

  inline void lock(int bit);
  inline void unlock(int bit);

  inline int locked() const;

  static int allocBit();
  inline static void freeBit(int bit);

protected:

  virtual HepLockable *lockableChild(int) const { return 0; }
  virtual int nLockChildren() const { return 0; }

  void lockChildren(int bit);
  void unlockChildren(int bit);
  int areChildrenLocked() const;

private:

  unsigned long _lockBits;
  static unsigned long _lockAlloc;

};

#ifdef HEP_NO_INLINE_IN_DECLARATION
#undef inline
#endif

#ifdef HEP_SHORT_NAMES
typedef HepLockable Lockable;
#endif

#ifndef HEP_DEBUG_INLINE
#include "CLHEP/Combination/Lockable.icc"
#endif

#endif

// -*- C++ -*-
// CLASSDOC OFF
// $Id: Lock.h 478 2010-01-22 08:54:39Z stroili $
// CLASSDOC ON
//
// Lock.h - maintain a list of objects to be locked/unlocked as a group
//
// Copyright (C)  1993  The Board of Trustees of The Leland Stanford
//                      Junior University.  All Rights Reserved.
//
// Author: Paul Rensing (rensing@slac.stanford.edu)
//
// Usage:
//  HepLock maintains a list of objects, and on demand, locks and unlocks them.
//  The locks are turned on/off immediately. When an object is added, it is
//  immediately locked if the lock is on. When an object is removed, it is
//  first unlocked (if needed). When the HepLock object is deleted (or goes out
//  of scope), its members are first unlocked (if needed).
//

#ifndef _LOCK_H_
#define _LOCK_H_

#ifdef GNUPRAGMA
#pragma interface
#endif

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Combination/Lockable.h"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define inline
#endif

class HepLock {

public:
    
  inline HepLock();
  inline HepLock(HepLockable *p);
  inline HepLock(HepLockable &p);
    
  inline ~HepLock();
     	 
  void add(HepLockable *p);
  inline void add(HepLockable &p);

  void remove(HepLockable *p);
  inline void remove(HepLockable &p);
 
  void lock();
  void unlock();

protected:

  HepAList<HepLockable> _members;
  int _lockBit;

};

#ifdef HEP_NO_INLINE_IN_DECLARATION
#undef inline
#endif

template <class T>
void HepLockAddList(HepLock &lck, const HepAList<T> &lst) {
  HepAListIterator<T> i(lst);
  T * p;
  while ( p = i() ) lck.add(p);
}

template <class T>
void HepLockRemoveList(HepLock &lck, const HepAList<T> &lst) {
  HepAListIterator<T> i(lst);
  T* p;
  while ( p = i() ) lck.remove(p);
}

#ifdef HEP_SHORT_NAMES
typedef HepLock Lock;
#define LockRemoveList HepLockRemoveList 
#define LockAddList HepLockAddList 
#endif

#ifndef HEP_DEBUG_INLINE
#include "CLHEP/Combination/Lock.icc"
#endif

#endif

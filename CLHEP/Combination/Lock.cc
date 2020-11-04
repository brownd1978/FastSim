// -*- C++ -*-
// $Id: Lock.cc 192 2009-03-04 12:20:53Z stroili $
//
// Lock.cc -- classes dealing with combinatorics
//

#ifdef GNUPRAGMA
#pragma implementation
#endif

#include "CLHEP/Combination/Lock.h"
#include "CLHEP/Alist/AIterator.h"

void HepLock::add(HepLockable *p) {
  _members.append(p);
  if (_lockBit >= 0) p->lock(_lockBit);
}

void HepLock::remove(HepLockable *p) {
  if (_lockBit >= 0) p->unlock(_lockBit);
  _members.remove(p);
}

void HepLock::lock() {
  if (_lockBit >= 0) return;
  _lockBit = HepLockable::allocBit();

  HepAListIterator<HepLockable> i(_members);
  HepLockable *p;
  while (p=i()) p->lock(_lockBit);
}

void HepLock::unlock() {
  if (_lockBit < 0) return;
  HepAListIterator<HepLockable> i(_members);
  HepLockable *p;
  while (p=i()) p->unlock(_lockBit);

  HepLockable::freeBit(_lockBit);
  _lockBit = -1;
}


	  


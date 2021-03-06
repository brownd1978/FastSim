// -*- C++ -*- 
// $Id: Lockable.cc 600 2010-06-21 07:46:30Z stroili $
//
// Lockable.cc -- classes dealing with combinatorics
//

#ifdef GNUPRAGMA
#pragma implementation
#endif

#include <iostream>
#include "CLHEP/Combination/Lockable.h"

ClassId HepLockable::ClassID() {
  static int x; return &x;
}

unsigned long HepLockable::_lockAlloc = 0;

int HepLockable::allocBit() {

  // bit #0 is reserved for Choosers
  for (int i=1; i<sizeof(_lockAlloc)*8; i++) {
    if (! (_lockAlloc & (1<<i)) ) {
      _lockAlloc |= (1<<i);
      return i;
    }
  }
  // error: ran out of bits
  std::cerr << "Lockable : ran out of user bits" << std::endl;
  std::cerr << "   You may have too many Locks turned on." << std::endl;
  exit(1);
  return 0;			// get rid of warning
}

int HepLockable::areChildrenLocked() const {
  int i;
  int n = nLockChildren();
  for (i=0; i<n; i++) {
    if (lockableChild(i)->locked()) return 1;
  }
  return 0;
}

void HepLockable::lockChildren(int bit) {
  int i;
  int n = nLockChildren();
  for (i=0; i<n; i++) lockableChild(i)->lock(bit);
}

void HepLockable::unlockChildren(int bit) {
  int i;
  int n = nLockChildren();
  for (i=0; i<n; i++) lockableChild(i)->unlock(bit);
}


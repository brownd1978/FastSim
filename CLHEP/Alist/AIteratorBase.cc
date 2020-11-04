// -*- C++ -*-
// $Id: AIteratorBase.cc 192 2009-03-04 12:20:53Z stroili $
//
// This file is a part of what might become the CLHEP -
// a Class Library for High Energy Physics.
// 
// This is the implementation of the HepAListIteratorBase class.
//

#ifdef GNUPRAGMA
#pragma implementation
#endif

#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Alist/AIteratorBase.h"

#ifdef HEP_DEBUG_INLINE
#include "CLHEP/Alist/AIteratorBase.icc"
#endif

void HepAListIteratorBase::skip(int si) {
  if ( si + int(i) < 0 )
    rewind();
  else
    i += si;
}


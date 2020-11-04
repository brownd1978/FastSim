//--------------------------------------------------------------------------
//
// Description:
//	Class BbrCounter
//
//  This class is a small mixin for keeping track of how many of
//  a particular subclass exists.  Just publically inherit from it,
//  then use the static nAlloc() function to ask how many exist.
//
//  And don't forget to define the counter in the using .cc file
//  with a line like:
//    int BbrCounter<MyClass>::_nAlloc = 0;
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		        Original Author
//
// Copyright Information:
//	Copyright (C) 1998		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef BbrCounter_hh
#define BbrCounter_hh


//		---------------------
// 		-- Class Interface --
//		---------------------

template <class T>
class BbrCounter {

//--------------------
// Instance Members --
//--------------------

public:
  static int nAlloc() { return _nAlloc; }
  BbrCounter() { _nAlloc++; }
  ~BbrCounter() {--_nAlloc;}

private:
  static int _nAlloc;
};

#ifdef BABAR_COMP_INST
#include "CommonUtils/BbrCounter.icc"
#endif // BABAR_COMP_INST

#endif





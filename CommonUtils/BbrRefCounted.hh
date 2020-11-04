//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BbrRefCounted.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class BbrRefCounted
//	
//      Reference counting
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Gautier Hamel de Monchenault,
//         from Bob Jacobsen, original author of BtaCandidate & BtaCandBase,
//         and discussions with Steve Schaffner, Massimo Marino,
//                              Yury Kolomensky, Ed Franck
//
// Copyright Information:
//	Copyright (C) 1998		CE Saclay
//
//------------------------------------------------------------------------

#ifndef BbrRefCounted_hh
#define BbrRefCounted_hh

#include <assert.h>

//		---------------------
// 		-- Class Interface --
//		---------------------

template <class T>
class BbrRefCounted
{
  //--------------------
  // Instance Members --
  //--------------------

  // number of Referencers
  int _nLinks; 

protected:
  //
  // for safety, subclass have to grant friendship to their referencers
  //
  
  //
  // Constructor
  // 
  BbrRefCounted() : _nLinks(0) {}

  // 
  // Destructor
  //
  virtual ~BbrRefCounted() {}

  //
  // Protected modifiers
  //
  int addLink()   { return ++_nLinks; }
  int dropLink()  { return --_nLinks; }

  //
  // Protected access
  //
  int nLinks() const { return _nLinks; }

public:
  //
  // Clone - forces sublcasses to define a clone function
  //
  virtual T* clone() const =0;

};

#endif





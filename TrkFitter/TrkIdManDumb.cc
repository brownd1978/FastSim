//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkIdManDumb.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkIdManDumb.hh"

//------------------------------------------------------------------------
TrkIdManDumb::TrkIdManDumb(long val) {
//------------------------------------------------------------------------
  _theValue = val;
}

//------------------------------------------------------------------------
TrkIdManDumb::~TrkIdManDumb() {}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
long
TrkIdManDumb::nextId() {
//------------------------------------------------------------------------
  return theValue();
}

//------------------------------------------------------------------------
long
TrkIdManDumb::lastId() const {
//------------------------------------------------------------------------
  return theValue();
}

//------------------------------------------------------------------------
TrkIdManDumb*
TrkIdManDumb::instance() {
//------------------------------------------------------------------------
  static TrkIdManDumb theSingleton(-1);
  return &theSingleton;
}

void
TrkIdManDumb::setMax(long maxid) {
  _theValue = maxid;
}

//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkIdManDefault.cc 104 2010-01-15 12:13:14Z stroili $
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
#include "TrkFitter/TrkIdManDefault.hh"

//------------------------------------------------------------------------
TrkIdManDefault::TrkIdManDefault(long firstId) {
//------------------------------------------------------------------------
  _nextId = firstId;
}

//------------------------------------------------------------------------
TrkIdManDefault::~TrkIdManDefault() {
//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
long
TrkIdManDefault::nextId() {
//------------------------------------------------------------------------
  long id = _nextId;
  _nextId++;
  return id;
}

//------------------------------------------------------------------------
long
TrkIdManDefault::lastId() const {
//------------------------------------------------------------------------
  return (_nextId - 1);
}

void
TrkIdManDefault::setMax(long maxid) {
  _nextId = maxid+1;
}


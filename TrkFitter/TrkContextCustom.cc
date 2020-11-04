//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkContextCustom.cc 104 2010-01-15 12:13:14Z stroili $
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
#include "TrkFitter/TrkContextCustom.hh"
#include "TrkBase/TrkRecoTrk.hh"

//------------------------------------------------------------------------
TrkContextCustom::TrkContextCustom(const BField* bf, const TrkId& idIn) : 
  TrkContext(bf), _id(idIn) {
//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
TrkContextCustom::TrkContextCustom(const TrkRecoTrk& trk) : 
  TrkContext(&trk.bField()), _id(trk.id().idManager()) {
//------------------------------------------------------------------------
}


//------------------------------------------------------------------------
TrkContextCustom::~TrkContextCustom() {
//------------------------------------------------------------------------
}


//------------------------------------------------------------------------
TrkId 
TrkContextCustom::getId() const {
//------------------------------------------------------------------------
  return _id;
}

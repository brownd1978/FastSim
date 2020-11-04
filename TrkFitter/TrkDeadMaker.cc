//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkDeadMaker.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Authors: Steve Schaffner
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkDeadMaker.hh"
#include "TrkFitter/TrkDeadRep.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include <assert.h>

//------------------------------------------------------------------------
TrkDeadMaker::~TrkDeadMaker() {}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
TrkDeadMaker::TrkDeadMaker() {}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void
TrkDeadMaker::addHypo(TrkRecoTrk* trk, PdtPid::PidType hypo) {
//------------------------------------------------------------------------
  assert (trk != 0);


  TrkDeadRep* newRep = new TrkDeadRep(trk, hypo);

  // Install via TrkFitMaker fcn
  addHypoTo(*trk, newRep, hypo);  
}

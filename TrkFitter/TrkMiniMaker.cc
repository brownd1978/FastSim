//--------------------------------------------------------------------------
// File and Version Information:
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
#include "TrkFitter/TrkMiniMaker.hh"
#include "TrkFitter/TrkHelixRep.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkExchangePar.hh"
#include <assert.h>
#include "ErrLogger/ErrLog.hh"

//------------------------------------------------------------------------
TrkMiniMaker::~TrkMiniMaker() {}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
TrkMiniMaker::TrkMiniMaker() {}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
TrkRecoTrk* 
TrkMiniMaker::makeTrack(const TrkExchangePar& helix, 
			int nact, int nsv, int ndc, double chi2, 
			double startFoundRng, double endFoundRng, 
			PdtPid::PidType def, const TrkContext& ctxt, 
			double trackT0) {
//------------------------------------------------------------------------
  TrkRecoTrk* newTrack = createTrack(def, ctxt, trackT0);
  TrkHelixRep* newRep = new 
    TrkHelixRep(helix, newTrack, def, nact, nsv, ndc, chi2, startFoundRng, 
		endFoundRng);
  newRep->setValid(true);
  newRep->setCurrent(true);
  newRep->setMultScat(true);

  // Gives ownership of newRep to newTrack
  setRep(*newTrack, newRep);

  return newTrack;
}

//------------------------------------------------------------------------
void
TrkMiniMaker::addHypo(TrkRecoTrk& trk, const TrkExchangePar& helix, int nact, 
	  int nsv, int ndc, double chi2, double stRng, double endRng, 
	  PdtPid::PidType hypo) {
//------------------------------------------------------------------------
  if(hypo>PdtPid::proton ||hypo<PdtPid::null ){
    ErrMsg(error) 
      << "TrkMiniMaker::addHypo -- crasy hypothesis !!" << hypo 
      << endmsg;
    return;
  }
  if ( trk.whichFit(hypo) == hypo) {
    ErrMsg(error) 
      << "TrkMiniMaker::addHypo -- fit already exists for this hypothesis" 
      << endmsg;
    return;
  }
  TrkHelixRep* newRep = new 
    TrkHelixRep(helix, &trk, hypo, nact, nsv, ndc, chi2, stRng, endRng);
  newRep->setValid(true);
  newRep->setCurrent(true);
  newRep->setMultScat(true);

  // Install via TrkFitMaker fcn
  addHypoTo(trk, newRep, hypo);  
}

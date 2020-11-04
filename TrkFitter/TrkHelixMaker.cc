//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkHelixMaker.cc 104 2010-01-15 12:13:14Z stroili $
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
#include "TrkFitter/TrkHelixMaker.hh"
#include "TrkFitter/TrkHelixRep.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "ErrLogger/ErrLog.hh"
#include "TrkBase/TrkHitOnTrk.hh"

//------------------------------------------------------------------------
TrkHelixMaker::~TrkHelixMaker() {}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
TrkHelixMaker::TrkHelixMaker() {}
//------------------------------------------------------------------------


void
TrkHelixMaker::addZValues(TrkRecoTrk& theTrack, double z0,
                          double tanDip, double chi2)
{
  const TrkFit* theFit = theTrack.fitResult();
  if (theFit == 0) {
    ErrMsg(error)
      << "TrkSimpleMaker::addZValues() cannot get helix from invalid fit."
      << endmsg;
    return;
  }

  TrkExchangePar oldPar = theFit->helix(0.0);
  oldPar.setZ0(z0);
  oldPar.setTanDip(tanDip);

  PdtPid::PidType hypo = theTrack.defaultType();
  TrkHotList* hl = getRep(theTrack,hypo)->hotList();
  TrkHelixRep* newRep = new TrkHelixRep(oldPar, &theTrack, hypo, *hl, true); // FIXME: stealing hots?
  newRep->fitter().setFittingPar(false, false);
  newRep->addHistory(theTrack.status()->history());
  setRep(theTrack, newRep);   // theTrack will delete all old Reps
  newRep->setChisq(chi2);
  if (chi2 > 0.0) newRep->setValid(true);
  newRep->setCurrent(false);
}

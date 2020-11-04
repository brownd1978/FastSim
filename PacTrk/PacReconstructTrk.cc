//--------------------------------------------------------------------------
// Name:
//   PacReconstructTrk
//   through the detector.
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "PacTrk/PacReconstructTrk.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacTrk/PacTrkHitMeas.hh"
#include "PacTrk/PacTrkHitViewSi.hh"
#include "PacTrk/PacTrkHitViewDch.hh"
#include "PacTrk/PacTrkdEdxMeas.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacGeom/PacDetElem.hh"
#include "PacGeom/PacMeasurement.hh"
#include "PacGeom/PacPieceTraj.hh"
#include "PacGeom/PacHelix.hh"
#include "PacGeom/PacDetElem.hh"
#include "PacEnv/PacConfig.hh"
#include "PacMC/PmcMaps.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkHotListFull.hh"
#include "TrkBase/TrkEnums.hh"
#include "TrkBase/HelixTraj.hh"
#include "TrkBase/TrkDifPieceTraj.hh"
#include "TrkFitter/TrkDeadRep.hh"
#include "KalmanTrack/KalRep.hh"
#include "KalmanTrack/KalInterface.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/Pdt.hh"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "DetectorModel/DetMaterial.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include <assert.h>
#include <algorithm>

PacReconstructTrk::PacReconstructTrk(const BField* field, const KalContext* kalcon) : 
_bfield(field), _kalcon(kalcon),_idnum(0),_fastfit(true)
{
// configure fit type (full/inwards only)
  _fastfit = gconfig.getbool("TrackReconstructor.fastfit", false);
  _minsim = gconfig.getint("TrackReconstructor.minsimhit", 4);
  _minsvt = gconfig.getint("TrackReconstructor.minsvthit", 8);
  _mindch = gconfig.getint("TrackReconstructor.mindchhit", 15);
  _hitchicut = gconfig.getfloat("TrackReconstructor.hitchicut", 5.);
  _trkchicut = gconfig.getfloat("TrackReconstructor.trkchicut", 5.);
  _weedhits = gconfig.getbool("TrackReconstructor.weedhits", true);
  _dopatrec = gconfig.getbool("TrackReconstructor.dopatrec", false);
  _domerge = gconfig.getbool("TrackReconstructor.domerge", true);
  _maxweed = gconfig.getint("TrackReconstructor.maxweed", 5);
  _minpt = gconfig.getfloat("TrackReconstructor.minpt", 0.04);
  _seedsmear = gconfig.getfloat("TrackReconstructor.seedsmear", 1.0);
  _maxgap = gconfig.getfloat("TrackReconstructor.maxgap", 0.98 );
  _debug = gconfig.getbool("TrackReconstructor.debug", false);
  _roughsize = gconfig.getfloat("TrackReconstructor.roughsize", 5.0);
  _nstripgap = gconfig.getfloat("TrackReconstructor.nstripgap", 2.0);
  // if a default pdg entry is specified, look it up
  int defpdgid = gconfig.getint("TrackReconstructor.default_pdgid", 0);
  if(defpdgid != 0){
    _defhypo = Pdt::lookup( (PdtPdg::PdgType)defpdgid);
    if(_defhypo == 0)ErrMsg(fatal) << "Can't find pdt entry for PdgId " << defpdgid << endmsg;
  } else {
    _defhypo = 0;
  }
// scattering truncation fraction
  double scatterfrac = gconfig.getfloat("TrackReconstructor.scatterfraction", 0.98 );
  DetMaterial::setScatterFraction(scatterfrac);
}

PacReconstructTrk::~PacReconstructTrk()
{
  clearMaps();
}

void
PacReconstructTrk::makeTracks(std::vector<PacSimTrack*>& simtrks,const SimTrkSel* sel) const {
  std::vector<const PacSimTrack*> csimtrks;
  csimtrks.reserve(simtrks.size());
  for(std::vector<PacSimTrack*>::const_iterator istrk=simtrks.begin();istrk!=simtrks.end();istrk++)
    csimtrks.push_back(*istrk);
  makeTracks(csimtrks,sel);
}

void
PacReconstructTrk::makeTracks(std::vector<const PacSimTrack*>& simtrks,const SimTrkSel* sel) const {
// clear maps from previous event
  clearMaps();
// loop over tracks and make hots
  for(std::vector<const PacSimTrack*>::const_iterator istrk=simtrks.begin();istrk!=simtrks.end();istrk++){
    makeHots(*istrk);
  }
// if either merging or patrec is requested, find the set of nearby hits
  std::vector<PacTrkHitProximity> closehits;
  if(_domerge || _dopatrec)
    findNearbyHits(closehits);
// if requested, merge the hots
  if(_domerge)doHitMerging(closehits);
// now fit the tracks and weed (if requested)
  std::vector<TrkRecoTrk*> trks;
  for(std::vector<const PacSimTrack*>::const_iterator istrk=simtrks.begin();istrk!=simtrks.end();istrk++){
// selection
    if(sel == 0 || sel->select(*istrk)){
      TrkRecoTrk* trk = makeTrack(*istrk);
      if(trk != 0 && trk->status() != 0 && trk->status()->fitCurrent())
        trks.push_back(trk);
    }
  }
// then simulate pat rec confusion
  if(_dopatrec){
    for(std::vector<TrkRecoTrk*>::iterator itrk=trks.begin();itrk!=trks.end();itrk++){
      doPatRec(*itrk,closehits);
    }
  }
}

TrkRecoTrk*
PacReconstructTrk::makeTrack(const PacSimTrack* simtrk) const {
// Find the true particle type if possible, otherwise use proton (the non-pid types are heavier)
  PdtPid::PidType ptype;
// if no explicit type is requested, use the true type in the fit
  bool reverse(false);
  if(_defhypo == 0){
    ptype = simtrk->pdt()->pidId();
    if(ptype == PdtPid::null) ptype = PdtPid::proton;
  } else {
    ptype = _defhypo->pidId();
// if this type's charge is opposite that of the true particle, we must reverse the hits to get the 'right' sign
    reverse = _defhypo->charge()*simtrk->pdt()->charge() < 0.0;
  }
// build a track
  TrkRecoTrk* retval = createTrack(ptype, const_cast<PacReconstructTrk*>(this)->nextId(), 0.0);
  assert(retval != 0);
  setBField(*retval,_bfield);
// regardless of the outcome, update the map for this simtrk
  _hotmap.addToMap(simtrk, retval);
  KalRep* kalrep(0);
// find the hots created for this track
  const std::vector<const PacHitOnTrk*>& hots = _hotmap.getHots(simtrk);
  if(goodHots(hots)){
// create seed and initial piecetraj
    HelixTraj* seed(0);
    TrkDifPieceTraj* ptraj(0);
    createTrajs(simtrk,hots,seed,ptraj);
// make sure these worked
    if(seed != 0 && ptraj != 0 ) {
      // extract the list of intersections, this speeds up the fit.
      std::vector<DetIntersection> dvect;
      double lasthot =  hots.back()->fltLen() + 0.5;
      extractIntersections(simtrk,ptraj,lasthot,dvect);      
// convert these to a TrkHotList, which TAKES OWNERSHIP of the hots
      TrkHotListFull* hotlist = new TrkHotListFull();
      assert(hotlist != 0);
      if(!reverse){
        for(std::vector<const PacHitOnTrk*>::const_iterator ihot = hots.begin();ihot != hots.end();ihot++){
          hotlist->append(const_cast<PacHitOnTrk*>(*ihot));
        }
      } else {
// reverse the order and the flightlength of these hots
        for(std::vector<const PacHitOnTrk*>::const_reverse_iterator ihot = hots.rbegin();ihot != hots.rend();ihot++){
          PacHitOnTrk* phot = const_cast<PacHitOnTrk*>(*ihot);
          phot->setFltLen(-phot->fltLen());
          hotlist->append(phot);
        }
// reverse the trajectories and intersections too
        seed->invert();
        ptraj->invert();
        std::vector<DetIntersection> other = dvect;
        dvect.clear();
        for(std::vector<DetIntersection>::reverse_iterator iinter = other.rbegin();iinter != other.rend();iinter++){
// reverse ranges
          iinter->pathlen *= -1;
          double prange[2] = { -iinter->pathrange[1], -iinter->pathrange[0] };
          iinter->pathrange[0] = prange[0];
          iinter->pathrange[1] = prange[1];
          dvect.push_back(*iinter);
        }
      }
// create the KalRep from these
      kalrep = new KalRep(seed, ptraj, hotlist, dvect, retval, *_kalcon, ptype);
      assert(kalrep != 0);
      setRep(*retval,kalrep);
// assume no fit
      TrkErrCode fitcode(TrkErrCode::fail);
// fast fitting (this should be a KalRep member function)
      if(_fastfit && !_weedhits)
        fitcode = kalrep->fit(trkIn);
      else
        fitcode = kalrep->fit();
      kalrep->addHistory(fitcode,"PacReconstructTrk");
//      kalrep->printAll();
      if(fitcode.failure() ) {
        if(_debug)std::cout << "fit not valid, " << fitcode << std::endl;
//        delete retval;
//        retval = 0;
      }
// hit weeding and pat. rec. simulation
    // We should now have a valid fit (if reval != 0).  Look for any hots that should
    // be swapped out for pattern recognition confusion.
    // Then, weed away hits with large chi2 contribution.  This is meant to simulate to some
    // extent what a pattern recognition program would do.  Must have a valid fit in
    // both directions to work.
      if (kalrep->fitCurrent() && kalrep->hasFit(trkIn) && kalrep->hasFit(trkOut)) {
        if(_weedhits)weedHots(retval);
        if ( (!kalrep->fitCurrent()) && _debug) cout << "Track killed in patrec/weeding." << endl;
      }
// final check for unphysical fits
      double fitmom = kalrep->momentum(0.0).mag();
      if(fitmom < 0.01){
        ErrMsg(error) << "unphysical fit" << endmsg;
        kalrep->setValid(false);
        kalrep->setCurrent(false);
      }
// Mark these Hots (and corresponding SimHits) as being used.  This also makes sure the
// ownership gets treated correctly
      markHotsUsed(hots);
      _hotmap.markSimTrkUsed(simtrk);
    }
  }
  if(0 == kalrep){
// create a 'dead' rep for this fit
    TrkDeadRep* deadr = new TrkDeadRep(retval,ptype);
    setRep(*retval,deadr); 
  }
  return retval;
}

void
PacReconstructTrk::makeHots(const PacSimTrack* simtrk) const {
  static const double _maxdmom(0.05); // stop track if its fractional momentum change in 1 step is larger than this
// Basic checks
  if(simtrk != 0  && simtrk->getTraj() != 0
    && simtrk->getHitList().size() > _minsim
    && simtrk->pdt()->charge() != 0
    && simtrk->particleMom().perp() > _minpt) {
// 
    const std::vector<PacSimHit>& simhits = simtrk->getHitList();
    std::vector<PacSimHit>::const_iterator ihit = simhits.begin();
    bool firsthot(false);
    bool stoptrack(false);
    while(ihit != simhits.end()){
      const PacDetElem* pelem = ihit->detElem();
// see if the track loses substiantial energy or hard-scstters (after the first measurement). if so, stop adding active hots
      if(firsthot){
        double dmomfrac = ihit->momentumChange().mag()/ihit->momentum().mag();
      // stop adding active hots material grossly changes the momentum.  We should just start another track in this case: FIXME!!!
        if(!stoptrack)stoptrack = dmomfrac > _maxdmom;
        if(_debug)cout << "Element " << pelem->surfElem()->elementName() << " Delta mom frac " << dmomfrac << " stops " << stoptrack << endl;
      }
// keep track of when we first enter and exit the tracking volume, and only keep the hits from the first pass through
// see if there's a measurement associated with this device
      if(pelem != 0 && pelem->measurementDevices().size() != 0 &&
        pelem->activeRegion(ihit->position())){
// commented out for now; FIXME!!  How could a simhit already be used???
//        (0 == ihit->mergeInfo() || ihit->mergeInfo()->mergeStatus() != PacSimHitMergeInfo::mergedAndDontUse) &&
//      (!_hotmap.simHitUsed(&(*ihit)))){
        for(std::vector<const PacMeasurement*>::const_iterator imdev = pelem->measurementDevices().begin();
        imdev != pelem->measurementDevices().end();imdev++){
          const PacMeasurement* meas = *imdev;
// position measurements (hits)
          if(meas->measurementType()==PacMeasurement::TrkHit) {
            const PacTrkHitMeas* hitmeas = dynamic_cast<const PacTrkHitMeas*>(meas);
// flag after creating the first HOT
            if(!firsthot)firsthot = true;
            std::vector<PacHitOnTrk*> hots;
            hitmeas->createHots(*ihit,hots,!stoptrack);
// add hots to map, which takes ownership
            for(std::vector<PacHitOnTrk*>::iterator ihot = hots.begin();ihot!= hots.end();ihot++){
// Add this sim-hot pair to the map.  NB: the following code assumes the simhit pointers never
// change, this WOULD BE VIOLATED if the SimTrk is modified.   FIXME!!!
              const PacSimHit* simPtr = &(*ihit);
              _hotmap.addToMap(simPtr, *ihot);
            }
// special map for pixels
            if (hitmeas->type() == PacTrkHitMeas::SiPixel) {
              assert(hots.size() == 2);
              _hotmap.addPixelPair(hots[0], hots[1]);
            }
// check for dedx measurement too
          } else if(meas->measurementType()==PacMeasurement::dEdx) {
            const PacTrkdEdxMeas* dedxmeas = dynamic_cast<const PacTrkdEdxMeas*>(meas);
            assert(dedxmeas != 0);
// find the hots associated with this simhit, and add dedx to them
// question: does this overwrite existing dedx data?  Is that the right thing to do?????
            std::vector<const PacHitOnTrk*> hots = _hotmap.getHots(&(*ihit));
            for(std::vector<const PacHitOnTrk*>::const_iterator ihot = hots.begin();ihot!=hots.end();ihot++){
              double dedx,ededx;
              dedxmeas->get_dedx(*ihit,dedx,ededx);
              PacHitOnTrk* pachot = const_cast<PacHitOnTrk*>(*ihot);
              pachot->setdEdx(dedx);
              pachot->setErrdEdx(ededx);
              pachot->setdEdxMeasure(dedxmeas);
            }
          }
        }
      }
      ihit++;
    }
  }
}

void
PacReconstructTrk::markHotsUsed(const std::vector<const PacHitOnTrk*>& hots) const {
  for (std::vector<const PacHitOnTrk*>::const_iterator ihot=hots.begin();ihot!=hots.end();ihot++){
//    if ((*ihot)->isActive()) {
    _hotmap.markHotUsed(*ihot);
//    }
  }
  return;
}

void
PacReconstructTrk::findNearbyHits(std::vector<PacTrkHitProximity>& closehits) const {
// loop over pairs of hots and test their proximity
  unsigned nhots = _hotmap.hots().size();
  for(unsigned ihot=0;ihot<nhots;ihot++){
    const PacHitOnTrk* hot1 = _hotmap.hots()[ihot];
    if(hot1->isActive()){
      for(unsigned jhot=ihot+1;jhot<nhots;jhot++){
        const PacHitOnTrk* hot2 = _hotmap.hots()[jhot];
        if(hot2->isActive()){
// see if these hots are mergable.  This checks that they are in the same element, etc.
          if(hot1->isMergable(hot2)){
// measure the proximity of these hots
            PacTrkHitProximity prox;
            if(hot1->proximity(hot2,prox)){
// save this association of hits, with their proximity
              closehits.push_back(prox);
            } 
          }
        }
      }
    }
  }
}

void
PacReconstructTrk::doHitMerging(const std::vector<PacTrkHitProximity>& closehits) const {
// loop over the close hits and see if we should merge them
  for(std::vector<PacTrkHitProximity>::const_iterator iclose=closehits.begin();iclose!=closehits.end();iclose++){
    bool merge(false);
// test for overlap.  This depends on measurement type, angle, position, ...
    if(iclose->_h1->measure()->type() == PacTrkHitMeas::SingleSiStrip || iclose->_h1->measure()->type() == PacTrkHitMeas::DoubleSiStrip){
      merge = mergeSiStrips(*iclose);
    } else if(iclose->_h1->measure()->type() == PacTrkHitMeas::SiPixel) {
      merge = mergeSiPixels(*iclose);
    } else if(iclose->_h1->measure()->type() == PacTrkHitMeas::Wire){
      merge = mergeWireHits(*iclose);
    }
  }
}

bool
PacReconstructTrk::mergeSiStrips(const PacTrkHitProximity& proximity) const {
  bool retval(false);
// check timing first
  if(!proximity._h1->measure()->timeResolved(proximity._h1->simHit()->time(),proximity._h2->simHit()->time())){
// find information about pitch, thickness, wafer size
    const PacTrkHitViewSi* siview = dynamic_cast<const PacTrkHitViewSi*>(proximity._h1->view());
    double ptRatio = siview->getPtoT();
    const DetType* detType = proximity._h1->simHit()->detIntersection().delem->detectorType();
    const DetSurfaceType* surfType = dynamic_cast<const DetSurfaceType*>(detType);
    assert(0 != surfType);
    TwoDCoord nullTwoD = TwoDCoord(0., 0.);
    double thickness = surfType->thickness(&nullTwoD);
    assert(0. < thickness);
    double pitch = ptRatio*thickness;

// For each hit, calculate angle w.r.t. normal in plane of measurement
// Should cache this stuff in the HOT, FIXME!!!

    Hep3Vector trkd1 = proximity._h1->simHit()->momentum().unit();
    Hep3Vector trkd2 = proximity._h2->simHit()->momentum().unit();

    Hep3Vector norm1 = proximity._h1->hitInfo()._wdir.cross(proximity._h1->hitInfo()._mdir);
    Hep3Vector norm2 = proximity._h2->hitInfo()._wdir.cross(proximity._h2->hitInfo()._mdir);

    double td1 = fabs(trkd1.dot(proximity._h1->hitInfo()._mdir)/trkd1.dot(norm1));
    double td2 = fabs(trkd2.dot(proximity._h2->hitInfo()._mdir)/trkd2.dot(norm2));

// Calculate effective cluster width based on track angle
// Set minimum value to 1. (In units of strips or pitch)

    double cWidth1 = std::max(td1/ptRatio, 1.);
    double cWidth2 = std::max(td2/ptRatio, 1.);

// Chi2 based on hit resolution for pat rec flagging.
//  chi2[iView] = dist[iView]*dist[iView]/(res1*res1 + res2*res2);
// average cluster width, including minimimum separation
    double width = 0.5*(cWidth1 + cWidth2);
// Gap between hits in units of cluster width
    double gap = fabs(proximity._msep)/(pitch*width);
// statistical test of separation.  Require a sscaled separation
    retval = gap - _nstripgap/width < RandFlat::shoot(_randengine);
    if(retval){
// select the highest-momentum track to keep its hot.  This simulates pat. rec. bias for higher-momentum
      double sep;
      PacHitOnTrk *primary, *secondary;
      if(proximity._h1->simHit()->momentumIn().mag() > proximity._h2->simHit()->momentumIn().mag()){
// actually merge the hots.  This requires casting-off const
        primary = const_cast<PacHitOnTrk*>(proximity._h1);
        secondary = const_cast<PacHitOnTrk*>(proximity._h2);
// signing convention on separation = h2 - h1
        sep = -proximity._msep;
      } else {
        primary = const_cast<PacHitOnTrk*>(proximity._h2);
        secondary = const_cast<PacHitOnTrk*>(proximity._h1);
        sep = proximity._msep;
      }
// overwrite the primary hit's position with the merged position, and disable the secondary
      primary->merge(secondary,sep);
    }
  }
  return retval; 
}

bool
PacReconstructTrk::mergeSiPixels(const PacTrkHitProximity& proximity) const {
  return false;
}

bool
PacReconstructTrk::mergeWireHits(const PacTrkHitProximity& proximity) const {
  bool retval(false);
// based on overlap within a cell and time, distance along wire is ignored
  const PacTrkHitViewDch* wview = dynamic_cast<const PacTrkHitViewDch*>(proximity._h1->view());
  if(wview != 0){
// compute spatial overlap relative to cell size.  This computes the probability that the
// 2 hits are in the same cell
    double cellover = fabs(proximity._msep)/wview->cellSize();
    if(cellover < 1.0 && cellover < RandFlat::shoot(_randengine)){
// check timing.  Compute a drift time for each hit, assuming a flat distribution
      double tdmax = 0.5*wview->cellSize()/wview->driftVelocity();
      double td1 = tdmax*RandFlat::shoot(_randengine);
      double td2 = tdmax*RandFlat::shoot(_randengine);
      double th1 = proximity._h1->simHit()->time() + td1;
      double th2 = proximity._h2->simHit()->time() + td2;
// if the charge arrival times are separated by more than the minimum 2-hit separation
// time, they remain separate hits.  Otherwise one of the hits must be disabled
      if( !proximity._h1->measure()->timeResolved(th1,th2)){
// the early hit always wins
        PacHitOnTrk* hearly;
        PacHitOnTrk* hlate;
        double tearly = std::min(th1,th2);
        if(th1 < th2){
          hearly = const_cast<PacHitOnTrk*>(proximity._h1);
          hlate = const_cast<PacHitOnTrk*>(proximity._h2);
          tearly = th1;
        } else {
          hearly = const_cast<PacHitOnTrk*>(proximity._h2);
          hlate = const_cast<PacHitOnTrk*>(proximity._h1);
          tearly = th2;
        }
// 2 possible outcomes: either the late is lost, or it is overwritten by the early hit.
// The late hit is overwritten if the early hit has a time which is between 0 and the maximum drift
// time, relative to the late hit time origin (IE, if the early hit gives a physical time
// and so might be mistaken for a real hit).
        double dt = tearly - hlate->simHit()->time();
        if(dt > 0.0 && dt < tdmax){
// if the early hit is higher-momentum, assume it will be correctly associated, and so
// the only effect is that the late hit is shadowed
          if(hearly->simHit()->momentumIn().mag() > hlate->simHit()->momentumIn().mag()){
            hlate->setUsability(-11);
            hlate->setActivity(false);
          } else {
// the late hit survives, but its position is overwritten by the position of the early hit
            hlate->overwrite(hearly);
          }
        } else {
// the late hit is shadowed and so ignored
          hlate->setActivity(false);
          hlate->setUsability(-11);
        }
      }
// if hits are within the drift time of each other they can shadow each other; only the
// early hit will
    }
  } else {
    cerr << "View is not a wire view" << endl;
  }
  return retval;
}
  
void 
PacReconstructTrk::doPatRec(TrkRecoTrk*& trk,const std::vector<PacTrkHitProximity>& closehits) const {
  if (0 == trk) return;
  bool needsToRefit = false;
  KalInterface kinter;
  if (!trk->attach(kinter, trk->defaultType()))
    return;
  KalRep* krep = kinter.kalRep();
  if (0 == krep)
    return;
  // Loop over all of the HoTs on this track
  TrkHotList* hots = trk->hots();
  for (TrkHotList::nc_hot_iterator iHot = hots->begin(); iHot != hots->end(); ++iHot) {
    if (!iHot->isActive()) continue;
    // Cast this as a PacHitOnTrk in order to get to the measurement object
    PacHitOnTrk* phot = dynamic_cast<PacHitOnTrk*>(iHot.get()); 
    assert(NULL != phot);
    // see if there are any appropriate nearby hits
    std::vector<PacHitOnTrk*> chots;
    for(std::vector<PacTrkHitProximity>::const_iterator iclose=closehits.begin();iclose!=closehits.end();iclose++){
      if(phot == iclose->_h1 || phot == iclose->_h2){
        const PacHitOnTrk* ohot = phot == iclose->_h1 ? iclose->_h2 : iclose->_h1;
    // don't allow low-momentum tracks to steal hits from high-momentum tracks
        if(ohot->simHit()->momentum().mag() < phot->simHit()->momentum().mag()){
    // if there's a nearby hot, clone it
          PacHitOnTrk* chot = ohot->clone(krep);
    // reset the flightlength
          chot->setFltLen(phot->fltLen());
          chots.push_back(chot);
        }
      }
    }
    // start off with the hots inactive
    for(std::vector<PacHitOnTrk*>::iterator ichot = chots.begin();ichot != chots.end();ichot++){
      (*ichot)->setActivity(false);
      krep->addHot(*ichot);
    }
  // if there are any nearby appropriate hots, test their chisquqred WRT this track
    if(chots.size() > 0){
    // Get the residual and error (unbiased) for the current active HoT
      double originalResid = 9999.;
      double originalResidError = 9999.;
      bool originalResidGood = phot->resid(originalResid, originalResidError, true);
      if (!originalResidGood) continue;
    // compute the probability of this hot being on the track
      double ochi2 = pow(originalResid/originalResidError, 2);
      ChisqConsistency chicon(ochi2,1);
      double oprob = chicon.significanceLevel();
    // find the nearby hit with the best probability
      double bestprob(0.0);
      PacHitOnTrk* bhot(0);
      for(std::vector<PacHitOnTrk*>::iterator ohot = chots.begin();ohot != chots.end();ohot++){
    // Get this HoTs residual.  We will use the original Residual Error to determine the
    // patrec "road".  This is a little hinky since this residual is determined with the 
    // original HoT still on the track so the trajectory will be biased toward that HoT.
    // Could try to grab the filter parameters from inside the rep instead?
        double thisResid;
        double thisResidError;
    // Since this HoT is inactive, value of "exclude" flag shouldn't matter?
        bool thisResidGood = (*ohot)->resid(thisResid, thisResidError, true);
        if (!thisResidGood) continue;
        double roadSquared = originalResidError*originalResidError;
    // correct for the hit errors
        roadSquared -= phot->hitRms()*phot->hitRms();
        roadSquared += (*ohot)->hitRms()*(*ohot)->hitRms();
        double thisChi2 = thisResid*thisResid/roadSquared;
        ChisqConsistency ocon(thisChi2,1);
        double oprob = ocon.significanceLevel();
        if(oprob > bestprob){
          bestprob = oprob;
          bhot = *ohot;
        }
      }
    // test for swapping.  This measures the probability that the original hit is right (and the oth is wrong) vs the converse
      if(bestprob*(1.0-oprob) > RandFlat::shoot(_randengine) * oprob*(1.0-bestprob)){
        if (_debug) cout << "*******Doing a PatRec Swap*********" << endl;
        phot->setActivity(false);
        bhot->setActivity(true);
        needsToRefit = true;	  
      }
    }
  } // End loop over HoTs on trk

  // Only do the refit once after all of the HoTs have been looped over.  Faster, but maybe
  // ideally we would re-fit for each swap?  Then patrec could follow a stray branch.  Just move
  // this code to right after the if (doSwap) clause
  if (needsToRefit) {
    TrkErrCode fitcode;
    fitcode = krep->fit();
    krep->addHistory(fitcode, "PacRecoTrk::doPatrec");
    if (fitcode.failure()) {
      if (_debug) std::cout << "fit not valid on PatRec re-fit, " << fitcode
        << std::endl;
      return;
    }
  }
  return;
}

void
PacReconstructTrk::weedHots(TrkRecoTrk*& trk, int iterCnt) const {

  iterCnt++;
  if (0 == trk || iterCnt > _maxweed) return;

  // Loop over HoTs and find HoT with largest contribution to chi2.  If this value
  // is greater than some cut value, deactivate that HoT and reFit

  bool needsFit = false;
  double worst = -1.;
  TrkHitOnTrk* worstHot = 0;
  TrkHotList* hots = trk->hots();
  for (TrkHotList::nc_hot_iterator iHot = hots->begin(); iHot != hots->end(); ++iHot) {
    if (iHot->isActive()) {
      double resid, residErr;
      bool test = iHot->resid(resid, residErr, true);
      if (test) {
        double value = abs(resid/residErr);
        if (value > _hitchicut && value > worst) {
          worst = value;
          worstHot = iHot.get();
          needsFit = true;
        }
      }
    }
  }

  if (needsFit) {
    // Turn off the worst HoT on the track
    assert(0 != worstHot);

    worstHot->setActivity(false);

    // Check DOF...
//    if (hots->nActive(TrkEnums::xyView) < _kalcon->minDOF(TrkEnums::xyView) ||
//      hots->nActive(TrkEnums::zView) < _kalcon->minDOF(TrkEnums::zView) ||
//    (hots->nSvt() < _minsvt && hots->nDch() < _mindch)) {
//      return;
//    }

    KalInterface kinter;
    if (trk->attach(kinter, trk->defaultType())) {
      KalRep* krep = kinter.kalRep();
      TrkErrCode fitcode;
      if (krep != 0) {
        fitcode = krep->fit();
  //	std::stringstream sstr;
  //	sstr << "PacRecoTrk::weed_" << iterCnt;
  //	krep->addHistory(fitcode, sstr.str().c_str());
        krep->addHistory(fitcode, "PacRecoTrk::weed");
        if (fitcode.failure()) {
          if (_debug) std::cout << "fit not valid on re-fit, " << fitcode 
            << std::endl;
          return;
        }
      }
    }
    // Recursively iterate
    weedHots(trk, iterCnt);

  }

  return;
}

void
PacReconstructTrk::estimateCovar(const HepVector& sdiff, double herr_avg, double dlen, unsigned nhits,
const HepVector& seedparams, HepSymMatrix& covar) const {
  // use the parameter difference to estimate the scattering component
  covar = vT_times_v(sdiff);  
  // add terms for the measurement errors; position errors are direct, as we extrapolate to the IP
  double pcov = herr_avg*herr_avg;
  covar.fast(PacHelix::d0Index+1,PacHelix::d0Index+1) += pcov;
  covar.fast(PacHelix::z0Index+1,PacHelix::z0Index+1) += pcov;
  // angle errors use the lever arm, assuming uniform hit distribution
  double acov = 4*pcov/(dlen*dlen*nhits);
  covar.fast(PacHelix::phi0Index+1,PacHelix::phi0Index+1) += acov;
  double td2 = 1.0 + seedparams[PacHelix::tanDipIndex]*seedparams[PacHelix::tanDipIndex];
  covar.fast(PacHelix::tanDipIndex+1,PacHelix::tanDipIndex+1) += acov*td2*td2;
  // 2 regimes for omega; length >> radius, or length << radius.  Choose = for divide
  double tlen = dlen/sqrt(td2);
  double omega = fabs(seedparams[PacHelix::omegaIndex]);
  double ocov;
  if(tlen > 1.0/omega){
// here, we are measuring the radius.  Assume 1/2 the hits measure transverse
    ocov = 4*pow(omega,4)*pcov/nhits;
  } else {
// here, we are measuring the sagitta.  Use the classic formula
    ocov = 4*acov/(tlen*tlen);
  }
  covar.fast(PacHelix::omegaIndex+1,PacHelix::omegaIndex+1) += ocov;
}

void
PacReconstructTrk::smearSeed(const HepSymMatrix& covar,HepVector& params) const {
// use diagonal terms only
  for(int iparam=0;iparam<params.num_row();iparam++){
    params[iparam] += RandGauss::shoot(_randengine,0.0,sqrt(covar.fast(iparam+1,iparam+1))*_seedsmear);
  }
}

void
PacReconstructTrk::createTrajs(const PacSimTrack* simtrk,const std::vector<const PacHitOnTrk*>& hots,
  HelixTraj*& seed, TrkDifPieceTraj*& ptraj) const {
  seed = 0;
  ptraj = 0;
// create parameter and covariance estimates for the seed
  const PacPieceTraj* htraj = simtrk->getTraj();
  assert(htraj != 0);
  HepSymMatrix seedcovar(5,0);
  HepVector seedparams(5,0);
  HepVector sdiff(5,0);
// find the helix pieces at the first and last hit
  double flen = hots.front()->fltLen();
  const PacHelix* fhelix =   dynamic_cast<const PacHelix*>(htraj->localTrajectory(flen));
  double llen = hots.back()->fltLen();
  const PacHelix* lhelix =   dynamic_cast<const PacHelix*>(htraj->localTrajectory(llen));
  double dlen = llen-flen;
  if(fhelix != 0 && lhelix != 0){
      // merge the 2 parameters to define the seed
    seedparams = 0.5*(fhelix->parameters() + lhelix->parameters());
      // estimate the covariance matrix as the difference between the first and last helix
    sdiff = fhelix->parameters() - lhelix->parameters();
  } else if (fhelix != 0){
    seedparams = fhelix->parameters();
  } else if (lhelix != 0){
    seedparams = lhelix->parameters();
  } else {
    ErrMsg(error) << "no helix trajs! " << endmsg;
    return;
  }
// compute the average hit error
  double herr_avg(0.0);
  unsigned nhot(0);
  for(std::vector<const PacHitOnTrk*>::const_iterator ihot = hots.begin(); ihot != hots.end(); ihot++){
    if((*ihot)->isActive()){
      nhot++;
      herr_avg += (*ihot)->hitRms();
    }
  }
  if(nhot>0)
    herr_avg /= nhot;
  else {
    ErrMsg(error) << "no hots! " << endmsg;
    return;
  }
  // use the average hit error and the parameter difference (= scattering) to estimate the covariance
  estimateCovar(sdiff, herr_avg, dlen, nhot, seedparams, seedcovar);
      // smear the seed parameter accordingly
  smearSeed(seedcovar,seedparams);
  seed = new HelixTraj(seedparams,seedcovar,htraj->lowRange(),htraj->hiRange());
      // copy the full trajectory to a difpiecetraj, but only the helix parts!
  std::vector<TrkSimpTraj*> trajs;
  bool first(true);
  double fglen(0.0);
  for(int itraj=0;itraj<htraj->trajList().size();itraj++){
    const PacHelix* helix = dynamic_cast<const PacHelix*>(htraj->trajList()[itraj].second);
    if(helix != 0){
      if(first){
        fglen = htraj->trajList()[itraj].first;
        first = false;
      }
      trajs.push_back(new HelixTraj(helix->parameters(),seedcovar,helix->lowRange(),helix->hiRange()));
    }
  }
// create piece traj.  synchronize the global flightlength with the original PacSimTraj
  ptraj = new TrkDifPieceTraj(trajs,fglen);
}

void
PacReconstructTrk::extractIntersections(const PacSimTrack* simtrk, const TrkDifPieceTraj* ptraj,
  double lasthot, std::vector<DetIntersection>& dvect) const {
  const std::vector<PacSimHit>&hits = simtrk->getHitList();
  dvect.reserve(hits.size());
  for(std::vector<PacSimHit>::const_iterator ihit=hits.begin();ihit!= hits.end();ihit++){
  // only inlcude the 'normal' material hits on helices
    if(ihit->detEffect() ==PacSimHit::normal && ihit->globalFlight() < lasthot){
    // check to see the gap of this element.  If it's very large, assume that we wouldn't be
    // able to identify this interaction and so skip it.
      const PacDetElem* elem = ihit->detElem();
      if(elem != 0 && elem->gapFraction() > _maxgap)continue;
    // update to global flightlengths and the reference trajectory
      DetIntersection dinter(ihit->detIntersection());
      double oldpath = dinter.pathlen;
      dinter.pathlen = ihit->globalFlight();
      dinter.pathrange[0] += dinter.pathlen - oldpath;
      dinter.pathrange[1] += dinter.pathlen - oldpath;
      dinter.trajet = ptraj;
    // test
      HepPoint dpoint = dinter.trajet->position(dinter.pathlen);
      const HepPoint& spoint = ihit->originalPosition();
      if(spoint.distanceTo(dpoint) > 1e-1)
        cerr << "intersection points don't match " << endl;
      dvect.push_back(dinter);
    }
  } 
}

bool
PacReconstructTrk::goodHots(const std::vector<const PacHitOnTrk*>& hots) const {
  unsigned nxy(0), nz(0), nsvt(0), ndch(0), nactive(0);
  for(std::vector<const PacHitOnTrk*>::const_iterator ihot=hots.begin();ihot!=hots.end();ihot++){
    if((*ihot)->svtHitOnTrack())nsvt++;
    if((*ihot)->dchHitOnTrack())ndch++;
    if((*ihot)->isActive()){
      nactive++;
      if((*ihot)->whatView() == TrkEnums::xyView)nxy++;
      if((*ihot)->whatView() == TrkEnums::zView)nz++;
    }
  }
  return nxy >=_kalcon->minDOF(TrkEnums::xyView)
    && nz >=_kalcon->minDOF(TrkEnums::zView)
    && nactive >=_kalcon->minDOF(TrkEnums::bothView)
    && ( nsvt >= _minsvt || ndch >= _mindch);
}

void
PacReconstructTrk::clearMaps() const {
// delete any unused HOTs
  _hotmap.Clear();
}

const TrkRecoTrk*
PacReconstructTrk::findTrack(const PacSimTrack* simtrk) const 
{
  return _hotmap.getRecoTrk(simtrk);
  
}


// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkHitMeas
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacTrk/PacTrkHitMeas.hh"
#include "PacTrk/PacTrkHitViewDch.hh"
#include "PacSim/PacSimHit.hh"
#include "PacTrk/PacHit.hh"
#include "PacTrk/PacHitOnTrk.hh"

#include "PacEnv/PacConfig.hh"
#include "GenEnv/GenEnv.hh"
#include "AbsEnv/AbsEnv.hh"
#include "DetectorModel/DetMaterial.hh"

#include "G3Data/GTrack.hh"

#include "DetectorModel/DetIntersection.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include <assert.h>
#include <iostream>

PacTrkHitMeas::PacTrkHitMeas(HitType type,std::vector<PacTrkHitView*>& views) : 
  PacMeasurement(PacMeasurement::TrkHit),_htype(type),_views(views)  
{
  if( _htype == SiPixel || _htype == DoubleSiStrip){
    assert(_views.size()==2);
  } else {
    assert(_views.size()==1);
  }
  // 2-time hit separation should come from measurement: FIXME!!!
  _dt2hit = 64e-9;
}	

PacTrkHitMeas::~PacTrkHitMeas()
{
	for(std::vector<PacTrkHitView*>::const_iterator iview=_views.begin();iview!=_views.end();iview++){
		delete *iview;
	}
}

PacMeasurement::MeasurementTime
PacTrkHitMeas::inTime(double time,double& fraction) const{
  if(_htype == Wire && _views.size() > 0 ){
    const PacTrkHitViewDch* wview = dynamic_cast<PacTrkHitViewDch*>(_views[0]);
// wire hits must account for drift time; take the worst case of full drift time
    if(wview != 0 && fabs(time) < halfSensitiveTimeWindow() + 0.5*wview->cellSize()/wview->driftVelocity()){
      return PacMeasurement::InTime;
    } else
      return PacMeasurement::OutOfTime;      
  } else {
// everything else uses a simple window
    return PacMeasurement::inTime(time,fraction);
  }
}

void PacTrkHitMeas::createHots(const PacSimHit& simhit, std::vector<PacHitOnTrk*>& hotlist,
			       bool isActive) const {
  // check timing
  double fraction;
  if(inTime(simhit.time(),fraction) == PacMeasurement::OutOfTime) return;
  // get random engine
  HepRandomEngine* rng = HepRandom::getTheEngine();
  // for pixels, efficiency is given as the single view efficiency
  if(_htype == SiPixel) {
    double eff = _views[0]->efficiency(simhit);
    if(RandFlat::shoot(rng)> eff)return;
  }
  // loop over measurement views
  for(std::vector<PacTrkHitView*>::const_iterator iview=_views.begin();iview!=_views.end();iview++){
    // for non-pixels, test efficiency separately
    if(_htype != SiPixel) {
      double eff = (*iview)->efficiency(simhit);
      if(RandFlat::shoot(rng)> eff) continue;
    }
    std::vector<PacRecHitInfo> hvec;
    (*iview)->getHitInfo(simhit,hvec);
// loop over the hits
    for(std::vector<PacRecHitInfo>::const_iterator ih = hvec.begin();ih!= hvec.end();ih++){
    // Create the hot
      TrkDetElemId::systemIndex sys = type() == PacTrkHitMeas::Wire ? TrkDetElemId::dch : TrkDetElemId::svt;
      PacHit* pachit = new PacHit((*iview)->whatView(), simhit.detIntersection().delem->elementNumber(),sys);
      PacHitOnTrk* pachot = new PacHitOnTrk(pachit, *ih, this, *iview, &simhit);
      pachot->setActivity(isActive);
      hotlist.push_back(pachot);
    }
  }
}


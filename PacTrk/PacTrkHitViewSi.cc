// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkHitViewSi
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BbrGeom/BbrAngle.hh"
#include "PacTrk/PacTrkHitViewSi.hh"
#include "PacTrk/PacTrkResModelSi.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacSim/PacSimHit.hh"

#include "PacGeom/PacPieceTraj.hh"
#include "PacSim/PacSimHit.hh"
#include "DetectorModel/DetSurface.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"

#include <iostream>

#include <assert.h>
#include <algorithm>


// c'tor that uses PacTrkResModelSi
PacTrkHitViewSi::PacTrkHitViewSi(TrkEnums::TrkViewInfo view,PacTrkResModelSi* resModel, 
				 double sign, double pitch, double ngfrac, double ngfact, double eff) :
  PacTrkHitView(view),_resModel(resModel),_sign2(sign*sign), _ptrat(pitch),_ngfrac(ngfrac),_ngfact(ngfact)
  ,_eff(eff)
{
  // must have either xy or Z view
  assert(view== TrkEnums::xyView || view == TrkEnums::zView);
	
  // _rms is calculated for each track angle (in getHitInfo)
  _rms=0;

}


PacTrkHitViewSi::~PacTrkHitViewSi()
{
  if (0!=_resModel) {
    delete _resModel;
  }
}

void
PacTrkHitViewSi::getHitInfo(const PacSimHit& simhit,std::vector<PacRecHitInfo>& hvec) const {
// get random engine
  HepRandomEngine* rng = HepRandom::getTheEngine();
// create hit info
  PacRecHitInfo hinfo;
// flightlength is at the simhit
  hinfo._flentrue = simhit.globalFlight();
  hinfo._simpos = simhit.globalTraj()->position(hinfo._flentrue);
  // compute surface point of this point in space
  const DetSurfaceElem* delem = simhit.detElem()->surfElem();
  assert(delem != 0);
  const DetSurface* surf = delem->surface();
  assert(surf != 0);
  SurfacePoint sp;
// the following fills the sp, don't remove the call
  
  if(0 != surf->surfacePoint(hinfo._simpos,sp) )
    ErrMsg(error) << "hit point not on surface ! " << endmsg;
  Hep3Vector norm = surf->normal(sp);
  
  // determine which view the hit strips trajectory should be in
  if(whatView() == TrkEnums::zView){
    // z direction means strips go along local 'x'
    hinfo._wdir = surf->surfaceDirection(sp,0);
  } else {
    // xy view means strips go along local 'y'
    hinfo._wdir = surf->surfaceDirection(sp,1);
  }
  // track/surface angle projected along measurement direction
  Hep3Vector tdir = simhit.momentum();
  hinfo._mdir = norm.cross(hinfo._wdir); // direction of the measurement (across the strips/segmentation)
  double y = tdir.dot(hinfo._mdir);
  double x = tdir.dot(norm);

  double angle = atan2(y,x); // this is the angle relative to the normal (i.e. dip angle for z-view)

  // J. Walsh - I'm not sure if I have to multiply angle by -1 for negative z side of detector. 
  // in any case, the res functions thus far implemented are symmetric in angle.
  double hitres=0;
  if (0!=_resModel) {
    hitres = _resModel->hitRes(angle);
  }
  else {
    hitres=_sig0;
  }
  
  // calculate weighted hit error
  double sig02 = hitres*hitres;
  // compute effective rms, including non-Gaussian effects
  // simple constant model of hit resolution.  Use the RMS to define the estimated error.
  hinfo._herr = sqrt((1.0-_ngfrac)*sig02 + _ngfrac*_ngfact*_ngfact*sig02);
 
  // the following code was useful for debugging
  //  if (whatView() == TrkEnums::zView && 0 != _resModel ) {
  // compare z/sqrt(x*x+y*y) to tan lambda
  //  std::cout << "  TANGENT " << sqrt(pow(simhit.position().x(),2)+pow(simhit.position().y(),2)) << " " 
  //	      << tan(angle) << " " 
  //	      << simhit.position().z()/sqrt(pow(simhit.position().x(),2)+pow(simhit.position().y(),2))
  //	      << " " << res << std::endl;
  //}

// see if we have non-Gaussian tails
  if(_ngfrac > RandFlat::shoot(rng))
    hitres *= _ngfact;
// model effects of out-of-plane distortions
  if(_sign2 > 0.0){
// compute the track direction in the measurement and normal plane
    double tm = tdir.dot(hinfo._mdir);
    double tn = tdir.dot(norm);
    double tm2 = tm*tm;
    double tn2 = tn*tn;
    double sind2 = tm2/(tm2+tn2);
// add the normal distortion incoherently with the intrinsic resolution
    hitres = sqrt(hitres*hitres +sind2*_sign2);
  }
  hinfo._sres = hitres;
// POCA is computed in space, so is perpendicular to both the track and hit.  This
// is the dimension we actually measure in BaBar, so this is what we perturb.
  hinfo._pdir = tdir.cross(hinfo._wdir).unit();
// Randomly perturb the hit location along the poca direction.
  hinfo._hitpos = simhit.position() + RandGauss::shoot(rng,0,hitres)*hinfo._pdir;
// store this information
  hvec.push_back(hinfo);
}

double PacTrkHitViewSi::PacTrkHitViewSi::efficiency(const PacSimHit& simhit) const {
	return _eff;
}

double
PacTrkHitViewSi::measurementAngle() const {
  if(whatView() == TrkEnums::xyView){
    return 0;
  } else {
    static const double halfpi = -Constants::pi/2.0;
    return halfpi;
  }   
}

double
PacTrkHitViewSi::sensorSeparation() const {
// take a nomial value: this should come from the configuration FIXME!!!!
  return 0.1;
}

double
PacTrkHitViewSi::sensorSize() const {
// take a nomial value: this should come from the configuration FIXME!!!!
  return 5.0;
}



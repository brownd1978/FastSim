/*******************************************************************************
 * PacHitOnTrk
 *******************************************************************************/

#include "BaBar/BaBar.hh"
#include "BbrGeom/BbrAngle.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacTrk/PacRecHitInfo.hh"
#include "PacTrk/PacTrkHitMeas.hh"
#include "PacSim/PacSimHit.hh"
#include "TrkBase/TrkErrCode.hh"
#include "TrkBase/TrkPoca.hh"
#include <iostream>
#include "ErrLogger/ErrLog.hh"

//class PacTrkHitMeas
//{  
//};

using std::cerr;
using std::endl;

PacHitOnTrk::PacHitOnTrk(PacHit* hit, const PacRecHitInfo& hinfo,const PacTrkHitMeas* measure,
  const PacTrkHitView* view, const PacSimHit* simhit)
    : TrkHitOnTrk(hit, 1e-5),_hinfo(hinfo),_pachit(hit),_hittraj(new TrkLineTraj(hinfo._hitpos, hinfo._wdir, 1)),
    _hitrms(hinfo._herr),_measure(measure),_view(view),_simhit(simhit)
{
  assert(hit != 0 && _hittraj!= 0 && measure != 0);
  setHitRms(_hitrms);
  setFltLen(hinfo._flentrue);
}

PacHitOnTrk::PacHitOnTrk(const PacHitOnTrk& other, TrkRep* rep, PacHit* hit, TrkLineTraj* traj)
    : TrkHitOnTrk(other, rep, 0),_hinfo(other._hinfo),_pachit(hit),_hittraj(traj),_hitrms(other._hitrms),_measure(other._measure),_simhit(other._simhit)
{
  assert(hit != 0 && traj!= 0);
}

PacHitOnTrk::~PacHitOnTrk() {
  delete _hittraj;
  delete _pachit;
// ugly trick to keep the base class from trying to delete _pachit
  _parentRep=0;
}

bool
PacHitOnTrk::isMergable(const PacHitOnTrk* other) const {
// logical checks first: must be same measurement, view, and element
// require different simhits.  If the simhits have the smae simtrack, they
// should be spatially separated to avoid merging overlaps in the Si
  double fraction;
  return other->measure() == measure() &&
    other->view() == view() && 
    other->simHit() != simHit() &&
    (other->simHit()->getSimTrack() != simHit()->getSimTrack() ||
    fabs(other->simHit()->globalFlight() - simHit()->globalFlight()) > 10.0) &&
    other->simHit()->detElem() == simHit()->detElem() &&
    PacMeasurement::InTime == measure()->inTime(fabs(simHit()->time() - other->simHit()->time()),fraction);
}


PacHitOnTrk* PacHitOnTrk::clone(TrkRep* parentRep, const TrkDifTraj* trkTraj) const {
    return new PacHitOnTrk(*this, parentRep, _pachit->clone(), _hittraj->clone());
}

TrkErrCode PacHitOnTrk::updateMeasurement(const TrkDifTraj* traj, bool maintainAmbiguity) {
    TrkErrCode status = updatePoca(traj, maintainAmbiguity);
    if(status.success()) {
        setHitResid(_poca->doca());
        setHitRms(_hitrms);
    }
    else {
        ErrMsg(routine) << "PacHitOnTrk::updateMeasurement() failed" << endmsg;
        setHitResid(999999);
        setHitRms(999999);
        setUsability(0);
    }
    return status;
}

bool
PacHitOnTrk::proximity(const PacHitOnTrk* other, PacTrkHitProximity& hprox) const {
// first, logical check, and check timing (loose cut)
  bool retval(false);
  if(isMergable(other)){
// Get measurement angle
    double angle = _view->measurementAngle();
    double cosTheta = cos(angle);
    double sinTheta = sin(angle);
// separation depends on geometry.
    const PacDetElem* delem = simHit()->detElem();
    if(delem->surfaceType() == PacDetElem::plane){
// plane separation is just the cartesian separation
      Hep3Vector hsep = hitInfo()._hitpos - other->hitInfo()._hitpos;
      hprox._msep = hitInfo()._mdir.dot(hsep);
      hprox._wsep = hitInfo()._wdir.dot(hsep);
    } else if(delem->surfaceType() == PacDetElem::cylinder){
// we must compute the separation on the surface
      double deltaZ = hitInfo()._hitpos.z() - other->hitInfo()._hitpos.z();
      BbrAngle deltaPhi = BbrAngle(hitInfo()._hitpos.phi() - other->hitInfo()._hitpos.phi());
      double r = other->hitInfo()._hitpos.perp();
      assert(r > 0.0);
// sign convention is from this hit to the other (along the measurement direction)
      hprox._msep = r*cosTheta*deltaPhi.rad() - sinTheta*deltaZ;
      hprox._wsep = r*sinTheta*deltaPhi.rad() + cosTheta*deltaZ;
    } else if(delem->surfaceType() == PacDetElem::cone){
// cone implementation is approximate, as cell size is not constant
      double deltaZ = hitInfo()._hitpos.z() - other->hitInfo()._hitpos.z();
      BbrAngle deltaPhi = BbrAngle(hitInfo()._hitpos.phi() - other->hitInfo()._hitpos.phi());
      double r1 = hitInfo()._hitpos.perp();
      double r2 = other->hitInfo()._hitpos.perp();
      // define the phi separation at the average radius
      double deltaP = 0.5*(r1+r2)*deltaPhi.rad();
      double deltaR = sqrt(pow(r1-r2,2) + deltaZ*deltaZ);
      hprox._msep = cosTheta*deltaP - sinTheta*deltaR;
      hprox._wsep = sinTheta*deltaP + cosTheta*deltaR;
    } else {
      cerr <<"Unknown geometry" << endmsg;
    }
//  check separation; include a buffer
    if(fabs(hprox._msep) < 2*view()->sensorSeparation() &&
    fabs(hprox._wsep) < 2*view()->sensorSize()){
      hprox._h1 = this;
      hprox._h2 = other;
      retval = true;
    }
  }
  return retval;
}

bool
PacHitOnTrk::merge(PacHitOnTrk* other,double msep) {
// only merge active hots
  bool retval = false;
  if(isActive() && other->isActive() && isMergable(other)){    
// bookkeeping
    addMerged(other);
    other->addMerged(this);
    double weight = 1.0/(1.0+_merged.size());
// Adjust the hit position by 1/2 the separation along the measurement direction.
//  This isn't quite right for cylinders, but it's good enough
// start with the current position, to account for multiple merges
    HepPoint old = _hittraj->position(0.0);
    Hep3Vector vpos = Hep3Vector(old.x(),old.y(),old.z()) + (weight*msep)*_hinfo._mdir;
    HepPoint mpos(vpos.x(),vpos.y(),vpos.z());
// overwrite the trajectory for this position
    *_hittraj = TrkLineTraj(mpos,_hinfo._wdir, 1);
// completely disable the other hit;
    other->setActivity(false);
// flag the merge with the 'usability'
    other->setUsability(-10);
    setUsability(10);
    retval = true;
  }
  return retval;
}

void
PacHitOnTrk::overwrite(PacHitOnTrk* other){
  delete _hittraj;
  _hittraj = other->hitTraj()->clone();
  setUsability(10);
  other->setActivity(false);
  other->setUsability(-11);
}


void
PacHitOnTrk::addMerged(const PacHitOnTrk* other) {
  _merged.push_back(other);
}

const DchHitOnTrack*
PacHitOnTrk::dchHitOnTrack() const {
  if(_measure->type() == PacTrkHitMeas::Wire)
    return (DchHitOnTrack*)1;
  else
    return 0;
}
const SvtHitOnTrack*
PacHitOnTrk::svtHitOnTrack() const{
  if(_measure->type() == PacTrkHitMeas::DoubleSiStrip ||
    _measure->type() == PacTrkHitMeas::SiPixel ||
    _measure->type() == PacTrkHitMeas::SingleSiStrip )
    return (SvtHitOnTrack*)1;
  else
    return 0;
}

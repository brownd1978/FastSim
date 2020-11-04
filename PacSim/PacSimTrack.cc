//--------------------------------------------------------------------------
// Name:
//   PacSimTrack
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "PacSim/PacSimTrack.hh"
#include "PacGeom/PacPieceTraj.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/Pdt.hh"
#include "BaBar/Constants.hh"
#include "TParticle.h"

PacSimTrack::PacSimTrack(const GTrack* gtrk) :
	_gtrk(gtrk),_tpart(0)
{
  _traj = new PacPieceTraj;
  _hits.reserve(64);

  // Init timing information
  _timeElapsed = 0.0;
  if( 0 != gtrk && 0 != gtrk->vertex() ) {
    _timeAtOrigin = gtrk->vertex()->time();
    //cout << "GTrack: id = " << gtrk->id()
    //<< " timeAtOrigin = " << _timeAtOrigin 
    //<< endl;
  } else {
    //cout << "!!! timeAtOrigin not available for new PacSimTrack !!!"
    //<< endl;
    _timeAtOrigin = -1.0; // negative value means not properly initialized
  }
  _pdt = _gtrk->pdt();
}

PacSimTrack::PacSimTrack(const TParticle* tpart) :
	_gtrk(0),_tpart(tpart)
{
  _traj = new PacPieceTraj;
  _hits.reserve(64);

  // Init timing information
  _timeElapsed = 0.0;
  if( 0 != _tpart) {
    _timeAtOrigin = _tpart->T();
  } else {
    _timeAtOrigin = -1.0; // negative value means not properly initialized
  }
  _pdt = Pdt::lookup((PdtPdg::PdgType)_tpart->GetPdgCode());
}

	
PacSimTrack::PacSimTrack(const PacSimTrack& other) :
  _gtrk(other._gtrk),_tpart(other._tpart),_traj(other._traj->clone()),_hits(other._hits),
  _timeAtOrigin(other._timeAtOrigin),_timeElapsed(other._timeElapsed)
{
}

PacSimTrack&
PacSimTrack::operator =(const PacSimTrack& other) {
  if(this != &other){
    _gtrk = other._gtrk;
    _tpart = other._tpart;
    delete _traj; _traj = other._traj->clone();
    _hits = other._hits;
    _timeAtOrigin = other._timeAtOrigin;
    _timeElapsed = other._timeElapsed;
  }
  return *this;
}

PacSimTrack::~PacSimTrack(){
	delete _traj;
}

int
PacSimTrack::pdgId() const {
  if(_tpart != 0)
    return _tpart->GetPdgCode();
  else if(_gtrk != 0)
    return _gtrk->pdt()->pdgId();
  else
    return 0;
}

double
PacSimTrack::particleMass() const {
  if(_tpart != 0)
    return const_cast<TParticle*>(_tpart)->GetMass();
//    return _tpart->GetCalcMass();
  else if(_gtrk != 0)
    return _gtrk->pdt()->mass();
  else
    return -1.0;
}

bool
PacSimTrack::addHit(const PacSimHit& hit) {
  bool retval(true);
  bool timeUpdated(true);
  // if this is the first hit, it's local flightlength defines the global flightlength.
  // otherwise, compute the global flightlength by comparing to the last hit
  double globalflt(0.0);
  // compute the time elapsed since last hit or track creation
  double deltaT( 0.0 );
  if(_hits.size() > 0){
    const PacSimHit& last = _hits.back();
    globalflt = last.globalFlight();
    double dflt;
// if the new hit and the old refer to the same trajectory, increment the global flight
// by the flight difference.  Otherwise, increment since the start of this trajectory
    if(last.detIntersection().trajet == hit.detIntersection().trajet){
      dflt = hit.detIntersection().pathlen - last.detIntersection().pathlen;
    } else {
      dflt = hit.detIntersection().pathlen - hit.detIntersection().trajet->lowRange();
    }
// check that hits are being added in the correct order.  Allow for the same flightlength, in overlap cases
    if(dflt >= 0.0) {
      globalflt += dflt;
      // Compute the time elapsed since last hit
      double mass = particleMass();
      deltaT = computeDeltaT( mass, hit.momentumIn().mag(), dflt );
      if( deltaT < 0 ) { timeUpdated = false; } // beta == 0 !!!
    } else {
      retval      = false;
      timeUpdated = false;
    }
  } else {
    // no previous hits: this hit defines the global flightlength
    globalflt = hit.detIntersection().pathlen;
    // Compute the time elapsed
    double mass = particleMass();
    deltaT = computeDeltaT( mass, hit.momentumIn().mag(), globalflt );
      if( deltaT < 0 ) { timeUpdated = false; } // beta == 0 !!!
  }
  if(retval){
    // add the hit, and update its global flightlength
    _hits.push_back(hit);
    _hits.back().setGlobalTraj(_traj);
    _hits.back().setGlobalFlight(globalflt);
    // Update timing information
    if( timeUpdated ) {
      _timeElapsed += deltaT;
      _hits.back().setTime( _timeAtOrigin + _timeElapsed );
    }
  }

  return retval;
}

bool
PacSimTrack::addTraj(TrkGeomTraj* traj) {
// use the last hit to set the global flight.  It's OK if there
// is no last hit as long as the trajectory is empty.
  bool retval(true);
  if(_hits.size() > 0)
    _traj->append(_hits.back().globalFlight(),traj);
  else if(_traj->trajList().size() == 0)
    _traj->append(0.0,traj);
  else
    retval = false;
  return retval;
}

void
PacSimTrack::finalize() {
// get the last hit's global fltlen
  double globalflt;
  if(_hits.size()>0)
    globalflt = _hits.back().globalFlight();
  else
    globalflt = _traj->lowRange();
// set the upper range, including a small buffer
  static const double trajbuffer(0.1);
  double range[2] = {_traj->lowRange(),globalflt+trajbuffer};
  _traj->setFlightRange(range);
}

void
PacSimTrack::truncate(double fltlen) {
// loop over the hits to find the first one beyond the specified range
  std::vector<PacSimHit>::iterator ihit = _hits.begin();
  while(ihit != _hits.end()){
    if(ihit->globalFlight() > fltlen) break;
    ihit++;
  }
  _hits.erase(ihit,_hits.end());
// now trim the trajectory
  double range[2] = {_traj->lowRange(),fltlen};
  _traj->setFlightRange(range);
}

double
PacSimTrack::computeDeltaT( double massInGeV, double momentumInGeV,
		      double distanceInCm )
{
  double beta;
  if( massInGeV > 0 ) {
    double betaGamma = momentumInGeV / massInGeV;
    beta = betaGamma / sqrt( betaGamma * betaGamma + 1.0 );
  } else {
    beta = 1.0;
  }
  
  // time = distance / speed 
  // distance in cm, time in second
  if( beta > 0 ) {
    return( distanceInCm / ( beta * Constants::c ) ); 
  } else {
    // Time doesn't make sense if beta is 0!
    return( -1.0 );
  }
}

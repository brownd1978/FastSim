//--------------------------------------------------------------------------
// Name:
//   PacSimulate
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "BaBar/Constants.hh"
#include "BField/BField.hh"
#include "BField/BFieldIntegrator.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandomEngine.h"

#include "DetectorModel/DetIntersection.hh"
#include "DetectorModel/DetSet.hh"
#include "DetectorModel/DetMaterial.hh"
#include "TrkBase/TrkHelixUtils.hh"
#include "PDT/PdtEntry.hh"

#include "G3Data/GTrack.hh"
#include "G3Data/GVertex.hh"

#include "PacEnv/PacConfig.hh"
#include "PacSim/PacSimulate.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PacGeom/PacHelix.hh"
#include "PacGeom/PacDetector.hh"
#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "PacGeom/PacConeDetElem.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/TrkDirection.hh"
#include "TrkBase/TrkVolume.hh"
using namespace std;
#include <vector>
#include "TParticle.h"
#include "TParticlePDG.h"

PacSimulate::PacSimulate(const BField* b, const PacDetector* d) :
  _bfield(b), _detector(d)
{
  // Get the random number enginae
  _randengine = HepRandom::getTheEngine();
  // Find the volumes
  _vols = _detector->detectorModel()->setList();
  DetSetList::const_iterator ivol = _vols.begin();
  // create the list of elments in each volume
  while(ivol != _vols.end()){
    const DetSet* vol = *ivol;
    DetElemList elems;
    vol->listAllElements(elems);
    if(elems.size() > 0)
      _elements.push_back(elems);
    ivol++;
  }
  _maxflt = gconfig.getdouble("PacSimulate.maxflt",1e4); // maximum particle flightlength to simulate in cm
  _minmom = gconfig.getdouble("PacSimulate.minmom",0.01); // minimum momentum to simulate
  _mine_shower = gconfig.getdouble("PacSimulate.mine_shower",0.001); // minimum shower energy to simulate
  _mine_hadshower = gconfig.getdouble("PacSimulate.mine_hadshower",0.01); // minimum hadronic shower energy to simulate
  _xtol = gconfig.getdouble("PacSimulate.xtolerance",1e-1); // maximum transverse deviation
  _tstep = gconfig.getdouble("PacSimulate.teststep",1.0); // test step for field inhomogeneity integration (cm)
  _fewn = gconfig.getdouble("PacSimulate.fewn",5); // maximum # of steps to extrapolate before stepping
  _maxnstep = _maxflt/_tstep; // maximum # of steps
}

PacSimulate::~PacSimulate()
{}

bool
PacSimulate::continues(PacSimHit::effect eff, double mommag ) const {
  return (eff <= PacSimHit::normal && mommag > _minmom) ||
         (eff == PacSimHit::shower && mommag > _mine_shower) ||
         (eff == PacSimHit::hadshower && mommag > _mine_hadshower);
}

bool
PacSimulate::momchange(PacSimHit::effect eff) const {
  return eff > PacSimHit::mark;
}

PacSimTrack*
PacSimulate::simulateGTrack(const GTrack* gtrk) const {
  // get GTrack information
  const PdtEntry* pdt = gtrk->pdt();
  assert(pdt != 0);
  double charge = pdt->charge();
  Hep3Vector mom = gtrk->p4().vect();
  const GVertex* gvtx = gtrk->vertex();
  HepPoint pos;
  double gtime(0.0);
  if(gvtx != 0){
    pos = gvtx->position();
    gtime = gvtx->time();
  } else
    cout << "No GVertex found for GTrack!!" << endl;
// temp data
  DetIntersection dinter,linter;
  PacSimHit::effect eff=PacSimHit::none;
// create the output PacSimTrack.
  PacSimTrack* strk = new PacSimTrack(gtrk);
// make sure particle origin is placed on the 1st winding
  dinter.pathlen = 0.0;
  TrkGeomTraj* htraj =  makeTraj(charge,pos,mom,dinter);
  strk->addTraj(htraj);
// create a simhit for the particle origin
  PacSimHit firsthit(strk,dinter,pos,mom,mom,PacSimHit::creation,gtime);
  strk->addHit(firsthit);
//loop until the particle stops or exits the detector volume
  while(_detector->findNextInter(htraj,dinter) && dinter.pathlen < _maxflt){
    eff = processInter(dinter,mom,pos,strk);
    if(!continues(eff,mom.mag())) break;
// create a new trajectory piece if necessary, and add it to the simtrack trajectory
    if(momchange(eff)){
      htraj = makeTraj(charge,pos,mom,dinter);
      strk->addTraj(htraj);
    }
  }
  // finalize the simtrack if it has any content
  if(strk->getHitList().size() > 0)
    strk->finalize();
  else {
    delete strk;
    strk = 0;
  }
  // return the PacSimTrack object
  return strk;
}

PacSimTrack*
PacSimulate::simulateParticle(const TParticle* tpart) const {
  static const double onethird(1.0/3.0);
  double charge = onethird*(const_cast<TParticle*>(tpart))->GetPDG()->Charge();
  Hep3Vector mom(tpart->Px(),tpart->Py(),tpart->Pz());
  HepPoint pos(tpart->Vx(),tpart->Vy(),tpart->Vz());
  double gtime(tpart->T());
// temp data
  DetIntersection dinter,linter;
  PacSimHit::effect eff=PacSimHit::none;
// create the output PacSimTrack.
  PacSimTrack* strk = new PacSimTrack(tpart);
// make sure particle origin is placed on the 1st winding
  dinter.pathlen = 0.0;
  TrkGeomTraj* htraj =  makeTraj(charge,pos,mom,dinter);
  strk->addTraj(htraj);
// create a simhit for the particle origin
  PacSimHit firsthit(strk,dinter,pos,mom,mom,PacSimHit::creation,gtime);
  strk->addHit(firsthit);
//loop until the particle stops or exits the detector volume
  while(_detector->findNextInter(htraj,dinter) && dinter.pathlen < _maxflt){
    eff = processInter(dinter,mom,pos,strk);
    if(!continues(eff,mom.mag())) break;
// create a new trajectory piece if necessary, and add it to the simtrack trajectory
    if(momchange(eff)){
      htraj = makeTraj(charge,pos,mom,dinter);
      strk->addTraj(htraj);
    }
  }
  // finalize the simtrack if it has any content
  if(strk->getHitList().size() > 0)
    strk->finalize();
  else {
    delete strk;
    strk = 0;
  }
  // return the PacSimTrack object
  return strk;
}

PacSimHit::effect
PacSimulate::processInter(const DetIntersection& dinter, Hep3Vector& mom, HepPoint& pos, PacSimTrack* strk) const {
  PacSimHit::effect retval = PacSimHit::none;
  const Trajectory* htraj = dinter.trajet;
  Hep3Vector oldmom;
// Update the position and momentum direction at intersection. This accounts for bending
  if(htraj->curvature(dinter.pathlen) != 0.0){
    const PacHelix* helix = dynamic_cast<const PacHelix*>(htraj);
    if(helix != 0){
// update for normal bending
      double mommag = mom.mag();
      helix->getInfo(dinter.pathlen,pos,mom);
      oldmom = mom;
      oldmom.setMag(mommag);
// compuate the inhomgeneous field integral and corresponding momentum change
      double range[2] = {helix->lowRange(),dinter.pathlen};
      BFieldIntegrator fieldint(*_bfield,Hep3Vector(0.0,0.0,helix->bnom()));
      Hep3Vector dmom = helix->charge()*fieldint.deltaMomentum(helix,range)/mommag;
// update momentum DIRECTION
      mom += dmom;
// update momentum, making sure the magnitude doesn't change
      mom.setMag(mommag);
    } else {
      cout << "Can't cast trajectory to helix " << endl;
    }
  } else {
  // straight trajectory, simply update position, momentum vector is constant
    pos = htraj->position(dinter.pathlen);
  }
  if(dinter.delem != 0){
  // convert to PacDetElem
    const PacDetElem* pelem = dynamic_cast<const PacDetElem*>(dinter.delem);
  // double-process this element if we're in an overlap
    int nover(1);
    if (pelem->overlapFraction() != 0.0 && pelem->overlapFraction() > RandFlat::shoot(_randengine))nover = 2;
    int iover(0);
    do {
      iover++;
        // check if we're in a gap
      if( pelem->gapFraction() == 0.0 || pelem->gapFraction() < RandFlat::shoot(_randengine)) {
  // decide what effect has occured (if any).  This updates the momentum and simtrack
        retval = _effect.detEffect(dinter,pos,mom,strk);
      }
  // loop till we're through with overlaps or the particle stops
    } while(iover < nover && continues(retval,mom.mag()));
  } else {
// no element means a field update; return pure bend
    retval = PacSimHit::bend;
    PacSimHit newhit(strk,dinter,pos,oldmom,mom,retval,strk->timeOfLastHit());
    strk->addHit(newhit);
  }
  return retval;
}

bool
PacSimulate::nextIntersection(DetElemList::const_iterator ielem,
			      const TrkGeomTraj* traj,DetIntersection& dinter) const {
  // cast to the appropriate type of element
  const PacDetElem* pelem;
  if( (pelem = dynamic_cast<const PacDetElem*>(*ielem)) != 0)
    return pelem->intersect(traj,dinter);
  else
    cerr << "Error: unknown element " << *ielem << endl;
  return false;
}

TrkGeomTraj*
PacSimulate::makeTraj(double charge,const HepPoint& pos,const Hep3Vector& mom,DetIntersection& dinter) const {
  TrkGeomTraj* retval(0);
  static HepVector hpars(5);
  // charged particles
    // check for bfield
  double bz = _bfield->bFieldZ(pos);
  if(charge != 0 && fabs(bz) > 0.0){
  // compute helix parameters and create helix trajectory
    TrkHelixUtils::helixFromMom(hpars,dinter.pathlen,pos,mom,charge,bz);
// pathlen is updated by the above call, it could now be longer than _maxflt
    PacHelix* htraj = new PacHelix(hpars,dinter.pathlen,dinter.pathlen+_maxflt);
  // check
    HepPoint newpos = htraj->position(dinter.pathlen);
    if(pos.distanceTo(newpos) > 1e-3)
      cerr << "error computing helix and localflight" << endl;
    dinter.trajet = htraj;
// set reference values for this trajectory
    htraj->setReference( bz, mom.mag());
// calculate and set the range on this traj based on field inhomgenity
    setRange(htraj);
    retval = htraj;
  } else {
  // neutral particle, or no BField; create line traj
    retval = new TrkLineTraj(pos,mom,_maxflt);
  // line traj fltlen starts at the point.
    dinter.pathlen = 0.0;
    dinter.trajet = retval;
  }
  assert(retval != 0);
  return retval;
}

void 
PacSimulate::setRandomEngine(HepRandomEngine* engine){
  _randengine = engine;
  _effect.setRandomEngine(engine);
  _matint.setRandomEngine(engine);
}

GVertex*
PacSimulate::makeGVertex(GTrack* mother,
const HepPoint& position,double time,
  GVertex::Cause cause, bool terminal) {
  /* Make new vertex */
  GVertex *gVertex = new GVertex();
  assert(gVertex != 0);
  gVertex->setPosition( position);
  gVertex->setCause( cause );
  gVertex->setParentTrimMarker( 0 );
  gVertex->setParentTrack( mother );
  gVertex->setTime( time);
// if this is a terminal vertex, terminate the mother and set the flag
  gVertex->setTerminal( terminal );
  if(terminal)mother->setTerminalVertex( gVertex ); 
  return gVertex;
}

GTrack* 
PacSimulate::makeGTrack(GVertex* origin,
const HepLorentzVector& p4,const PdtEntry* dauEntry) {
  GTrack* gTrack = new GTrack();
  gTrack->setP4(p4);
  gTrack->setPDT( const_cast<PdtEntry*>(dauEntry) ); // bug in GTrack interface:: FIXME!!!
  gTrack->setVertex( origin );
  return gTrack;
}

void
PacSimulate::setRange(PacHelix* htraj) const {
// define traj references
  double range[2] = {htraj->lowRange(),htraj->hiRange()};
// find the position change due to field inhomogeneity taking 1 step along the trajectory
  Hep3Vector delx(0,0,0);
  double fltlen = range[0];
  deltaX(htraj,fltlen,delx);
  double dx = delx.mag();
// if there's no inhomgeneity, no need to alter the range.  Likewise if the range is beyond the current limit
  double nstep = dx > 0.0 ? sqrt(_xtol/dx) : _maxnstep;
  if(nstep < _maxnstep){
// for fractional steps or small # of steps, we're done
    if(nstep < _fewn ){
      range[1] = fltlen + nstep*_tstep;
    } else {
// otherwise, step until we reach tolerance, or the step limit
      unsigned istep(1);
      while(dx < _xtol && istep < _maxnstep && fltlen < _maxflt){
        fltlen += _tstep;
// note: this call accumulates delx, it doesn't reset it
        deltaX(htraj,fltlen,delx);
        dx = delx.mag();
        istep++;
      }
      range[1] = fltlen;
    }
// reset the range for this traj
    htraj->setFlightRange(range);
  }
}

void
PacSimulate::deltaX(const PacHelix* htraj,double fltlen,Hep3Vector& delx) const {
//  htraj->charge(),htraj,htraj->lowRange()
  Hep3Vector bnom(0.0,0.0,htraj->bnom());  
// get information at this flightlength
  HepPoint pos0;
  Hep3Vector dir0;
  htraj->getInfo(fltlen,pos0,dir0);
// get the field at this point relative to the nominal field
  Hep3Vector bvect0 = _bfield->bFieldVect(pos0);
// same information 1 step along the trajectory
  HepPoint pos1;
  Hep3Vector dir1;
  htraj->getInfo(fltlen+_tstep,pos1,dir1);
  Hep3Vector bvect1 = _bfield->bFieldVect(pos1);
// quick check; constant, nominal fields don't need any correction
  if(bvect0 == bnom && bvect1 == bnom)return;
// compute position change due to field difference integral (2nd order calculation)
  Hep3Vector d0 = dir0.cross(bvect0-bnom);
  Hep3Vector d1 = dir1.cross(bvect1-bnom);
// note delx is accumulated, not reset
  delx += (0.5*htraj->charge()*_tstep*_tstep/htraj->mom())*(d0 + d1)*BField::cmTeslaToGeVc;
}


//--------------------------------------------------------------------------
// Name:
//   PacSimulate
// Description:
//   Simulate the passage of a true particle through the PacDetector
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------

#ifndef PacSimulate_HH
#define PacSimulate_HH

class BField;
class PacDetector;
class GTrack;
class PacSimTrack;
class TrkGeomTraj;
class PacCylDetElem;
class DetIntersection;
class PacHelix;
class TParticle;

#include "PacSim/PacSimHit.hh"
#include "PacSim/PacDetEffect.hh"
#include "PacSim/PacMatInt.hh"
#include "PDT/PdtEntry.hh"
#include "DetectorModel/DetElemList.hh"
#include "DetectorModel/DetSetList.hh"
#include "CLHEP/Random/RandomEngine.h"
#include "G3Data/GVertex.hh"

class PacSimulate {
public:
	// Build from the detector (material) and field
	PacSimulate(const BField* bfield, const PacDetector* detector);
	~PacSimulate();
// accessors
	const BField* getBField() const { return _bfield;}
	const PacDetector* getDetector() const { return _detector;}
// Generate a PacTrack from a GTrack.  This function returns ownership.
	PacSimTrack* simulateGTrack(const GTrack*) const;
// Generate a PacTrack from a TParticle.  This function returns ownership.
	PacSimTrack* simulateParticle(const TParticle*) const;
// allow setting the engine.  This operates recursively
  void setRandomEngine(HepRandomEngine* engine);
protected:
// fast intersection for the whole DetSet
	bool nextIntersection(DetElemList::const_iterator ielem,
		const TrkGeomTraj* traj,DetIntersection& dinter) const;
// effects for which the original particle continues
  bool continues(PacSimHit::effect, double mommag ) const;
// effects which change the momentum
  bool momchange(PacSimHit::effect eff) const;
// create an appropriate trajectory piece
  TrkGeomTraj*
    makeTraj(double charge,const HepPoint& pos,const Hep3Vector& mom,DetIntersection& dinter) const;
// process the effect of an intersection
  PacSimHit::effect processInter(const DetIntersection& dinter, Hep3Vector& mom, HepPoint& pos, PacSimTrack* strk) const;
public:
// generate discrete material interaction vertex and daughters.
  void makeInteraction(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const
    { return _matint.makeInteraction(hit,vertex,dautrks);}

// utility functions.  These RETURN OWNERSHIP

  static GVertex* makeGVertex(GTrack* mother,
    const HepPoint& position,double time,
    GVertex::Cause cause, bool terminal);

  static GTrack* makeGTrack(GVertex* origin,
    const HepLorentzVector& p4,const PdtEntry* dauEntry);
private:
// utility functions
  void setRange(PacHelix* htraj) const;
  void deltaX(const PacHelix* htraj,double fltlen,Hep3Vector& delx) const;
  
private:
	const BField* _bfield;
	const PacDetector* _detector;
  DetSetList _vols; // volumes in the detector
	std::vector<DetElemList> _elements; // list of DetectorElements in each volume
	PacDetEffect _effect;
	mutable HepRandomEngine* _randengine;  
// discrete interaction models
  PacMatInt _matint;  
  double _maxflt; // maximum flight length
  double _minmom; // minimum momentum to simulate
  double _mine_shower; // minimum shower energy to simulate
  double _mine_hadshower; // minimum hadronic shower energy to simulate
  double _xtol; // tolerance for trajectory deviation
  double _tstep; // trajectory step guess for field integration
  double _fewn; // how many steps to extrapolate field integral before explicit stepping
  double _maxnstep; // cache
  
};

#endif // PacSimulate_HH

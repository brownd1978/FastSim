//--------------------------------------------------------------------------
// Name:
//    PacSimHit
// Description:
//    Information struct for a simulated hit (energy deposit) computed in PacSimulate
// Environment:
//    Software developed for PACRAT / SuperB
// Copyright Information:
//	  Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//    Dave Brown 23 APR 2008
//------------------------------------------------------------------------

#ifndef PacSimHit_HH
#define PacSimHit_HH

#include "DetectorModel/DetIntersection.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/HepPoint.h"
#include "PacGeom/PacDetElem.hh"
#include "PacSim/PacSimHitMergeInfo.hh"

class PacSimTrack;
class DetElem;
class PacShowerInfo;
class PacDKChainInfo;
class PacPieceTraj;
class PdtEntry;

class PacSimHit {
public:
	// none means no interaction
	// creation means the particle is created here
	// mark means no material, hit exists to mark particle position for measurements
	// normal means adiabatic energy loss and scattering
	// stop means particle does not exit material and produces no daughters
	// interact means hadronic (nuclear) interaction, with destruction of original particle and daughter creation
	// brems means photon radiation by electron
	// compt means compton scattering, with electron emission
	// convert means photon conversion into e+e- pair
	// decay means electroweak decay
	// shower means EM cascade.  The simhit represents part of the shower
	// hadshower means hadronic cascade.  The simhit represents part of the shower
	// in processes 'normal' and below the particle is assumed to continue to exist and propagate
	enum effect {none=0,creation,mark,brems,compton,bend,normal,stop,interact,convert,decay,shower,hadshower,neffects};

	PacSimHit();
// construct from numbers
	PacSimHit(const PacSimTrack* strk,const DetIntersection& dinter,const HepPoint& point,
	const Hep3Vector& momin, const Hep3Vector& momout,PacSimHit::effect x);
	PacSimHit(const PacSimTrack* strk,const DetIntersection& dinter,const HepPoint& point,
	const Hep3Vector& momin, const Hep3Vector& momout,PacSimHit::effect deteffect, double hitTime);
	PacSimHit(const PacSimHit&);
	PacSimHit& operator=(const PacSimHit&);
	virtual ~PacSimHit();
// simple accessors
  const PacSimTrack* getSimTrack() const { return _strk;}
  const PdtEntry* pdt() const;
	const DetIntersection& detIntersection() const { return _dinter;}
  const HepPoint& originalPosition() const { return _pos;}
  const HepPoint& position() const;
  double time() const { return( _time ); }
	const Hep3Vector& momentumIn() const { return _pin; }
	const Hep3Vector& momentumOut() const { return _pout;}
	const Hep3Vector& momentum() const { return momentumIn();}
	Hep3Vector momentumChange() const { return _pout-_pin;}
  double momChange() const { return _pout.mag() - _pin.mag();}
	effect detEffect() const { return _effect;}
// name the effects
	static const char* detEffectName(effect);

  const PacPieceTraj* globalTraj() const { return _globtraj;}
  double globalFlight() const { return _globlen;}
  double energyChange() const;

  PacSimHitMergeInfo* mergeInfo() {return _merge;}
  const PacSimHitMergeInfo* mergeInfo() const {return _merge;}

// access to 'decorators'.  These can be null pointers, always check!!!
  const PacShowerInfo* showerInfo() const { return _shower;}
  const PacDKChainInfo* decayInfo() const { return _decay;}
// derived information
  const PacDetElem* detElem() const;
// modify specific hit types.  This object will TAKE OWNERSHIP of the argument objects if successful
  bool addShowerInfo(const PacShowerInfo&);
  bool addDecayInfo(PacDKChainInfo*);
  void addMergeInfo(PacSimHitMergeInfo*);
// set the global trajectory
  void setGlobalTraj(const PacPieceTraj* gtraj) { _globtraj = gtraj;}
// set the global flightlength
  void setGlobalFlight(double globlen) { _globlen = globlen;}
  // set the hit time
  void setTime( double time ) { _time = time; }
  // print hit contents
  void print( std::ostream& o );

protected:
  const PacSimTrack* _strk; // associated simtrack
	DetIntersection _dinter; // intersection with this element
  const PacPieceTraj* _globtraj; // global trajectory
  double _globlen;  // flightlength along the global (piecewise) trajectory of this intersection point
	HepPoint _pos;  // position at element
  double _time; // time of the hit
	Hep3Vector _pin;  // particle momentum at entrance
	Hep3Vector _pout;  // particle momentum at exit
	effect _effect; // effect of traversing this element
  PacShowerInfo* _shower; // shower info (if appropriate)
  PacDKChainInfo* _decay; // decay info (if appropriate)
  PacSimHitMergeInfo* _merge; // information about hit merging (if appropriate)
  mutable const PacDetElem* _delem; // cache of pac detector element
	static const char* effectNames[neffects]; // effect names
};

#endif


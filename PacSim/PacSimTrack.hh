//--------------------------------------------------------------------------
// Name:
//   PacSimTrack
//  Description:
//   Simulated particle in Pacrat
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#ifndef PacSimTrack_hh
#define PacSimTrack_hh

#include <vector>
#include "G3Data/GTrack.hh"
#include "DetectorModel/DetIntersection.hh"
#include "PacSim/PacSimHit.hh"
#include "PacGeom/PacPieceTraj.hh"
class PdtEntry;
class PacSimulate;
class TParticle;

// NA
#include "G3Data/GVertex.hh"

using namespace std;

class PacSimTrack{

public:
// constructfrom a GTrack
  PacSimTrack(const GTrack*);
// construct from a TParticle
  PacSimTrack(const TParticle*);
// copy
  PacSimTrack(const PacSimTrack&);
  PacSimTrack& operator =(const PacSimTrack&);
  // Allow sorting by momentum
  bool operator<(const PacSimTrack& pst) const 
  {return _gtrk->p4().vect().mag() < pst.getGTrack()->p4().vect().mag();}
  bool operator>(const PacSimTrack& pst) const 
  {return _gtrk->p4().vect().mag() > pst.getGTrack()->p4().vect().mag();}
  ~PacSimTrack();
// accessors
  const GTrack* getGTrack() const {return _gtrk;}
  const TParticle* getTParticle() const { return _tpart;}
  const PacPieceTraj* getTraj() const {return _traj;}
  const std::vector<PacSimHit>& getHitList() const {return _hits;}
// NB: all explicit references to pdt, etc need to be removed.  I need a PacSimParticle type
// to hide these, and then eventually move them away FIXME!!!!
// derived information
  const PdtEntry* pdt() const { return _pdt; }
// explicit accessors which don't depend on Pdt or gtrack: all new code should reference these instead
//  particle code
  int pdgId() const;
  double particleMass() const;
// particle momentum
  Hep3Vector particleMom() const { return _hits[0].momentumIn();}
// modifiers; add hit to this track.  This also updates the global flight length in the hit
  bool addHit(const PacSimHit& hit);
// add new trajectory piece to this track.  This takes the global flightlength of the last added
// hit as starting point
  bool addTraj(TrkGeomTraj*);
// truncate the simtrack at a given global flightlength.  This also truncates the trajectory
  void truncate(double fltlen);
// finalize the track after all hits are added.  This sets the correct range on the traj
  void finalize();
// access to the last hit
  const PacSimHit* lastHit() const {
    if(_hits.size()>0)
      return &_hits.back();
    else
      return 0;}
  // Access to timing information
  const double timeAtOrigin() const { return( _timeAtOrigin ); }
  const double timeElapsed()  const { return( _timeElapsed  ); }
  const double timeOfLastHit()  const { return( _timeAtOrigin + 
						_timeElapsed ); }
						
  static double computeDeltaT( double massInGeV, double momentumInGeV, double distanceInCm );

private:
// utility functions
// data members
  const GTrack* _gtrk; // input
  const TParticle* _tpart;
  mutable const PdtEntry* _pdt;
  PacPieceTraj* _traj; // true trajectory
  std::vector<PacSimHit> _hits; // simulated hits
  double _timeAtOrigin; // GEANT time at track creation
  double _timeElapsed; // time since track creation
};

#endif // PacSimTrack_hh

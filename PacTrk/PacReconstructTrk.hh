//--------------------------------------------------------------------------
// Name:
//   PacReconstructTrk
// Description:
//   Simulate the reconstrucion of a charged particle given its simulated passage
//   through the detector.
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------

#ifndef PacReconstructTrk_HH
#define PacReconstructTrk_HH

class BField;
class PacDetector;
class PacSimTrack;
class PacSimHit;
class TrkRecoTrk;
class PacHitOnTrk;
class HelixTraj;
class TrkDifPieceTraj;
class TrkHotList;
class PdtEntry;

#include "TrkBase/TrkFitMaker.hh"
#include "TrkBase/TrkIdManager.hh"
#include "TrkEnv/KalContext.hh"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Random/RandomEngine.h"
#include "DetectorModel/DetIntersection.hh"
#include "PacTrk/PacTrkSimHotMap.hh"
#include "PacTrk/PacTrkHitProximity.hh"

#include <vector>
class PacSimTrack;

// selector functor
class SimTrkSel {
public:
  virtual bool select(const PacSimTrack*) const = 0;
};

class PacReconstructTrk : public TrkFitMaker , public TrkIdManager {
public:
	// Build from the detector (material) and field
	PacReconstructTrk(const BField* bfield,const KalContext* kalcon);
	~PacReconstructTrk();
// accessors
	const BField* getBField() const { return _bfield;}
// access to map
  const PacTrkSimHotMap& simHotMap() const { return _hotmap;}
  const TrkRecoTrk* findTrack(const PacSimTrack* simtrk) const;

// interface
  void makeTracks(std::vector<const PacSimTrack*>& tracks,const SimTrkSel* sel=0) const;
  void makeTracks(std::vector<PacSimTrack*>& tracks,const SimTrkSel* sel=0) const;
  
// allow resetting the random engine.
  void setRandomEngine(HepRandomEngine* engine) { _randengine = engine; }
  
// idmanager interface
	virtual long nextId() { return ++_idnum;}
	virtual long lastId() const { return _idnum;}
	virtual void setMax(long maxid) { _idnum = maxid;}
private:
  
  // create hots for a simtrk.  This is separated from creating the track to allow hit merging.
  // note this must be called BEFORE makeTrack, or that will fail (return 0)
  void makeHots(const PacSimTrack* simtrk) const;

  // reset for next 'event'
  void clearMaps() const;  

  // generate a TrkRecoTrk from the PacSimTrack.  This function returns ownership
  // A null pointer returned means the particle was unreconstructable.
  TrkRecoTrk* makeTrack(const PacSimTrack* simtrk) const;
  

  // Weed away stray hits from track.  Need in absence of real patrec.
  void weedHots(TrkRecoTrk*& trk, int iterCnt=0) const;

  // Simulate pat rec
  void doPatRec(TrkRecoTrk*& trk,const std::vector<PacTrkHitProximity>& closehits) const;

  // hit merging
  void  doHitMerging(const std::vector<PacTrkHitProximity>& closehits) const;
  void findNearbyHits(std::vector<PacTrkHitProximity>& closehits) const;
  bool mergeSiStrips(const PacTrkHitProximity& proximity) const;
  bool mergeSiPixels(const PacTrkHitProximity& proximity) const;
  bool mergeWireHits(const PacTrkHitProximity& proximity) const;

  // Mark hots as used once fit is finalized
  void markHotsUsed(const std::vector<const PacHitOnTrk*>& hots) const;

  // estimate the covariance matrix, given the initial values
  void estimateCovar(const HepVector& sdiff, double herr_avg, double dlen, unsigned nhits,
    const HepVector& seedparams, HepSymMatrix& covar) const;

  // smear the seed parameters according to the covariance, to emulate initial mis-reconstruction
  void smearSeed(const HepSymMatrix& covar,HepVector& params) const;
  
  void createTrajs(const PacSimTrack* simtrk,const std::vector<const PacHitOnTrk*>& hots, HelixTraj*& seed, TrkDifPieceTraj*& ptraj) const;  
  
  void extractIntersections(const PacSimTrack* simtrk, const TrkDifPieceTraj* ptraj,  double lasthot, std::vector<DetIntersection>& dvect) const;
  
  bool goodHots(const std::vector<const PacHitOnTrk*>& hots) const;
  
	const BField* _bfield;
	const KalContext* _kalcon;
	long _idnum;
  bool _fastfit;
  bool _debug;
// define reconstructable tracks
  int _minsim;
  int _minsvt;
  int _mindch;
  float _hitchicut;
  float _trkchicut;
  bool _weedhits;
  bool _dopatrec;
  bool _domerge;
  int _maxweed;
  float _minpt;
  float _seedsmear;
  float _maxgap;
  float _roughsize;
  float _nstripgap;
// default pdt type
  const PdtEntry* _defhypo;
  mutable PacTrkSimHotMap _hotmap;
  mutable HepRandomEngine* _randengine;
	
};

#endif // PacReconstructTrk_HH

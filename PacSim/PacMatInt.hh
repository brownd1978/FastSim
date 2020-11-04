//--------------------------------------------------------------------------
// Name:
//   PacMatInt
//  Description:
//  Class to describe discrete material interactions in Pacrat
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 26 Jan 2009
//------------------------------------------------------------------------
#ifndef PacMatInt_hh
#define PacMatInt_hh
#include "PacSim/PacSimHit.hh"
#include "CLHEP/Random/RandomEngine.h"

#include <vector>

class GVertex;
class GTrack;

class PacMatInt {
  public:
  PacMatInt();
  virtual ~PacMatInt();
// decide if an effect constitutes an interaction
  static bool isMatInt(PacSimHit::effect eff);
// decide if an effect should be the last one on a track
  static bool isTerminal(PacSimHit::effect eff);
// generate an interaction for a hit  
  void makeInteraction(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const;
// allow setting the engine.
  void setRandomEngine(HepRandomEngine* engine) { _randengine = engine;}
protected:
// specific interaction types
  void brems(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const;
  void convert(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const;
  void compton(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const;
  void hadronic(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const;
  
private:
// hadronic interaction parameterization.  The first describe the distribution of
// the number of daughters, given as a poissonian whose constant depends linearly on the kinetic energy
   double _offset;
   double _slope;
   double _minmom_gamma;
   int _maxndau; // maximum # of daughters to allow
   std::vector<double> _nprob; // nucleon probability for N daughters.  Constant for n>=2
   mutable HepRandomEngine* _randengine;
};
#endif

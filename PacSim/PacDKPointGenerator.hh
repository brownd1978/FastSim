// PacDKPointGenerator.hh     26 October 2008
//
// Carol Fabby / Mike Sokoloff 
//



#ifndef PACDKPOINTGENERATOR_HH
#define PACDKPOINTGENERATOR_HH

// add random number engine   caf 05Aug08
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Geometry/HepPoint.h"
#include "PacSim/PacSimTrack.hh"
#include "G3Data/GTrack.hh"
#include "PacSim/PacDKPointInfo.hh"



class PacDKPointGenerator {
public:

  // Constructors

  PacDKPointGenerator();

  //
  // Creates a PacDKPointInfo object and fills
  // it with decay information. Returns ownership. 
  PacDKPointInfo* generateDecayPoint(PacSimTrack* simtrk); 

  // this method finds the decay point associated
  // with the PacSimTrack object and stores the
  // relevant information in the PacDKPointInfo
  // object

  void generateDecayPoint(PacSimTrack* simtrk, PacDKPointInfo& info);

  // Destructors

  virtual ~PacDKPointGenerator();
  // allow setting the engine.
  void setRandomEngine(HepRandomEngine* engine) { _randengine = engine;}

  // add Private section for random generator   caf 05Aug08

private:
  mutable HepRandomEngine* _randengine;

};

#endif   //PacDKPointGenerator.hh

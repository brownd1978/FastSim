// PacDKChainGenerator.hh     04 November 2008
//
//  Mike Sokoloff
//



#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "PacSim/PacDKEvtGen.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtSimpleRandomEngine.hh"

#include "Framework/AppFileName.hh"

#include "PacSim/PacDKPointInfo.hh"
#include "PacSim/PacDKChainInfo.hh"

using namespace std;

#ifndef PACDKCHAINGENERATOR_HH
#define PACDKCHAINGENERATOR_HH

class PacDKChainGenerator {
public:

  // Constructors

  PacDKChainGenerator();
  PacDKChainGenerator(PacDKPointInfo* theDecayPointInfo, 
    PacDKChainInfo* theDecayChainInfo);

  //
  // Creates a PacDKChainInfo object and fills
  // it with decay information. Returns ownership.
  PacDKChainInfo* generateDecayChain(PacDKPointInfo* theDecayPointInfo);

  // this method finds the decay point associated
  // with the PacSimTrack object and stores the
  // relevant information in the PacDKChainInfo
  // object

  void generateDecayChain(PacDKPointInfo* theDecayPointInfo, 
         PacDKChainInfo* info);

  // Destructors

  virtual ~PacDKChainGenerator();

  // add Private section for random generator 

private:

  EvtRandomEngine* _myRandomEngine;
  PacDKEvtGen* _myGenerator; 
  bool  _first;

// names of files to read in, as in GfiEvtGen/GfiEvtGen.hh
  AppFileName _decay;
  AppFileName _pdttable;
};

#endif   //PacDKChainGenerator.hh

// PacDKChainGenerator.cc    04 November 2008
//  revised version 23 November 2008
//
//  Mike Sokoloff
//


#include "BaBar/BaBar.hh"
#include "PacSim/PacDKChainInfo.hh"
#include "PacSim/PacDKChainGenerator.hh"


// add the following to allow use related random number generator
#include "CLHEP/config/CLHEP.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandFlat.h"
#include "GfiBase/GfiRandomManager.hh"

#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
/*******************************************************************************
 * Utility functions
 *******************************************************************************/
// also added from EvtGen/example1.cc

//Define random number fcn used by Jetset
 extern "C" {
//   extern float rlu_();
//   extern float begran_(int *);
//   extern float phoran_(int *);
 }

/*
 float rlu_(){
   return EvtRandom::Flat();
 }
 float begran_(int *){
   return EvtRandom::Flat();
 }

float phoran_(int *){
  return EvtRandom::Flat();
}
*/

//define class for generating random nubers
class EvtCLHEPRandomEngine:public EvtRandomEngine{
public:
  double random();
};

double EvtCLHEPRandomEngine::random(){
  // use the default (static) CLHEP engine, controlled from outside this class
  HepRandomEngine* rng = HepRandom::getTheEngine();
  return RandFlat::shoot(rng);
}

using namespace std;


//
//  the basic constructor instantiates the
//  random number generator it uses and
//  otherwise does nothing
//
PacDKChainGenerator::PacDKChainGenerator() :
  _decay("PacSim/DK.DEC"),
  _pdttable("PDT/pdt.table")
 {
  //add random number engine 
  //Get the random number engine
  _myRandomEngine=new EvtCLHEPRandomEngine();
  _myGenerator = 0;
// the following flag is used to go around a problem
//  related to using EvtGen for "cleanup" when it is
//  not the primary generator.
  _first = true;
}

//  this version of the constructor generates
//  decay point information which is stored
//  in the PacDKPointInfo object
//
PacDKChainGenerator::PacDKChainGenerator(PacDKPointInfo* theDecayPointInfo,
        PacDKChainInfo* theDecayChainInfo) :
  _decay("PacSim/DK.DEC"),
  _pdttable("PDT/pdt.table")
{
  //Get the random number engine
  _myRandomEngine=new EvtCLHEPRandomEngine();
// the following flag is used to go around a problem
//  related to using EvtGen for "cleanup" when it is
//  not the primary generator.
  _first = true;
  this->generateDecayChain(theDecayPointInfo, theDecayChainInfo);
}

//
// Creates a PacDKChainInfo object and fills
// it with decay information. Returns ownership.
PacDKChainInfo*
PacDKChainGenerator::generateDecayChain(PacDKPointInfo* theDecayPointInfo){
  PacDKChainInfo* theDecayChainInfo = new PacDKChainInfo();
  generateDecayChain(theDecayPointInfo, theDecayChainInfo);
  return theDecayChainInfo;
}

//
// this method finds the particle type and momentum
// associated with the PacDKPointInfo object, 
// generates a decay for it, and stores the
// relevant information in the PacDKChainInfo object.
void
PacDKChainGenerator::generateDecayChain(PacDKPointInfo* theDecayPointInfo,
        PacDKChainInfo* theDecayChainInfo){
  assert(theDecayPointInfo!=0);
  assert(theDecayChainInfo!=0);

//  mds 090518  try instantiating the generator here rather than
//  when the object is instantiated to go around issues related
//  to interactions with EvtGen
  if (_first) {
    _myGenerator =  new PacDKEvtGen(_decay.pathname().c_str());
    _first = false;
  }
  EvtParticle *parent;

 
  //  added in parallel with code from EvtGen/example1.cc
  //  Set up the parent particle
  const PdtEntry* pdt = theDecayPointInfo->pdtEntry();
  std::string sName = pdt->name();
  EvtId particleID = EvtPDL::getId(sName);
  double gtrkMass = pdt->mass();
  double decayMom = theDecayPointInfo->decayPointMom();
  double decayEnergy = sqrt(gtrkMass*gtrkMass+decayMom*decayMom);
  double decayPx = decayMom*theDecayPointInfo->decayDirection().x();
  double decayPy = decayMom*theDecayPointInfo->decayDirection().y();
  double decayPz = decayMom*theDecayPointInfo->decayDirection().z();
  EvtVector4R pInit(decayEnergy, decayPx, decayPy, decayPz);

 // Tell GfiRandomManager that we are generating events, so it's safe
  // to generate random numbers:
  GfiRandomManager::setEventGeneration(true); 
  parent=EvtParticleFactory::particleFactory(particleID,pInit);
  theDecayChainInfo->decayChainInit(parent);
  _myGenerator->generateDecay(parent);
  GfiRandomManager::setEventGeneration(false);
//   cout<<"decayed: "; 
//     parent->printParticle();  //  should this be here ??  081120 mds
    
  int nDaughters = parent->getNDaug();
  for (int i = 0; i < nDaughters; i++) {
     EvtParticle*  aDaughter = parent->getDaug(i);
     theDecayChainInfo->addDaughter(aDaughter);
  } // end of "for (int i =0, ..." 

}

// a basic destructor
PacDKChainGenerator::~PacDKChainGenerator(){
  delete _myRandomEngine;
  if(_myGenerator) delete _myGenerator;
}

//  PacDKPointGenerator.cc   26 October 2008
//   revised version 23 November 2008
//
//  Description:  This class determines whether
//    the track in question (specified by both
//    the GTrack* associated with the PacSimTrack* arguments 
//    of the constructor) decays inside the detector
//    volume.  If so, then it find the decay point
//    in space and the corresponding hit index in
//    the PacSimTrack*. All results are passed to the
//    PacDKPointInfo object which is the second argument
//    of the constructor.
//
//  Carol Fabby, Mike Sokoloff  
//

#include "BaBar/BaBar.hh"
#include "PacGeom/PacPieceTraj.hh"
#include "PacSim/PacDKPointGenerator.hh"
#include "PacSim/PacDKPointInfo.hh"
#include "PacSim/PacSimTrack.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtLund.hh"
#include "G3Data/GTrack.hh"
#include <iostream>
#include <assert.h>
#include <float.h>

// add random generator   caf 05Aug08
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandExponential.h"


using namespace std;


//
//  the basic constructor instantiates the
//  random number generator it uses and
//  otherwise does nothing
//
PacDKPointGenerator::PacDKPointGenerator() {
  //add random number engine   caf 05Aug08
  //Get the random number engine
  _randengine = HepRandom::getTheEngine();
}

//
// Creates a PacDKPointInfo object and fills
// it with decay information. Returns ownership. 
PacDKPointInfo* 
PacDKPointGenerator::generateDecayPoint(PacSimTrack* simtrk){
  PacDKPointInfo* ret = new PacDKPointInfo();
  generateDecayPoint(simtrk, *ret); 
  return ret; 
}

//
// this method finds the decay point associated
// with the PacSimTrack object and stores the
// relevant information in the PacDKPointInfo
// object
void
PacDKPointGenerator::generateDecayPoint(PacSimTrack* simtrk, 
        PacDKPointInfo& info){
  assert(simtrk!=0);
//  provide default values for all the class variables
 
  const GTrack* theGTrack = simtrk->getGTrack();
  const PacPieceTraj* simtraj = simtrk->getTraj();
  double smax = simtraj->lowRange();
  info.setDecayPoint(simtraj->position(smax));

  //Get the particle lifetime from pdt file

  const PdtEntry* pdt = theGTrack->pdt();
  assert(pdt != 0);
// as of Nov. 20, 2008 each PacDKPointInfo has a PdtEntry data member.
// set it here
  info.setPdtEntry(pdt);


  bool mightDecayInDetector(false);
  double properDecayLength(-1.0); 

  const std::vector<PacSimHit>& hits = simtrk->getHitList();
  double betaGamma;
  double gtrkMass = pdt->mass();
  if (0. == gtrkMass) {
    mightDecayInDetector = false;
  } else
  {
    int lastHit = hits.size()-1;
    betaGamma = hits[lastHit].momentumIn().mag()/gtrkMass;
    if ( (hits[lastHit].globalFlight())/betaGamma < properDecayLength ) {
      mightDecayInDetector = false;
     }
    else {
// note that the "lifetime" is c*tau in cm
      double lifetimeDecayDist = pdt->lifetime();
      properDecayLength = 
        RandExponential::shoot(_randengine,lifetimeDecayDist);
      info.setProperDecayLength(properDecayLength);
      mightDecayInDetector = true;
     }
  }  // end of (0 == gtrkMass) else

  info.setMightDecayInDetector(mightDecayInDetector);

  int decayHitIndex = -99;
  bool decaysInDetector = false;
  double decayPointMom = -99.;
  double decayFlightLength = 0.;
  HepPoint decayPoint;
  Hep3Vector decayDirection;
  if (mightDecayInDetector && hits[0].detEffect()==PacSimHit::creation) {
   double globalProperFlight = 0.;
   for(int i = 1; i < hits.size(); i++) {
     double betaGamma = hits[i].momentumIn().mag()/gtrkMass;
     double previousGlobalFlight = hits[i-1].globalFlight();
     double deltaGlobalFlight =
     hits[i].globalFlight() - previousGlobalFlight;
     double deltaProperFlight = deltaGlobalFlight/betaGamma;
     double enteringGlobalProperFlight = globalProperFlight;
     globalProperFlight = globalProperFlight+deltaProperFlight;
     if (globalProperFlight > properDecayLength) {
       decayHitIndex = i;
       decaysInDetector = true;
       double additionalProperFlight = 
        properDecayLength-enteringGlobalProperFlight;
        decayPointMom = hits[i].momentumIn().mag();
       double additionalFlightLength = betaGamma*additionalProperFlight;
       decayFlightLength = previousGlobalFlight + additionalFlightLength;
       const PacPieceTraj* simtraj = simtrk->getTraj();
       simtraj->getInfo(decayFlightLength, decayPoint, decayDirection);
       break;
     }
   } // end of for loop
// check if the particle stopped: those decay at the stopping point with 0 momentum
   if(!decaysInDetector && hits.back().detEffect()==PacSimHit::stop){
     decayHitIndex = hits.size();
     decaysInDetector = true;
     decayPointMom = 0.0;
     decayFlightLength = hits.back().globalFlight();
     decayPoint = hits.back().position();
     decayDirection = hits.back().momentumIn().unit();
   }
  }  // end of if (mightDecayInDetector)
  
  info.setDecaysInDetector(decaysInDetector);
  info.setDecayPoint(decayPoint);
  info.setDecayDirection(decayDirection);
  info.setDecayPointMom(decayPointMom);
  info.setDecayHitIndex(decayHitIndex);
  info.setDecayFltLen(decayFlightLength);
}


// a basic destructor
PacDKPointGenerator::~PacDKPointGenerator(){
}

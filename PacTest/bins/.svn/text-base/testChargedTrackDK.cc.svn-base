//
/* testTrackReconstruction

//  add code to testTrackReconstruction.cc 081018 from
//  EvtGen/example1.cc to see if I can generate the
//  daughters of particles which decay in the detector.

//  revised 23 November 2008


 *
 * Generate several tracks, fit them with KalmanTrack, and analyze the results.
 */

#include <cmath>
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <time.h>

//
// add the following two includes 081022 to use the strcmp command - mds
// following EvtGen/testEvtGen.cc
#include <string.h>
#include <stdio.h>
//

#include <TTree.h>
#include <TBranch.h>
#include <Gtypes.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TPolyLine3D.h>
#include <TPolyMarker3D.h>
#include <TRandom3.h>

#include "BField/BFieldFixed.hh"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/Rotation.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/config/TemplateFunctions.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/config/CLHEP.h"
// add the following from EvtGenBase to allow
//  generation of decays
//  ...
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtVector4R.hh"

#include "Framework/AppFileName.hh"
//  ...
#include "KalmanTrack/KalRep.hh"
#include "KalmanTrack/KalInterface.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtPid.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/TrkDifPieceTraj.hh"
#include "TrkBase/HelixTraj.hh"
#include "TrkBase/TrkDifPieceTraj.hh"
#include "TrkBase/TrkMomCalculator.hh"
#include "TrkBase/TrkPoca.hh"
#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkHelixUtils.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "BbrGeom/TrkPieceTraj.hh"
#include "DetectorModel/DetSet.hh"

#include "PacEnv/PacConfig.hh"
#include "PacDetector/PacCylDetector.hh"
#include "PacEnv/PacBuildEnv.hh"
#include "PacSim/PacSimulate.hh"
#include "PacTrk/PacReconstructTrk.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PacGeom/PacHelix.hh"
#include "PacGeom/PacPieceTraj.hh"

#include "ProxyDict/Ifd.hh"
#include "AbsEnv/AbsEnv.hh"
#include "DetectorModel/DetSet.hh"
#include "BField/BField.hh"

#include "G3Data/GVertex.hh"
#include "G3Data/GTrack.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RanecuEngine.h"

//  add the following for testing PacDK code 
#include "PacSim/PacDKChainInfo.hh"
#include "PacSim/PacDKChainGenerator.hh"
#include "PacSim/PacDKPointInfo.hh"
#include "PacSim/PacDKPointGenerator.hh"

//  add the following from EvtGen/example1.cc
//  Sample test program for running EvtGen
//
//  Created 1999-12-27 Ryd/Lange
//
#include "Experiment/Experiment.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGen/EvtGen.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtSimpleRandomEngine.hh"

using namespace std;

#define RNGSEED 9082459

// add the following to allow use related random number generator
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandExponential.h"

/*******************************************************************************
 * Utility functions
 *******************************************************************************/
// also added from EvtGen/example1.cc

//Define random number fcn used by Jetset
 extern "C" {
   extern float rlu_();
   extern float begran_(int *);
   extern float phoran_(int *);
 }

 float rlu_(){
   return EvtRandom::Flat();
 }
 float begran_(int *){
   return EvtRandom::Flat();
 }

float phoran_(int *){
  return EvtRandom::Flat();
}
// xxx/
// xxx///define class for generating random nubers
// xxx/class EvtCLHEPRandomEngine:public EvtRandomEngine{
// xxx/public:
// xxx/  double random();
// xxx/};
// xxx/
// xxx/double EvtCLHEPRandomEngine::random(){
// xxx/  static HepJamesRandom randengine;
// xxx/  return randengine.flat();
// xxx/}
// xxx/
// xxx/
/*******************************************************************************
 * Main
 *******************************************************************************/

int main(int argc, char* argv[]) {


  gconfig.verbose(true);

  gconfig.parsefile("../PacTest/testChargedTrackDK.xml");
  if(argc <= 1)
  gconfig.parsefile("../PacDetector/pacrat_BaBar.xml");
  gconfig.parseargs(argc, argv);

 // build environment.  This creates the BField, PID tables, materials, etc.
  PacBuildEnv penv;
  penv.buildCore();
  penv.buildTrk();

 // get back the bfield
  const BField* bfield = Ifd<BField>::get(gblPEnv,"Default");
 // build detector
  PacCylDetector* detector = new PacCylDetector();
  
  HepRandomEngine* engine = new RanecuEngine();
	HepRandom::setTheEngine(engine);
	

 // put the DetectorModel into the event 
  if(! Ifd< DetSet >::put( gblPEnv,const_cast<DetSet*>(detector->detectorModel()),"Tracking Set"))
  cout << "Can't put Detector Set" << endl;

    // Read helix generation parameters 
//  const int numpoints = gconfig.getint("numpoints", 200);
  double flightlen_min = double(gconfig["flightlen_min"]);
  double flightlen_max = double(gconfig["flightlen_max"]);
  double pt_mean = double(gconfig["pt_mean"]);
  double pt_sigma = double(gconfig["pt_sigma"]);
  double pz_mean = double(gconfig["pz_mean"]);
  double pz_sigma = double(gconfig["pz_sigma"]);
  double r0_sigma = double(gconfig["r0_sigma"]);

  cout << "some params from xml file" << flightlen_min << "  " 
       << flightlen_max << "   " << pt_mean << "   " 
       <<  pt_sigma << "   " <<  pz_mean  << "   "
       <<  pz_sigma << "   " << r0_sigma << endl;
//
//  add an instantiation of an EvtGen object as it
//  seems that some intialization for 
//  PacDecayTable::readDecayFile is done in the 
//  in code invoked by EvtGen.
  AppFileName pdttable("PDT/pdt.table");
  AppFileName stdDecay("EvtGen/DECAY.DEC");
  EvtGen* anEvtGen = new EvtGen(stdDecay.pathname().c_str(),
                                 pdttable.pathname().c_str(),
                                 0,0);

//  instantiate a PacDKPointInfo for passing decay point
//  information between the PacDKPointGenerator object and 
//  this code.
  PacDKPointInfo* dkPointInfo= new PacDKPointInfo();
  PacDKPointGenerator* dkPointGen = new PacDKPointGenerator();
  PacDKChainGenerator* dkChainGen = new PacDKChainGenerator();
  PacDKChainInfo* dkChainInfo = 0;

    // Prepare to construct tracks 
  int numtracks = int(gconfig["numtracks"]);
  cout << "numtracks from xml file = " << numtracks << endl;

  const int rndseed = gconfig.getint("rndseed", 0);
  TRandom3 rng(RNGSEED + rndseed);

//   bool writetracks = gconfig.getbool("writetracks", false);
   bool veryVerbose = gconfig.getbool("veryVerbose", false);
   bool verbose = gconfig.getbool("verbose", false);

 //Create File  test.root
  const char* outputfile = gconfig.get("outputfile", "test.root");
  TFile file(outputfile,"RECREATE");
 //Create Directory test.root:/chargedtrj/
  TDirectory dirchtraj ("chgtrj", "chgtrj","",0);
 //Create Tree to store decay point info
  TTree* decayPointTree = new TTree("decayPoint", "DecayPoint");


//  set up ROOT-tuple stuff for decay point
 Int_t lundID;
 Bool_t mightDecay;
 Bool_t doesDecay;
 Double_t decayLength;
 Double_t magMom;

 decayPointTree->Branch("lundID", &lundID, "lundID/I");
 decayPointTree->Branch("mightDecay", &mightDecay, "mightDecay/B");
 decayPointTree->Branch("doesDecay", &doesDecay, "doesDecay/B");
 decayPointTree->Branch("decayLength", &decayLength, "decayLength/D");
 decayPointTree->Branch("magMom", &magMom, "magMom/D");
//

//  int tracknum = 1;
  const int printfreq = gconfig.getint("printfreq", 100);

//  variables added to study decays in flight
  double globalProperFlight;
 
  PacSimulate sim(bfield,detector);
	sim.setRandomEngine(engine);
	detector->setRandomEngine(engine);

  for(int m = 0; m < numtracks; m++) {
   if(0 == (m+1)%printfreq) {
   printf("Count: %i \n",m+1);
  }
        // Generate track parameters 
  double x = rng.Gaus(0, r0_sigma);
  double y = rng.Gaus(0, r0_sigma);
  double z = rng.Gaus(0, r0_sigma);
  HepPoint pos(x, y, z);
  double pt   = fabs(rng.Gaus(pt_mean, pt_sigma));         // transverse momentum
  double phi   = rng.Uniform(0, 2*M_PI);
  double pz   = rng.Gaus(pz_mean, pz_sigma);         // longitudinal momentum
  Hep3Vector mom(pt*cos(phi), pt*sin(phi), pz);  
  double flightlen = rng.Uniform(flightlen_min, flightlen_max);

  //Create Initial Track
  GVertex gvtx;
  gvtx.setPosition(pos);
  GTrack gtrk;
  PdtPdg::PdgType pdgid = (PdtPdg::PdgType)gconfig.getint("PdtPdg",13);
  PdtEntry* pdt = Pdt::lookup(pdgid);
  double q = pdt->charge();
         double gtrkMass = pdt->mass();
  gtrk.setVertex(&gvtx);
  gtrk.setPDT(pdt);
  HepLorentzVector p4; p4.setVectM(mom,pdt->mass());
  gtrk.setP4(p4);


  //Simulate Track through detectors
  PacSimTrack* simtrk = sim.simulateGTrack(&gtrk);
  const PacPieceTraj* simtraj = simtrk->getTraj();
 
  // find out if/where simtrk decays 
  dkPointGen->generateDecayPoint(simtrk, *dkPointInfo);

  // test the getPacSimTrack() method
  const PdtEntry* theDecayPointPdt = dkPointInfo->pdtEntry();

  std::string sName = pdt->name();
  std::string sDecayPointName = theDecayPointPdt->name();

  // Local objects, will go out of scope
  PacDKPointInfo theCopy = *dkPointInfo;
  PacDKPointInfo theSecondCopy(*dkPointInfo);
  // Pointers, will remain in memory
  PacDKPointInfo* thirdCopy = new PacDKPointInfo();
  (*thirdCopy) = (*dkPointInfo);
  PacDKPointInfo* fourthCopy = new PacDKPointInfo(*dkPointInfo);
  PacDKPointInfo* theSecondTrackDecay = dkPointGen->generateDecayPoint(simtrk);

//  find the decay products if the track decays in the detector
//  the code should assert if the PacSimTrack does not decay in detector.
  if (dkPointInfo->decaysInDetector()) {
    // dkChainInfo was "declared" earlier so that it will stay in scope
    // outside this "if (..) { ... }"
    dkChainInfo = dkChainGen->generateDecayChain(dkPointInfo);
  } // end of "if (dkPointInfo->decaysInDetector)"

  //Fill initial parameters
  HepVector simparams(5);
  TrkHelixUtils::helixFromMom(simparams,flightlen,pos,mom,q,*bfield);
  
  //Generated Track
  PacHelix gentraj(simparams,flightlen,simtraj->hiRange());

  
//   EvtId 
  
  if (verbose)  {

    cout << endl << endl << "  ------------------------------" << endl;
    cout << "track number = " << m << endl;
    cout << "pdgid and pdt->name() =   " << pdgid << "   " << 
     pdt->name() << endl << endl;
    cout << " name and sName = " << pdt->name() << "  " << sName << endl;
    cout << " sDecayPointName = " << sDecayPointName << endl;
    cout << endl;
    cout << "Initial parameters " << simparams << endl;

    const std::vector<PacSimHit>& hits = simtrk->getHitList();

    if (veryVerbose) {
     cout << "[Hit locations]" << endl;
     globalProperFlight = 0.;
     for(int i = 0; i < hits.size(); i++) {
       cout << " SimHit " << i << ": " << hits[i].position() << " effect " << hits[i].detEffect()
         << " dmom " << hits[i].momentumChange().mag() << endl;
         if(hits[i].detIntersection().delem != 0)
           cout << " element " << hits[i].detIntersection().delem->elementName() << endl;
       cout << "momentumIn =  " << hits[i].momentumIn().mag() << endl;
       cout << "momentumOut = " << hits[i].momentumOut().mag() << endl;
       cout << "effect =      " << hits[i].detEffect() << endl;
  //       cout << " momentum   = " << hits[i].momentum().mag << endl;
       cout << "pathlength  = " << hits[i].detIntersection().pathlen << endl;
       cout << "globalFlight =  " << hits[i].globalFlight() << endl;
       double betaGamma = hits[i].momentumIn().mag()/gtrkMass;
       cout << "betaGamma =  " << betaGamma << endl;
       double previousGlobalFlight;
     
       if (0 == i) {
        previousGlobalFlight = 0.;
        }
       else {
        previousGlobalFlight = hits[i-1].globalFlight(); 
        }
       double deltaGlobalFlight = 
        hits[i].globalFlight() - previousGlobalFlight;
       double deltaProperFlight = deltaGlobalFlight/betaGamma;
       globalProperFlight = globalProperFlight+deltaProperFlight;
       cout << "delta and global ProperFlight  = " <<  deltaProperFlight
            << "   "  << globalProperFlight << endl;
  
       cout << endl << endl;
      } // end of for loop
  
    }   // end of if (veryVerbose)
  
     
  
    float l = dkPointInfo->properDecayLength();
    bool b = dkPointInfo->mightDecayInDetector();
    cout << "dkPointInfo->properDecayLength = " <<
      l  << endl;
    cout << "theCopy.properDecayLength = " <<
             theCopy.properDecayLength() << endl << endl;
    cout << "theSecondCopy.properDecayLength = " <<
             theSecondCopy.properDecayLength() << endl << endl;

    cout << "dkPointInfo->mightDecayInDetector = " <<
      b  << endl;
    cout << "theCopy.mightDecayInDetector() = " <<
             theCopy.mightDecayInDetector() << endl << endl;

    cout << "dkPointInfo->decaysInDetector() = " <<
      dkPointInfo->decaysInDetector() << endl;
    cout << "theCopy.decaysInDetector() = " <<
             theCopy.decaysInDetector() << endl;

//  begin the "if (dkPointInfo->decaysInDetector())" test  ****
    if (dkPointInfo->decaysInDetector()) {
      int iHit = dkPointInfo->decayHitIndex();
      cout << "dkPointInfo->decayHitIndex =  " <<
        dkPointInfo->decayHitIndex() << endl;
      cout << " SimHit " << iHit << ": " << hits[iHit].position() 
        << " effect " << hits[iHit].detEffect()
        << " dmom " << hits[iHit].momentumChange().mag();
        if(hits[iHit].detIntersection().delem != 0)
          cout << " element " << hits[iHit].detIntersection().delem->elementName() 
        << endl;
       cout << "dkPointInfo->decayPoint =  " <<
         dkPointInfo->decayPoint() << endl;
       cout << "dkPointInfo->decayDirection =  " <<
         dkPointInfo->decayDirection() << endl << endl;

       cout << "theCopy->decayHitIndex =  " <<
         theCopy.decayHitIndex() << endl;
       cout << "theCopy.decayPoint =  " <<
         theCopy.decayPoint() << endl;
       cout << "theCopy.decayDirection =  " <<
         theCopy.decayDirection() << endl << endl;

       double previousGlobalFlight;
       double decayIndexGlobalProperFlight=0.;
       globalProperFlight = 0.;
       for(int i = 0; i < dkPointInfo->decayHitIndex()+1; i++) {
         if (0 == i) {
         previousGlobalFlight = 0.;
         }
         else {
          previousGlobalFlight = hits[i-1].globalFlight(); 
         }
         double deltaGlobalFlight = 
           hits[i].globalFlight() - previousGlobalFlight;
         double betaGamma = hits[i].momentumIn().mag()/gtrkMass;
         double deltaProperFlight = deltaGlobalFlight/betaGamma;
         globalProperFlight = globalProperFlight+deltaProperFlight;
         decayIndexGlobalProperFlight = globalProperFlight;
        } // end of for loop
   
        cout << "decayIndexGlobalProperFlight = " <<
                 decayIndexGlobalProperFlight << endl;   
        cout << "dkPointInfo->decayPointMom = " <<
                 dkPointInfo->decayPointMom() << endl;   


      int nFromInfo = dkChainInfo->getNDaughters();
      cout << "nFromInfo =  " << nFromInfo << endl;

//
// add code to check out conversion of EvtParticle's to EvtStdHep's 081120 - mds

        if (veryVerbose) {

//

      const std::vector<const EvtParticle *>& theDaughters 
                                        = dkChainInfo->getDaughters();
      int nFromTheDaughters = theDaughters.size();
      cout << "nFromTheDaughters =  " << nFromTheDaughters << endl;

      for(int i = 0; i < theDaughters.size(); i++) {
        cout << endl << "information about aDaughter" << endl;
        const EvtParticle*  aDaughter = theDaughters[i];    
        aDaughter->printParticle();
        cout << "aDaughter->getId() = " << aDaughter->getId() << endl;
        std::string aDaughterName = EvtPDL::name(aDaughter->getId());
        cout << "aDaughterName = " << aDaughterName << endl;
        int aDaughterStdHep = EvtPDL::getStdHep(aDaughter->getId());
        cout << "aDaughterStdHep = " << aDaughterStdHep << endl;
        EvtVector4R aDaughterP4 = aDaughter->getP4Lab();
        cout << " aDaughterP4 = " << aDaughterP4 << endl;
        cout << " aDaughter->getLifetime() = " << aDaughter->getLifetime()
             << endl;

        }

      } // end of "for (int i =0, ..."

//  end of additions

   } // end of if (dkPointInfo->decaysInDetector)
  }  // end of "if (verbose) .. "

 //  create the ROOT variables for the ROOT-tuple
       lundID = pdt->lundId();
       mightDecay = dkPointInfo->mightDecayInDetector();
       doesDecay =  dkPointInfo->decaysInDetector();
       magMom = mom.mag();
       if (dkPointInfo->decaysInDetector()) {
        decayLength = dkPointInfo->properDecayLength();
       }
       else {
        decayLength = -99.;
       }

 //and fill the ROOT-tuple for this event
       decayPointTree->Fill();
 //  ------------------------------------------------------

  delete thirdCopy;
  delete fourthCopy;
  delete theSecondTrackDecay;

  } //  end of  "for(int m = 1; m <= M-1; m++)" loop over tracks

 decayPointTree->Write();
 delete decayPointTree;

 file.Close();
 cout << endl;
 return 0;
}


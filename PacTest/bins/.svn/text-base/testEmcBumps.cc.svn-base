//===============================================================
//  testEmcBumps:  test mearging
//
//===============================================================
#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <Gtypes.h>

#include <TRandom3.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TH2F.h>
#include <TStopwatch.h>

#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"

#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/TrkDifPieceTraj.hh"
#include "TrkBase/HelixTraj.hh"
#include "TrkBase/TrkMomCalculator.hh"
#include "TrkBase/TrkSimpTraj.hh"

#include "PacEnv/PacConfig.hh"
#include "PacDetector/PacCylDetector.hh"
#include "PacEnv/PacBuildEnv.hh"
#include "PacSim/PacSimulate.hh"
#include "PacGeom/PacDetElem.hh"

#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PacGeom/PacHelix.hh"
#include "PacGeom/PacPieceTraj.hh"
#include "PacGeom/PacMeasurement.hh"

#include "PacEnv/pstring.hh"

#include "ProxyDict/Ifd.hh"
#include "AbsEnv/AbsEnv.hh"
#include "DetectorModel/DetSet.hh"
#include "BField/BField.hh"

#include "G3Data/GVertex.hh"
#include "G3Data/GTrack.hh"

#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RanecuEngine.h"

#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacReconstructEmc.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcXClMoments.hh"
#include "PacEmc/PacEmcClusterEnergySums.hh"
#include "PacEmc/PacEmcClusterSplitter.hh"

using namespace std;

void fillHisto(const PacEmcCluster *cluster, TH2F *hist) {

  std::vector< PacEmcDigi* >::const_iterator iter= cluster->pacEmcDigis()->begin();
  while ( iter!= cluster->pacEmcDigis()->end() ) {
    hist->Fill((*iter)->theta(),(*iter)->phi(),(*iter)->energy());
    iter++;
  }

}


int main(int argc, char* argv[]) {

  // get configuration
  TString conffile= "PacDetector/pacrat_SuperB.xml";
  if ( argc >= 2 ) {
    conffile= argv[1];
  }
  
  gconfig.parsefile(conffile.Data());
  gconfig.parsefile("PacTest/testEmcBumps.xml");
  gconfig.parseargs(argc, argv);

  //const double twoPi= TMath::TwoPi();  
  //  const double onePi= TMath::Pi();

  // build environment.  This creates the BField, PID tables, materials, etc.
  PacBuildEnv penv;
  penv.buildCore();
  penv.buildTrk();

  // get back the bfield
  const BField* bfield = Ifd<BField>::get(gblPEnv,"Default");

  // build detector
  PacCylDetector* detector = new PacCylDetector();
  
  
  /* put the DetectorModel into the event */
  if(! Ifd< DetSet >::put( gblPEnv,const_cast<DetSet*>(detector->detectorModel()),"Tracking Set"))
    cout << "Can't put Detector Set" << endl;

  // Get initial parameter
  PdtPdg::PdgType pdgid1 = (PdtPdg::PdgType)gconfig.getint("PdtPdg1",22);
  PdtPdg::PdgType pdgid2 = (PdtPdg::PdgType)gconfig.getint("PdtPdg2",22);


  HepPoint initpos = gconfig.getHepPoint("initpos");
  double initpmag1 = gconfig.getdouble("initpmag1",1);
  double inittheta1 = gconfig.getdouble("inittheta1",0);
  double initphi1 = gconfig.getdouble("initphi1",0);
  double initpmag2 = gconfig.getdouble("initpmag2",1);
  double inittheta2 = gconfig.getdouble("inittheta2",0);
  double initphi2 = gconfig.getdouble("initphi2",0);

  double rMoliere= gconfig.getdouble("rMoliere", 3.7);
  int    maxIter= gconfig.getint("maxIter", 10);
  double tolerance= gconfig.getdouble("tolerance",0.1);
  double exponent= gconfig.getdouble("exponent", -2.5);

  Hep3Vector initmom1(initpmag1*sin(inittheta1)*cos(initphi1),
		      initpmag1*sin(inittheta1)*sin(initphi1),
		      initpmag1*cos(inittheta1));
  Hep3Vector initmom2(initpmag2*sin(inittheta2)*cos(initphi2),
		      initpmag2*sin(inittheta2)*sin(initphi2),
		      initpmag2*cos(inittheta2));
		     

  // Create GTrack
  GVertex gvtx;
  gvtx.setPosition(initpos);
  GTrack gtrk1, gtrk2;
  PdtEntry* pdt1 = Pdt::lookup(pdgid1);
  PdtEntry* pdt2 = Pdt::lookup(pdgid2);

  HepLorentzVector p41;
  gtrk1.setVertex(&gvtx);
  gtrk1.setPDT(pdt1);
  p41.setVectM(initmom1,pdt1->mass());
  gtrk1.setP4(p41);

  HepLorentzVector p42;
  gtrk2.setVertex(&gvtx);
  gtrk2.setPDT(pdt2);
  p42.setVectM(initmom2,pdt2->mass());
  gtrk2.setP4(p42);
  

  cout << "Particle 1 " << endl;
  cout << " Initial position\n" << initpos << endl;
  cout << " Initial momentum\n" << p41 << endl;
  cout << " Initial theta = " << p41.theta() << endl;
  cout << " Initial phi = " << p41.theta() << endl;
  cout << " Initial p3 = " << p41.rho() << endl;

  cout << "Particle 2 " << endl;
  cout << " Initial position\n" << initpos << endl;
  cout << " Initial momentum\n" << p42 << endl;
  cout << " Initial theta = " << p42.theta() << endl;
  cout << " Initial phi = " << p42.theta() << endl;
  cout << " Initial p3 = " << p42.rho() << endl;


  // Generate the track
  PacSimulate sim(bfield,detector);
  HepRandomEngine* engine = new RanecuEngine();
	HepRandom::setTheEngine(engine);    
  sim.setRandomEngine(engine);
	detector->setRandomEngine(engine);
  PacSimTrack* simtrk1 = sim.simulateGTrack(&gtrk1);
  PacSimTrack* simtrk2 = sim.simulateGTrack(&gtrk2);

  // make Emc clusters
  PacEmcCluster::CentroidMethod algo = (PacEmcCluster::CentroidMethod)gconfig.getint("centroidAlgo",PacEmcCluster::logarithmic);
  PacReconstructEmc* emcreso = new PacReconstructEmc(algo);
  std::vector<PacEmcCluster*> vclusters1 = emcreso->makeEmcCluster(simtrk1);
  std::vector<PacEmcCluster*> vclusters2 = emcreso->makeEmcCluster(simtrk2);
  PacEmcCluster *cluster1= vclusters1.front();
  PacEmcCluster *cluster2= vclusters2.front();


  const char* outputfile = gconfig.get("outputfile", "testEmcBumps.root");
  TFile* file = new TFile(outputfile, "RECREATE");

  long thetalo= cluster1->tciThetaLow();
  if ( thetalo > cluster2->tciThetaLow() ) thetalo= cluster2->tciThetaLow();
  long thetahi= cluster1->tciThetaHi();
  if ( thetahi < cluster2->tciThetaHi() ) thetahi= cluster2->tciThetaHi();

  long philo= cluster1->tciPhiLow();
  if ( philo > cluster2->tciPhiLow() ) philo= cluster2->tciPhiLow();
  long phihi= cluster1->tciPhiHi();
  if ( phihi < cluster2->tciPhiHi() ) phihi= cluster2->tciPhiHi();

  int ntheta = thetahi-thetalo+1;
  int nphi= phihi-philo+1;

  TH2F *hclus1= new TH2F("hclus1","Cluster 1",ntheta,thetalo,thetahi,nphi,philo,phihi);
  TH2F *hclus2= new TH2F("hclus2","Cluster 2",ntheta,thetalo,thetahi,nphi,philo,phihi);
  TH2F *hclus3= new TH2F("hclus3","Merged cluster",ntheta,thetalo,thetahi,nphi,philo,phihi);
  

  fillHisto(cluster1,hclus1);
  fillHisto(cluster2,hclus2);

  cout << "### Merging" << endl;

  PacEmcCluster *merged= new PacEmcCluster(cluster1->centroidAlgo());
  merged->mergeCluster(cluster1);
  merged->mergeCluster(cluster2);

  cout << " original cluster1 " << endl;
  cluster1->print(cout,true);
  cout << " original cluster2 " << endl;
  cluster2->print(cout,true);

  delete cluster1;
  delete cluster2;

  cout << " ### merged " << endl;
  merged->print(cout,true);

  fillHisto(merged,hclus3);


  // split cluster
  if ( merged->nBumps() > 1 ) {
    cout << "######## Split cluster ############" << endl;

    PacEmcClusterSplitter splitter;
    splitter.setRMo(rMoliere);
    splitter.setMaxIterations(maxIter);
    splitter.setTolerance(tolerance);
    splitter.setExponent(exponent);

    std::vector< PacEmcCluster*> splittedClusters;
    
    splitter.splitCluster(merged, splittedClusters);
    
    std::vector< PacEmcCluster*>::const_iterator iter= splittedClusters.begin();

    cout << "  after " << splitter.numIterations() << " iterations ... " << endl;
    while ( iter!=  splittedClusters.end() ) {
      (*iter)->print(cout,true);
      iter++;
    }
  }











  hclus1->Write();
  hclus2->Write();
  hclus3->Write();

  file->Close();
}

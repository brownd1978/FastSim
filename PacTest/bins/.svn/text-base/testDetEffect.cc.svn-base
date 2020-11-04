#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TRandom3.h>
#include "AbsEnv/AbsEnv.hh"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandLandau.h"
#include "CLHEP/Random/RanecuEngine.h"
#include "DetectorModel/DetIntersection.hh"
#include "DetectorModel/DetElem.hh"
#include "DetectorModel/DetType.hh"
#include "DetectorModel/DetMaterial.hh"
#include "GenEnv/GenEnv.hh"
#include "PacSim/PacSimHit.hh"
#include "PacSim/PacSimulate.hh"
#include "PacSim/PacDetEffect.hh"
#include "PacSim/PacShowerInfo.hh"
#include "PacEnv/PacConfig.hh"
#include "PacEnv/PacBuildEnv.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtPid.hh"

int main(int argc, char* argv[]) {
	gconfig.verbose(true);
	gconfig.parsefile("PacTest/testDetEffect.xml");
  gconfig.parseargs(argc, argv);
	bool verbose = gconfig.getbool("verbose", false);
	// build environment
	PacBuildEnv penv;
	penv.buildCore();
	penv.buildTrk();
	// Initialize particle data 
  double plow = gconfig.getdouble("plow",0.20);
	double phigh = gconfig.getdouble("phigh",2.00);
	PdtPdg::PdgType pdgid = (PdtPdg::PdgType)gconfig.getint("PdtPdg",211);
	const PdtEntry* pdt = Pdt::lookup(pdgid);
	const double tlow = gconfig.getdouble("tlow", 0.03);
	const double thigh = gconfig.getdouble("thigh", 0.05);
	//Number of test
	int numtracks = gconfig.getint("numtracks",10);
	//Printing Frequency
	const int printfreq = gconfig.getint("printfreq", 100);
	const char* outputfile = gconfig.get("outputfile", "testDetEffect.root");
	//Detector Material
	const char* matname = gconfig.get("material", "svt-RecoWafer");
	const DetMaterial* mat = gblEnv->getGen()->findDetMaterial(matname);
  assert(mat != 0);
	//Saving result
	Int_t trknum;
	Int_t effect;	
	Double_t thick;
	Double_t pi_mag;
	Double_t pf_mag;
	Double_t pf_theta;
	Double_t pf_phi;
  Double_t eloss_mean;
  Double_t eloss_rms;
	TFile *file = new TFile(outputfile,"RECREATE");
	TTree* trackT = new TTree("tracks","Tracks");
	trackT->Branch("trknum",&trknum,"trknum/I");
	trackT->Branch("effect",&effect,"effect/I");
	trackT->Branch("thick",&thick,"thick/D");
	trackT->Branch("pi_mag",&pi_mag,"pi_mag/D");
	trackT->Branch("pf_mag",&pf_mag,"pf_mag/D");
	trackT->Branch("pf_theta",&pf_theta,"pf_theta/D");
	trackT->Branch("pf_phi",&pf_phi,"pf_phi/D");
	trackT->Branch("eloss_mean",&eloss_mean,"eloss_mean/D");
	trackT->Branch("eloss_rms",&eloss_rms,"eloss_rms/D");

	// Get the random number enginae
	HepRandomEngine* randengine = new RanecuEngine();
	HepRandom::setTheEngine(randengine);
	//Effect
	PacDetEffect deffect;
  deffect.setRandomEngine(randengine);
  PacShowerInfo sinfo;

	for(int i = 0; i < numtracks; i++) {
		if(0 == (i+1)%printfreq) {
			printf("Count: %i \n",i+1);
		}
		//set initial mom
    pi_mag = RandFlat::shoot(randengine,plow,phigh);
		Hep3Vector oldmom(0.0,0.0,pi_mag);
    Hep3Vector newmom(oldmom);
		// set thickness
		double pathlen = RandFlat::shoot(randengine,tlow,thigh);
    thick = pathlen*mat->inverseX0();
    PacSimHit::effect eff = deffect.detEffect(PacSimHit::none,pathlen,mat,pdt,(const PacShowerInfo*)0,newmom,sinfo);
    effect = eff;
		//Save Data
		trknum = i;
		pf_mag = newmom.mag();
    pf_phi = newmom.x()/pf_mag;
    pf_theta = newmom.y()/pf_mag;
    eloss_mean = mat->energyLoss(pi_mag,pathlen,pdt->mass());
    eloss_rms = mat->energyLossRMS(pi_mag,pathlen,pdt->mass());
  	
		trackT->Fill();

		if(verbose) {
			printf("trknum  : %4.d \n",trknum);
			printf("\n");
		}
	}//for
	file->cd();
	trackT->Write();
	delete trackT;	
	return 1;
}


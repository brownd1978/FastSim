#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "DetectorModel/DetMaterial.hh"
#include "PacEnv/PacConfig.hh"
#include "PacEnv/PacBuildEnv.hh"
#include "PacSim/PacEMShower.hh"
#include "PacSim/PacHadShower.hh"
#include "PacSim/PacShowerInfo.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtPid.hh"

using namespace std;

int main(int argc, char* argv[]) {
	gconfig.verbose(true);
	gconfig.parsefile("PacTest/testShowerModel.xml");
	gconfig.parseargs(argc, argv);
//	bool verbose = gconfig.getbool("verbose", false);
	// build environment
	PacBuildEnv penv;
	penv.buildCore();
	// Initialize particle data
	
  HepPoint initpos = gconfig.getHepPoint("initpos");
  Hep3Vector initmom = gconfig.getHep3Vector("initmom");
  PdtPdg::PdgType pdgid = (PdtPdg::PdgType)gconfig.getint("PdtPdg",22);
  PdtEntry* pdt = Pdt::lookup(pdgid);
  cout << "Simulating particle type ";
  pdt->printOn(cout);
    
	//Saving result
	Int_t ienergy;
  Int_t istep;
  Int_t imat;
  Double_t energy;  
  Double_t pathlen;
  Double_t depthlen;
	Double_t Ein;
	Double_t Eout;
	Double_t Edep;
	Double_t din;
  Double_t dout;
	Double_t shin;
  Double_t shout;
  Double_t shstart;
	const char* outputfile = gconfig.get("outputfile", "testShowerModel.root");
	TFile *file = new TFile(outputfile,"RECREATE");
	TTree* showerT = new TTree("showers","showers");
	showerT->Branch("ienergy",&ienergy,"ienergy/I");
	showerT->Branch("istep",&istep,"istep/I");
	showerT->Branch("imat",&imat,"imat/I");
  showerT->Branch("energy",&energy,"energy/D");
  showerT->Branch("pathlen",&pathlen,"pathlen/D");
  showerT->Branch("depthlen",&depthlen,"depthlen/D");
	showerT->Branch("Ein",&Ein,"Ein/D");
	showerT->Branch("Eout",&Eout,"Eout/D");
	showerT->Branch("Edep",&Edep,"Edep/D");
	showerT->Branch("din",&din,"din/D");
	showerT->Branch("dout",&dout,"dout/D");
	showerT->Branch("shin",&shin,"shin/D");
	showerT->Branch("shout",&shout,"shout/D");
	showerT->Branch("shstart",&shstart,"shstart/D");

	//Detector Material
  const int nmaterial(2);
  const char* material[nmaterial] = {gconfig.get("material1", "drc-Quartz"),gconfig.get("material2", "emc-CsI")};
	double thick1 =  gconfig.getdouble("thick1",0.2);
//	double thick2 =  gconfig.getdouble("thick2",26);
	// find material.  We have to deal with the broken DetectorModel interface
  const DetMaterial* mat[nmaterial] = {gblEnv->getGen()->findDetMaterial(material[0]),
    gblEnv->getGen()->findDetMaterial(material[1])};
  // compute physical pathlength
  double dstep = gconfig.getdouble("dstep",0.01);
  double pathstep(0.0);
  // setup shower models
  PacEMShower emshower;
  PacHadShower hadshower;
  bool isem = abs((int)pdt->pdgId())==PdtPdg::e_minus || pdt->pdgId()==PdtPdg::gamma;
  bool charged = pdt->charge() != 0;
  PacShowerModel* shower(0);
  if(isem)
    shower = &emshower;
  else
    shower = &hadshower;
  double cterm(0.5);
  if(charged)
    cterm = -0.5;
  double bterm = 0.5;
	//Number of test
	int numenergy = gconfig.getint("numenergy",1);
	int numsteps = gconfig.getint("numsteps",100);
	double elow =gconfig.getdouble("elow",0.1);
	double ehigh =gconfig.getdouble("ehigh",0.1);
	//Printing Frequency
	const int printfreq = gconfig.getint("printfreq", 100);
  double estep = (ehigh-elow)/numenergy;
  energy = elow;
	for(ienergy = 0; ienergy < numenergy; ienergy++) {
		if(0 == (ienergy+1)%printfreq) {
			printf("Track: %ienergy \n",ienergy+1);
		}
// step through material
    PacShowerInfo sinfo(energy);
    pathlen = 0;
    depthlen = 0;
    imat = 0;
    for(istep=0;istep<numsteps;istep++){
      if(depthlen > thick1)imat = 1;
      double ecrit = 0.8/(1.2+ mat[imat]->zeff());
      if(isem){
        pathstep = dstep/mat[imat]->radiationFraction(1.0);
      } else {
        pathstep = dstep*mat[imat]->intLength();
      }
      if(sinfo.energyOut() > 0.0){
        shower->showerStep(mat[imat],pathstep,pdt,sinfo,sinfo);
		//Save Data
        Ein     = sinfo.energyIn();
        Eout    = sinfo.energyOut();
        Edep    = sinfo.energyDeposit();
        din     = sinfo.depthIn();
        dout    = sinfo.depthOut();
        shin    = sinfo.fractionIn();
        shout   = sinfo.fractionOut();
        if(energy/ecrit > 1.0)
          shstart = emshower.findShowerStart(energy/ecrit,bterm,cterm,din,shin);
        else
          shstart = din;
        showerT->Fill();
        pathlen += pathstep;
        depthlen += dstep;
      }
    }
    energy += estep;
	}//for
	file->cd();
	showerT->Write();
	delete showerT;	
	return 0;
}


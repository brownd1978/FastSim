#include <iostream>
#include <assert.h>
#include <TFile.h>
#include <TH1D.h>
#include "PacEnv/PacConfig.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandLandau.h"
#include "Framework/AppFileName.hh"

using namespace std;

int main(int argc, char* argv[]) {
	gconfig.verbose(true);
	if(argc <= 1){
    gconfig.parsefile(AppFileName("PacTest/testLandau.xml").pathname().c_str());
  }
	const char* outputfile = gconfig.get("outputfile", "testLandau.root");
	TFile* output = new TFile(outputfile, "RECREATE");
	assert(output);

//most probable energy loss.
  double elossmean = gconfig.getdouble("elossmean",1.0);
//energy loss RMS.
  double elossrms = gconfig.getdouble("elossrms",0.5);
//landau curve.  The most probable value is slightly shifted,
//and the (restricted) sigma is wider than 1.  The negative sigma is =1.
//double lanmean = gconfig.getdouble("lanmean",0.0);
  double lanshift = gconfig.getdouble("lanshift",0.0);
  double lanrms = gconfig.getdouble("lanrms",1.0);
  	
	TH1D* landau = new TH1D("Landau","Landau",500,-5.,40.);
	TH1D* DE = new TH1D("DE","DE",500,elossmean-10*elossrms,elossmean+10*elossrms);	
	int numtracks = gconfig.getint("numtracks",10);
	cout << "looping " << numtracks << " times." << endl;
	HepRandomEngine* randengine = HepRandom::getTheEngine();

	
	for(int iloop=0;iloop<numtracks;iloop++){
		//Landau
		double ranlan = RandLandau::shoot(randengine);
		//energy loss
		//double eloss = max(elossmean + (ranlan+lanmean)*elossrms*invlanrms,0.0);
		//double eloss = elossmean+ranlan*elossrms*lanrms+lanmean;
		double eloss = elossmean*(1.+lanshift)+ranlan*elossrms*lanrms;
		
		landau->Fill(ranlan);
		DE->Fill(eloss);
	}
	output->Write();
	return 0;
}

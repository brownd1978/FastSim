//===============================================================
//  testEmcCluster
//
//===============================================================
#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <Gtypes.h>
#include <TBRIK.h>
#include <TTUBE.h>
#include <TString.h>
#include <TNode.h>
#include <TRotMatrix.h>

#include <TRandom3.h>
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <TH2F.h>
#include <TStopwatch.h>
#include <TGraph.h>
#include <TGraph2D.h>

#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtPid.hh"
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

using namespace std;

void DrawCluster(const char* name, const PacEmcCluster *cluster, int color = kRed, bool write= false) {

  PacEmcModel *model=  PacEmcModel::getModel();

  TBRIK *dummy= new TBRIK("dummy","dummy","void",0,0,0);
  dummy->SetLineColor(0);
  TNode *node0= new TNode(name,name,"dummy");
  node0->cd();

  for ( unsigned i=0; i< cluster->pacEmcDigis()->size(); i++) {
    const PacEmcDigi *digi= (*cluster->pacEmcDigis())[i];
    TString bname= Form("%s_%d",name,i);

    double theta= digi->thetaReal();
    double phi= digi->phiReal();
    HepPoint pos= model->where(*digi->tci());
    
    double thdegree= theta * Constants::radToDegrees;
    double phdegree= phi * Constants::radToDegrees;

    double x(0),y(0),z(0);
    double dx(0),dy(0), dz(20*digi->energy());
    //double thick= model->thickness(region);
    double rho(0),rr(0);
    TRotMatrix *rot= new TRotMatrix();
    TString rname= Form("%s_r%d",name,i);

    rot->SetName(rname);
    rot->SetTitle(rname);
    rot->SetAngles(90+thdegree,phdegree,90,90+phdegree,thdegree,phdegree);

    z = pos.z();
    rho = z * tan(theta);
    rr= pos.mag();
    dx = 0.5 * rr * model->deltaTheta( digi->theta() );
    dy = 0.5 * rho * model->deltaPhi( digi->theta() );
    x = (rr+dz)* sin(theta)*cos(phi);
    y = (rr+dz)* sin(theta)*sin(phi);
    z += dz*cos(theta);

    if ( dx != 0 ) {
      TBRIK *brik= new TBRIK(bname,bname,"void",dx,dy,dz);
      brik->SetLineColor(color);
      brik->SetLineWidth(1);
      
      TString nn= Form("digi%d",i);
      TNode *node= new TNode(nn, nn, brik, x,y,z, rot);
      node->SetFillStyle(1001);
      node->SetFillColor(color);
    }
  }

  if (write) node0->Write();
}



// void DrawDetector(const char* name) {

//   PacEmcModel *model=  PacEmcModel::getModel();

//   TBRIK *dummy= new TBRIK("dummy","dummy","void",0,0,0);
//   dummy->SetLineColor(0);
//   TNode *node0= new TNode(name,name,"dummy");
//   node0->cd();

//   double rho= model->barrelRadius();
//   double barz1= rho / tan(model->barrelEnd());
//   double barz2= rho / tan(model->barrelBegin());

//   TTUBE *barrel= new TTUBE("EmcBarrel","EmcBarrel","void",rho-1.0, rho, 0.5*fabs(barz2-barz1));
//   barrel->SetNumberOfDivisions(40);
//   TString bname= Form("%s_barrel",name);
//   TNode *node1= new TNode(bname,bname,barrel,0,0,0.5*(barz1+barz2));
//   node1->SetLineColor(kYellow);

//   double fwdz= model->fwdZ();
//   double rholo= model->fwdLowR();
//   double rhohi= model->fwdHiR();
//   TTUBE *fwd= new TTUBE("EmcFWD","EmcFWD","void", rholo, rhohi, 0.5);
//   fwd->SetNumberOfDivisions(40);
//   bname= Form("%s_fwd",name);
//   TNode *node2= new TNode(bname,bname,fwd,0,0,fwdz-0.5);
//   node2->SetLineColor(kYellow);

//   node0->Write();

// }

void MakeHistogram(int j, const PacEmcCluster *cluster) {

  PacEmcModel *model=  PacEmcModel::getModel();

  TString name= Form("hist_%d",j);
  int nbins= 21;
  float bnd= nbins/2 + 0.5;
  TH2F *hist= new TH2F(name,name,nbins,-bnd,bnd,nbins,-bnd,bnd);

  const HepPoint where= cluster->where();
  const TwoCoordIndex *tci= model->realCoordToTci(where);

  PacEmcDigiConstIterator iter= cluster->pacEmcDigis()->begin();
  while ( (iter != cluster->pacEmcDigis()->end() ) ) {
    hist->Fill((*iter)->theta()-tci->itsXCoord(),(*iter)->phi()-tci->itsYCoord(), (*iter)->energy());

    iter++;
  }

  hist->Write();
}


void MakeHitGraph(TString head, int j, PacSimTrack *simtrk) {

  TGraph2D *gr= new TGraph2D();
  gr->SetMarkerStyle(20);
  TGraph *grz= new TGraph();
  grz->SetMarkerStyle(20);

  const std::vector<PacSimHit>& hits = simtrk->getHitList();
  for(int i = 0; i < hits.size(); i++) {
    HepPoint pt= hits[i].position();
    const double x= pt.x();
    const double y= pt.y();
    const double z= pt.z();
    gr->SetPoint(i,x,y,z);
    grz->SetPoint(i,z,sqrt(x*x+y*y));
  }
  gr->SetName(head+Form("_%d",j));
  grz->SetName(head+Form("z_%d",j));
//   gr->GetXaxis()->SetTitle("X");
//   gr->GetYaxis()->SetTitle("Y");
//   gr->GetZaxis()->SetTitle("Z");
  gr->Write();
  grz->Write();

}

// Main ==========================================================
int main(int argc, char* argv[]) {
  // get configuration
  TString conffile= "PacDetector/pacrat_SuperB.xml";
  if ( argc >= 2 ) {
    conffile= argv[1];
  }
  
  gconfig.parsefile(conffile.Data());
  gconfig.parsefile("PacTest/testEmcCluster.xml");
  gconfig.parseargs(argc, argv);

  //const double twoPi= TMath::TwoPi();  
  const double onePi= TMath::Pi();

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
  PdtPdg::PdgType pdgid = (PdtPdg::PdgType)gconfig.getint("PdtPdg",22);

  HepPoint initpos = gconfig.getHepPoint("initpos");
  const double pmin= gconfig.getdouble("minmomentum");
  const double pmax= gconfig.getdouble("maxmomentum");
  // momentum distribution. Exponential if the coefficient is non-zero
  // otherwise uniform.
  const double exptau= gconfig.getdouble("Exponential lifetime", 0.);
  const double thmin = gconfig.getdouble("mintheta");
  const double thmax = gconfig.getdouble("maxtheta");
  const double phimin = gconfig.getdouble("minphi",-onePi);
  const double phimax = gconfig.getdouble("maxphi",onePi);
  const int nevents = gconfig.getint("nevents");
  const int printFreq = gconfig.getint("printFreq");
  const char* outputfile = gconfig.get("outputfile", "testEmcCluster.root");
  const int nDraws= gconfig.getint("numDraws", 10 );
  const bool verbose= gconfig.getbool("verbose", false);

  PdtEntry* pdt = Pdt::lookup(pdgid);
  cout << "particle = " << pdt->name() << endl;
  cout << "min momentum = " << pmin << endl;
  cout << "max momentum = " << pmax << endl;
  cout << "Exponential coef = " << exptau << endl;
  cout << "min theta = " << thmin << endl;
  cout << "max theta = " << thmax << endl;
  cout << "verbose  = " << verbose << endl;
  const double dp= pmax-pmin;
  const double dth= thmax-thmin;
  const double dphi= phimax-phimin;

  TRandom3 rnd(234234);
  

  TFile *of= new TFile(outputfile,"RECREATE");
  TTree *tree= new TTree("tr","cluster");
  int ievt(0);
  TLorentzVector *gtrkp4= new TLorentzVector();
  TLorentzVector *hitp4= new TLorentzVector();
  TLorentzVector *firsthitp4= new TLorentzVector();
  TVector3 *hitpos= new TVector3();
  TVector3 *firsthitpos= new TVector3();
  int hitnumber(-1);
  TLorentzVector *clp4= new TLorentzVector();
  TVector3 *clpos= new TVector3();
  float simhitE(0), firstsimhitE(0);
  int nCrys(0);
  float clenergy(0),e1(0), e9(0), e25(0), e1e9(0), e9e25(0);
  float mom2nd(0), mom2ndTP(0), mom2ndPhi(0), mom2ndTheta(0);
  float lat(0), a42(0), a20(0);
  float eDigi[128];
  float pathlenin(0), pathlenout(0), delpathlen(0);
  int deteffect(0);
  for ( int k=0; k<128;k++) eDigi[k]=0;
  bool foundEmc(false);

  tree->Branch("ievt",&ievt,"ievt/I");
  tree->Branch("gtrkp4","TLorentzVector",&gtrkp4,128000,0);
  tree->Branch("hitp4","TLorentzVector",&hitp4,128000,0);
  tree->Branch("firsthitp4","TLorentzVector",&firsthitp4,128000,0);
  tree->Branch("hitpos","TVector3", &hitpos, 128000,0);
  tree->Branch("deteffect", &deteffect, "deteffect/I");
  tree->Branch("pathlenin", &pathlenin ,"pathlenin/F");
  tree->Branch("pathlenout", &pathlenout ,"pathlenout/F");
  tree->Branch("delpathlen", &delpathlen ,"delpathlen/F");
  tree->Branch("firsthitpos","TVector3", &firsthitpos, 128000,0);
  tree->Branch("hitnumber",&hitnumber,"hitnumber/I");
  tree->Branch("clp4","TLorentzVector",&clp4,128000,0);
  tree->Branch("clpos","TVector3", &clpos, 128000,0);
  tree->Branch("simhitE", &simhitE, "simhitE/F");
  tree->Branch("firstsimhitE", &firstsimhitE, "simhitE/F");
  tree->Branch("foundEmc", &foundEmc, "foundEmc/O");
  tree->Branch("nCrys",&nCrys,"nCrys/I");
  tree->Branch("energy",&clenergy, "energy/F");
  tree->Branch("lat", &lat, "lat/F");
  tree->Branch("a42", &a42, "a42/F");
  tree->Branch("a20", &a20, "a20/F");
  tree->Branch("e1", &e1, "e1/F");
  tree->Branch("e9", &e9, "e9/F");
  tree->Branch("e25", &e25, "e25/F");
  tree->Branch("e1e9", &e1e9, "e1e9/F");
  tree->Branch("e9e25", &e9e25, "e9e25/F");
  tree->Branch("mom2nd", &mom2nd, "mom2nd/F");
  tree->Branch("mom2ndTP", &mom2ndTP, "mom2ndTP/F");
  tree->Branch("mom2ndPhi", &mom2ndPhi, "mom2ndPhi/F");
  tree->Branch("mom2ndTheta", &mom2ndTheta, "mom2ndTheta/F");
  tree->Branch("eDigi", eDigi, "eDigi[nCrys]/F");


  TStopwatch timer;
  
  timer.Start();
  unsigned nprints(0);


  //DrawDetector("emcDet");

  TBRIK *dummy= new TBRIK("dummy","dummy","void",0,0,0);
  dummy->SetLineColor(0);
  TNode *node= new TNode("emcClusters","emcClusters","dummy");
  node->cd();
  
  int ndraw=0;
//  int ngraphs2=0;
//  int ngraphs1=0;
  for (unsigned j=0; j< nevents; j++) {
    ievt= j;

    if ( verbose ) cout << "processing event # " << j << endl;
    else {
      if ( fmod((double)j+1,(double)printFreq)== 0 ) {
	cout << "processing event # " << j+1 << endl;
      }
    }
    // create GTrack
    GVertex gvtx;
    gvtx.setPosition(initpos);
    GTrack gtrk;
    gtrk.setVertex(&gvtx);
    gtrk.setPDT(pdt);

    double pp(-1);
    if ( exptau != 0 ) {
      while ( pp > pmax || pp < pmin ) {
	pp = pmin+ dp* rnd.Exp(exptau);
      }
    } else {
      pp = pmin+ dp* rnd.Uniform();
    }

    double theta= thmin+ dth* rnd.Uniform();
    double phi= phimin+ dphi* rnd.Uniform();
    Hep3Vector p3(pp*sin(theta)*cos(phi),  pp*sin(theta)*sin(phi), pp*cos(theta) );

    HepLorentzVector p4; 
    p4.setVectM(p3,pdt->mass());

    gtrk.setP4(p4);

    gtrkp4->SetXYZM(p4.x(),p4.y(),p4.z(),pdt->mass());

    // Generate the track
    PacSimulate sim(bfield,detector);
    HepRandomEngine* engine = new RanecuEngine();
  	HepRandom::setTheEngine(engine);    
    sim.setRandomEngine(engine);
  	detector->setRandomEngine(engine);
    PacSimTrack* simtrk = sim.simulateGTrack(&gtrk);
    //    const PacPieceTraj* simtraj = simtrk->getTraj();
    HepVector gparams(5);
    foundEmc= false;
    // find the hit at the EMC
    const std::vector<PacSimHit>& hits = simtrk->getHitList();
    int nemc=0;
    HepPoint emchit1, emchit2;
    TString mess="";
    double jump=0;
    TString jumpmess="";

    for(int i = 0; i < hits.size(); i++) {
      const DetIntersection& dinter = hits[i].detIntersection();
      const PacDetElem* pelem = dynamic_cast<const PacDetElem *>(dinter.delem);


	mess+= "SimHit : ";
	if(hits[i].detIntersection().delem != 0){
	  mess+= hits[i].detIntersection().delem->elementName();
	  mess+= " elem# ";
	  mess+= hits[i].detIntersection().delem->elementNumber();
  }
	mess+= Form(" (%f,%f,%f)",hits[i].position().x(),hits[i].position().y(),hits[i].position().z());
	mess+= " theta= ";
	mess+= hits[i].position().theta();
	mess+= " phi= ";
	mess+= hits[i].position().phi();
	//mess+= "  momentumIn ";
	//mess+= hits[i].momentumIn();
	mess+= " dE= ";
	mess+= hits[i].energyChange();
	mess+= " effect= ";
	mess+= hits[i].detEffect();
	mess+= "\n";

	if ( i>0 ) {
	  jump= hits[i].position().distanceTo(hits[i-1].position());
	}
	
	if ( jump > 150 ) {
	  jumpmess+= " Jump from ";
	  if( hits[i-1].detIntersection().delem != 0){
	    jumpmess+= hits[i-1].detIntersection().delem->elementName();
	    jumpmess+= " #";
	    jumpmess+= hits[i-1].detIntersection().delem->elementNumber();
    }
	  jumpmess+= " @";
	  jumpmess+= Form("(%f,%f,%f)",hits[i-1].position().x(),hits[i-1].position().y(),hits[i-1].position().z());
	  jumpmess+= " effect ";
	  jumpmess+= hits[i-1].detEffect();

	  jumpmess+= "\n      To ";
	  if( hits[i].detIntersection().delem != 0){
      jumpmess+= hits[i].detIntersection().delem->elementName();
      jumpmess+= " #";
      jumpmess+= hits[i].detIntersection().delem->elementNumber();
    }
	  jumpmess+= " @";
	  jumpmess+= Form("(%f,%f,%f)",hits[i].position().x(),hits[i].position().y(),hits[i].position().z());
	  jumpmess+= " effect ";
	  jumpmess+= hits[i].detEffect();
	  jumpmess+= "\n";
	}


      if( pelem != 0 && pelem->measurementDevices().size() != 0){
        for(std::vector<const PacMeasurement*>::const_iterator imdev = pelem->measurementDevices().begin();
        imdev != pelem->measurementDevices().end();imdev++){
          const PacMeasurement* meas = *imdev;
          if(meas->measurementType()==PacMeasurement::Calor ) {

            if ( nemc==0 ) emchit1= hits[i].position();
            else if ( nemc==1 ) emchit2= hits[i].position();

            nemc++;

            if ( !foundEmc ) {
              pathlenin= dinter.pathrange[0];
              pathlenout= dinter.pathrange[1];
              delpathlen= pathlenout-pathlenin;
              deteffect= hits[i].detEffect();
              hitnumber = i;
              simhitE= hits[i].energyChange();
              firstsimhitE= hits[0].energyChange();
              hitp4->SetXYZM(hits[i].momentumIn().x(),hits[i].momentumIn().y(),hits[i].momentumIn().z(),hits[i].pdt()->mass());
              firsthitp4->SetXYZM(hits[0].momentumIn().x(),hits[0].momentumIn().y(),hits[0].momentumIn().z(),hits[0].pdt()->mass());
              hitpos->SetXYZ(hits[i].position().x(),hits[i].position().y(),hits[i].position().z());
              firsthitpos->SetXYZ(hits[0].position().x(),hits[0].position().y(),hits[0].position().z());

              foundEmc= true;
            }
          }
        }
      }
    }
//     if ( nemc>1 ) {
//       MakeHitGraph("g2",ngraphs2,simtrk);
//       ngraphs2++;

//       cout << "EMC hit 1 : " << emchit1 << endl;
//       cout << "EMC hit 2 : " << emchit2 << endl;

//     } else {
//       if ( ngraphs1<20 ) {
// 	MakeHitGraph("g1",ngraphs1,simtrk);
//       }
//       ngraphs1++;
//     }


//     if ( j<10 ) {
//       cout << "event # " << j+1 << endl;
//       cout << mess << endl;
//     }

    if ( jumpmess!= "") {
      cout << "event # " << j+1 << endl;
      cout << mess << endl;
      cout << jumpmess << endl;
    }

    // skip if not hitting the emc
    if ( !foundEmc ) continue;

    PacReconstructEmc emcreco;
    std::vector< PacEmcCluster*> vclusters= emcreco.makeEmcCluster(simtrk);
    
    for (int k=0; k<vclusters.size(); k++) {
      
      PacEmcCluster *cluster= vclusters[k];
      // cluster position relative to the initial position
      HepPoint relpos= cluster->position(initpos); 
      Hep3Vector vrelpos(relpos.x(),relpos.y(), relpos.z());
      Hep3Vector cldir= vrelpos.unit();
      
      if ( nprints < 10 ) {
	cout << " testEmcCluster: " << endl;
	cluster->print(std::cout, true);
// 	cout << relpos << endl;
// 	cout << vrelpos << endl;
// 	cout << cldir << endl;
	
	nprints++;
      }
      
      HepPoint where= cluster->where();
      clenergy= cluster->energy();
      clp4->SetXYZM(clenergy*cldir.x(),clenergy*cldir.y(),clenergy*cldir.z(),0 );
      clpos->SetXYZ(where.x(),where.y(),where.z());
      const PacEmcClusterEnergySums esum= cluster->esums();
      const PacEmcXClMoments xmom= cluster->Xmoments();
      nCrys= cluster->numberOfDigis();

      PacEmcDigiConstIterator iter= cluster->pacEmcDigis()->begin();
      int mm=0;
      while ( (iter != cluster->pacEmcDigis()->end() ) ) {
	eDigi[mm]= (*iter)->energy();
	iter++;
	mm++;
      }

      lat= xmom.lat();
      a42= xmom.absZernikeMoment(4,2);
      a20= xmom.absZernikeMoment(2,0);
      mom2nd= xmom.secondMoment();
      mom2ndTP= xmom.secondMomentTP();
      mom2ndPhi= xmom.secondMomentPhi();
      mom2ndTheta= xmom.secondMomentTheta();
      e1= esum.e1();
      e9= esum.e9();
      e25= esum.e25();
      e1e9= esum.e1e9();
      e9e25= esum.e9e25();
      

//       if ( lat > 0.9 ) {
// 	cout << "event # " << j ;
      
// 	cout << "  lat= " << lat << endl;
// 	cout << mess << endl;
// 	cluster->print(std::cout, true);
//       }


      tree->Fill();
      
      if ( nCrys>0) {
	if ( ndraw < nDraws ) {
	  of->cd();
	  MakeHistogram(ndraw,cluster);
	  node->cd();
	  TString dname= Form("cluster%d",j);
	  DrawCluster(dname,cluster,kRed);
	  ndraw++;
	}
      }

      delete cluster;

    } // found cluster
    
  } // loop events

  node->Write();

  timer.Stop();
  timer.Print();

  tree->Write();
  of->Close();

}

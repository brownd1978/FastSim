//===============================================================
//  testOneNeutral
//
//===============================================================
#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <Gtypes.h>
#include <TFile.h>
#include <TGraph.h>
#include <TPolyLine3D.h>
#include <TPolyMarker3D.h>
#include <TBRIK.h>
#include <TTUBE.h>
#include <TString.h>
#include <TNode.h>
#include <TRotMatrix.h>
#include <TH2F.h>

#include "GenEnv/GenEnv.hh"

#include "DetectorModel/DetMaterial.hh"

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

#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PacGeom/PacHelix.hh"
#include "PacGeom/PacPieceTraj.hh"
#include "PacGeom/PacDetElem.hh"

#include "PacEnv/pstring.hh"

#include "ProxyDict/Ifd.hh"
#include "AbsEnv/AbsEnv.hh"
#include "DetectorModel/DetSet.hh"
#include "BField/BField.hh"

#include "G3Data/GVertex.hh"
#include "G3Data/GTrack.hh"
#include "BbrGeom/BbrError.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RanecuEngine.h"

#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacReconstructEmc.hh"
#include "PacEmc/PacEmcModel.hh"
#include "AbsEnv/TwoCoordIndex.hh"
#include "PacSim/PacShowerInfo.hh"
#include "Framework/AppFileName.hh"

using namespace std;

void DrawTrajectory(const char* name, const Trajectory* traj, int N = 100, int color = kBlue) {
  TPolyLine3D line(N);
  double smin = traj->lowRange();
  double smax = traj->hiRange();
  for(int n = 0; n < N; n++) {
    double s = smin + (smax - smin)*n/N;
    HepPoint p = traj->position(s);
    line.SetPoint(n, p.x(), p.y(), p.z());
  }
  line.SetLineColor(color);
  line.Write(name);
}

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

  if (write)  node0->Write();
}



void DrawDetector(const char* name) {

  PacEmcModel *model=  PacEmcModel::getModel();

  TBRIK *dummy= new TBRIK("dummy","dummy","void",0,0,0);
  dummy->SetLineColor(0);
  TNode *node0= new TNode(name,name,"dummy");
  node0->cd();

  double rho= model->barrelRadius(0);
  double barz1= rho / tan(model->barrelEnd());
  double barz2= rho / tan(model->barrelBegin());

  TTUBE *barrel= new TTUBE("EmcBarrel","EmcBarrel","void",rho-1.0, rho, 0.5*fabs(barz2-barz1));
  barrel->SetNumberOfDivisions(40);
  TString bname= Form("%s_barrel",name);
  TNode *node1= new TNode(bname,bname,barrel,0,0,0.5*(barz1+barz2));
  node1->SetLineColor(kYellow);

//   double fwdz= model->fwdZ();
//   double rholo= model->fwdLowR();
//   double rhohi= model->fwdHiR();
//   TTUBE *fwd= new TTUBE("EmcFWD","EmcFWD","void", rholo, rhohi, 0.5);
//   fwd->SetNumberOfDivisions(40);
//   bname= Form("%s_fwd",name);
//   TNode *node2= new TNode(bname,bname,fwd,0,0,fwdz-0.5);
//   node2->SetLineColor(kYellow);

  node0->Write();

}


void MakeHistogram(TString name, const PacEmcCluster *cluster) {

  if ( ! cluster ) return;

  PacEmcModel *model=  PacEmcModel::getModel();

  int nbins= 11;
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


void MakeGlobalHist(TH2F* hist, const PacEmcCluster *cluster) {

  PacEmcModel *model=  PacEmcModel::getModel();

  if ( hist==0 ) {
    // Get the number of theta and phi bins
    int ntheta= model->fwdNRings()+model->barrelNRings()+model->bwdNRings();
    int nphi= model->barrelNPhis();
    for ( int i=0; i<model->fwdNRings(); i++) {
      if ( nphi < model->fwdNPhis()[i] ) {
	nphi = model->fwdNPhis()[i];
      }
    }
    if ( nphi < model->bwdNPhis() ) {
      nphi = model->bwdNPhis();
    }
    
    TString name= "ghist";
    cout << "create a histogram with " << ntheta << " x bins and "
	 << nphi << " y bins. " << endl;
    
    hist= new TH2F(name,name,ntheta,-0.5,ntheta-0.5,nphi,-0.5,nphi-0.5);
  }

  const HepPoint where= cluster->where();

  PacEmcDigiConstIterator iter= cluster->pacEmcDigis()->begin();
  while ( (iter != cluster->pacEmcDigis()->end() ) ) {
    hist->Fill((*iter)->theta(),(*iter)->phi(), (*iter)->energy());
    iter++;
  }

}


// Main ==========================================================
int main(int argc, char* argv[]) {
  // get configuration
  
  if(argc <= 1){
    gconfig.parsefile(AppFileName("PacTest/testOneNeutral.xml").pathname().c_str());
    gconfig.parsefile(AppFileName("PacDetector/pacrat_SuperB.xml").pathname().c_str());
  }
  gconfig.parseargs(argc, argv);
  
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


  // print material property
  const char* material = gconfig.get("material", "emc-CsI");
  const DetMaterial* mat = gblEnv->getGen()->findDetMaterial(material);
  cout << "CsI interaction length = " << mat->intLength() << endl;



  // Get initial parameter
  HepPoint initpos = gconfig.getHepPoint("initpos");
  Hep3Vector initmom(0,0,0);
  initmom= gconfig.getHep3Vector("initmom");
  double initpmag = gconfig.getdouble("initpmag",1);
  double inittheta = gconfig.getdouble("inittheta",0);
  double initphi = gconfig.getdouble("initphi",0);

  if ( initmom.mag2()== 0 ) {
    initmom = Hep3Vector(initpmag*sin(inittheta)*cos(initphi),
			 initpmag*sin(inittheta)*sin(initphi),
			 initpmag*cos(inittheta));
  }

  PdtPdg::PdgType pdgid = (PdtPdg::PdgType)gconfig.getint("PdtPdg",22);
  PacEmcCluster::CentroidMethod algo = (PacEmcCluster::CentroidMethod)gconfig.getint("centroidAlgo",PacEmcCluster::logarithmic);
  
  // Create GTrack
  GVertex gvtx;
  gvtx.setPosition(initpos);
  GTrack gtrk;
  PdtEntry* pdt = Pdt::lookup(pdgid);
  //double q = pdt->charge();
  gtrk.setVertex(&gvtx);
  gtrk.setPDT(pdt);
  HepLorentzVector p4; p4.setVectM(initmom,pdt->mass());
  gtrk.setP4(p4);

  cout << "Initial position\n" << initpos << endl;
  cout << "Initial momentum\n" << p4 << endl;
  cout << "Initial theta = " << p4.theta() << endl;
  cout << "Initial phi = " << p4.theta() << endl;
  cout << "Initial p3 = " << p4.rho() << endl;

  // Generate the track
  PacSimulate sim(bfield,detector);
  HepRandomEngine* engine = new RanecuEngine();
	HepRandom::setTheEngine(engine);
	sim.setRandomEngine(engine);
	detector->setRandomEngine(engine);
	
  PacSimTrack* simtrk = sim.simulateGTrack(&gtrk);
  const PacPieceTraj* simtraj = simtrk->getTraj();
  HepVector gparams(5);
  //  double fltlen;

  const std::vector<PacSimHit>& hits = simtrk->getHitList();
  cout << "[SimHit]" << endl;
  cout << " hits.size() = " << hits.size() << endl;
  for(int i = 0; i < hits.size(); i++) {
    cout << " SimHit " << i << ": " << hits[i].position() << " effect " << hits[i].detEffect() 
      << " dmom " << hits[i].momentumChange().mag();
    if(hits[i].detIntersection().delem != 0){
      cout << " element " << hits[i].detIntersection().delem->elementName() 
        << " elem# = " << hits[i].detIntersection().delem->elementNumber();
    }
	  cout << endl;
    const double mass= hits[i].pdt()->mass();
    const double pin = hits[i].momentumIn().mag();
    const double pout = hits[i].momentumOut().mag();
    double eloss;

    if ( pout > 0 ) {
      eloss = sqrt(mass*mass+pin*pin)-sqrt(mass*mass+pout*pout);
    } else {
      eloss = sqrt(mass*mass+pin*pin);
    }

    cout << " Momentum in  " << hits[i].momentumIn() << endl;
    cout << " Momentum out " << hits[i].momentumOut() << endl;
    cout << " Energy loss = " << eloss << endl;

    cout << " first hit pos " << hits[0].position() << endl;
    cout << " this hit pos  " << hits[i].position() << endl;

    cout << " hit p3 = " << hits[i].momentum().mag() << endl;
    cout << " hit energyChange = " << hits[i].energyChange() << endl;
    if ( hits[i].showerInfo() ) {
      cout << " shower energy = " << hits[i].showerInfo()->showerEnergy() << endl;
    } else {
      cout << " no shower" << endl;
    }

    cout << " hit theta = " << hits[i].momentum().theta() << endl;
    cout << " hit phi = " << hits[i].momentum().phi() << endl;

  }

//   PacEmcModel *model= PacEmcModel::getModel();
//   const TwoCoordIndex *tci= model->getTci(4,0);
//   tci->print(cout);
//   cout << "neighbors" << endl;
//   std::set<AbsDetIndex*>::const_iterator iter= tci->itsNeighbours()->begin();
//   while ( (iter!= tci->itsNeighbours()->end()) ) {
//     (*iter)->print(cout);
//     iter++;
//   }


  const char* outputfile = gconfig.get("outputfile", "testOneNeutral.root");
  TFile* file = new TFile(outputfile, "RECREATE");

  TH2F *ghist(0);
  DrawDetector("emcDet");
  if ( pdt->charge() == 0 ) {
    DrawTrajectory("simtrack", simtraj, 200, kGreen);
  } else {
    DrawTrajectory("simtrack", simtraj, 200, kBlue);
  }

  PacReconstructEmc* emcreso = new PacReconstructEmc(algo);
  std::vector<PacEmcCluster*> vclusters= emcreso->makeEmcCluster(simtrk);

  for ( int k=0; k<vclusters.size(); k++) {
    PacEmcCluster* cluster= vclusters[k];

    if ( cluster ) {
      cluster->print(std::cout, true);
    } else {
      cout << "No emc cluster is created." << endl;
      return -1;
    }


//   cluster->removeNoise(0.010);
//   cluster->print();

//   cout << "mapsize = " << cluster->digiTciMap()->size() << endl;


  // make another cluster
//   PacEmcCluster *cluster2= emcreso->makeEmcCluster(simtrk);
//   cluster2->print();

//   // merge the two
//   cluster->mergeCluster(cluster2);
//   cluster->print();
    
    TString cname= Form("emc_%d",k);
    DrawCluster(cname,cluster,kRed, true);
  
    TString hname= Form("hemc");
    MakeHistogram(hname, cluster);

    MakeGlobalHist(ghist,cluster);

    delete cluster;

  }

  if ( ghist ) ghist->Write();
  delete emcreso;
  file->Close();
}

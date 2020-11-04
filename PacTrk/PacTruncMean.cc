#include "BaBar/BaBar.hh"
#include "PacTrk/PacTruncMean.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacTrk/PacTrkHitMeas.hh"
#include "PacTrk/PacTrkdEdxMeas.hh"
#include "TrkBase/TrkHotList.hh"
#include "PacTrk/PacTrkHitViewDch.hh"

#include "TrkBase/TrkRecoTrk.hh"
#include <vector>
#include "CLHEP/Random/RandFlat.h"

#include "PDT/Pdt.hh"
#include "PDT/PdtPid.hh"

#include "GenEnv/GenEnv.hh"
#include "PacEnv/PacConfig.hh"
#include "PacEnv/PacBuildEnv.hh"
#include "DetectorModel/DetMaterial.hh"
#include "AbsEnv/AbsEnv.hh"
#include "PacSim/PacSimHit.hh"

#include <iostream>

// jw --
#include "PacSim/PacSimTrack.hh"
#include "G3Data/GTrack.hh"
#include "CLHEP/Vector/LorentzVector.h"

// --

using namespace std;

PacTruncMean::PacTruncMean(const TrkRecoTrk* recotrk, const GTrack* gtrk) {

  InitDch(recotrk,gtrk);
  InitSvt(recotrk,gtrk);

}

void PacTruncMean::InitDch(const TrkRecoTrk* recotrk, const GTrack* trkGtrk){

  _dedx_dch=0;
  _ededx_dch=0;
  _truncFrac_dch=0;
  _ndchsample=0;
  assert(recotrk);

  //Fill a vector with the dE/dx measured in the DCH cells
  vector<double> dedx_vec;
  vector<double> ededx_vec;
  double dedx_hit,ededx_hit;

  //vectors with the dE/dx expected in the DCH cells
  vector<double> dedxEle_vec;
  vector<double> dedxMu_vec;
  vector<double> dedxPi_vec;
  vector<double> dedxK_vec;
  vector<double> dedxP_vec;

  //get the material properties of the DCH gas used to compute the _expected_ dE/dx
  const char* material = gconfig.getcstr("material", "dch-He-Ibu");
  const DetMaterial* mat = gblEnv->getGen()->findDetMaterial(material);
  double density = mat->density();
  assert (density>0);

  const TrkHotList *hotlist = recotrk->hots();
  for(TrkHotList::hot_iterator i(hotlist->begin());i!=hotlist->end();++i){
    const PacHitOnTrk* pachit = dynamic_cast<const PacHitOnTrk*>( i.get() );
    const PacTrkHitMeas* trkHitMeas = pachit->measure();
    assert(trkHitMeas != 0);
    if (trkHitMeas->type()==PacTrkHitMeas::Wire) {

      dedx_hit = pachit->getdEdx();
      ededx_hit = pachit->getErrdEdx();

      if (ededx_hit>0) { // dedx exists for this hit

	//take the expected dE/dx of the hit for different particle Id hypotheses
	// take out bg hits

	const PacSimHit* simHit = pachit->simHit();
	double psimhit = simHit->momentum().mag();
	const GTrack* hitGtrk = simHit->getSimTrack()->getGTrack();
	//	cout << endl << "   hitGtrk: PidId = " << hitGtrk->pdt()->pidId() << ", p = " << hitGtrk->p4().vect().mag() << endl;

	// I assume here that all simHits must have a parent GTrack
	assert(hitGtrk!=0);

	//	cout << "+++              psimhit: " << psimhit; 
	// cout << ", pion dedx: " << fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::pion))))/density << endl;

	bool acceptHit=false;
	if (trkGtrk==0) { 
	  cout << "      PacTruncMean: trkGrk = 0.  Accepting all hits for dedx. " << endl;
	  acceptHit=true;
	}
        else {
	  if (*trkGtrk == *hitGtrk) {
	    //cout <<"      PacTruncMean: trk and hit GTrack match."  << endl;
	    acceptHit = true;
	  }
	  else {
	    // cout <<"      PacTruncMean: trk and hit GTrack do not match."  << endl;
	    acceptHit = false;
	  }
	}
	// for expected dedx, require that the trk and hit GTracks are the same. 
	if (acceptHit) {

      
	  dedx_vec.push_back(dedx_hit);
	  ededx_vec.push_back(ededx_hit);

	  dedxEle_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::electron))))/density);
	  dedxMu_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::muon))))/density);
	  dedxPi_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::pion))))/density);
	  dedxK_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::kaon))))/density);
	  dedxP_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::proton))))/density);
	  
	  //get the truncated mean fraction
	  _truncFrac_dch = pachit->dEdxMeasure()->truncFrac();
	}
      }
    }
  }

  //Remove (1-_truncFrac_dch)*Total_hits randomly
  //Note: the 'random truncation' is consistent with the effective Gaussian distribution of dE/dx in FastSim
  int ntail = (int)round(dedx_vec.size()*(1.-_truncFrac_dch));
  HepRandomEngine* rng = HepRandom::getTheEngine();
  for (int i=0;i<ntail;i++){
    //take random integer in [0,dedx_vec.size())
    int i_hit=(int)floor(dedx_vec.size()*RandFlat::shoot(rng));
    //remove random element of dedx_vec, ededx_vec and dedx<particleType>_vec
    dedx_vec.erase(dedx_vec.begin()+i_hit);
    ededx_vec.erase(ededx_vec.begin()+i_hit);
    dedxEle_vec.erase(dedxEle_vec.begin()+i_hit);
    dedxMu_vec.erase(dedxMu_vec.begin()+i_hit);
    dedxPi_vec.erase(dedxPi_vec.begin()+i_hit);
    dedxK_vec.erase(dedxK_vec.begin()+i_hit);
    dedxP_vec.erase(dedxP_vec.begin()+i_hit);

  }
  //Compute the average dE/dx and error of the remaining _truncFrac_dch hits (truncated mean)
  _ndchsample = dedx_vec.size();
  if (_ndchsample==0) return;
  
  double dedx_ave=0;
  double dedx_invsq_err=0;

  //double dedxEle_ave(0),dedxMu_ave(0),dedxPi_ave(0),dedxK_ave(0),dedxP_ave(0);
  double dedxEle_ave=0;
  double dedxMu_ave=0;
  double dedxPi_ave=0;
  double dedxK_ave=0;
  double dedxP_ave=0;

  for (int i=0;i<_ndchsample;i++){
    assert(ededx_vec[i]>0);
    dedx_ave+=dedx_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedx_invsq_err+=1./(ededx_vec[i]*ededx_vec[i]);
    dedxEle_ave+=dedxEle_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxMu_ave+=dedxMu_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxPi_ave+=dedxPi_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxK_ave+=dedxK_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxP_ave+=dedxP_vec[i]/(ededx_vec[i]*ededx_vec[i]);
  }
  dedx_ave/=dedx_invsq_err;
  dedxEle_ave/=dedx_invsq_err;
  dedxMu_ave/=dedx_invsq_err;
  dedxPi_ave/=dedx_invsq_err;
  dedxK_ave/=dedx_invsq_err;
  dedxP_ave/=dedx_invsq_err;

  _dedx_dch = dedx_ave;
  _ededx_dch = 1./sqrt(dedx_invsq_err);
  _expectedTrkDedxDch[0]=dedxEle_ave;
  _expectedTrkDedxDch[1]=dedxMu_ave;
  _expectedTrkDedxDch[2]=dedxPi_ave;
  _expectedTrkDedxDch[3]=dedxK_ave;
  _expectedTrkDedxDch[4]=dedxP_ave;

  return;
}


PacTruncMean::~PacTruncMean() {
}

//dE/dx from the drift chamber
void PacTruncMean::getdEdxDch(double& dedx,double& ededx){
  dedx=_dedx_dch;
  ededx=_ededx_dch;

  return;
}

//dE/dx from the silicon tracker is not implemented yet. It returns 0.
void PacTruncMean::getdEdxSvt(double& dedx,double& ededx) {
  dedx=_dedx_svt;
  ededx=_ededx_svt;

  return;
}

void PacTruncMean::InitSvt(const TrkRecoTrk* recotrk, const GTrack* trkGtrk){

  // J. Walsh -- there is much repeated code from InitDch(), but let's keep them separate to allow
  // future developments on the Svt dedx algorithm.

  _dedx_svt = 0;
  _ededx_svt = 0;
  _truncFrac_svt = 0;
  _nsvtsample = 0;

  for (int i=0;i<5;i++) _expectedTrkDedxSvt[i] = -1;

  assert(recotrk);

  //Fill a vector with the dE/dx measured in the Svt layers
  vector<double> dedx_vec;
  vector<double> ededx_vec;
  double dedx_hit,ededx_hit;

  //vectors with the dE/dx expected in the SVT layers
  vector<double> dedxSvtEle_vec;
  vector<double> dedxSvtMu_vec;
  vector<double> dedxSvtPi_vec;
  vector<double> dedxSvtK_vec;
  vector<double> dedxSvtP_vec;
  
  //get the material properties of the silicon used to compute the _expected_ dE/dx
  const char* material = gconfig.getcstr("material", "svt-Silicon");
  const DetMaterial* mat = gblEnv->getGen()->findDetMaterial(material);
  double density = mat->density();
  assert (density>0);

  //  cout << endl << "+++ PacTruncMean: new track: " << endl;
  //cout << endl << "    trkGtrk: PidId = " << trkGtrk->pdt()->pidId() << ", p = " << trkGtrk->p4().vect().mag() << endl;
  const TrkHotList *hotlist = recotrk->hots();
  for(TrkHotList::hot_iterator i(hotlist->begin());i!=hotlist->end();++i){
    const PacHitOnTrk* pachit = dynamic_cast<const PacHitOnTrk*>( i.get() );
    const PacTrkHitMeas* trkHitMeas = pachit->measure();
    assert(trkHitMeas != 0);
    if ( trkHitMeas->type() == PacTrkHitMeas::DoubleSiStrip ) {
      dedx_hit = pachit->getdEdx();
      ededx_hit = pachit->getErrdEdx();

      if (ededx_hit>0) {  // dedx exists for this hit

	// require that the hit not be from a background source

	const PacSimHit* simHit = pachit->simHit();
	double psimhit = simHit->momentum().mag();
	const GTrack* hitGtrk = simHit->getSimTrack()->getGTrack();
	//	cout << endl << "   hitGtrk: PidId = " << hitGtrk->pdt()->pidId() << ", p = " << hitGtrk->p4().vect().mag() << endl;

	// I assume here that all simHits must have a parent GTrack
	assert(hitGtrk!=0);

	//	cout << "+++              psimhit: " << psimhit; 
	// cout << ", pion dedx: " << fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::pion))))/density << endl;

	bool acceptHit=false;
	if (trkGtrk==0) { 
	  cout << "      PacTruncMean: trkGrk = 0.  Accepting all hits for dedx. " << endl;
	  acceptHit=true;
	}
        else {
	  if (*trkGtrk == *hitGtrk) {
	    //cout <<"      PacTruncMean: trk and hit GTrack match."  << endl;
	    acceptHit = true;
	  }
	  else {
	    // cout <<"      PacTruncMean: trk and hit GTrack do not match."  << endl;
	    acceptHit = false;
	  }
	}
	// for expected dedx, require that the trk and hit GTracks are the same. 
	if (acceptHit) {

	  dedx_vec.push_back(dedx_hit);
	  ededx_vec.push_back(ededx_hit);

	  dedxSvtEle_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::electron))))/density);
	  dedxSvtMu_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::muon))))/density);
	  dedxSvtPi_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::pion))))/density);
	  dedxSvtK_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::kaon))))/density);
	  dedxSvtP_vec.push_back(fabs(mat->dEdx(psimhit,(DetMaterial::dedxtype) 0,(Pdt::mass(PdtPid::proton))))/density);
	}

	//get the truncated mean fraction
	  _truncFrac_svt = pachit->dEdxMeasure()->truncFrac();

      }
    }
  }

  //Remove (1-_truncFrac_svt)*Total_hits randomly
  //Note: the 'random truncation' is consistent with the effective Gaussian distribution of dE/dx in FastSim

  int ntail = (int)round(dedx_vec.size()*(1.-_truncFrac_svt));
  HepRandomEngine* rng = HepRandom::getTheEngine();
  for (int i=0;i<ntail;i++){
    //take random integer in [0,dedx_vec.size())
    int i_hit=(int)floor(dedx_vec.size()*RandFlat::shoot(rng));
    //remove random element of dedx_vec and ededx_vec
    dedx_vec.erase(dedx_vec.begin()+i_hit);
    ededx_vec.erase(ededx_vec.begin()+i_hit);
    dedxSvtEle_vec.erase(dedxSvtEle_vec.begin()+i_hit);
    dedxSvtMu_vec.erase(dedxSvtMu_vec.begin()+i_hit);
    dedxSvtPi_vec.erase(dedxSvtPi_vec.begin()+i_hit);
    dedxSvtK_vec.erase(dedxSvtK_vec.begin()+i_hit);
    dedxSvtP_vec.erase(dedxSvtP_vec.begin()+i_hit);
  }
  
  //Compute the average dE/dx and error of the remaining _truncFrac_svt hits (truncated mean)
  _nsvtsample = dedx_vec.size();

  // maybe put a cut on minimum sample size here?
  if (_nsvtsample==0) return;
  
  double dedx_ave=0;
  double dedx_invsq_err=0;
  double dedxSvtEle_ave=0;
  double dedxSvtMu_ave=0;
  double dedxSvtPi_ave=0;
  double dedxSvtK_ave=0;
  double dedxSvtP_ave=0;

  for (int i=0;i<_nsvtsample;i++){
    assert(ededx_vec[i]>0);
    dedx_ave+=dedx_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedx_invsq_err+=1./(ededx_vec[i]*ededx_vec[i]);
    dedxSvtEle_ave+=dedxSvtEle_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxSvtMu_ave+=dedxSvtMu_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxSvtPi_ave+=dedxSvtPi_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxSvtK_ave+=dedxSvtK_vec[i]/(ededx_vec[i]*ededx_vec[i]);
    dedxSvtP_ave+=dedxSvtP_vec[i]/(ededx_vec[i]*ededx_vec[i]);
  }

  dedx_ave/=dedx_invsq_err;
  dedxSvtEle_ave/=dedx_invsq_err;
  dedxSvtMu_ave/=dedx_invsq_err;
  dedxSvtPi_ave/=dedx_invsq_err;
  dedxSvtK_ave/=dedx_invsq_err;
  dedxSvtP_ave/=dedx_invsq_err;

  _dedx_svt = dedx_ave;
  _ededx_svt = 1./sqrt(dedx_invsq_err);
  _expectedTrkDedxSvt[0]=dedxSvtEle_ave;
  _expectedTrkDedxSvt[1]=dedxSvtMu_ave;
  _expectedTrkDedxSvt[2]=dedxSvtPi_ave;
  _expectedTrkDedxSvt[3]=dedxSvtK_ave;
  _expectedTrkDedxSvt[4]=dedxSvtP_ave;

  return;

}

double PacTruncMean::getExpectedTrkDedxSvt(PdtPid::PidType pidType) const {

  int idx=-1;

  switch (pidType) {
  case PdtPid::electron:
    idx = 0;
    break;
  case PdtPid::muon:
    idx = 1;
    break;
  case PdtPid::pion:
    idx = 2;
    break;
  case PdtPid::kaon:
    idx = 3;
    break;
  case PdtPid::proton:
    idx = 4;
    break;
  default:
    idx = -1;
    break;
  }
  
  if (idx >= 0)
    return _expectedTrkDedxSvt[idx]; 
  else 
    return -1.;

}

double PacTruncMean::getExpectedTrkDedxDch(PdtPid::PidType pidType) const {

  if (pidType==PdtPid::electron) return _expectedTrkDedxDch[0];
  else if (pidType==PdtPid::muon) return _expectedTrkDedxDch[1];
  else if (pidType==PdtPid::pion) return _expectedTrkDedxDch[2];
  else if (pidType==PdtPid::kaon) return _expectedTrkDedxDch[3];
  else if (pidType==PdtPid::proton) return _expectedTrkDedxDch[4];
  else return -1;

}

//--------------------------------------------------------------------------
// Name:
//   PacReconstructEmc
/// Description:
//   Simulate the reconstruction of an EMC cluster.
// Environment:
//   Software developed for PACRAT / SuperB
//
//  Copyright Information:
//      Copyright (C) 2008      Caltech
//
//  Authors:  Chih-hsiang Cheng    2008/07/21
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"
#include "PacEmc/PacReconstructEmc.hh"
#include "ErrLogger/ErrLog.hh"
#include "PDT/PdtEntry.hh"

#include "PacGeom/PacMeasurement.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcCalibrator.hh"

#include "PacSim/PacSimTrack.hh"
#include "PacGeom/PacDetElem.hh"

#include "TH2F.h"
#include <iostream>
using namespace std;

PacReconstructEmc::PacReconstructEmc() :
  _centroidAlgo(PacEmcCluster::logarithmic)
  , _model(0), _strkH1(0), _strkH2(0), _verbose(false)
{
  init();
}

PacReconstructEmc::PacReconstructEmc(const PacEmcCluster::CentroidMethod algo) :
  _centroidAlgo(algo)
  , _model(0), _strkH1(0), _strkH2(0), _verbose(false)
{
  init();
}

PacReconstructEmc::~PacReconstructEmc() {
  delete _strkH1;
  delete _strkH2;
}

void PacReconstructEmc::init() {
  _model= PacEmcModel::getModel();
  _strkH1= new TH2F("strkH1","SimTrk KE map", _model->nbinsThetaSimTrkKE(), 0, Constants::pi, _model->nbinsPhiSimTrkKE(), -Constants::pi, Constants::pi); 
  _strkH2= new TH2F("strkH2","SimTrk KE map, merge nearby bins", _model->nbinsThetaSimTrkKE(), 0, Constants::pi, _model->nbinsPhiSimTrkKE(), -Constants::pi, Constants::pi); 
}

// Fill a histogram with simtrk KE at EMC
void PacReconstructEmc::fillSimTrkKEMap(const PacSimTrack *simtrk) {
  
  // Basic sanity checks
  if(simtrk != 0 && simtrk->getGTrack() != 0 && simtrk->getTraj() != 0 ) {    
    const std::vector<PacSimHit>& hitlist= simtrk->getHitList();
    for ( std::vector<PacSimHit>::const_iterator ihit= hitlist.begin();
	  ihit!= hitlist.end(); ihit++) {

      const DetIntersection& dinter = ihit->detIntersection();
      const PacDetElem* pelem = dynamic_cast<const PacDetElem *>(dinter.delem);
      
      if( pelem != 0 && pelem->measurementDevices().size() != 0){
        for(std::vector<const PacMeasurement*>::const_iterator imdev = pelem->measurementDevices().begin();
        imdev != pelem->measurementDevices().end();imdev++){
          const PacMeasurement* meas = *imdev;
          if(meas->measurementType()==PacMeasurement::Calor &&
          pelem->activeRegion(ihit->position()) ) {

            double t0= ihit->time();
            PacEmcModel::EmcRegion region= _model->region(ihit->position());
            double frac=1;
            if ( t0< _model->signalTLo(region) || t0> _model->signalTHi(region) ) {
              frac= _model->signalFraction(t0, region);
            }

            const GTrack* gt= simtrk->getGTrack();
            double ke= gt->p4().e() - gt->pdt()->mass();
            _strkH1->Fill(ihit->position().theta(), ihit->position().phi(), ke*frac );

  // remember this simtrack's first emc hit position
            _stptmap[ simtrk ]= ihit->position();

            break;  // only look at the first emc hit
          }
        }
      }
    }
  }

  if ( _verbose ) _strkH1->Print();

}


void PacReconstructEmc::clearSimTrkKEMap() { 
  _strkH1->Reset();
  _strkH2->Reset();
}

std::vector< PacEmcCluster* > PacReconstructEmc::makeEmcCluster(const PacSimTrack *simtrk) const {

  if ( _verbose ) ErrMsg(routine) << "makeEmcCluster" << endmsg;

  std::vector< PacEmcCluster* > vclusters;
  PacEmcCluster *cluster(0);

  // Basic sanity checks
  if(simtrk != 0 && simtrk->getGTrack() != 0 && simtrk->getTraj() != 0 ) {

    // check if this simtrk is over threshold
    //   find the bin
    std::map< const PacSimTrack*, HepPoint >::const_iterator iter= _stptmap.find( simtrk );
    if ( iter == _stptmap.end() ) return vclusters;

    HepPoint pt= iter->second;  // first EMC hit position
    int ix= _strkH1->GetXaxis()->FindBin(pt.theta());
    int iy= _strkH1->GetYaxis()->FindBin(pt.phi());

    // check the content of the second map, if empty, fill it with the content
    //  of the first map and the nearby bins  ( 3X3 )
    if ( _strkH2->GetBinContent(ix,iy) == 0 ) {
      double ee= 0;
      for ( int i=ix-1; i<= ix+1; i++) {
	for ( int j=iy-1; j<= iy+1; j++) {
	  int ii=i;  int jj=j;
	  if ( ii < 1 || ii > _strkH1->GetNbinsX() ) continue;
	  if ( jj < 1 ) jj= _strkH1->GetNbinsY() ;  // wrap around in phi
	  else if ( jj > _strkH1->GetNbinsY() ) jj= 1;
	  ee+= _strkH1->GetBinContent(ii,jj);
	}
      }
      _strkH2->SetBinContent(ix,iy,ee);
    }

    if ( _strkH2->GetBinContent(ix,iy) < _model->simTrkKEThreshold() ) {
      // too low, return 0 
      return vclusters;
    }

    int i=0;
    const std::vector<PacSimHit>& hitlist= simtrk->getHitList();
    for ( std::vector<PacSimHit>::const_iterator ihit= hitlist.begin();
	  ihit!= hitlist.end(); ihit++) {

      const DetIntersection& dinter = ihit->detIntersection();
      const PacDetElem* pelem = dynamic_cast<const PacDetElem *>(dinter.delem);

      if( pelem != 0 && pelem->measurementDevices().size() != 0){
        for(std::vector<const PacMeasurement*>::const_iterator imdev = pelem->measurementDevices().begin();
        imdev != pelem->measurementDevices().end();imdev++){
          const PacMeasurement* meas = *imdev;
          if(meas->measurementType()==PacMeasurement::Calor &&
          pelem->activeRegion(ihit->position()) ) {

            if ( _verbose ) {
              cout <<  "SimHit " << i << ": " << (*ihit).position() << " effect " << (*ihit).detEffect()
                << " de= " << (*ihit).energyChange();
              if ( (*ihit).detIntersection().delem ) {
                cout << " element " << (*ihit).detIntersection().delem->elementName()
                  << " elem# = " << (*ihit).detIntersection().delem->elementNumber();
              } else {
                cout << " no det element";
              }
              cout << endl;
              i++;
            }

	// create a cluster if necessary
            if(cluster == 0)cluster = new PacEmcCluster(_centroidAlgo);
  // Create a cluster, or add energy deposition to the cluster
            PacEmcCluster *newcl= new PacEmcCluster(_centroidAlgo);
            meas->createEmcCluster(*ihit, newcl, _verbose);
	
  // If it is a new cluster, or if the newly created cluster is connected
  // to previously created one, merge them
            if ( cluster->numberOfDigis()==0 || newcl->isConnectedTo(cluster) ) {
              cluster->mergeCluster(newcl);
              delete newcl;
            } else {
    // The previously created cluster is separated from the new one.
    // Add the old one to the list.
              pushClusterToList(cluster,vclusters);
              cluster= newcl;
            }
          }
        }
      }
    } // end looping simhits in the simtrack
    pushClusterToList(cluster,vclusters);
  }

  return vclusters;
}

void PacReconstructEmc::pushClusterToList(PacEmcCluster* cluster, std::vector<PacEmcCluster*> &v) const {

  if ( cluster==0 ) return;
  if ( cluster->numberOfDigis() == 0 ) {
    delete cluster;
    cluster=0;
  } else if ( isnan(cluster->energy()) ) {
    ErrMsg(warning) << "This cluster's energy is nan?" << endmsg;
    cluster->print(cout,1);
    delete cluster;
    cluster= 0;
  } else {
    cluster->fluctuateDigis(); // while keeping the cluster energy unchanged
    //cluster->removeNoise(); // remove stuff under threshold
    if ( cluster->numberOfDigis() > 0 ) {
      //  cluster->addNoise(); // add some noise (all above threshold)
    } else {
      delete cluster;
      cluster= 0;
    }
  }
  if ( cluster ) {
    cluster->simulateHitTime();
    v.push_back(cluster);
  }
}

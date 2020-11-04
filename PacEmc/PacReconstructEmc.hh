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
#ifndef PacReconstructEmc_HH
#define PacReconstructEmc_HH

#include "PacEmc/PacEmcCluster.hh"
#include "CLHEP/HepPoint.h"
#include <map>

class GTrack;
class PacSimTrack;
class PacSimHit;
class PacEmcModel;
class TH2F;

//class TrkRecoTrk;

class PacReconstructEmc {

public:

  PacReconstructEmc();
  PacReconstructEmc(const PacEmcCluster::CentroidMethod algo);
  virtual ~PacReconstructEmc();

  void init();
  void setCentroidMethod(const PacEmcCluster::CentroidMethod algo) {_centroidAlgo= algo;}

  // Fill a histogram with simtrk KE at EMC
  void fillSimTrkKEMap(const PacSimTrack *simtrk) ;
  void clearSimTrkKEMap();

  // Generate some reco object here
  // It is difficult to generate EmcCand or EmcBump because
  // they require EmcCluster.
  // Now let's just return values for BtaMicroAdapter
  std::vector< PacEmcCluster* > makeEmcCluster(const PacSimTrack *simtrk) const;
  void pushClusterToList(PacEmcCluster* cluster, std::vector<PacEmcCluster*> &v) const;

  void setVerbose() { _verbose= true; }

private:

  PacEmcCluster::CentroidMethod _centroidAlgo;
  PacEmcModel *_model;
  TH2F *_strkH1, *_strkH2;
  bool _verbose;
  std::map< const PacSimTrack*, HepPoint > _stptmap;
};
#endif


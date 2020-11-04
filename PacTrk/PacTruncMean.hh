//--------------------------------------------------------------------------
// Name:
//   PacTruncMean
// Description:
//    Compute the dE/dx of a track
//
// Environment:
//   Software developed for PACRAT / SuperB
//
//  Copyright Information:
//      Copyright (C) 2009
//
//  Authors: Matteo Rama  -  Laboratori Nazionali di Frascati (INFN)
//-----------------------------------------------------------------------
#ifndef PacTruncMean_HH
#define PacTruncMean_HH

#include "PDT/PdtPid.hh"

class TrkRecoTrk;
class GTrack;

class PacTruncMean {

public:

  PacTruncMean(const TrkRecoTrk* recotrk);
  PacTruncMean(const TrkRecoTrk* recotrk, const GTrack* gtrk);
  virtual ~PacTruncMean();
  
  void getdEdxDch(double&,double&) ;
  void getdEdxSvt(double&,double&) ;
  int getnDchSampleHits(){ return _ndchsample; }
  int getnSvtSampleHits(){ return _nsvtsample; }

  double getExpectedTrkDedxSvt(PdtPid::PidType pidType) const; 
  double getExpectedTrkDedxDch(PdtPid::PidType pidType) const; 

private:

  void InitDch(const TrkRecoTrk*, const GTrack*);
  void InitSvt(const TrkRecoTrk*, const GTrack*);
  double _truncFrac_dch;
  double _dedx_dch;
  double _ededx_dch;
  int _ndchsample;
  double _truncFrac_svt;
  double _dedx_svt;
  double _ededx_svt;
  int _nsvtsample;

  double _expectedTrkDedxSvt[5];
  double _expectedTrkDedxDch[5];

};
#endif

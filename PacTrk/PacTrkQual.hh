//--------------------------------------------------------------------------
// Name:
//   PacTrkQual
// Description:
//
// Environment:
//   Software developed for PACRAT / SuperB
//
//  Copyright Information:
//      Copyright (C) 2008
//
//  Authors: Gabriele Simi  -  U{niversity of Maryland
//-----------------------------------------------------------------------
#ifndef PacTrkQual_HH
#define PacTrkQual_HH

class BtaTrkQual;
class TrkRecoTrk;

class PacTrkQual {

public:

  PacTrkQual();
  virtual ~PacTrkQual();

  void makeTrkQual(BtaTrkQual* trkQual, const TrkRecoTrk* recotrk);

private:

};
#endif

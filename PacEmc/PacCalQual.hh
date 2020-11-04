//--------------------------------------------------------------------------
// Name:
//   PacCalQual
// Description:
//
// Environment:
//   Software developed for PACRAT / SuperB
//
//  Copyright Information:
//      Copyright (C) 2008
//
//  Authors:
//-----------------------------------------------------------------------
#ifndef PacCalQual_HH
#define PacCalQual_HH

class BtaCalQual;
class PacEmcCluster;

class PacCalQual {

public:

  PacCalQual();
  virtual ~PacCalQual();

  void makeCalQual(BtaCalQual* calQual, const PacEmcCluster* cluster);

private:

};
#endif

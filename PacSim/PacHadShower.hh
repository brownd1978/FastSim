//--------------------------------------------------------------------------
// Name:
//   PacHadShower
//  Description:
//  Class to describe hadronic showering in Pacrat
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 26 Jan 2009
//------------------------------------------------------------------------
#ifndef PacHadShower_hh
#define PacHadShower_hh
#include "PacSim/PacShowerModel.hh"

class PacHadShower : public PacShowerModel {
public:
// shower should know what type it is
  PacHadShower();
  virtual ~PacHadShower();
// step through the material as specified and compute the shower effect.
  virtual void showerStep(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,
    const PacShowerInfo& sinfo,PacShowerInfo& newshower) const;
  double hadShowerFraction(double xlow, double dx) const;
private:
  double _ehad;
  double _hadscale;
  int _hadorder;
};
#endif

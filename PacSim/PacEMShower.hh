//--------------------------------------------------------------------------
// Name:
//   PacEMShower
//  Description:
//  Class to describe EM showering in Pacrat
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 26 Jan 2009
//------------------------------------------------------------------------
#ifndef PacEMShower_hh
#define PacEMShower_hh
#include "PacSim/PacShowerModel.hh"

class PacEMShower : public PacShowerModel {
public:
// shower should know what type it is
  PacEMShower();
  virtual ~PacEMShower();
// step through the material as specified and compute the shower effect.
  virtual void showerStep(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,
    const PacShowerInfo& sinfo,PacShowerInfo& newshower) const;
  // helper function
  double approxGammaIntegral(double efrac, double bterm, double cterm, double xlow, double dx) const;
  double findShowerStart(double eval,double bterm,double cterm, double din,double showerfrac) const;
  
private:
  static const double _tolerance; // fractional energy tolerance for integration
};
#endif

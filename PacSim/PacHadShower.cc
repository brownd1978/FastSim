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
#include "BaBar/BaBar.hh"
#include "PacSim/PacHadShower.hh"
#include "PacSim/PacShowerInfo.hh"
#include "DetectorModel/DetMaterial.hh"
#include "PDT/PdtEntry.hh"
#include "PacEnv/PacConfig.hh"
#include "CLHEP/Random/RandPoisson.h"


#include <cassert>

PacHadShower::PacHadShower() : PacShowerModel("StandardHadShower") 
{
  _ehad = gconfig.getdouble("PacShower.hadshowerquantum",-1.0);
  _hadorder = gconfig.getint("PacShower.hadorder",-1);
  _hadscale = gconfig.getdouble("PacShower.hadscale",-1.0);
  assert(_hadscale >0 && _hadorder > 0 && _ehad > 0.0);
}

PacHadShower::~PacHadShower()
{}

void
PacHadShower::showerStep(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,
const PacShowerInfo& oldshower,PacShowerInfo& newshower) const {
  double eshower = oldshower.showerEnergy();
  double showerfrac = oldshower.fractionOut();
  double din = oldshower.depthOut();
  // compute the average normalized shower profile fraction deposited in this element (shower profile integral)
  double intfrac = pathlen/mat->intLength();
  double efrac = hadShowerFraction(showerfrac,intfrac);
  // randomize the energy actually deposited in quanta of energy
  double ein = oldshower.energyOut();
  double nq = efrac*eshower/_ehad;
  // we need to account for transition radiation someday: FIXME!  
  double edep = _ehad*RandPoisson::shoot(_randengine,nq);
  double eout = std::max(ein-edep,0.0);
  // transverse size; FIXME!!!
  double trans = 1.0;
  newshower = PacShowerInfo(eshower,ein,eout,edep,mat,din,din+intfrac,trans,trans,0.0,this);
}

double
PacHadShower::hadShowerFraction(double xlow, double dx) const {
// use fixed power, fixed scale
// compute the integral; all the terms have an exponential in them
  double xplow = xlow/_hadscale;
  double xphi = (xlow+dx)/_hadscale;
  double vhi = exp(-xphi)*polyfun(xphi,_hadorder);
  double vlow = exp(-xplow)*polyfun(xplow,_hadorder);
  return vhi - vlow;
}

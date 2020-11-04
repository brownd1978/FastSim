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
#include "BaBar/BaBar.hh"
#include "PacSim/PacEMShower.hh"
#include "PacSim/PacShowerInfo.hh"
#include "DetectorModel/DetMaterial.hh"
#include "PDT/PdtEntry.hh"
#include "PacEnv/PacConfig.hh"
#include <iostream>
#include "ErrLogger/ErrLog.hh"

const double PacEMShower::_tolerance(1e-5);

PacEMShower::PacEMShower() : PacShowerModel("StandardEMShower") 
{}

PacEMShower::~PacEMShower()
{}

void
PacEMShower::showerStep(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,
const PacShowerInfo& oldshower,PacShowerInfo& newshower) const {
  const double mineshower(0.001); // minimum shower energy
  // the output state of the current shower info is the input to this showering step.
  double eshower = oldshower.showerEnergy();
  double showerfrac = oldshower.fractionOut();
  double din = oldshower.depthOut();
  double ein = oldshower.energyOut();
    // critical energy in this material
  double ecrit = 0.8/(1.2+ mat->zeff());    
  double radfrac = mat->radiationFraction(pathlen);
  double eout(ein);
  double edep(0.0);
  static const double maxradfrac(20);

  if ( radfrac > maxradfrac ) {
    eout= 0;
    edep = ein - eout;
  } else if(ein > ecrit){
// the shower formula is only valid when ein > ecrit.
    bool charged = pdt->charge() != 0;
// compute the average normalized shower profile fraction deposited in this element (shower profile integral)
    double cterm = charged ? -0.5 : 0.5; // shower offset for gammas vs electrons
    double bterm = 0.5; // ignore material effects on shower parameters for now
// if the material is the same, comput the integral starting with the input rad. len.  If
// not, we must adjust the input radiation length to give physically meaningfull results
    if(oldshower.material()!=0 && mat != oldshower.material())
// find the rad. len. that gives the same initial shower fraction in this material.
      din = findShowerStart(eshower/ecrit,bterm,cterm,din,showerfrac);
// compute the shower integral over this radiation
    double efrac = approxGammaIntegral(eshower/ecrit,bterm,cterm,din,radfrac);
    if(efrac+showerfrac - 1.0 > _tolerance )
      std::cerr << "Shower integral not working! " << std::endl;
    eout = std::max(ein - efrac*eshower,0.0);
// check for the remaining energy being below the threshold: if so, we deposit all the energy
    if(eout < mineshower)eout = 0.0;
// we need to account for transition radiation and shower fluctuations: FIXME!
    edep = ein-eout;    
  } else {
// We're below the critical energy for this material, need to kludge
    static const double minedep(0.02);
    if(ein > minedep){
      edep = minedep;
      eout = ein - edep;
    } else {
      eout = 0;
      edep = ein - eout;
    }
    
    
  }
// compute transverse size of shower at this point
  double trans = 1.0; // dummy, FIXME!!!
    // update the shower info for this material
  newshower = PacShowerInfo(eshower,ein,eout,edep,mat,din,din+radfrac,trans,trans,0.0,this);
}

// approximate integral of the gamma-function shower profile.
double
PacEMShower::approxGammaIntegral(double efrac,double bterm, double cterm,
 double xlow, double dx) const {
// bound the integral power to the one implied by the true shower formula
// this allows closed-form integration
  double elog = log(efrac);
  int ihi = (int)rint(std::max(1.0,ceil(bterm*(elog + cterm))));
// compute the scaling factor (puts shower max in the right place)
  double hifac = (ihi/bterm - cterm)/elog;
  double xhilow = bterm*xlow*hifac;
  double xhihi = bterm*(xlow+dx)*hifac;
  double hiint = powerExpIntegral(ihi,xhilow,xhihi);
  double retval;
  int ilo = ihi-1;
  if(ilo > 0){
    double lofac = (ilo/bterm - cterm)/elog;
    double xlolow = bterm*xlow*lofac;
    double xlohi = bterm*(xlow+dx)*lofac;
    double loint = powerExpIntegral(ilo,xlolow,xlohi);
  // linear interpolation
    double hifac = bterm*(elog + cterm) - ilo;
    retval = hifac * hiint + (1.0 - hifac)*loint;
  } else {
    retval = hiint;
  }
  return retval;
}

double
PacEMShower::findShowerStart(double eval,double bterm,double cterm,
double din,double showerfrac) const {
  static unsigned maxstep(1000);
  static const double delta(0.01); // interval for derivative calculation
  static double delta2 = delta*delta;
//  static const double tiny(1.0e-5); // small number
  double dout = din;
  double diff = 1.0;
  unsigned nstep(0);
  do {
// quadratic interpolation
    double funval = approxGammaIntegral(eval,bterm,cterm,0,dout);
    double pfunval = approxGammaIntegral(eval,bterm,cterm,0,dout+delta);
    double mfunval = approxGammaIntegral(eval,bterm,cterm,0,dout-delta);
    double deriv1 = 0.5*(pfunval-mfunval)/delta;
    double deriv2 = (pfunval+mfunval-2*funval)/delta2;
    diff = showerfrac - funval;
    double sterm = deriv1*deriv1+4*deriv2*diff;
    double dx(0.0);
    if(sterm > 0.0){
      double sq = sqrt(sterm);
      double dx1 = 0.5*(-deriv1+sq)/deriv2;
      double dx2 = 0.5*(+deriv1+sq)/deriv2;
// choose positive 
      if(dx1+dout > 0.0 && dx2+dout > 0.0){
        if(fabs(dx1)<fabs(dx2))
          dx = dx1;
        else
          dx = dx2;
      } else if(dx1+dout > 0.0) {
        dx = dx1;
      } else if(dx2+dout > 0.0){
        dx = dx2;
      } else {
// very non-linear behavior: simply decrease the value by two
        dx = -dout/2.0;
      }
    } else {
      dx = diff/deriv1;
    }
    dout += dx;
    nstep++;
  } while (nstep < maxstep && fabs(diff) > _tolerance);
  if(fabs(diff) > _tolerance)
    ErrMsg(error) << "Interpolation failed in findShowerStart" << endmsg;
  return dout;
}

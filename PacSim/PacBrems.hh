//--------------------------------------------------------------------------
// Name:
//   PacBrems
//  Description:
//  Class to describe Bremsstrahlung
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2010	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 10 Feb 2010
//------------------------------------------------------------------------
#ifndef PacBrems_HH
#define PacBrems_HH

#include "CLHEP/Random/RandomEngine.h"
using CLHEP::HepRandomEngine;

class PacBrems  {
public:
  PacBrems();
  virtual ~PacBrems();
// compute the energy loss and brems photon energies given the radiation fraction and incoming energy.
//  Note that total energy loss is larger than
// the sum of the brems photon energies, as only photons above a minimum energy are counted as particles
// ( the total # of photons is IR divergent).
  unsigned nBrems(double radfrac, double& energy) const;  
// allow resetting the random engine.
  void setRandomEngine(HepRandomEngine* engine) { _randengine = engine; }
// 
  static double dndy(double radfrac,double efrac); // number density function
  static double ngam(double radfrac, double efrac); // (upper) integrated number density function
  static double elow(double radfrac, double efrac); // (lower) integrated energy function
private:
// inverse integral of the photon number density as a function of the photon energy fraction
  double invngam(double x) const;
  mutable HepRandomEngine* _randengine;
// constants
  static const double _ft;
  static const double _ot;
//
  double _minegam;
  double _minefrac;
};
#endif

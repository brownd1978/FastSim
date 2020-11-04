//--------------------------------------------------------------------------
// Name:
//   PacShowerModel
//  Description:
//   Base class to describe particle showering in Pacrat
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 26 Jan 2009
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacSim/PacShowerModel.hh"
#include "CLHEP/Random/Random.h"
#include <cmath>
#include <cassert>

PacShowerModel::PacShowerModel(const char* name) : _name(name)
{
  // Get the random number enginae
	_randengine = HepRandom::getTheEngine();
}

PacShowerModel::~PacShowerModel()
{}

double
PacShowerModel::powerExpIntegral(int n, double xlow, double xhi) {
// this function returns the integral of the (normalized) function x^n exp(-x) over the range xlow to xhi.
  double vhi = exp(-xhi)*polyfun(xhi,n);
  double vlow = exp(-xlow)*polyfun(xlow,n);
  return vhi - vlow;
}

double
PacShowerModel::polyfun(double x,int n) {
// integral of x^n exp(-x), divided by exp(-x).  This is solved for recursively
  assert(n <= 9);
  static int factorial[10] = {1,1,2,6,24,120,720,5040,35280,282240};
  double retval = -pow(x,n)/factorial[n];
  if(n>0) retval += polyfun(x,n-1);
  return retval;
}

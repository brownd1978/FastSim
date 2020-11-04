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
#ifndef PacShowerModel_hh
#define PacShowerModel_hh

class DetMaterial;
class PacShowerInfo;
class PdtEntry;
#include "CLHEP/Random/RandomEngine.h"

#include <string>

class PacShowerModel {
public:
  PacShowerModel(const char* name);
  virtual ~PacShowerModel();
// identification
  const std::string& name() { return _name;}
// step through the material as specified and compute the shower effect.
  virtual void showerStep(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,
    const PacShowerInfo& sinfo,PacShowerInfo& newshower) const = 0;
  // helper functions
  static double powerExpIntegral(int n, double xlow, double xhi);
  static double polyfun(double x,int n);
  // allow resetting the random engine.
  void setRandomEngine(HepRandomEngine* engine) { _randengine = engine;}
protected:
  mutable HepRandomEngine* _randengine;
private:
  std::string _name;
};
#endif

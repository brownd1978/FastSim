//--------------------------------------------------------------------------
// Name:
//   PacDetEffect
//  Description:
//   Describe particle-detector interaction in Pacrat
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 23 Apr 2008
//------------------------------------------------------------------------
#ifndef PacDetEffect_HH
#define PacDetEffect_HH

class DetMaterial;
class DetIntersection;
class PdtEntry;
class PacShowerInfo;
class PacSimTrack;
#include "PacSim/PacSimHit.hh"
#include "PacSim/PacEMShower.hh"
#include "PacSim/PacHadShower.hh"
#include "PacSim/PacBrems.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Random/RandomEngine.h"


class PacDetEffect {
public:
	PacDetEffect();
	~PacDetEffect(){};
// compute the effect, and update the simtrack appropriately
  PacSimHit::effect detEffect(const DetIntersection& dinter,const HepPoint& pos,Hep3Vector& mom,
    PacSimTrack* strk) const;
// decide what effect affects the given particle and interaction
// see PacSimHit.hh for an explanation of the simulated effects
  PacSimHit::effect detEffect(PacSimHit::effect oldeff,double pathlen, const DetMaterial* mat,const PdtEntry* pdt,
    const PacShowerInfo* oldshower, Hep3Vector& mom, PacShowerInfo& sinfo) const;
// model the specifics of a given interaction.
// first 'normal' ionization energy loss and multiple Coulomb scattering
  PacSimHit::effect normal(const DetMaterial*, double pathlen ,const PdtEntry*,Hep3Vector& mom) const;
// hadronic interaction and showering
  PacSimHit::effect hadEffect(const DetMaterial*, double pathlen, const PdtEntry*,Hep3Vector& mom) const;
// electromagnetic interactions and showering
  PacSimHit::effect emEffect(const DetMaterial*, double pathlen ,const PdtEntry* ,Hep3Vector& mom) const;
// Pdt functions to separate particle types
  bool isHadron(const PdtEntry* pdt) const;
  bool isEM(const PdtEntry* pdt) const;
// find the point where a shower actually starts, and update the momentum for ionization/scattering up to that point
  void findShowerStart(const DetMaterial* mat,const PdtEntry* pdt,double& pathlen,Hep3Vector& mom) const;
// allow resetting the random engine.  This operates recursively
  void setRandomEngine(HepRandomEngine* engine);
// detailed scattering model from CPC 141(2001 230-246 (Fruhwirth etal))
  void scatter(double scatter[2],const DetMaterial* mat,double mom, double pathlen, double mass) const;
  void fewScatter(double scatter[2], double nsinglescat,const DetMaterial* mat,double mom) const;
  void singleScatter(double scatter[2],double aparam, double bparam) const;
  void projectScatter(double scatter[2],double spangle) const;
  void semiGaussScatter(double scatter[2],const DetMaterial* mat,double nsinglescatter, double mom,double pathlen,double mass) const;
  void gaussScatter(double scatter[2],double sigma) const;
  void multiGaussScatter(double scatter[2],const DetMaterial* mat,double mom,double pathlen,double mass) const;
private:
// Function to find the correct shower model
  const PacShowerModel* showerModel(const DetMaterial* mat,const PdtEntry* pdt) const;
	mutable HepRandomEngine* _randengine;
// energy-loss parameters	
  double _lanshift; // shift to landau
  double _lanrms; // RMS of (truncated) landau
// scattering parameters
  double _smallangle;
  double _tailprob; // fraction of scattering in tail
  double _tailfact; // RMS scaling for tail of scattering
  double _corefact; // RMS scaling for core of scattering
  double _thinfract; // fraction of int length to call a material 'thin'
  double _minmom_e;  // minimum momenta for various particles
  double _minmom_gamma;
  double _minmom_had;
  double _mine;
  double _maxnsingle; // maximum number of single scatters to explicitly simulate
  double _maxradfrac1; // radiation length fraction to switch from semi-Gauss to multi-Gauss
  double _maxradfrac2; // radiation length fraction to switch from multi-Gauss to single-Gauss
// shower parameters
  double _showerradfrac; // radiation fraction which always starts a shower
  int _nshower; // number of brems photon to consider as a shower
// shower models
  PacEMShower _emshower;
  PacHadShower _hadshower;
  PacBrems _brems;
// statics
  static const Hep3Vector _nullmom;
};

#endif

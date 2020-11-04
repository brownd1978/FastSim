// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkHitViewDch; Implementation of PacTrkHitView for an Drift Chamber with
//   axial and/or stereo wires
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
//
//  28 jan 2009: replaced the DCH flat reso. function with 5-order polynomial [Matteo Rama]
//  28 may 2010: DCH hit efficiency dependent on path length [Matteo Rama]
// ------------------------------------------------------------------------------
#ifndef PacTrkHitViewDch_HH
#define PacTrkHitViewDch_HH

#include "PacTrk/PacTrkHitView.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "PacTrk/PacHitRes.hh"

class PacTrkHitViewDch : public PacTrkHitView {
public:

  // construct with stereo angle, signed as the angle WRT the z axis when viewed from
  // outside the chamber
  PacTrkHitViewDch(const PacHitRes& hres,double ngfrac, double ngfactor,
		   double eff_par0,double eff_par1,double cell_size,double sangle=0.0);
  
  virtual ~PacTrkHitViewDch();

  // override the base class functions with real implementations
  virtual void getHitInfo(const PacSimHit& simhit,std::vector<PacRecHitInfo>&) const;
  virtual double efficiency(const PacSimHit& simhit) const;
  virtual double measurementAngle() const;
  virtual double sensorSeparation() const;
  virtual double sensorSize() const;
  // accessors
  inline bool isAxial() const { return whatView()==TrkEnums::xyView;}
  double stereoAngle() const { return _sangle;}
  double cellSize() const { return _cell_size; }
  double resFun(double x) const;
  double driftVelocity() const { return _vdrift;}
private:

  //parameters of the spatial resolution function
  PacHitRes _hres;
  // non-Gaussian tail parameters
  double _ngfrac;
  double _ngfactor;
  double _ngcor;
  //parameters of the hit efficiency
  double _eff_par0;
  double _eff_par1;

  //Cell size
  double _cell_size;
  // average drift velocity
  double _vdrift;
  // minimum fractional transverse path to (possibly) generate additional hits in the same layer, due
  // to track inclination.  This is relative to the cell size, so dimensionless.
  double _mincpath;
  double _sangle;  // stereo wire angle WRT z axis

  // cache to improve efficiency
  double _sins;
  double _coss;

};

#endif
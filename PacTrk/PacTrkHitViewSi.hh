// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkHitViewSi; Simple implementation of PacTrkHitView as a 'cylinder' of 
//   Si, with readout in axial and z direction
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------
#ifndef PacTrkHitViewSi_HH
#define PacTrkHitViewSi_HH


#include "PacTrk/PacTrkHitView.hh"
#include "CLHEP/Vector/ThreeVector.h"

class PacTrkResModelSi;

class PacTrkHitViewSi : public PacTrkHitView {
public:

  PacTrkHitViewSi(TrkEnums::TrkViewInfo view,PacTrkResModelSi* resModel, 
		  double sign, double ptrat, double ngfrac, double ngfact, double eff);

  virtual ~PacTrkHitViewSi();

  // override the base class functions with real implementations
  virtual void getHitInfo(const PacSimHit& simhit,std::vector<PacRecHitInfo>&) const;
  virtual double efficiency(const PacSimHit& simhit) const;
  virtual double measurementAngle() const;
// rough measure of how far apart active elements are in the measurement direction
  virtual double sensorSeparation() const;
// rough measure of sensor size perpendicular to measurement direction;
  virtual double sensorSize() const;

  // Access the pitch-to-thickness ratio
  double getPtoT() const { return _ptrat; }

private:
// resolution model;
  PacTrkResModelSi* _resModel;

  double _sig0; // core in-plane resolution
  double _sign2; // (squared) resolution normal to the wafer plane (aplanarity rms)
  double _ptrat; // strip pitch/wafer thicknes ratio
  double _ngfrac; // fraction of resolution in the non-Gaussian tail
  double _ngfact; // factor to scale sigma by for hits in the NG tail
  double _eff; // hit efficiency
  double _rms; // cache of rms (weighted average of core and tail sigmas), used for hit errors

};

#endif

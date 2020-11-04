// ------------------------------------------------------------------------------
// File and Version Information:
//   Planar detector type modeled as a ring.
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//  Authors: Dave Brown, 01 August 2008
// ------------------------------------------------------------------------------
#ifndef PacRingDetType_HH
#define PacRingDetType_HH
//
//  Includes
//
#include "PacGeom/PacPlaneDetType.hh"
//
//  Define the class
//
class PacRingDetType: public PacPlaneDetType {
public:
// Specify high and low radii
	PacRingDetType(double thick,double lowrad,double highrad, const DetMaterial*,int);
//  Destructor
  virtual ~PacRingDetType();
//
//  real versions of the virtual functions
//
  virtual bool physicalMaterial(const TypeCoord*) const;
  virtual double estimatedSize() const;
// direct accessors
  double lowrad() const { return _lowrad;}
  double highrad() const { return _highrad;}
// data members
private:
	double _lowrad;
	double _highrad;
	double _r2limits[2]; // limits on radius squared
};
#endif


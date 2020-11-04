// ------------------------------------------------------------------------------
// File and Version Information:
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//  Authors: Dave Brown, 01 August 2008
// ------------------------------------------------------------------------------
#ifndef PacRectDetType_HH
#define PacRectDetType_HH
//
//  Includes
//
#include "PacGeom/PacPlaneDetType.hh"
class DetMaterial;
//
//  Define the class
//
class PacRectDetType: public PacPlaneDetType {
public:
// specify limits in (local) cartesian coordinates
	PacRectDetType(double thick, double usize, double vsize,
		     const DetMaterial*,int);
//  Destructor
  virtual ~PacRectDetType();
//
//  real versions of the virtual functions
//
  virtual bool physicalMaterial(const TypeCoord*) const;
  virtual double estimatedSize() const;
// direct accessors
  double uSize() const { return _halfusize*2.0;}
  double vSize() const { return _halfvsize*2.0;}
  double halfuSize() const { return _halfusize;}
  double halfvSize() const { return _halfvsize;}
// data members
private:
	double _halfusize;  // half sizes are more efficient
	double _halfvsize;
};
#endif

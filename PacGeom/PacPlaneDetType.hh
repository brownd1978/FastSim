// ------------------------------------------------------------------------------
// File and Version Information:
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//  Authors: Dave Brown, 01 August 2008
// ------------------------------------------------------------------------------
#ifndef PacPlaneDetType_HH
#define PacPlaneDetType_HH
//
//  Includes
//
#include "DetectorModel/DetSurfaceType.hh"
class DetMaterial;
//
//  Define the class
//
class PacPlaneDetType: public DetSurfaceType {
public:
// DetSurfaceType based on a plane
	PacPlaneDetType(double thick,const char* name,const DetMaterial*,int);
//  Destructor
  virtual ~PacPlaneDetType();
//
//  real versions of the virtual functions
//
  const DetMaterial& material(const TypeCoord*) const { return *_thematerial;}
	double thickness(const TwoDCoord*) const { return _thick;}
	double thick() const { return _thick;}
  virtual double estimatedSize() const = 0;
// data members
private:
  double _thick; // Plane thickness
	const DetMaterial* _thematerial; // material of this type
};
#endif

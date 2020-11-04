// ------------------------------------------------------------------------------
// File and Version Information:
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Subclass of PacCylDetType with a constant projective thickness 
//  Authors: Dave Brown, 12/01/08
// ------------------------------------------------------------------------------
#ifndef PacEmcCylType_HH
#define PacEmcCylType_HH
//
//  Includes
//
#include "PacGeom/PacCylDetType.hh"
//
//  Define the class
//
class PacEmcCylType : public PacCylDetType {
public:
//
//  Constructor; build from the primitive
//
  PacEmcCylType(double radius,double thick,double zends[2],
		     const DetMaterial*,int);
//  Destructor
  virtual ~PacEmcCylType();
//
// the purpose of this class is to override the following
//
  virtual double thickness(const TwoDCoord*) const;
private:
// need to know the cylinder radius to compute the angle
  double _radius;
// cache radius^2
  double _rad2;
};
#endif

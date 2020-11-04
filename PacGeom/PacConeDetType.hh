// ------------------------------------------------------------------------------
// File and Version Information:
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#ifndef PacConeDetType_HH
#define PacConeDetType_HH
//
//  Includes
//
#include "DetectorModel/DetSurfaceType.hh"
class DetMaterial;
//
//  Define the class
//
class PacConeDetType: public DetSurfaceType {
public:
//
//  Constructor; build from the primitive
//
  PacConeDetType(double thick,double rends[2],
		     const DetMaterial*,int);
//  Destructor
  virtual ~PacConeDetType();
//
//  real versions of the virtual functions
//
  bool physicalMaterial(const TypeCoord*) const ;
  const DetMaterial& material(const TypeCoord*) const { return *_thematerial;}
	double thickness(const TwoDCoord*) const { return _thick;}
	double thick() const { return _thick;}
	double lowR() const {return _rend[0];}
	double hiR() const {return _rend[1];}
private:
// data members
  double _thick; // tube wall thickness
  double _rend[2]; // limits in terms of distance in space from cone vertex to point on cone
  const DetMaterial* _thematerial; // material of this type
};
#endif

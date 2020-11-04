// ------------------------------------------------------------------------------
// File and Version Information:
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#ifndef PacCylDetType_HH
#define PacCylDetType_HH
//
//  Includes
//
#include "DetectorModel/DetSurfaceType.hh"
class DetMaterial;
//
//  Define the class
//
class PacCylDetType: public DetSurfaceType {
public:
//
//  Constructor; build from the primitive
//
  PacCylDetType(double thick,double zends[2],
		     const DetMaterial*,int);
//  Destructor
  virtual ~PacCylDetType();
//
//  real versions of the virtual functions
//
  bool physicalMaterial(const TypeCoord*) const ;
  const DetMaterial& material(const TypeCoord*) const { return *_thematerial;}
	double thickness(const TwoDCoord*) const { return _thick;}
	double thick() const { return _thick;}
	double lowZ() const {return _zend[0];}
	double hiZ() const {return _zend[1];}
// apply a shift to the Z position
  void applyZShift(double zshift);
private:
// data members
private:
  double _thick; // tube wall thickness
  double _zend[2]; // z position of the std::ends
  const DetMaterial* _thematerial; // material of this type
};
#endif

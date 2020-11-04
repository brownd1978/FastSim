// ------------------------------------------------------------------------------
// File and Version Information:
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacCylDetType.hh"
#include "DetectorModel/DetMaterial.hh"
#include "BaBar/Constants.hh"
#include <utility>
#include <math.h>
//
//  Construct from a 2-d perimeter array.  The array must descStrute points IN ORDER
//  clockwise around the Strut (the starting point is irrelevant).
//
PacCylDetType::PacCylDetType(double thick,double zend[2],
				       const DetMaterial* themat,int idnum) :
  DetSurfaceType("Cylinder",idnum),  _thick(thick),
  _thematerial(themat)
{
  // Make sure z0 < z1
  if ( zend[0] < zend[1] ) {
    _zend[0] = zend[0];
    _zend[1] = zend[1];
  } else {
    _zend[0] = zend[1];
    _zend[1] = zend[0];
  }
}
//  Destructor
PacCylDetType::~PacCylDetType(){;}
//
//
bool
PacCylDetType::physicalMaterial(const TypeCoord* thispoint) const {
//
//  Check if the point is on the strut
//
  double zval = (*thispoint)[1];
  return zval >= _zend[0] && zval <= _zend[1];
}

void
PacCylDetType::applyZShift(double zshift){
  _zend[0] += zshift;
  _zend[1] += zshift;
}

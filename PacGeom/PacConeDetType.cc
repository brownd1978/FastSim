// ------------------------------------------------------------------------------
// File and Version Information:
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacConeDetType.hh"
#include "DetectorModel/DetMaterial.hh"
#include "BaBar/Constants.hh"
#include <utility>
#include <math.h>
//
//  Construct from a 2-d perimeter array.  The array must descStrute points IN ORDER
//  clockwise around the Strut (the starting point is irrelevant).
//
PacConeDetType::PacConeDetType(double thick,double rend[2],
				       const DetMaterial* themat,int idnum) :
  DetSurfaceType("Cone",idnum),  _thick(thick),
  _thematerial(themat)
{
  _rend[0] = rend[0];
  _rend[1] = rend[1];
}
//  Destructor
PacConeDetType::~PacConeDetType(){;}
//
//
bool
PacConeDetType::physicalMaterial(const TypeCoord* thispoint) const {
//
//  Check if the point is on the strut
//
  double rval = (*thispoint)[1];
  return rval >= _rend[0] && rval <= _rend[1];
}

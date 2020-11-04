// ------------------------------------------------------------------------------
// File and Version Information:
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacRectDetType.hh"
#include <math.h>
//
//
PacRectDetType::PacRectDetType(double thick, double usize, double vsize,
				       const DetMaterial* themat,int idnum) :
  PacPlaneDetType(thick,"Rect",themat,idnum),_halfusize(usize/2.0),_halfvsize(vsize/2.0)
{
}
//  Destructor
PacRectDetType::~PacRectDetType(){;}
//
//
bool
PacRectDetType::physicalMaterial(const TypeCoord* tcoord) const {
	return fabs((*tcoord)[0])< _halfusize && fabs((*tcoord)[1])< _halfvsize;
}

double
PacRectDetType::estimatedSize() const{
  static double mysize = std::min(2*_halfusize,2*_halfvsize);
  return mysize;
}

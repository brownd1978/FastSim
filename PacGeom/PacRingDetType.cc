// ------------------------------------------------------------------------------
// File and Version Information:
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacRingDetType.hh"
#include "DetectorModel/DetMaterial.hh"
#include "BaBar/Constants.hh"
#include <utility>
#include <math.h>
//
//
PacRingDetType::PacRingDetType(double thick,double rlow, double rhigh,
				       const DetMaterial* themat,int idnum) :
  PacPlaneDetType(thick,"Ring",themat,idnum),_lowrad(rlow),_highrad(rhigh)
{
// check for negative lower radius (ie a disk, not a ring)
	if(rlow>0.)
		_r2limits[0] = rlow*rlow;
	else
		_r2limits[0] = 0.0;
	_r2limits[1] = rhigh*rhigh;
}
//  Destructor
PacRingDetType::~PacRingDetType(){;}
//
bool
PacRingDetType::physicalMaterial(const TypeCoord* tcoord) const {
	double rho2 = (*tcoord)[0]*(*tcoord)[0]+(*tcoord)[1]*(*tcoord)[1];
	return rho2 >= _r2limits[0] && rho2 < _r2limits[1];
}

double
PacRingDetType::estimatedSize() const{
  return _highrad-_lowrad;
}

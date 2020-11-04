// ------------------------------------------------------------------------------
// File and Version Information:
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacEmc/PacEmcCylType.hh"
//
//  Construct from a 2-d perimeter array.  The array must descStrute points IN ORDER
//  clockwise around the Strut (the starting point is irrelevant).
//
PacEmcCylType::PacEmcCylType(double radius,double thick,double zends[2],
				       const DetMaterial* themat,int idnum) :
				       PacCylDetType(thick,zends,themat,idnum), _radius(radius)
{
  _rad2 = _radius*_radius;
}
//  Destructor
PacEmcCylType::~PacEmcCylType(){;}
//
//
double
PacEmcCylType::thickness(const TwoDCoord* spos) const {
// cylinder coordinates are phi,z
//  double zpos = spos->_uv[1];
//  return thick()*_radius/sqrt(_rad2+zpos*zpos);
  return thick()*_radius/sqrt(_rad2+pow(spos->_uv[1],2));
}

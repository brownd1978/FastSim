//--------------------------------------------------------------------------
// Name:
//   PacCylBField
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 16 July 2008
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacCylBField.hh"

PacCylBField::PacCylBField(const Hep3Vector& fieldvector,double radius,double zlow, double zhi,double delnom)
	: BFieldFixed(fieldvector,delnom), _radius(radius),_zlow(zlow),_zhi(zhi){
	}

PacCylBField::~PacCylBField(){}

Hep3Vector
PacCylBField::bFieldVect(const HepPoint &point)const {
	static Hep3Vector _null(0.0,0.0,0.0);
	if(inField(point))
		return BFieldFixed::bFieldVect(point);
	else
		return _null;
}

bool
PacCylBField::inField(const HepPoint& point) const {
	static double _sqrrad = _radius*_radius;
	return point.z() >= _zlow && point.z() <= _zhi &&
		point.x()*point.x()+point.y()*point.y()<= _sqrrad;
}

//--------------------------------------------------------------------------
// Name:
//   PacGradBField
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 16 July 2008
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacGradBField.hh"
#include <iostream>
PacGradBField::PacGradBField(double b0, double b1, double zg0, double zg1, double radius, double zlow, double zhi) :
  _b0(0,0,b0), _b1(0,0,b1), _z0(zg0), _z1(zg1), _radius(radius), _zlow(zlow), _zhi(zhi) {
  _grad = (b1 - b0)/(zg1-zg0);
  std::cout << "Gradient field, B(z<" << _z0 << ") = " << b0 << ", B(z>" <<_z1 << ") = " << b1 << " gradient = " << _grad << std::endl;
}

PacGradBField::~PacGradBField(){}

Hep3Vector
PacGradBField::bFieldVect (const HepPoint &point)const {
	static Hep3Vector _null(0.0,0.0,0.0);
  if (point.z() < _zlow || point.z() > _zhi)
    return _null;
  else if(point.z() < _z0)
    return _b0;
  else if(point.z() > _z1)
    return _b1;
  else {
    double bgrad = _grad*(point.z()-_z0);
// work in cylindrical coordinates
    double bz = _b0.z()+bgrad;
    double bx = -0.5*_grad*point.x();
    double by = -0.5*_grad*point.y();
    return Hep3Vector(bx,by,bz);
  }
}

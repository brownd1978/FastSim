//--------------------------------------------------------------------------
// Name:
//   PacCylBField
// Description:
//   Extension of BFieldFixed limiting the non-zero field region to a
//   finite cylinder
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 16 July 2008
//------------------------------------------------------------------------
#ifndef PacCylBField_HH
#define PacCylBField_HH

#include "BField/BFieldFixed.hh"

// class interface //
class PacCylBField : public BFieldFixed {

public:
  //construct a fixed field within a cylinder 
  PacCylBField(const Hep3Vector& fieldvector,double radius,double zlow, double zhi,double delnom=0.0);

  //destroy
  virtual ~PacCylBField();

  //basics
  virtual Hep3Vector bFieldVect
  	(const HepPoint &point=HepPoint(0,0,0))const;

	// test if a point is inside the field volume
	bool inField(const HepPoint& point) const;
	// accessors
	double radius() const { return _radius;}
	double zlow() const { return _zlow;}
	double zhi() const { return _zhi;}

private:
	double _radius;
	double _zlow;
	double _zhi;
};

#endif

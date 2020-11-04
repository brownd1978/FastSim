//--------------------------------------------------------------------------
// Name:
//   PacGradBField
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
#ifndef PacGradBField_HH
#define PacGradBField_HH

#include "BField/BField.hh"

// class interface //
class PacGradBField : public BField {

public:
  // construct a graded field
  PacGradBField(double b0, double b1 ,double zg0, double zg1, double radius,double zlow, double zhi);

  //destroy
  virtual ~PacGradBField();

  //basics
  virtual Hep3Vector bFieldVect
  	(const HepPoint &point=HepPoint(0,0,0))const;

	// accessors
	double radius() const { return _radius;}
  double zlow() const { return _zlow;}
	double zhi() const { return _zhi;}

private:
  Hep3Vector _b0;
  Hep3Vector _b1;
  double _z0, _z1;
  double _grad;
	double _radius;
	double _zlow;
	double _zhi;
};

#endif

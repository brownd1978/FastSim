//--------------------------------------------------------------------------
// Name:
//   PacCylDetElem
// Description:
//   Cylindrical element of PacDetector
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#include "PacGeom/PacDetElem.hh"
#include "PacGeom/PacCylDetType.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "DetectorModel/DetCylinder.hh"
class PacHelix;
class TrkLineTraj;
class TrkGeomTraj;

class PacCylDetElem : public PacDetElem, public DetSurfaceElem {
public:
// construct from a type, radius, and measurement
	PacCylDetElem(const PacCylDetType* type,double r, const char* name,int id,const std::vector<const PacMeasurement*>& mdevs,
	double gap, double overlap);
	virtual ~PacCylDetElem();
// PacDetElem function
	virtual bool activeRegion(const HepPoint& pos) const;
  virtual double normTo(const HepPoint& point,Hep3Vector& norm) const;
// accessors
	double radius() const { return _radius;}
	const PacCylDetType* cylType() const { return _ctype;}	
// explicit intersection functions
	virtual bool intersect( const TrkGeomTraj* traj, double& fltlen) const;
	virtual const DetSurfaceElem* surfElem() const {return this;}
	virtual double thickness() const { return _ctype->thick();}
	bool intersect( const PacHelix* helix, double& fltlen) const;
	bool intersect( const TrkLineTraj* line, double& fltlen) const;
	virtual double estimatedSize() const;
  virtual double curvature(const HepPoint& pos) const;
// apply z shift
  virtual void applyZShift(double zshift);
private:
	double _radius;
  const PacCylDetType* _ctype;
};

//--------------------------------------------------------------------------
// Name:
//   PacConeDetElem
// Description:
//   Conical element of PacDetector.  The cone axis is assumed to be
//   Z, and the element shape is defined by a z range
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#include "PacGeom/PacDetElem.hh"
#include "PacGeom/PacConeDetType.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "DetectorModel/DetCone.hh"

class PacHelix;
class TrkLineTraj;
class TrkGeomTraj;
class PacCylDetElem;
class PacCylDetType;
class PacPlaneDetElem;
class PacRingDetType;

class PacConeDetElem : public PacDetElem, public DetSurfaceElem {
public:
// construct from a type, angle, vertex, and measurement (optional)
	PacConeDetElem(const PacConeDetType* type,double tantheta, double zvertex,
	const char* name,int id,const std::vector<const PacMeasurement*>& mdevs, double gap, double overlap);
	virtual ~PacConeDetElem();
// PacDetElem function
	virtual bool activeRegion(const HepPoint& pos) const;
	virtual double normTo(const HepPoint& point,Hep3Vector& norm) const;
// accessors
	double tanTheta() const { return _cone->tanTheta();}
  double zVertex() const { return _cone->vertex();}
// 
  const PacConeDetType* coneType() const { return _conetype;}
  const DetCone* cone() const { return _cone;}
// explicit intersection functions
	virtual bool intersect( const TrkGeomTraj* traj, double& fltlen) const;
	virtual const DetSurfaceElem* surfElem() const { return this;}
  virtual double thickness() const { return _conetype->thick();}
	bool intersect( const PacHelix* helix, double& fltlen,double fltguess) const;
  bool intersect( const TrkLineTraj* line, double& fltlen) const;
  virtual double estimatedSize() const;
  virtual double curvature(const HepPoint& pos) const;
// transverse radius at a given z
  double radius(const double& zpos) const;
protected:
// point+direction intersection.  Optionally require the new flt to be larger than the old
  bool intersect(const HepPoint& tpoint, const Hep3Vector& tdir, double& fltlen,bool positive) const;
// apply z shift
  virtual void applyZShift(double zshift);
private:
  const DetCone* _cone;
// to seed intersections, we use bounding cylinder and rings
  PacCylDetElem* _cyls[2];
  PacPlaneDetElem* _rings[2];
// types for these, to keep the memory clean
  PacCylDetType* _ctype;
  PacRingDetType* _rtype;
// cache my type
  const PacConeDetType* _conetype;
// statics
  static const double _small;
  static const double _tiny;
  
};

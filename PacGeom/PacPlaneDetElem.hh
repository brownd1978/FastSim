//--------------------------------------------------------------------------
// Name:
//   PacPlaneDetElem
// Description:
//   Planar element of PacDetector
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------

#ifndef PacPlaneDetElem_HH
#define PacPlaneDetElem_HH

#include "PacGeom/PacDetElem.hh"
#include "PacGeom/PacPlaneDetType.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "DetectorModel/DetPlane.hh"
class PacHelix;
class TrkLineTraj;
class TrkGeomTraj;

class PacPlaneDetElem : public PacDetElem, public DetSurfaceElem {
public:
// only 2 orientations are supported: plane normal parallel or orthogonal to the z axis
  enum orientation {zparallel=0,zperp};
// construct either parallel or perpendicular to Z axis.  center is the planar coord origin in global coords.
// for parallel, local 'u' axis makes angle 'phi' WRT x axis.
// For perp, normal makes angle phi WRT x axis
  PacPlaneDetElem(const PacPlaneDetType* type, orientation orient, const Hep3Vector& center, double phi,
    const char* name, int id,
    const std::vector<const PacMeasurement*>& mdevs,double gap, double overlap);
  
  virtual ~PacPlaneDetElem();
// PacDetElem function
  virtual bool activeRegion(const HepPoint& pos) const;
  virtual double normTo(const HepPoint& point,Hep3Vector& norm) const;
// accessors
	orientation planeOrientation() const { return _orient;}
	const HepPoint& midpoint() const { return _midpoint;}
  const Hep3Vector& normal() const { return _norm;} // normal direction
  const Hep3Vector& udir() const { return _udir;}
  const Hep3Vector& vdir() const { return _vdir;}
	const PacPlaneDetType* planeType() const { return static_cast<const PacPlaneDetType*>(detectorType());}
// explicit intersection functions
	virtual bool intersect( const TrkGeomTraj* traj, double& fltlen) const;	
	virtual const DetSurfaceElem* surfElem() const { return this;}
	virtual double thickness() const { return _ctype->thick();}
	bool intersect( const PacHelix* helix, double& fltlen) const;
	bool intersect( const TrkLineTraj* line, double& fltlen) const;
	virtual double estimatedSize() const;
  virtual double curvature(const HepPoint& pos) const;
// apply Z shift
  virtual void applyZShift(double zshift);
protected:
private:
	orientation _orient;
	HepPoint _midpoint; // midpoint of the plane, in global coordinates
  Hep3Vector _norm; // plane normal direction
  Hep3Vector _udir; // local plane 'x' direction
  Hep3Vector _vdir; // local plane 'y' direction
  double _normphi;
  const PacPlaneDetType* _ctype;
  double _zmin;
  double _zmax; // cache of z limits
};

#endif

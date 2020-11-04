//--------------------------------------------------------------------------
// Name:
//   PacPlaneDetElem
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "PacGeom/PacPlaneDetType.hh"
#include "PacGeom/PacRectDetType.hh"
#include "PacGeom/PacHelix.hh"
#include "CLHEP/Geometry/Transformation.h"
#include "DetectorModel/DetAlignElem.hh"
#include "CLHEP/Geometry/AngleSets.h"
#include "TrajGeom/TrkLineTraj.hh"
#include <math.h>

PacPlaneDetElem::PacPlaneDetElem(const PacPlaneDetType* type, orientation orient,
  const Hep3Vector& center, double eta,
  const char* name, int id,
  const std::vector<const PacMeasurement*>& mdevs,double gap, double overlap) :
	PacDetElem(PacDetElem::plane,mdevs,gap,overlap),
	DetSurfaceElem(const_cast<PacPlaneDetType*>(type),name,
		id,DetPlane(HepTransformation())),
		 _orient(orient)
{
// create transform into appropriate plane
  Hep3Vector normal;
  Hep3Vector xaxis;
  if(_orient == zparallel){
    normal = Hep3Vector(0.0,0.0,1.0);
    xaxis = Hep3Vector(cos(eta),sin(eta),0.0);
  } else if(_orient == zperp) {
// take the center angle
    normal = Hep3Vector(-sin(eta),cos(eta),0.0);
    xaxis = Hep3Vector(-cos(eta),-sin(eta),0.0);  // local 'u' axis, along increasing phi.  'v' axis is along global z
  } else
    ErrMsg(fatal) << "unknown plane orientation" << endmsg;
  HepTransformation mytrans(center,normal,xaxis);
  transf() = mytrans;
// set local cache for fast intersection functions
	_midpoint = surface()->transform()->origin();
  _norm = surface()->basis();
  _udir = surface()->basis(I_x);
  _vdir = surface()->basis(I_y);
	_ctype = static_cast<const PacPlaneDetType*>(detectorType());
  _normphi = _norm.phi();

  if(_orient == zparallel){
    _zmin = _zmax = _midpoint.z();
  } else {
 // this must be a rectangle type
    const PacRectDetType* rtype = dynamic_cast<const PacRectDetType*>(detectorType());
    assert(rtype != 0);
    _zmin = _midpoint.z() - rtype->halfvSize();
    _zmax = _midpoint.z() + rtype->halfvSize();
  }
// test
  Hep3Vector midvect(_midpoint.x(),_midpoint.y(),_midpoint.z());
  if(fabs((midvect-center).mag())>1.0e-8)
    ErrMsg(fatal) << "plane element center doesn't match!" << endmsg;
  if(_orient == zparallel){
    if(fabs(_norm.dot(Hep3Vector(0.0,0.0,1.0))-1.0) > 1.0e-8)
      ErrMsg(fatal) << "plane element normal direction doesn't match!" << endmsg;
  } else if(_orient == zperp){
    if(fabs(_norm.dot(Hep3Vector(0.0,0.0,1.0))) > 1.0e-8)
      ErrMsg(fatal) << "plane element normal direction doesn't match!" << endmsg;
  }
}


PacPlaneDetElem::~PacPlaneDetElem() {
}

double
PacPlaneDetElem::normTo(const HepPoint& point,Hep3Vector& norm) const {
// normal is fixed
  norm = _norm;
// distance is the projection along norm
  Hep3Vector delta = point-_midpoint;
  return norm.dot(delta);
}


bool
PacPlaneDetElem::activeRegion(const HepPoint& pos) const {
// convert to local coordinates
  Hep3Vector delta = pos-_midpoint;
  double u = _udir.dot(delta);
  double v = _vdir.dot(delta);
	TwoDCoord twod(u,v);
	return _ctype->physicalMaterial(&twod);
}

bool
PacPlaneDetElem::intersect( const TrkGeomTraj* traj, double& fltlen) const {
// helix intersection
	const PacHelix* helix = dynamic_cast<const PacHelix*>(traj);
	if(helix != 0)return intersect(helix,fltlen);
// line intersection
	const TrkLineTraj* line = dynamic_cast<const TrkLineTraj*>(traj);
	if(line != 0)return intersect(line,fltlen);	
	return false;
}

bool
PacPlaneDetElem::intersect( const PacHelix* helix, double& fltlen) const {
	bool retval(false);
	// first, check for obvious misses in Z
  double zpos = helix->z(fltlen);
  if(helix->tanDip() >= 0 && zpos > _zmax ||
    helix->tanDip() < 0 && zpos < _zmin)
    return false;
	if(_orient == zparallel) {
		double cost = helix->sinDip();
		static const double smallcost(1e-10);
		if(fabs(cost) > smallcost){
	// intersection is possible: compute the surface position and test against the physical material
			double deltaz = _midpoint.z()-helix->z0();
			double pathlen = deltaz/cost;
	// require this to be beyond the previous intersection
			if(pathlen > fltlen){
				HepPoint ipoint = helix->position(pathlen);
				if( activeRegion(ipoint)){
					fltlen = pathlen;
					retval = true;
				}
			}
		}
	} else {
	  // find the distance between the plane and the helix center: if this is greater than the transverse
	  // radius, there can be no intersection
    double rho = 1.0/helix->omega();
    double td = helix->tanDip();
    double cfactor = rho+helix->d0();
    HepPoint hcenter(-cfactor*sin(helix->phi0()),cfactor*cos(helix->phi0()),0.0);
    Hep3Vector diff = _midpoint - hcenter;
    double hdist = diff.dot(_norm);
    if(fabs(hdist) < fabs(rho)){
      double delta = _normphi - helix->phi0();
      double gamma = asin(hdist/rho);
      double circ = fabs(rho)*Constants::twoPi;
  // there are 2 solutions, plus helical ambiguities; choose the one with the smallest flightlength
      double flt1 = rho*(delta + gamma);
      if(flt1 < 0)flt1 += circ;
      if(flt1 > circ) flt1 -= circ;
      double flt2 = rho*(delta + Constants::pi - gamma);
      if(flt2 < 0)flt2 += circ;
      if(flt2 > circ)flt2 -= circ;
      double cosd = helix->cosDip();
      flt1 /= cosd;
      flt2 /= cosd;
      double oneturn = circ/cosd;
  // move to the correct loop of the trajectory.
      flt1 += ceil((fltlen-flt1)/oneturn)*oneturn;
      flt2 += ceil((fltlen-flt2)/oneturn)*oneturn;
  // adjust to be in the right Z range
  // also try to force Z into range, in case the particle loops before hiting this surface
      double zloop = Constants::twoPi*fabs(rho)*td;
      double z1 = helix->z(flt1);
      if( ( z1 < _zmin && td > 0.0 ) ||
          ( z1 > _zmax && td < 0.0 ) ){
        double dz = td > 0.0 ? _zmin-z1 : _zmax - z1;
        flt1 += ceil(dz/zloop)*oneturn;
      }
      double z2 = helix->z(flt2);
      if( ( z2 < _zmin && td > 0.0 ) ||
          ( z2 > _zmax && td < 0.0 ) ){
        double dz = td > 0.0 ? _zmin-z2 : _zmax - z2;
        flt2 += ceil(dz/zloop)*oneturn;
      }
  // choose the smallest fltlen within the physical region of this element
      bool hit1 = activeRegion(helix->position(flt1));
      bool hit2 = activeRegion(helix->position(flt2));
      retval = hit1 || hit2;
      if(hit1 && hit2){
        if(flt1 < flt2){
          fltlen = flt1;
        } else {
          fltlen = flt2;
        }
      } else if(hit1) {
        fltlen = flt1;
      } else if(hit2) {
        fltlen = flt2;
      }
    }
	}
	return retval;
}

bool
PacPlaneDetElem::intersect( const TrkLineTraj* line, double& fltlen) const {
// This works for all plane orientations
	bool retval(false);
	HepPoint start;
	Hep3Vector dir;
	line->getInfo(0.0,start,dir);
  double cost = _norm.dot(dir);
	static const double smallcost(1e-3);
// reject cases of glancing intersection
	if(fabs(cost) > smallcost){
// intersection is possible: compute the surface position and test against the physical material
    Hep3Vector diff = _midpoint-start;
    double pathlen = diff.dot(_norm)/cost;
// require this to be beyond the previous intersection
    if(pathlen > fltlen){
      HepPoint ipoint = line->position(pathlen);
      if( activeRegion(ipoint)){
        fltlen = pathlen;
        retval = true;
      }
    }
  }
	return retval;
}

void
PacPlaneDetElem::applyZShift(double zshift){
  Hep3Vector vshift(0.0,0.0,zshift);
  _midpoint += vshift;
  _zmin += zshift;
  _zmax += zshift;
  AlignAngles angles(0.0,0.0,0.0);
  DetAlignElem ashift(vshift,angles,"dummy",0);
  applyGlobal(ashift);
}

double
PacPlaneDetElem::estimatedSize() const {
  return _ctype->estimatedSize();
}

double
PacPlaneDetElem::curvature(const HepPoint&) const {
  return 0.0;
}

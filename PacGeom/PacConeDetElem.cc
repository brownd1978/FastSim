/*******************************************************************************
* PacConeDetElem
	*******************************************************************************/

#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"
#include "PacGeom/PacConeDetElem.hh"
#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacCylDetType.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "PacGeom/PacRingDetType.hh"
#include "PacGeom/PacHelix.hh"
#include "CLHEP/Geometry/Transformation.h"
#include "TrajGeom/TrkLineTraj.hh"
#include <assert.h>
#include <iostream>

const double PacConeDetElem::_small(1.0e-2); // 100 micron precision is good enough
const double PacConeDetElem::_tiny(1.0e-8); // test for degeneracy


PacConeDetElem::PacConeDetElem(const PacConeDetType* type, double tantheta, double zvertex,
 const char* name,int id,const std::vector<const PacMeasurement*>& mdevs, double gap, double overlap)
	: PacDetElem(PacDetElem::cone,mdevs,gap,overlap),
	DetSurfaceElem(const_cast<PacConeDetType*>(type),name,id,DetCone(HepTransformation(),tantheta,zvertex)),
	_conetype(type)
{
  _cone = dynamic_cast<const DetCone*>(surface());
  assert(_cone != 0);
// create the bounding cylinders and rings.  Note that the cone 'radius' can be negative
// (for the negative branch of the cone), hence the need to test signs
  double zends[2];
  double rmin = fabs(type->lowR()*_cone->sinTheta());
  double rmax = fabs(type->hiR()*_cone->sinTheta());
  zends[0] = std::min(type->lowR(),type->hiR())*_cone->cosTheta()+zvertex;
  zends[1] = std::max(type->lowR(),type->hiR())*_cone->cosTheta()+zvertex;
// create type objects.  No material here
  _ctype = new PacCylDetType(type->thick(),zends,0,0);
  _rtype = new PacRingDetType(type->thick(),rmin,rmax,0,0);
// use these to create the cyl and ring elements.  No measurement here.
  std::vector<const PacMeasurement*> nodevs;
  _cyls[0] = new PacCylDetElem(_ctype,rmin,name,id,nodevs,0,0);
  _cyls[1] = new PacCylDetElem(_ctype,rmax,name,id,nodevs,0,0);
  _rings[0] = new PacPlaneDetElem(_rtype,PacPlaneDetElem::zparallel,Hep3Vector(0.0,0.0,zends[0]),0.0,name,id,nodevs,0,0);
  _rings[1] = new PacPlaneDetElem(_rtype,PacPlaneDetElem::zparallel,Hep3Vector(0.0,0.0,zends[1]),0.0,name,id,nodevs,0,0);
}

PacConeDetElem::~PacConeDetElem() {
  delete _cyls[0];
  delete _cyls[1];
  delete _rings[0];
  delete _rings[1];
  delete _ctype;
  delete _rtype;
}

bool
PacConeDetElem::activeRegion(const HepPoint& pos) const {
// convert to local coordinates
//	HepPoint locpos = transform().transTo(pos);
// check against the z range of this type
  double rpos = (pos.z()-_cone->vertex())/_cone->cosTheta();
	return (rpos >= _conetype->lowR() && rpos <= _conetype->hiR());
}

bool
PacConeDetElem::intersect( const TrkGeomTraj* traj, double& fltlen) const {
// line intersection is closed-form: check for that first
  const TrkLineTraj* line = dynamic_cast<const TrkLineTraj*>(traj);
  if(line != 0)
    return intersect(line,fltlen);
// For a helix the intersection is transcendental, find intersections with the bounding rings and cylinders
// and use those to seed the calculation
  const PacHelix* helix = dynamic_cast<const PacHelix*>(traj);
  if(helix != 0){
    double flen;
    int ninter(0);
    double fltsum(0.0);
    for(int ibnd=0;ibnd<2;ibnd++){
      flen = fltlen;
      if(_cyls[ibnd]->intersect(traj,flen)){
        ninter++;
        fltsum += flen;
      }
      if(_rings[ibnd]->intersect(traj,flen)){
        ninter++;
        fltsum += flen;
      }
    }
// If there are any intersections, pass to the cone-specific function
    if(ninter > 0){
      double fltavg = fltsum/ninter;
      return intersect(helix,fltlen,fltavg);
    } else
      return false;
  } else
    std::cout << "Unknown trajectory type: cannot intersect!!!" << std::endl;
  return false;
}

bool
PacConeDetElem::intersect( const PacHelix* helix, double& fltlen, double fltavg) const {
  bool retval(false);
// constants
  static const int _maxiter(10);
// compute quantities that don't change with iteration
  double flen(fltavg);
  bool converged(false);
  bool diverging(false);
  double olddelta = 1e3;
  int niter(0);
  while( (!converged) && (!diverging)){
    niter++;
// get the trajectory position and direction at the current point
    HepPoint tpoint;
    Hep3Vector tdir;
    helix->getInfo(flen,tpoint,tdir);
// use these to find the change in flen
    double delta(0.0);
    if(intersect(tpoint,tdir,delta,false)){
// test for convergence
      converged = fabs(delta) < _small;
// test for divergence
      if(!converged){
        diverging = fabs(delta)>olddelta || niter > _maxiter;
        olddelta = fabs(delta);
      }
    } else {
// soln. is diverging or unphysical: abandon
      diverging = true;
    }
// update flen for this iteration
    flen += delta;
  }
  if(converged){
// consistency test
    HepPoint tpoint;
    Hep3Vector tdir;
    helix->getInfo(flen,tpoint,tdir);
    double rho = sqrt(tpoint.x()*tpoint.x()+tpoint.y()*tpoint.y());
    double dz = tpoint.z() - _cone->vertex() - rho/_cone->tanTheta();
    if(fabs(dz) < _small){
// check we haven't iterated out of valid range
      if(flen > fltlen){
// fill intersection object
        retval = true;
        fltlen = flen;
      }
    } else
      std::cout << "Helix-cone intersection convergence error!! " << dz << std::endl;
  }
  return retval;
}

bool
PacConeDetElem::intersect( const TrkLineTraj* line, double& fltlen) const {
  bool retval(false);
// get basic information from the trajectory
  double flen = fltlen;
  Hep3Vector tdir = line->direction(flen);
  HepPoint tpoint = line->position(flen);
// test for intersection.  Choose the minimum solution larger than the input
  if(intersect(tpoint,tdir,flen,true)){
// test consistency
    tpoint = line->position(flen);
    double rho = sqrt(tpoint.x()*tpoint.x()+tpoint.y()*tpoint.y());
    double dz = tpoint.z() - _cone->vertex() - rho/_cone->tanTheta();
    if(fabs(dz) < _small){
// fill intersection object
      retval = true;
      fltlen = flen;
    } else
      std::cout << "Line-cone intersection onvergence error!! " << dz << std::endl;
  }
	return retval;
}

bool
PacConeDetElem::intersect(const HepPoint& tpoint, const Hep3Vector& tdir, double& fltlen,bool positive) const {
  bool retval(false);
  double oldflt(fltlen);
  Hep3Vector pvec(tpoint.x(),tpoint.y(),0.0);
// compute the POCA to the Z axis
  double cost = tdir.z();
  double cost2 = cost*cost;
  double sint2 = 1.0-cost2;
  double pperp = tdir.dot(pvec);
  double f0(0.0);
  if(sint2 != 0.0)
    f0 = -pperp/sint2;
  double rho02 = pvec.perp2() - sint2*f0*f0;
// compute the quadratic solns
  double dz = tpoint.z()+f0*cost-_cone->vertex();
  double tanc = _cone->tanTheta();
  double tanc2 = tanc*tanc;
  double denom = cost2*tanc2 - sint2;
  double term1 = -dz*cost*tanc2;
  double sqrterm2 = rho02*(cost2*tanc2-sint2) + dz*dz*sint2*tanc2;
  if(sqrterm2 > 0.0 and denom != 0.0){
    double term2 = sqrt(sqrterm2);
// test both solutions
    static const double sign[2] = {1.0,-1.0};
    double deltaflt(1.0e10);
    for(int isign=0;isign<2;isign++){
      double soln = (term1 +sign[isign]*term2)/denom;
// check we are on the right branch (angle)
      double zval = dz+soln*cost;
      double tanc_test = sqrt(rho02+soln*soln*sint2)/zval;
      if(fabs(tanc_test - tanc) < _tiny){
// found the right branch; compute the cone 'radius' and check the range
        double radius = zval/_cone->cosTheta();
        if(radius >= _conetype->lowR() && radius <= _conetype->hiR()){
// check the flightlength; sometimes it should be positive, sometimes we just want the smallest change
          double dflt = soln + f0;
          if( (positive && dflt>0.0 && dflt < deltaflt) ||
            (!positive && fabs(dflt) < fabs(deltaflt)) ) {
            retval = true;
            fltlen = oldflt+dflt;
            deltaflt = dflt;
          }
        }
      }
    }
  }
	return retval;
}

void
PacConeDetElem::applyZShift(double zshift){
// call down for the bounding elements
  _cyls[0]->applyZShift(zshift);
  _cyls[1]->applyZShift(zshift);
  _rings[0]->applyZShift(zshift);
  _rings[1]->applyZShift(zshift);
// now shift the actual cone
  const_cast<DetCone*>(_cone)->applyZShift(zshift);
}

double
PacConeDetElem::normTo(const HepPoint& point,Hep3Vector& norm) const {
  return surface()->normTo(point,norm);
}

double
PacConeDetElem::estimatedSize() const {
  static double mysize = sqrt( pow(_cyls[1]->radius() - _cyls[0]->radius(),2) + 
    pow(_rings[1]->midpoint().z() - _rings[0]->midpoint().z(),2));
  return mysize;
}

double
PacConeDetElem::curvature(const HepPoint& pos) const {
  double rad = radius(pos.z());
  if(rad > 0.0)
    return 1.0/rad;
  else
    return 0.0;
}

double
PacConeDetElem::radius(const double& zpos) const {
  double dz = zpos - _cone->vertex();
  return fabs(_cone->tanTheta()*dz);
}


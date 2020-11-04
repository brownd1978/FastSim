/*******************************************************************************
* PacCylDetElem
	*******************************************************************************/

#include "BaBar/BaBar.hh"

#include <limits>

#include "BaBar/Constants.hh"
#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacCylDetType.hh"
#include "PacGeom/PacHelix.hh"
#include "CLHEP/Geometry/Transformation.h"
#include "TrajGeom/TrkLineTraj.hh"
#include <math.h>

PacCylDetElem::PacCylDetElem(const PacCylDetType* type, double r, const char* name,int id,
const std::vector<const PacMeasurement*>& mdevs, double gap, double overlap)
	: PacDetElem(PacDetElem::cylinder,mdevs,gap,overlap),
	DetSurfaceElem(const_cast<PacCylDetType*>(type),name,id,DetCylinder(HepTransformation(), r)),
	_radius(r),_ctype(type)
{}

PacCylDetElem::~PacCylDetElem() {
}

bool
PacCylDetElem::activeRegion(const HepPoint& pos) const {
// check against the z range of this type
	return (pos.z() >= _ctype->lowZ() && pos.z() <= _ctype->hiZ());
}

bool
PacCylDetElem::intersect( const TrkGeomTraj* traj, double& fltlen) const {
// helix intersection
	const PacHelix* helix = dynamic_cast<const PacHelix*>(traj);
	if(helix != 0)return intersect(helix,fltlen);
// line intersection
	const TrkLineTraj* line = dynamic_cast<const TrkLineTraj*>(traj);
	if(line != 0)return intersect(line,fltlen);	
	return false;
}

bool
PacCylDetElem::intersect( const PacHelix* helix,double& fltlen) const {
  bool retval(false);
// first, check for obvious misses in Z
  double zpos = helix->z(fltlen);
  if(helix->tanDip() >= 0 && zpos > _ctype->hiZ() ||
      helix->tanDip() < 0 && zpos < _ctype->lowZ())
      return false;
// geometry
	double tr = fabs(1.0/helix->omega());
	double wd0 = helix->omega()*helix->d0();
	double wr = radius()*helix->omega();
  double td = helix->tanDip();
  double z0 = helix->z0();
// test inner and outer intersection
	if(wd0+2.0<fabs(wr) || fabs(wd0) > fabs(wr)) return false;
// use law of cosines, theta is the rotation angle since pathlen=0
	double onewd0 = 1.0+wd0;
	double costheta = 0.5*(onewd0+(1-wr*wr)/onewd0);
	if(fabs(costheta)<=1.0){
// previous rotation angle
	  double oldtheta = fltlen*helix->cosDip()/tr;
// test sign ambiguity explicitly
    double theta[2];
    theta[0]= acos(costheta);
    theta[1]= Constants::twoPi-theta[0];
    double bestpathlen = std::numeric_limits<double>::max();
    for(int isign=0;isign<2;isign++){
//    resolve 2-pi looping ambiguity by forcing the flightrange to be in range
      if(theta[isign] < oldtheta)
        theta[isign] += Constants::twoPi*ceil((oldtheta-theta[isign])/Constants::twoPi);
// also try to force Z into range, in case the particle loops before hiting this surface
      double zpos = z0 + td*theta[isign]*tr;
      double zloop = Constants::twoPi*tr*td;
      if( ( zpos < _ctype->lowZ() && td > 0.0 ) ||
        ( zpos > _ctype->hiZ() && td < 0.0 ) ){
          double dz = td > 0.0 ? _ctype->lowZ()-zpos : _ctype->hiZ() - zpos;
          theta[isign] += Constants::twoPi*ceil(dz/zloop);
      }
// compute the path length IN SPACE
		  double pathlen = theta[isign]*tr/helix->cosDip();		  
// require this to be beyond the previous intersection and in Z range
		  if(pathlen > fltlen+_mindpath && pathlen < bestpathlen){
// check z range
        zpos = helix->z(pathlen);
        if(zpos >= _ctype->lowZ() && zpos < _ctype->hiZ()){
// successful intersection
          bestpathlen = pathlen;
          retval = true;
		    }
	    }
    }
    if(retval){
      fltlen = bestpathlen;
    }
  }
  return retval;
}

bool
PacCylDetElem::intersect( const TrkLineTraj* line, double& fltlen) const {
  bool retval(false);
	HepPoint lpos = line->position(fltlen);
	Hep3Vector ldir = line->direction(fltlen);
// convert to plane perpendicular to cylinder axis (xy)
	Hep3Vector lpos2d(lpos.x(),lpos.y(),0.0);
	Hep3Vector ldir2d = Hep3Vector(ldir.x(),ldir.y(),0.0).unit();
	double proj = lpos2d.dot(ldir2d);
	double proj2 = proj*proj;
	double lposmag2 = lpos2d.mag2();
	double rad2 = radius()*radius();
	double square = rad2 + proj2 - lposmag2;
	if(square > 0.0){
		double sqrtval = sqrt(square);
		double flt;
		if(-proj > sqrtval)
			flt = -proj - sqrtval;
		else
			flt = -proj + sqrtval;
		if(flt > 0.0){
// 3-d flightlenght
			double flen = flt/ldir.dot(ldir2d) + fltlen;
//check
			HepPoint ipos = line->position(flen);
//			double dist2 = rad2 - ipos.x()*ipos.x()-ipos.y()*ipos.y();
//			if(fabs(dist2)> 1e-4)
//				std::cout << "Error: line intersection is not on cylinder" << std::endl;
// check z range
			if(flen-fltlen>_mindpath && ipos.z() > _ctype->lowZ() && ipos.z() < _ctype->hiZ() ){
        fltlen = flen;
        retval = true;
      }
		}
	}
	return retval;
}

void
PacCylDetElem::applyZShift(double zshift){
  const_cast<PacCylDetType*>(_ctype)->applyZShift(zshift);
}

double
PacCylDetElem::normTo(const HepPoint& point,Hep3Vector& norm) const {
// Normal points radially outwards
  double rho = point.perp();
  if(rho > 0.0)
    norm = Hep3Vector(point.x()/rho,point.y()/rho,0.0);
  else
    norm = Hep3Vector(1.0,0.0,0.0);
// distance is the projection along norm
  return rho - _radius;
}

double
PacCylDetElem::estimatedSize() const {
  static double mysize = std::min(_ctype->hiZ() - _ctype->lowZ(), _radius);
  return mysize;
}

double
PacCylDetElem::curvature(const HepPoint&) const {
  static const double invrad = 1.0/_radius;
  return invrad;
}


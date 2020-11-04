/*******************************************************************************
 * PacDetElem
 *******************************************************************************/

#include "BaBar/BaBar.hh"
#include "PacGeom/PacDetElem.hh"
#include "DetectorModel/DetIntersection.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "BbrGeom/TrkGeomTraj.hh"

const double
PacDetElem::_mindpath(1e-6);
const double
PacDetElem::_mindotp(5e-2);


PacDetElem::PacDetElem(SurfaceType surf,const std::vector<const PacMeasurement*>& mdevs, double gap, double overlap) : 
  _surf(surf),_mdevs(mdevs),_gap(gap),_over(overlap)
{}

PacDetElem::~PacDetElem() {
}

bool
PacDetElem::activeRegion(const HepPoint&) const {
	return active();
}

bool
PacDetElem::intersect( const TrkGeomTraj* traj, DetIntersection& dinter) const {
  double fltlen = dinter.pathlen;
  if(intersect(traj,fltlen)){
    fillDetInter(traj,dinter,fltlen);
    return true;
  }
  return false;
}

void
PacDetElem::fillDetInter(const TrkGeomTraj* traj,DetIntersection& dinter,double fltlen) const {
  dinter.delem = surfElem();
  dinter.trajet = traj;
  dinter.pathlen = fltlen;
// get trajectory information
  Hep3Vector tdir;
  HepPoint pos;
  traj->getInfo(fltlen,pos,tdir);
// find surface normal at this point
  Hep3Vector normdir;
  normTo(pos,normdir);
// compute intersection angle
  double dotp = fabs(tdir.dot(normdir));
// if the angle is large, we can use a linear approximation for the path length
  double halfnormthick = 0.5*thickness();
  double halfmaxlen = 0.5*estimatedSize();
  static const double epsilon(1.0e-6);  
  if(dotp > _mindotp){
    double halfthick = halfnormthick/dotp;
    dinter.pathrange[0]= dinter.pathlen-halfthick;
    dinter.pathrange[1]= dinter.pathlen+halfthick;
    dinter.flag[0] = dinter.flag[1] = 1;
  } else if(dotp > epsilon) {
// flag this as a glancing intersection
    dinter.flag[0] = dinter.flag[1] = -1;
// intermediate region; curvature may be important, must check
    Hep3Vector tcurv = traj->delDirect(fltlen);
    double trajcurv = tcurv.mag();
    double surfcurv = curvature(pos);
    if( (trajcurv > epsilon && fabs(tcurv.dot(normdir)) > epsilon) || surfcurv > epsilon  ){
// net curvature is the difference of surface and trajectory
      double netcurv;
      if(tcurv.dot(normdir) < 0.0)
        netcurv = fabs(trajcurv - surfcurv);
      else
        netcurv = trajcurv + surfcurv;
// compute transverse track direction relative to transverse  normal
// divide track direction into transverse (bending plane) and Z parts
      Hep3Vector tperp = tdir.perpPart();
      Hep3Vector nperp = normdir.perpPart();
      double pangle = tperp.angle(nperp);
      double zfactor = 1/tperp.mag();
      double bval = 1.0/tan(pangle);
      double bval2 = bval*bval;
      static double pm[2] = {-1.0,1.0};
// solve quadratic equation for inner and outer surface boundaries
      for(unsigned iend=0;iend<2;iend++){
        double cval = -1*pm[iend]*halfnormthick;
        double rootval = bval2 -4*netcurv*cval;
        double tlen;
        if(rootval > 0.0) {
// choose the smaller magnitude solution
          tlen = fabs(0.5*(bval-sqrt(rootval))/netcurv );
        } else {
// no solution: take the parallel point
          tlen = fabs(0.5*bval/netcurv);
        }
        dinter.pathrange[iend]= dinter.pathlen + pm[iend]*std::min(zfactor*tlen,halfmaxlen);
      }
      dinter.flag[0] = dinter.flag[1] = -1;
    } else {
      double halfthick = halfnormthick/dotp;
    // double-check the pathlength against the physical extent of this element!
      halfthick = std::min(halfthick,halfmaxlen);
      dinter.pathrange[0]= dinter.pathlen-halfthick;
      dinter.pathrange[1]= dinter.pathlen+halfthick;
      dinter.flag[0] = dinter.flag[1] = 1;
    }
  } else {
    double halfthick = halfmaxlen;
    dinter.pathrange[0]= dinter.pathlen-halfthick;
    dinter.pathrange[1]= dinter.pathlen+halfthick;
    dinter.flag[0] = dinter.flag[1] = -1;
  }
  if(dinter.pathrange[1] < dinter.pathrange[0]){
    std::cout << "Error: unphysicsl path range!!" << std::endl;
  }
}

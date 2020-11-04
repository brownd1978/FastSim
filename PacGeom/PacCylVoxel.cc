// ------------------------------------------------------------------------------
// File and Version Information:
// Class representing a single cylindrical voxel used in PacGeom
// Copyright Information:
//	Copyright (C) 2010	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 8/2/2010
// ------------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "PacGeom/PacCylVoxel.hh"
#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "BbrGeom/TrkGeomTraj.hh"
#include "BaBar/Constants.hh"
#include <limits>
#include <iostream>

PacCylVoxel::PacCylVoxel(double rbound[2],double zbound[2],double fbound[2],
  const PacCylDetElem* rsurf[2],
  const PacPlaneDetElem* zsurf[2],
  const PacPlaneDetElem* fsurf[2], PacVoxelId vid) : PacVoxel(vid) {
  for (unsigned ibnd=0;ibnd<2;ibnd++){
    _rbound[ibnd] = rbound[ibnd];
    _zbound[ibnd] = zbound[ibnd];
    _fbound[ibnd] = fbound[ibnd];
    _rsurf[ibnd] = rsurf[ibnd];
    _zsurf[ibnd] = zsurf[ibnd];
    _fsurf[ibnd] = fsurf[ibnd];
  }
}

PacCylVoxel::PacCylVoxel(const PacCylVoxel& other) : PacVoxel(other)
{
  for (unsigned ibnd=0;ibnd<2;ibnd++){
    _rbound[ibnd] = other._rbound[ibnd];
    _zbound[ibnd] = other._zbound[ibnd];
    _fbound[ibnd] = other._fbound[ibnd];
    _rsurf[ibnd] = other._rsurf[ibnd];
    _zsurf[ibnd] = other._zsurf[ibnd];
    _fsurf[ibnd] = other._fsurf[ibnd];
  }
}

PacCylVoxel::PacCylVoxel()
{
  for (unsigned ibnd=0;ibnd<2;ibnd++){
    _rbound[ibnd] = 0.0;
    _zbound[ibnd] = 0.0;
    _fbound[ibnd] = 0.0;
    _rsurf[ibnd] = 0;
    _zsurf[ibnd] = 0;
    _fsurf[ibnd] = 0;
  }
}

PacCylVoxel& 
PacCylVoxel::operator = (const PacCylVoxel& other) {
  if(&other != this){
    PacVoxel::operator=(other);
    for (unsigned ibnd=0;ibnd<2;ibnd++){
      _rbound[ibnd] = other._rbound[ibnd];
      _zbound[ibnd] = other._zbound[ibnd];
      _fbound[ibnd] = other._fbound[ibnd];
      _rsurf[ibnd] = other._rsurf[ibnd];
      _zsurf[ibnd] = other._zsurf[ibnd];
      _fsurf[ibnd] = other._fsurf[ibnd];
    }    
  }
  return *this;
}

PacCylVoxel::~PacCylVoxel()
{}

bool
PacCylVoxel::isInside(double value,dimension dim) const {
  return value >= lowBound(dim) && value < hiBound(dim);
}

HepPoint
PacCylVoxel::center() const {
  double rho = 0.5*(_rbound[0]+_rbound[1]);
  double zed = 0.5*(_zbound[0]+_zbound[1]);
  double phi = 0.5*(_fbound[0]+_fbound[1]);
  return HepPoint(rho*cos(phi),rho*sin(phi),zed);
}

double
PacCylVoxel::lowBound(dimension dim) const {
  switch (dim) {
    case rho:
    return _rbound[0];
    case zed:
    return _zbound[0];
    case phi:
    return  _fbound[0];
    default:
    assert(0==1);
  }
  return false;
}

double
PacCylVoxel::hiBound(dimension dim) const {
  switch ( dim) {
    case rho:
    return _rbound[1];
    case zed:
    return _zbound[1];
    case phi:
    return  _fbound[1];
    default:
    assert(0==1);
  }
  return false;
}

bool
PacCylVoxel::isInside(const HepPoint& point,dimension dim) const {
  double value;
  dval(point,dim,value);
  return isInside(value,dim);
}

void
PacCylVoxel::dval(const HepPoint& point,dimension dim,double& value) const {
  switch ( dim) {
    case rho:
    value = point.perp();
    break;
    case zed:
    value = point.z();
    break;
    case phi:
// check if there are no phi boundaries, if so avoid the phi (atan2) call
    if(_fsurf[0] == 0)
      value = Constants::pi;
    else
      value = point.phi();
    if(value<0) value += Constants::twoPi;
    if(value<_fbound[0]) value += Constants::twoPi;
    break;
    default:
    assert(0==1);
  }
}

bool
PacCylVoxel::isInside(const HepPoint& point) const
{
  return isInside(point,(dimension)rho) && isInside(point,(dimension)phi) && isInside(point,(dimension)zed);
}

double
PacCylVoxel::bdist(const HepPoint& pos,dimension idim) const {
  double value;
  dval(pos,idim,value);
  return std::min(value - lowBound(idim),hiBound(idim) - value);
}

bool
PacCylVoxel::findExit(const TrkGeomTraj* traj,double startflt,double& exitflt) const
{
  static const double epsilon(1e-6);
  bool retval(false);
// test that we're inside
  HepPoint startpos = traj->position(startflt);
  bool startsinside = isInside(startpos);
  if(startsinside){
// find the closest outward-going intersection with the boundary elements
    double minflt = std::numeric_limits<double>::max();
//  double minbdist = std::numeric_limits<double>::max();
    for(unsigned idim=0;idim<PacVoxel::ndim;idim++){
      for(unsigned ibound=0;ibound<PacVoxel::nbnd;ibound++){
        const PacDetElem* belem = boundElem((dimension)idim,(bound)ibound);
        double fltlen = startflt;
        if(belem != 0 && belem->intersect(traj,fltlen) && fltlen < minflt){
// test if we're inside the other 2 dimension
          bool inside(true);
          HepPoint exitpoint;
          Hep3Vector tdir;
          traj->getInfo(fltlen,exitpoint,tdir);
          for(unsigned jdim=0;jdim<3;jdim++){
            if(jdim != idim){
              double bd = bdist(exitpoint,(dimension)jdim);
              bool dinside = bd > -epsilon;
              inside &= dinside;         
            }
          }      
          if(inside){
// test that the intersection direction corresponds to exit, not entrance.
            Hep3Vector norm;
            double dist = belem->normTo(exitpoint,norm);
            if(fabs(dist) > 1e-5 ) std::cerr << "Error finding exit " << std::endl;
            double sign=tdir.dot(norm);
            if(exits(sign,(dimension)idim,(bound)ibound) ){
              exitflt = fltlen + epsilon;
              minflt = fltlen;
              retval = true;
            }
          }
        }
      }
    }
    if(!retval){
//  This could be because we're on a corner, or there's a glancing intersection
// revert to the brute-force method of stepping.
      double vsize = _rbound[1]-_rbound[0];
      double rmid = 0.5*(_rbound[1]+_rbound[0]);
      vsize = std::min(vsize,_zbound[1]-_zbound[0]);
      vsize = std::min(vsize,rmid*(_fbound[1]-_fbound[0]));
// step in an increment of the minimum voxel size
      double step = vsize/2.0;
      double fltlen = startflt + step;
      unsigned maxstep(1000);
      unsigned istep(0);
      HepPoint pt = traj->position(fltlen);
      while (istep < maxstep && isInside(pt)) {
        fltlen += step;
        istep++;
        pt = traj->position(fltlen);
      } 
      if(!isInside(pt)){
        retval = true;
        exitflt = fltlen;
      }
    }
    if(!retval){
// we STILL failed to find an exit: we are probably looping inside a volume
//      std::cerr << "Failed to find cylinder voxel exit" << std::endl;
    }
  } else {
    std::cerr << "Didn't start in voxel! " << std::endl;
  }
  return retval;
}

bool
PacCylVoxel::findEntrance(const TrkGeomTraj* traj,double startflt,double& enterflt) const
{
  static const double epsilon(1e-5);
  bool retval(false);
// see if we're inside; if so, we're done
  bool startsinside = isInside(traj->position(startflt));
  if(startsinside){
    retval = true;
    enterflt = startflt;
  } else {
// find the closest inward-going intersection with the boundary elements
    double minflt = std::numeric_limits<double>::max();
    for(unsigned idim=0;idim<PacVoxel::ndim;idim++){
      for(unsigned ibound=0;ibound<PacVoxel::nbnd;ibound++){
        const PacDetElem* belem = boundElem((dimension)idim,(bound)ibound);
        double fltlen = startflt;
        if(belem != 0 && belem->intersect(traj,fltlen) && fltlen < minflt){
// test if we're inside the other 2 dimension
          HepPoint exitpoint;
          Hep3Vector tdir;
          traj->getInfo(fltlen,exitpoint,tdir);
          bool inside(true);
          for(unsigned jdim=0;jdim<3;jdim++){
            if(jdim != idim){
              bool dinside = isInside(exitpoint,(dimension)jdim);
              inside &= dinside;         
            }
          }
          if(inside){
// test that the intersection direction corresponds to entrance, not exit;
            Hep3Vector norm;
            double dist = belem->normTo(exitpoint,norm);
            if(fabs(dist) > 1e-5 ) std::cerr << "Error finding exit " << std::endl;
            double sign=tdir.dot(norm);
            if(enters(sign,(dimension)idim,(bound)ibound)){
              enterflt = fltlen + epsilon;
              minflt = fltlen;
              retval = true;
            }
          }
        }
      }
    }
  }
  return retval;
}

bool
PacCylVoxel::exits(double sign, dimension idim, bound ibound) const {
  switch (idim) {
    case rho:
// cylinder normals point out
    return sign>0.0 && ibound==upper || sign<0.0 && ibound==lower;
    case zed:
// z planes normal point in z
    return sign>0.0 && ibound==upper || sign<0.0 && ibound==lower;
// phi planes point in increasing phi
    case phi:
    return sign>0.0 && ibound==upper || sign<0.0 && ibound==lower;
    default:
    assert(0==1);
  }
  return false;
}

bool
PacCylVoxel::enters(double sign, dimension idim, bound ibound) const {
  switch (idim) {
    case rho:
// cylinder normals point out
    return sign<0.0 && ibound==upper || sign>0.0 && ibound==lower;
    case zed:
// z planes normal point in z
    return sign<0.0 && ibound==upper || sign>0.0 && ibound==lower;
// phi planes point in increasing phi
    case phi:
    return sign<0.0 && ibound==upper || sign>0.0 && ibound==lower;
    default:
    assert(0==1);
  }
  return false;
}

double
PacCylVoxel::boundVal(dimension idim,bound ibound) const {
  switch(idim){
    case rho:
    return rBound(ibound);
    case zed:
    return zBound(ibound);
    case phi:
    return phiBound(ibound);
    default:
    assert(0==1);
  }
  return -1.0;
}

const PacDetElem*
PacCylVoxel::boundElem(dimension idim,bound ibound) const {
  switch(idim){
    case rho:
    return rElem(ibound);
    case zed:
    return zElem(ibound);
    case phi:
    return phiElem(ibound);
    default:
    assert(0==1);
  }
  return 0;
}

std::ostream& 
operator << (std::ostream& os, const PacCylVoxel& voxel){
  os << " VoxelId ir:iz:iphi = " 
    << voxel.id()._ind1 << ":"<< voxel.id()._ind2 << ":"<< voxel.id()._ind3;
  os << " Voxel boundaries r z phi = " 
    << voxel.rBound(PacCylVoxel::lower) << ":" << voxel.rBound(PacCylVoxel::upper) << " , "
    << voxel.zBound(PacCylVoxel::lower) << ":" << voxel.zBound(PacCylVoxel::upper) << " , "
    << voxel.phiBound(PacCylVoxel::lower) << ":" << voxel.phiBound(PacCylVoxel::upper);
  return os;
}


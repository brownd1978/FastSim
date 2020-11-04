// ------------------------------------------------------------------------------
// File and Version Information:
// Class to describe a voxel set of cylindrical voxels
// Copyright Information:
//	Copyright (C) 2010	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 8/2/2010
// ------------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "PacGeom/PacCylVoxelSet.hh"
#include "PacGeom/PacCylDetElem.hh"
#include "PacGeom/PacConeDetElem.hh"
#include "PacGeom/PacPlaneDetElem.hh"
#include "PacGeom/PacRingDetType.hh"
#include "PacGeom/PacRectDetType.hh"
#include "PacGeom/PacCylDetType.hh"
#include "DetectorModel/DetIntersection.hh"
#include <limits>
#include <algorithm>
#include <cmath>
#include <set>

const char* PacCylVoxelSet::_rname = {"RBoundary"};
const char* PacCylVoxelSet::_zname = {"ZBoundary"};
const char* PacCylVoxelSet::_fname = {"PhiBoundary"};

// comparator functor
struct PacCylVoxelSort {
  PacCylVoxelSort(const PacCylVoxelSet* set) : _set(set) {}
  const PacCylVoxelSet* _set;
  bool operator() (const PacVoxelId& vid1,const PacVoxelId& vid2) const {
    unsigned ind1,ind2;
    return _set->voxelIndex(vid1,ind1) && _set->voxelIndex(vid2,ind2) && ind1 < ind2;
  }
};

PacCylVoxelSet::PacCylVoxelSet(double rbounds[2],unsigned nrvoxels,
  double zbounds[2], unsigned nzvoxels,double minphisize) :
_rtype(0), _ztype(0), _ftype(0) {
// construct the boundaries; first, radial
  assert(rbounds[1]>rbounds[0] && rbounds[1] > 0.0 && nrvoxels>0);
// make sure the lower limit is at least 0
  rbounds[0] = std::max(0.0,rbounds[0]);
  double rstep = (rbounds[1]-rbounds[0])/nrvoxels;
  for (unsigned irbnd=0;irbnd<nrvoxels+1;irbnd++){
    double radius = rbounds[0] + irbnd*rstep;
    _rbounds.push_back(radius);
  }
// z boundaries
  assert(zbounds[1]>zbounds[0] && nzvoxels>1);
  double zstep = (zbounds[1]-zbounds[0])/nzvoxels;
  for (unsigned izbnd=0;izbnd<nzvoxels+1;izbnd++){
    double zed = zbounds[0]+izbnd*zstep;
    _zbounds.push_back(zed);
  }
// phi boundaries; compute the number as successive powers of 2
  unsigned nphi = 1;
  for (unsigned irbnd=0;irbnd<nrvoxels;irbnd++){
    double fsize = Constants::twoPi*_rbounds[irbnd]/nphi;
    while(fsize/2.0 > minphisize){
      nphi *= 2;
      fsize /= 2.0;
    }
    _nfvoxels.push_back(nphi);
  }
  _maxnphi = _nfvoxels.back();
  buildElems(0.0);
  buildVoxels();
}
  
// construct from a set of explicit boundaries
PacCylVoxelSet::PacCylVoxelSet(const std::vector<double>&rbounds,
  const std::vector<double>&zbounds,
  const std::vector<unsigned>&nphibounds,double phioffset) :
_rbounds(rbounds), _zbounds(zbounds), _nfvoxels(nphibounds), _rtype(0), _ztype(0), _ftype(0) {
// test monotonicity
  for (unsigned irbnd=0;irbnd<_rbounds.size()-1;irbnd++){
    assert(_rbounds[irbnd+1]>_rbounds[irbnd]);
  }
  for (unsigned izbnd=0;izbnd<_zbounds.size()-1;izbnd++){
    assert(_zbounds[izbnd+1] > _zbounds[izbnd]);
  }
// # of phi boundaries must match # of radial voxels
  assert(_nfvoxels.size() == _rbounds.size()-1);
//   Require all phi divisions to be an integer multiple
// so that all the boundardies can be shared
  _maxnphi = 0;
  for (unsigned ifvox=0;ifvox<_nfvoxels.size();ifvox++){
    _maxnphi = std::max(_maxnphi,_nfvoxels[ifvox] );
  }
  for (unsigned ifvox=0;ifvox<_nfvoxels.size();ifvox++){
    assert(_nfvoxels[ifvox] > 0);
    double mult = _maxnphi/float(_nfvoxels[ifvox]);
    assert(mult - floor(mult) == 0.0);
  }

  buildElems(phioffset);    
  buildVoxels();
}

void
PacCylVoxelSet::buildElems(double phioffset) {
// only 1 type is needed, shared between all cylinders
  double zb[2] = {_zbounds.front(),_zbounds.back()};
  _rtype = new PacCylDetType(0.0,zb,0,0);
  std::vector<const PacMeasurement*> nodevs;
  for (unsigned irbnd=0;irbnd<_rbounds.size();irbnd++){
// create the element associated with this boundary
    double radius = _rbounds[irbnd];
    PacCylDetElem* relem(0);
// no element if the radius is 0
    if(radius > 0.0){
      relem = new PacCylDetElem(_rtype,radius,_rname,irbnd,nodevs,0.0,0.0);
      assert(relem != 0);
    }
    _relems.push_back(relem);
  }
// z elements
// only 1 type is needed, shared between all z planes
  _ztype = new PacRingDetType(0.0,_rbounds.front(),_rbounds.back(),0,0);
  for (unsigned izbnd=0;izbnd<_zbounds.size();izbnd++){
    double zed = _zbounds[izbnd];
// create the element associated with this boundary
    Hep3Vector zpt(0.0,0.0,zed);
    PacPlaneDetElem* zelem = new PacPlaneDetElem(_ztype,PacPlaneDetElem::zparallel,zpt,0.0,_zname,izbnd,nodevs,0.0,0.0);
    assert(zelem != 0);
    _zelems.push_back(zelem);
  }
// phi elements
  double zsize = _zbounds.back()-_zbounds.front();
  double rhosize = _rbounds.back()-_rbounds.front();
  double zmid = 0.5*(_zbounds.front()+_zbounds.back());
// only 1 type is needed, shared between all phi planes
  _ftype = new PacRectDetType(0.0,rhosize,zsize,0,0);
// only 1 phi voxel means no boundaries
  if(_maxnphi > 1){
    double phistep = Constants::twoPi/_maxnphi;
    for (unsigned iphi=0;iphi<_maxnphi;iphi++){
      double phi = phistep*iphi+phioffset;
      double rhomid = 0.5*(_rbounds.back()+_rbounds.front());
      Hep3Vector center(rhomid*cos(phi),rhomid*sin(phi),zmid);
// create the element associated with this boundary
      PacPlaneDetElem* felem = new PacPlaneDetElem(_ftype,PacPlaneDetElem::zperp,center,phi,_fname,iphi,nodevs,0.0,0.0);
      assert(felem != 0);
      _felems.push_back(felem);
      _fbounds.push_back(phi);
    }
// last phi boundary is the first
    _felems.push_back(_felems.front());
    _fbounds.push_back(Constants::twoPi+phioffset);
  }
// count up how many voxels/radii for indexing purposes
  unsigned nvoxels(0);
  for (unsigned ir=0;ir<_rbounds.size()-1;ir++){
    _nvoxels.push_back(nvoxels);
    nvoxels += (_zbounds.size()-1)*_nfvoxels[ir];
  }
// last element is the total # of voxels
  _nvoxels.push_back(nvoxels);
}

void
PacCylVoxelSet::buildVoxels() {
  assert(_relems.size() == _rbounds.size() && _zelems.size() == _zbounds.size());
  _voxels.reserve(_nvoxels[_nfvoxels.size()]);
// nested loop.
  double rbnd[2];
  double zbnd[2];
  double fbnd[2];
  const PacCylDetElem* rlim[2] = {0,0};
  const PacPlaneDetElem* zlim[2] = {0,0};
  const PacPlaneDetElem* flim[2] = {0,0};
  unsigned icount(0);
// loop over voxels, not boundaries
  for (int ir=0;ir<_relems.size()-1;ir++){
    rlim[0] = _relems[ir];
    rlim[1] = _relems[ir+1];
    rbnd[0] = _rbounds[ir];
    rbnd[1] = _rbounds[ir+1];
// compute how the phi voxels in this ring step WRT the maximum (must be a power of 2)
// by default assume no segmentation
    unsigned nphi = _nfvoxels[ir];;
    unsigned ifstep = _maxnphi/nphi;
    
    for (unsigned iz=0;iz<_zelems.size()-1;iz++){
      zlim[0] = _zelems[iz];
      zlim[1] = _zelems[iz+1];

      zbnd[0] = _zbounds[iz];      
      zbnd[1] = _zbounds[iz+1];
      
      for (unsigned iphi=0;iphi<nphi;iphi++){
        unsigned jphi = iphi*ifstep;
// if there's only 1 voxel in phi, it has no boundaries
        if(nphi > 1) {
          flim[0] = _felems[jphi];
          flim[1] = _felems[jphi+ifstep];
          fbnd[0] = _fbounds[jphi];
          fbnd[1] = _fbounds[jphi+ifstep];
        } else {
          flim[0] = 0;
          flim[1] = 0;
          fbnd[0] = 0;
          fbnd[1] = Constants::twoPi;
        }

        PacVoxelId vid(ir,iz,iphi);
        unsigned index;
        assert(voxelIndex(vid,index));
        PacCylVoxel voxel(rbnd,zbnd,fbnd,rlim,zlim,flim,vid);
        _voxels.push_back(voxel);
        assert(index == icount);
        icount++;
      }
    }
  }
// finally, create the 'voxel' representing the bounds of this entire set.  Note there are no phi boundaries
  rbnd[0] = _rbounds.front();
  rbnd[1] = _rbounds.back();
  zbnd[0] = _zbounds.front();
  zbnd[1] = _zbounds.back();
  fbnd[0] = 0;
  fbnd[1] = Constants::twoPi;
  rlim[0] = _relems.front();
  rlim[1] = _relems.back();
  zlim[0] = _zelems.front();
  zlim[1] = _zelems.back();
  flim[0] = 0;
  flim[1] = 0;
  PacVoxelId vid(0,0,0);
  _voxel = PacCylVoxel(rbnd,zbnd,fbnd,rlim,zlim,flim,vid);
  
// set current iterator to null
  _vcurrent = _voxels.end();
}

PacCylVoxelSet::~PacCylVoxelSet()
{
  delete _rtype;
  delete _ztype;
  delete _ftype;
  for (unsigned irelem=0;irelem<_relems.size();irelem++){
    delete _relems[irelem];
  }
  for (unsigned izelem=0;izelem<_zelems.size();izelem++){
    delete _zelems[izelem];
  }
// do not double-delete the first (=last) phi boundary element
  unsigned endfelem = _felems.size() > 0 ? _felems.size()-1 : 0;
  for (unsigned ifelem=0; ifelem < endfelem; ifelem++){
    delete _felems[ifelem];
  }
}

unsigned
PacCylVoxelSet::voxelIndex(unsigned ir, unsigned iz, unsigned iphi) const {
  return iphi + iz*_nfvoxels[ir] + _nvoxels[ir];
}

bool
PacCylVoxelSet::voxelIndex(const PacVoxelId& vid,unsigned& index) const {
  if(validRhoIndex(vid._ind1)
    && validZedIndex(vid._ind2)
    && validPhiIndex(vid._ind1,vid._ind3)){
    index = voxelIndex(vid._ind1,vid._ind2,vid._ind3);
    return true;
  } else
    return false;
}

PacCylVoxelSet::VoxelIter
PacCylVoxelSet::findVoxel(const HepPoint& point) const {
  PacCylVoxelSet::VoxelIter viter = _voxels.end();
  double rho = point.perp();
  double z = point.z();
  double phi = point.phi();
  unsigned ir = rhoIndex(rho);
  unsigned iz = zedIndex(z);
  unsigned iphi = phiIndex(ir,phi);
  if(ir<0 || iz<0 || iphi<0)return viter;
  PacVoxelId vid(ir,iz,iphi);
  viter = findVoxel(vid);
// test
  if(viter != _voxels.end() && !viter->isInside(point)){
    std::cerr << " Point is not inside voxel " << std::endl;
  }
  return viter;
}

int
PacCylVoxelSet::rhoIndex(double rho) const {
// this sets the unsigned to its max value
  int ir(-1);
  std::vector<double>::const_iterator ilowr = std::upper_bound(_rbounds.begin(),_rbounds.end(),rho);
  if(ilowr != _rbounds.end())
    ir = std::distance(_rbounds.begin(),--ilowr);
  return ir;
}

int
PacCylVoxelSet::zedIndex(double zed) const {
  int iz(-1);
  std::vector<double>::const_iterator ilowz = std::upper_bound(_zbounds.begin(),_zbounds.end(),zed);
  if(ilowz != _zbounds.end())
    iz = std::distance(_zbounds.begin(),--ilowz);
  return iz;
}

int
PacCylVoxelSet::phiIndex(int ir,double phi) const {
  if(ir<0)return -1;
  if(_nfvoxels[ir]<=1)return 0;
  if(phi<0) phi += Constants::twoPi;
  if(phi<_fbounds[0]) phi += Constants::twoPi;
  int iphi(-1);
  if(ir < _nfvoxels.size()){
    double phisize = Constants::twoPi/_nfvoxels[ir];
    iphi = (int)floor((phi-_fbounds[0])/phisize);
// wrap phi
    if(iphi == _nfvoxels[ir])
      iphi = 0;
  }
  return iphi;
}

PacCylVoxelSet::VoxelIter
PacCylVoxelSet::findVoxel(const PacVoxelId& vid) const {
  unsigned index;
  if(voxelIndex(vid,index)) 
    return _voxels.begin()+index;
  else
    return _voxels.end();
}

std::vector<PacCylVoxel>::iterator
PacCylVoxelSet::findVoxel(const PacVoxelId& vid){
  unsigned index;
  if(voxelIndex(vid,index)) 
    return _voxels.begin()+index;
  else
    return _voxels.end();
}

// find the next voxel given a current voxel, a traj, and a starting fltlen
PacCylVoxelSet::VoxelIter
PacCylVoxelSet::findNextVoxel(const TrkGeomTraj* traj, double& fltlen) const {
  VoxelIter retval = end();
  HepPoint pos = traj->position(fltlen);
// first, see if we're inside a voxel already
  retval = findVoxel(pos);
  if(retval == end()){
// if not, find the entrance
    retval = findEntrance(traj,fltlen);
  }
  return retval;
}

PacCylVoxelSet::VoxelIter
PacCylVoxelSet::findEntrance(const TrkGeomTraj* traj, double& fltlen) const {
  VoxelIter retval = _voxels.end();
// find the entrance to the super-voxel
  double enterflt;
  if(_voxel.findEntrance(traj,fltlen,enterflt)){
    retval = findVoxel(traj->position(enterflt));
    if(retval != _voxels.end())
      fltlen = enterflt;
    else
      std::cerr << "can't find entrance " << std::endl;
  }
  return retval;
}

bool
PacCylVoxelSet::findExit(const TrkGeomTraj* traj, double& fltlen) const {
// find the entrance to the super-voxel
  bool retval(false);
  double exitflt;
  if(_voxel.findExit(traj,fltlen,exitflt)){
    retval = true;
    fltlen = exitflt;  
  }
  return retval;
}

bool
PacCylVoxelSet::insert(const PacDetElem* elem) {
// each element must be inserted uniquely; first cylinders
  const PacCylDetElem* celem = dynamic_cast<const PacCylDetElem*>(elem);
  if(celem != 0){
    return insertCylinder(celem);
  }
// then cones
  const PacConeDetElem* cnelem = dynamic_cast<const PacConeDetElem*>(elem);
  if(cnelem != 0){
    return insertCone(cnelem);
  }
// then planes
  const PacPlaneDetElem* pelem = dynamic_cast<const PacPlaneDetElem*>(elem);
  if(pelem != 0){
    return insertPlane(pelem);
  }
  return false;
}

bool
PacCylVoxelSet::insertCylinder(const PacCylDetElem* celem) {
// find the radial coordinate
  unsigned ir = rhoIndex(celem->radius());
  if(!validRhoIndex(ir)){
    std::cerr << "Cylinder element extends outside voxel set radial limits " << std::endl;
    return false;
  }
  unsigned izmin = zedIndex(celem->cylType()->lowZ());
  unsigned izmax = zedIndex(celem->cylType()->hiZ());
  if(!validZedIndex(izmin) || !validZedIndex(izmax)) {
    std::cerr << "Cylinder element extends outside voxel set zed limits " << std::endl;
    return false;
  }
// loop over all voxels which enclose this cylinder
  for(unsigned iz=izmin;iz <= izmax;iz++){
    for(unsigned iphi=0;iphi < _nfvoxels[ir]; iphi++){
      PacVoxelId vid(ir,iz,iphi);
      std::vector<PacCylVoxel>::iterator ifnd = findVoxel(vid);
      if(ifnd != _voxels.end()) {
        ifnd->insert(celem);
      } else {
        std::cerr << "Can't find voxel for vid " << vid << std::endl;
      }
    }
  }
  return true;
}

bool
PacCylVoxelSet::insertCone(const PacConeDetElem* cnelem) {
// find the endpoints in a random plane, to avoid problems with boundaries
  double rends[2];
  double zends[2];
  double randphi(0.14342343);
  rends[0] = fabs(cnelem->coneType()->lowR()*cnelem->cone()->sinTheta());
  rends[1] = fabs(cnelem->coneType()->hiR()*cnelem->cone()->sinTheta());
  zends[0] = cnelem->coneType()->lowR()*cnelem->cone()->cosTheta()+cnelem->zVertex();
  zends[1] = cnelem->coneType()->hiR()*cnelem->cone()->cosTheta()+cnelem->zVertex();
  HepPoint p1(rends[0]*cos(randphi),rends[0]*sin(randphi),zends[0]);
  HepPoint p2(rends[1]*cos(randphi),rends[1]*sin(randphi),zends[1]);
// build a trajectory from these, and store the voxels it goes through
  TrkLineTraj ltraj(p1,p2);
  std::vector<PacVoxelId> vids;
  findVoxels(&ltraj,vids);
// now loop over these voxels, and insert this element in the ring of voxels with the same
// rho and z
  for(std::vector<PacVoxelId>::iterator ivid=vids.begin();ivid!= vids.end();ivid++){
    for(unsigned iphi=0;iphi<_nfvoxels[ivid->_ind1];iphi++){
      PacVoxelId vid(ivid->_ind1,ivid->_ind2,iphi);
      std::vector<PacCylVoxel>::iterator viter = findVoxel(vid);
      if(viter != end()){
        viter->insert(cnelem);
      } else
        std::cerr << "Cone insertion: Failed to find voxel " << std::endl;
    }
  }
  return vids.size()>0;
}

bool
PacCylVoxelSet::insertPlane(const PacPlaneDetElem* pelem) {
// see what type of plane this is
  const PacRingDetType* ring = dynamic_cast<const PacRingDetType*>(pelem->planeType());
  if(ring != 0)
    return insertRing(pelem,ring);
  const PacRectDetType* rect = dynamic_cast<const PacRectDetType*>(pelem->planeType());
  if(rect != 0)
    return insertRect(pelem,rect);
  return false;
}

bool
PacCylVoxelSet::insertRing(const PacPlaneDetElem* pelem,const PacRingDetType* ring) {
// find the Z index: this is the same for the entire ring
  unsigned iz = zedIndex(pelem->midpoint().z());
  if(!validZedIndex(iz)) {
    std::cerr << "Ring element extends outside voxel set zed limits " << std::endl;
    return false;
  }
// find the range of radii
  unsigned irmin = rhoIndex(ring->lowrad());
  unsigned irmax = rhoIndex(ring->highrad());
  if(!validRhoIndex(irmin) || !validRhoIndex(irmax)){
    std::cerr << "Ring element extends outside voxel set radial limits " << std::endl;
    return false;
  }
// loop over phi, find the voxels, and insert the element
  for(unsigned ir=irmin;ir<=irmax;ir++){
    for(unsigned iphi=0;iphi<_nfvoxels[ir];iphi++){
      PacVoxelId vid(ir,iz,iphi);
      std::vector<PacCylVoxel>::iterator viter = findVoxel(vid);
      if(viter != end()){
        viter->insert(pelem);
      } else
        std::cerr << "Ring insertion: Failed to find voxel " << std::endl;

    }
  }
  return irmax >= irmin;
}

bool
PacCylVoxelSet::insertRect(const PacPlaneDetElem* pelem,const PacRectDetType* rect) {
// deal with 2 orientations separately
  if(pelem->planeOrientation() == PacPlaneDetElem::zperp){
    return insertPerpRect(pelem,rect);
  } else if(pelem->planeOrientation() == PacPlaneDetElem::zparallel){
    return insertParallelRect(pelem,rect);
  }
  return false;
}

bool
PacCylVoxelSet::insertPerpRect(const PacPlaneDetElem* pelem,const PacRectDetType* rect) {
// find the z ends.  Z corresponds to local 'v' dimension
  double zmin = pelem->midpoint().z() - rect->vSize()/2.0;
  double zmax = pelem->midpoint().z() + rect->vSize()/2.0;
  unsigned izmin = zedIndex(zmin);
  unsigned izmax = zedIndex(zmax);
  if(!validZedIndex(izmin) || !validZedIndex(izmax)) {
    std::cerr << "Rectangle element extends outside voxel set zed limits " << std::endl;
    return false;
  }
// find the endpoints points of the line of the transverse projection of this plane
  HepPoint p1 = pelem->midpoint() - pelem->udir()*rect->uSize()/2.0;
  HepPoint p2 = pelem->midpoint() + pelem->udir()*rect->uSize()/2.0;
  PacCylVoxelSet::VoxelIter viter1 = findVoxel(p1);
  PacCylVoxelSet::VoxelIter viter2 = findVoxel(p2);
  if(viter1 == end() || viter2 == end()){
    std::cerr << "Rectangle extends beyond voxel set!"<< std::endl;
    return false;
  }
// build a trajectory from these, and store the voxels it goes through
  TrkLineTraj ltraj(p1,p2);
  PacVoxelInter vexit;
  vexit._dinter.pathlen = 0.0;
  std::vector<PacVoxelId> vids;
  findVoxels(&ltraj,vids);
// now loop over these voxels, and insert this element in the line of voxels with the same 
// rho and phi
  for(std::vector<PacVoxelId>::iterator ivid=vids.begin();ivid!= vids.end();ivid++){
    for(unsigned iz=izmin;iz<=izmax;iz++){
      PacVoxelId vid(ivid->_ind1,iz,ivid->_ind3);
      std::vector<PacCylVoxel>::iterator viter = findVoxel(vid);
      if(viter != end()){
        viter->insert(pelem);
      } else
        std::cerr << "PerpRect insertion: Failed to find voxel " << std::endl;
    }
  }
  return vids.size()>0;
}

bool
PacCylVoxelSet::insertParallelRect(const PacPlaneDetElem* pelem,const PacRectDetType* rect) {
// find the corner points
  Hep3Vector udir = pelem->udir();
  Hep3Vector vdir = pelem->vdir();
  double halfu = rect->halfuSize();
  double halfv = rect->halfvSize();
  HepPoint mid = pelem->midpoint();
  std::vector<HepPoint> corners;
  corners.push_back(pelem->midpoint() + halfu*udir + halfv*vdir);
  corners.push_back(pelem->midpoint() - halfu*udir + halfv*vdir);
  corners.push_back(pelem->midpoint() - halfu*udir - halfv*vdir);
  corners.push_back(pelem->midpoint() + halfu*udir - halfv*vdir);
  // close the perimeter
  corners.push_back(corners.front());
// find the voxels intersected by the edges and add the element to them
  for(unsigned iedge=0;iedge<4;iedge++){
    TrkLineTraj ltraj(corners[iedge],corners[iedge+1]);
    std::vector<PacVoxelId> vids;
    findVoxels(&ltraj,vids);
// loop over all voxels and insert this element
    for(std::vector<PacVoxelId>::iterator ivid=vids.begin();ivid!=vids.end();ivid++){
      std::vector<PacCylVoxel>::iterator viter = findVoxel(*ivid);
      if(viter != end()){
        viter->insert(pelem);
      } else
        std::cerr << "Parallel Rect insertion: Failed to find voxel " << std::endl;
    }
  }
// now loop over all voxels in this plane and see if their center is inside the rectangle; if so, add the rectangle
// to them
  VoxelIter mvox = findVoxel(mid);
  if(mvox == end()){
    std::cerr << "Midpoint not in voxel" << std::endl;
    return false;
  }
  unsigned iz = mvox->id()._ind2;
  for(unsigned ir=0;ir<_rbounds.size()-1;ir++){
    for(unsigned iphi=0;iphi<_nfvoxels[ir];iphi++){
      PacVoxelId vid(ir,iz,iphi);
      std::vector<PacCylVoxel>::iterator viter = findVoxel(vid);
      if(viter != end()){
        HepPoint pos = viter->center();
        if(pelem->activeRegion(pos)) {
          viter->insert(pelem);
        }
      } else
        std::cerr << "Parallel Rect insertion: Failed to find voxel " << std::endl;
    }
  }
  return true;
}

void
PacCylVoxelSet::findVoxels(const TrkGeomTraj* traj,std::vector<PacVoxelId>& vids) const {
  double fltlen = traj->lowRange();
  VoxelIter viter = _voxels.end();
  while( (viter = findNextVoxel(traj,fltlen)) != _voxels.end()){
    vids.push_back(viter->id());
    if(!viter->findExit(traj,fltlen,fltlen)){
      std::cerr<< "Didn't find exit " << std::endl;
      break;
    }
  }
}

void 
PacCylVoxelSet::findVoxels(const PacDetElem* elem,std::vector<PacCylVoxel>& voxels) const {
  voxels.clear();
// exhaustive search: doesn't matter
  for(std::vector<PacCylVoxel>::const_iterator ivox = _voxels.begin();ivox!=_voxels.end();ivox++){
    std::vector<const PacDetElem*>::const_iterator ifnd = std::find(ivox->detElems().begin(),ivox->detElems().end(),elem);
    if(ifnd != ivox->detElems().end())
      voxels.push_back(*ivox);
  }
}

bool
PacCylVoxelSet::findNextInter(const TrkGeomTraj* traj,DetIntersection& dinter) const {
  bool retval(false);
// check the current voxel cache
  VoxelIter viter = end();
  if(_vcurrent != _voxels.end() && _vcurrent->isInside(traj->position(dinter.pathlen)))
    viter = _vcurrent;
  else
    viter = findNextVoxel(traj,dinter.pathlen);
  unsigned ntries(0);
  while( (!retval) && viter != end() && ntries < _voxels.size()){
// find where we exit this voxel
    double exitflt;
    if(viter->findExit(traj,dinter.pathlen,exitflt)){
// look for intersections in this range
      if(viter->findInter(traj,dinter,exitflt)){
        retval = true;
// cache for next time
        _vcurrent = viter;
      } else {
// find the next voxel
        dinter.pathlen = exitflt;
        viter = findNextVoxel(traj,dinter.pathlen);
      }
    } else {
      std::cerr << "Failed to find exit " << std::endl;
      viter = end();
// try to leave this voxel set
      if(_voxel.findExit(traj,dinter.pathlen,exitflt)){
        dinter.pathlen = exitflt;
      } else {
// give up!
        dinter.pathlen = std::numeric_limits<double>::max();
      }
    }
    ntries++;
  }
  return retval;
}

bool
PacCylVoxelSet::exit(const PacCylVoxel& voxel) const {
  return
    voxel.id()._ind1 == 0 ||   voxel.id()._ind1 == _rbounds.size()-2 ||
    voxel.id()._ind2 == 0 ||   voxel.id()._ind2 == _zbounds.size()-2;
}

bool
PacCylVoxelSet::isInside(const HepPoint& point) const{
  return _voxel.isInside(point);
}

std::ostream& operator << (std::ostream& str, const PacCylVoxelSet& cylset) {
  str << "Cylindrical voxel set with " << cylset._rbounds.size() << " radial boundaries, "
    << cylset._zbounds.size() << " zed boundaries, and phi voxels for each ring as follows:" << std::endl;
  for(unsigned irbnd=0;irbnd<cylset._rbounds.size()-1;irbnd++){
    str << cylset._rbounds[irbnd] << " < radius < " << cylset._rbounds[irbnd+1];
    str << " : N phi voxels = " << cylset._nfvoxels[irbnd] << std::endl;
  }
  str << "Z boundaries ";
  for (unsigned izbnd=0;izbnd<cylset._zbounds.size()-1;izbnd++){
    str << cylset._zbounds[izbnd] << ":";
  }
  str << cylset._zbounds.back();
  return str;
}



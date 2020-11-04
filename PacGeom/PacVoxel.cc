// ------------------------------------------------------------------------------
// File and Version Information:
// class representing a voxel in Pac geometry navigation.  This is a pure interface
// Copyright Information:
//	Copyright (C) 2010	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 8/2/2010
// ------------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "BbrGeom/TrkGeomTraj.hh"
#include "PacGeom/PacVoxel.hh"
#include "PacGeom/PacDetElem.hh"
#include <algorithm>
#include <limits>

// simple struct to describe the 3-d index of a voxel
PacVoxelId::PacVoxelId(unsigned i1, unsigned i2, unsigned i3) : _ind1(i1),_ind2(i2),_ind3(i3)
{}

bool
PacVoxelId::operator == (const PacVoxelId& other ) const {
  return _ind1 == other._ind1 && _ind2 == other._ind2 && _ind3 == other._ind3;
}

PacVoxel::PacVoxel(PacVoxelId vid) : _vid(vid)
{}

PacVoxel::PacVoxel(const PacVoxel& other) : _vid(other._vid), _delems(other._delems)
{}

PacVoxel::PacVoxel() : _vid(0,0,0)
{}

PacVoxel::~PacVoxel()
{}

PacVoxel&
PacVoxel::operator = (const PacVoxel& other) {
  if(&other != this) _vid = other._vid; 
  return *this;
}

bool
PacVoxel::operator == (const PacVoxel& other) const {
  return _vid == other._vid;
}

void
PacVoxel::insert(const PacDetElem* elem)
{
// make sure elements are enetered only once
  std::vector<const PacDetElem*>::const_iterator ifnd = std::find(_delems.begin(),_delems.end(),elem);
  if(ifnd == _delems.end())
    _delems.push_back(elem);
}

bool 
PacVoxel::findInter(const TrkGeomTraj* traj,DetIntersection& dinter,double exitflt) const {
  bool retval(false);
  unsigned ibest(0);
  const static double epsilon(1e-6);
  double mindist = std::min(exitflt,traj->hiRange());
  // loop over detector elements, and find the closest intersection before exit
  for(unsigned ielem=0;ielem<_delems.size();ielem++){
// start from the original point for each element
// add a tiny buffer to avoid finding the same element again and again
    double interflt = dinter.pathlen + epsilon;
// find the closest element intersection inside this voxel
    if(_delems[ielem]->intersect(traj,interflt) && interflt < mindist){
      ibest = ielem;
      mindist = interflt;
      retval = true;
    }
  }
  if(retval)
// fill the rest of the info
    _delems[ibest]->fillDetInter(traj,dinter,mindist);
// if no intersections were found and the exit is beyond the traj range, return the traj range
  if(!retval && traj->hiRange() < exitflt){
    retval = true;
    dinter.delem = 0;
    dinter.trajet = traj;
    dinter.pathlen = traj->hiRange();
    dinter.pathrange[0]= dinter.pathlen;
    dinter.pathrange[1]= dinter.pathlen;
    dinter.flag[0] = dinter.flag[1] = 0;
  }
  return retval;
}

std::ostream& operator << (std::ostream& stream, const PacVoxelId& vid) {
  stream << "Voxel Id " << vid._ind1 << ":" << vid._ind2 << ":" << vid._ind3;
  return stream;
}


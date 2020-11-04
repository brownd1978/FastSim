// ------------------------------------------------------------------------------
// File and Version Information:
// Class to describe a voxel set of cylindrical voxels
// Copyright Information:
//	Copyright (C) 2010	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 8/2/2010
// ------------------------------------------------------------------------------

#ifndef PacVoxelSet_HH
#define PacVoxelSet_HH

#include "PacGeom/PacVoxel.hh"
#include "CLHEP/Geometry/HepPoint.h"

class PacDetElem;
class TrkGeomTraj;
class DetIntersection;

class PacVoxelSet {
public:
  PacVoxelSet();
	virtual ~PacVoxelSet();
// Find the voxel containing a given a point in space
//  VoxelIter findVoxel(const HepPoint&) const;
// Find a voxel by it's id
//  VoxelIter findVoxel(const PacVoxelId& ) const;
// find next voxel after exiting the given one
//  VoxelIter findNextVoxel(const PacCylVoxel& voxel, const TrkGeomTraj* traj, double& fltlen) const;
// find the next detector element intersection, starting from the point described by the given intersection
  virtual bool findNextInter(const TrkGeomTraj* traj,DetIntersection& dinter) const = 0;
// is a point inside the voxel
  virtual bool isInside(const HepPoint& point) const = 0;
// begin and end of voxel storage (to allow iteration)  
//  VoxelIter begin() const { return _voxels.begin();}
//  VoxelIter end() const { return _voxels.end();}
// insert element into appropriate voxels.  Bool says if element was inserted into at least 1 voxel
  bool insert(const PacDetElem* elem);
// find the subset of voxels which contain the given detector element
//  void findVoxels(const PacDetElem* elem,std::vector<PacCylVoxel>& voxels) const;
// compact index on this set, fails if voxelID is outside allowed range
  virtual bool voxelIndex(const PacVoxelId&,unsigned&) const = 0;
// test if a give voxel intersection is an exit
//  bool exit(const PacCylVoxel& voxel) const;
private:
};

#endif

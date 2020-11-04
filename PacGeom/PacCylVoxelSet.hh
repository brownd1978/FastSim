// ------------------------------------------------------------------------------
// File and Version Information:
// Class to describe a voxel set of cylindrical voxels
// Copyright Information:
//	Copyright (C) 2010	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 8/2/2010
// ------------------------------------------------------------------------------

#ifndef PacCylVoxelSet_HH
#define PacCylVoxelSet_HH

#include "PacGeom/PacVoxelSet.hh"
#include "PacGeom/PacCylVoxel.hh"
#include <vector>
#include <map>
#include <iostream>
#include <ostream>

//#include <hash_multimap>

class PacCylDetType;
class PacRingDetType;
class PacRectDetType;
class PacCylDetElem;
class PacConeDetElem;
class PacPlaneDetElem;
class PacDetElem;
class TrkGeomTraj;
class DetIntersection;

class PacCylVoxelSet : public PacVoxelSet {
public:
  typedef std::vector<PacCylVoxel>::const_iterator VoxelIter;
// construct from a range and # of segments.  Phi will be divided by radius so no voxel has a size less than minphisize 
  PacCylVoxelSet(double rbounds[2], unsigned nrvoxels,
    double zbounds[2], unsigned nzvoxels,
    double minphisize);
// construct from a set of explicit r and z boundaries.  phi is uniform, the # of boundaries at each radius
// must be a perfect factor of the maximum number of boundaries at any radius.
  PacCylVoxelSet(const std::vector<double>& rbounds, const std::vector<double>& zbounds,
    const std::vector<unsigned>& nphivoxels,double phioffset=0.0);
	virtual ~PacCylVoxelSet();
// Find the voxel containing a given a point in space
  VoxelIter findVoxel(const HepPoint&) const;
// Find a voxel by it's id
  VoxelIter findVoxel(const PacVoxelId& ) const;
// find next voxel along a trajectory, starting from the given flightlen
  VoxelIter findNextVoxel(const TrkGeomTraj* traj, double& fltlen) const;
// find the next detector element intersection, starting from the point described by the given intersection
  virtual bool findNextInter(const TrkGeomTraj* traj,DetIntersection& dinter) const;
  virtual bool isInside(const HepPoint& point) const;
// begin and end of voxel storage (to allow iteration)  
  VoxelIter begin() const { return _voxels.begin();}
  VoxelIter end() const { return _voxels.end();}
// insert element into appropriate voxels.  Bool says if element was inserted into at least 1 voxel
  virtual bool insert(const PacDetElem* elem);
// printout
  friend std::ostream& operator << (std::ostream& str, const PacCylVoxelSet&);
// simple accessors;
  const std::vector<double>& rbounds() const { return _rbounds;}
  const std::vector<double>& zbounds() const { return _zbounds;}
  const std::vector<double>& fbounds() const { return _fbounds;}
  const std::vector<PacCylDetElem*>& relems() const { return _relems;}
  const std::vector<PacPlaneDetElem*>& zelems() const { return _zelems;}
  const std::vector<PacPlaneDetElem*>& felems() const { return _felems;}
// find the subset of voxels which contain the given detector element
  void findVoxels(const PacDetElem* elem,std::vector<PacCylVoxel>& voxels) const;
// compact index on this set, fails if voxelID is outside allowed range
  virtual bool voxelIndex(const PacVoxelId&,unsigned&) const;
// test if a give voxel intersection is an exit
  bool exit(const PacCylVoxel& voxel) const;
// find where we enter this set
  VoxelIter findEntrance(const TrkGeomTraj* traj, double& fltlen) const;
// find where we exit this set
  bool findExit(const TrkGeomTraj* traj, double& fltlen) const;
private:
  std::vector<PacCylVoxel>::iterator findVoxel(const PacVoxelId& vid);
// helper functions
  unsigned voxelIndex(unsigned ir, unsigned iz, unsigned iphi) const;
  void findVoxels(const TrkGeomTraj* traj,std::vector<PacVoxelId>& vids) const;
  
  int rhoIndex(double rho) const;
  int zedIndex(double zed) const;
  int phiIndex(int ir,double phi) const;
  bool validRhoIndex(unsigned ir) const { return ir < _rbounds.size()-1;}
  bool validZedIndex(unsigned iz) const { return iz < _zbounds.size()-1;}
  bool validPhiIndex(unsigned ir,unsigned iphi) const { return validRhoIndex(ir) && iphi < _nfvoxels[ir];}
  void buildElems(double phioffset);
  void buildVoxels();
// specific insertion methods for particular geometries
  bool insertCylinder(const PacCylDetElem* celem);
  bool insertCone(const PacConeDetElem* celem);
  bool insertPlane(const PacPlaneDetElem* pelem);
  bool insertRing(const PacPlaneDetElem* pelem,const PacRingDetType* ring);
  bool insertRect(const PacPlaneDetElem* pelem,const PacRectDetType* ring);
  bool insertPerpRect(const PacPlaneDetElem* pelem,const PacRectDetType* rect);  
  bool insertParallelRect(const PacPlaneDetElem* pelem,const PacRectDetType* rect);  

private:
// voxel storage
  std::vector<PacCylVoxel> _voxels;
// super-voxel of this set
  PacCylVoxel _voxel;
// boundaries
  std::vector<double> _rbounds;
  std::vector<double> _zbounds;
  std::vector<double> _fbounds;
  std::vector<unsigned> _nfvoxels; // # of phi voxels for each radius
  unsigned _maxnphi; // maximum # of phi divisions
  std::vector<unsigned> _nvoxels; // cache of the number of voxels for each radius
// This object owns the boundary elements referenced by individual voxels
  std::vector<PacCylDetElem*> _relems;
  std::vector<PacPlaneDetElem*> _zelems;
  std::vector<PacPlaneDetElem*> _felems;
// type ownership
  PacCylDetType* _rtype;
  PacRingDetType* _ztype;
  PacRectDetType* _ftype;
// hash function
  size_t voxelhash(const PacCylVoxel* voxel) const;
// map between voxels and elements
//  std::hash_multimap<const PacCylVoxel*,PacDetElem*,voxelhash,PacCylVoxelEqual);
// static names
  static const char* _rname;
  static const char* _zname;
  static const char* _fname;
// cache of current voxel info while navigating, to improve efficiency
  mutable VoxelIter _vcurrent;
};

#endif

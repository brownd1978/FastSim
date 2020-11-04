// ------------------------------------------------------------------------------
// File and Version Information:
// class representing a voxel in Pac geometry navigation.  This is a pure interface
// Copyright Information:
//	Copyright (C) 2010	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 8/2/2010
// ------------------------------------------------------------------------------

#ifndef PacVoxel_HH
#define PacVoxel_HH

#include "CLHEP/Geometry/HepPoint.h"
#include "DetectorModel/DetIntersection.hh"
#include <vector>

class TrkGeomTraj;
class PacDetElem;
class DetIntersection;
class PacVoxelInter;

// simple struct to describe the 3-d index of a voxel
struct PacVoxelId {
  PacVoxelId(unsigned i1, unsigned i2, unsigned i3);
  bool operator == (const PacVoxelId& other ) const;
  bool operator != (const PacVoxelId& other ) const { return ! operator ==(other); }
  unsigned _ind1, _ind2, _ind3;
};

// struct to define exit from a voxel

class PacVoxel {
public:
  enum bound{lower=0,upper,nbnd};
  enum dimension{dim1=0,dim2,dim3,ndim};
  
// Construct with a unique ID
  PacVoxel(PacVoxelId vid);
  PacVoxel(const PacVoxel& other);
  PacVoxel();
  virtual ~PacVoxel() = 0;
  PacVoxel& operator = (const PacVoxel& other);
  bool operator == (const PacVoxel& other) const;
// return Voxel ID
  const PacVoxelId& id() const { return _vid;}
// return detector elements
  const std::vector<const PacDetElem*>& detElems() const { return _delems;}
// is this point inside the voixel?
  virtual bool isInside(const HepPoint& point) const = 0;
// given a trajectory and a starting flightlength, find the exit flightlength
// returns false if the starting poing isn't inside the voxel
  virtual bool findExit(const TrkGeomTraj* traj,double startflt, double& exitflt) const = 0;
// Look for an intersection with a detector element in this voxel.  Will return false if there are none
  virtual bool findInter(const TrkGeomTraj* traj,DetIntersection& dinter,double exitflt) const;
// boundary information  
  virtual double boundVal(dimension idim,bound ibound) const = 0;
// boundaries are defined by elements
  virtual const PacDetElem* boundElem(dimension idim,bound ibound) const =0;
// insert new elements into this set
  virtual void insert(const PacDetElem* elem);
protected:
  PacVoxelId _vid;
// detector elements in this voxel
  std::vector<const PacDetElem*> _delems;
};

struct PacVoxelInter {
  enum intertype {boundary=0,detelem};
  intertype _type;
  DetIntersection _dinter;
  HepPoint _pt; // exitpoint
  PacVoxel::dimension _idim; // dimension = 0,1, or 2
  PacVoxel::bound _ibnd; // boundary=lower or upper
};

std::ostream& operator << (std::ostream& stream, const PacVoxelId& vid);

#endif

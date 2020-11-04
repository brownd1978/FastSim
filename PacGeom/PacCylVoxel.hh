// ------------------------------------------------------------------------------
// File and Version Information:
// Class representing a single cylindrical voxel used in PacGeom
// Copyright Information:
//	Copyright (C) 2010	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 8/2/2010
// ------------------------------------------------------------------------------

#ifndef PacCylVoxel_HH
#define PacCylVoxel_HH

#include "PacGeom/PacVoxel.hh"
#include <iostream>

class PacCylVoxelSet;
class PacPlaneDetElem;
class PacCylDetElem;
class PacDetElem;

class PacCylVoxel : public PacVoxel {
public:
  enum cyldim {rho=0,zed,phi};
  static dimension cyltodim(cyldim cyld);
// Construct from the bounding r,z, and phi
  PacCylVoxel(double rbound[2],double zbound[2],double phibound[2],
    const PacCylDetElem* rsurf[2],
    const PacPlaneDetElem* zsurf[2],
    const PacPlaneDetElem* fsurf[2],
    PacVoxelId vid);
  PacCylVoxel(const PacCylVoxel& other);
  PacCylVoxel();
	virtual ~PacCylVoxel();
  PacCylVoxel& operator = (const PacCylVoxel& other);
// inherited interface
  virtual bool isInside(const HepPoint& point) const;
// test if a given value is inside the limits of a given dimension
  bool isInside(double value,dimension dim) const;
  bool isInside(const HepPoint& point,dimension dim) const;
  
  virtual bool findExit(const TrkGeomTraj* traj,double startflt, double& exitflt) const;  
  virtual bool findEntrance(const TrkGeomTraj* traj,double startflt, double& enterflt) const;

  const PacCylDetElem* rSurface(bound ibound) const { return _rsurf[ibound];}
  const PacPlaneDetElem* phiSurface(bound ibound) const { return _fsurf[ibound];}
  const PacPlaneDetElem* zSurface(bound ibound) const { return _zsurf[ibound];}
  
  double rBound(bound ibound) const { return _rbound[ibound];}
  double zBound(bound ibound) const { return _zbound[ibound];}
  double phiBound(bound ibound) const { return _fbound[ibound];}
  
  const PacCylDetElem* rElem(bound ibound) const { return _rsurf[ibound];}
  const PacPlaneDetElem* zElem(bound ibound) const { return _zsurf[ibound];}
  const PacPlaneDetElem* phiElem(bound ibound) const { return _fsurf[ibound];}
  
  virtual double boundVal(dimension idim,bound ibound) const;
  virtual const PacDetElem* boundElem(dimension idim,bound ibound) const;
  
// return the center of this voxel
  HepPoint center() const;
  
private:
// test sign of traj direction and surface normal dot product to decide if intersection is exit or not
  bool exits(double sign, dimension idim, bound ibound) const;
  bool enters(double sign, dimension idim, bound ibound) const;
// how far is a point from the this dimension's boundary: negative means it's inside  
  double bdist(const HepPoint& pos,dimension idim) const;
  void dval(const HepPoint& pos,dimension idim,double& value) const;
  double hiBound(dimension idim) const;
  double lowBound(dimension idim) const;
    
private:
// bounding values
  double _rbound[2];
  double _zbound[2];
  double _fbound[2];
// bounding surfaces
  const PacCylDetElem* _rsurf[2];
  const PacPlaneDetElem* _zsurf[2];
  const PacPlaneDetElem* _fsurf[2];
};

std::ostream& operator << (std::ostream& stream, const PacCylVoxel& voxel);

#endif

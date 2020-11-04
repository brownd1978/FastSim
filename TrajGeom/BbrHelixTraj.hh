// ----------------------------------------------------------------------------
// File and Version Information:
//      $Id: BbrHelixTraj.hh 524 2010-01-15 12:09:29Z stroili $
//
//  Description:
//  Simple geometric helix class -- not intended to describe tracks
//
//  Authors: Steve Schaffner
//-----------------------------------------------------------------------------
#ifndef BBRHELIXTRAJ_HH
#define BBRHELIXTRAJ_HH
#include "BbrGeom/TrkGeomTraj.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"

class BbrHelixTraj : public TrkGeomTraj
{
public:
  BbrHelixTraj(HepPoint origin,     //x-y of helix axis; z of midpoint of helix
	       double radius, 
	       double phiStart,     //phi in local coords of midpoint of helix
	       int whichDir,        // +1 => phi increases with path length
	       double tanDip, 
	       double startRange,   // pathlength of start of traj
	       double endRange);    
  BbrHelixTraj(const BbrHelixTraj&);   // copy ctor
  ~BbrHelixTraj();

  virtual BbrHelixTraj* clone() const;

  BbrHelixTraj& operator=(const BbrHelixTraj&);

  HepPoint   position(double fltLen)   const;
  Hep3Vector direction(double fltLen)  const;
  double     curvature(double fltLen)  const;
  Hep3Vector delDirect(double fltLen)  const;
  void       getInfo(double fltLen, 
		     HepPoint& position, 
		     Hep3Vector& direction) const;
  void       getInfo(double fltLen, 
		     HepPoint& position, 
		     Hep3Vector& direction, 
		     Hep3Vector& delDirect) const;

  virtual double distTo1stError(double s, double tol, int pathDir) const;
  virtual double distTo2ndError(double s, double tol, int pathDir) const;

  // Support Visitor pattern (see TrkGeomTraj.hh)
  void accept(TrkGeomTrajVisitor& visitor) const;

  double phiStart() const {return _phiStart;}
  const HepPoint& origin() const                   {return _origin;}
  double omega() const                             {return _omega;}
  double tanDip() const                            {return _tanDip;}

protected:
  double cosDip() const; 
  double sinDip() const;
  double tanAngle(double flt) const;     // angle of tangent (local coord)
  double phi0() const;                   // in local coordinate system
  double xOff() const;
  double yOff() const;

// Data Members
private:
  HepPoint _origin;  // x-y of helix center; z of flt=0
  double _omega;     // > 0. => increasing phi
  double _tanDip;
  double _phiStart;      // direction of tangent at flt=0, in local coords
};
#endif

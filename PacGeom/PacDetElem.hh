/* Class: PacDetElem
 *
 * Base class for elements of PacDetector. */

#ifndef PacDetElem_HH
#define PacDetElem_HH

#include "CLHEP/Vector/ThreeVector.h"
using CLHEP::Hep3Vector;
#include <vector>
class PacMeasurement;
class HepPoint;
class TrkGeomTraj;
class DetIntersection;
class DetSurfaceElem;

class PacDetElem {
public:
// define the type of surface used to describe this element
  enum SurfaceType { cylinder=0, plane=1, cone=2};
// provide basic parameters plus measurements on construction
	PacDetElem(SurfaceType surf, const std::vector<const PacMeasurement*>& mdevs, double gap, double overlap );
  const std::vector<const PacMeasurement*>& measurementDevices() const { return _mdevs;}
	virtual ~PacDetElem() = 0;
// is this an active or passive element?
	bool active() const { return _mdevs.size() != 0;}
// specify the activity of a given point, default implementaiton is same as active
	virtual bool activeRegion(const HepPoint& pos) const;
// elements can have gaps and overlaps
  double gapFraction() const { return _gap;}
  double overlapFraction() const { return _over;}
// as gaps and overlaps are optional, allow setting these
  void setGapFraction(double gap) { _gap = gap;}
  void setOverlapFraction(double over) { _over = over;}
// intersection with full information
  virtual bool intersect( const TrkGeomTraj* traj, DetIntersection& dinter) const;
// intersection with just flightlength
  virtual bool intersect( const TrkGeomTraj* traj, double& fltlen) const = 0;
  virtual void applyZShift(double zshift) = 0;
// distance and normal direction to a point at closest approach to the surface
  virtual double normTo(const HepPoint& point,Hep3Vector& norm) const =0;
// return as a DetElem
  virtual const DetSurfaceElem* surfElem() const = 0;
// thicnkess
  virtual double thickness() const = 0;
// fill detector intersection after an intersection
  void fillDetInter(const TrkGeomTraj* traj,DetIntersection& dinter,double fltlen) const;
// estimated transverse (in-plane) size of this element
  virtual double estimatedSize() const = 0;
// curvature.  This is 1/rho
  virtual double curvature(const HepPoint& pos) const = 0;
// surface type
  SurfaceType surfaceType() const { return _surf;}
protected:
  SurfaceType _surf;
	std::vector<const PacMeasurement*> _mdevs;
  double _gap;
  double _over;
  static const double _mindpath; // minimum change in path length
  static const double _mindotp; // minimum intersection angle cosine for linear approximat
};

#endif

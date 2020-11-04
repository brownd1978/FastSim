// ----------------------------------------------------------------------------
// File and Version Information:
//      $Id: BbrHelixTraj.cc 524 2010-01-15 12:09:29Z stroili $
//
//  Authors: Steve Schaffner
//-----------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "CLHEP/Matrix/Matrix.h"
#include "TrajGeom/BbrHelixTraj.hh"
#include "BbrGeom/TrkGeomTrajVisitor.hh"
#include <assert.h>
#include "BaBar/Constants.hh"
#include "BbrGeom/BbrAngle.hh"

BbrHelixTraj::BbrHelixTraj(HepPoint orig,
                           double radius,
                           double phSt,
                           int whichDir,
                           double tD,
                           double startRange,
                           double endRange)
  :  TrkGeomTraj(startRange, endRange),
     _origin(orig),
     _tanDip(tD),
     _phiStart(phSt)
{
  assert(0. != radius);
  _omega = whichDir * 1./radius;
}

BbrHelixTraj::BbrHelixTraj(const BbrHelixTraj& other) :
  TrkGeomTraj(other.lowRange(),other.hiRange()),
  _origin(other._origin),
  _omega(other._omega), 
  _tanDip(other._tanDip), 
  _phiStart(other._phiStart)
{
}

BbrHelixTraj::~BbrHelixTraj()
{
}

BbrHelixTraj& BbrHelixTraj::operator = (const BbrHelixTraj& other)
{
  if(&other != this){
    Trajectory::operator=(other);
    _tanDip = other._tanDip;
    _omega = other._omega;
    _phiStart = other._phiStart;
    _origin = other._origin;
  }
  return *this;
}

BbrHelixTraj*
BbrHelixTraj::clone() const
{
  return new BbrHelixTraj(*this);
}

HepPoint
BbrHelixTraj::position(double fltLen) const
{
  double ang = tanAngle(fltLen);
  double cang = cos(ang);
  double sang = sin(ang);
  double sphi0 = sin(phi0());
  double cphi0 = cos(phi0());
  double xt = (sang - sphi0)/omega() + xOff();
  double yt = -(cang - cphi0)/omega() + yOff();
  double zt = fltLen*sinDip() + origin().z();
  return HepPoint(xt, yt, zt);
}

Hep3Vector
BbrHelixTraj::direction(double f) const
{
  double alpha = tanAngle( f );
  // Construct 3-D tangent vector of unit magnitude.
  double cDip = cosDip();
  return Hep3Vector ( cos(alpha)*cDip,
                      sin(alpha)*cDip,
                      sinDip() );
}

Hep3Vector
BbrHelixTraj::delDirect( double fltLen) const
{
  double cDip = cosDip();
  double delX = -omega() * cDip * cDip * sin(tanAngle(fltLen));
  double delY =  omega() * cDip * cDip * cos(tanAngle(fltLen));
  return Hep3Vector(delX, delY, 0);
}

double
BbrHelixTraj::distTo1stError(double , double tol, int ) const
{
  double sec = cosDip();
  sec = (sec != 0.) ? 1./sec : 9.99e9;
  double arg = 2. * tol / fabs(omega()) * sec * sec;
  assert (arg >= 0.);
  return sqrt(arg);
}

double
BbrHelixTraj::distTo2ndError(double, double tol, int ) const
{
  double sec = cosDip();
  sec = (sec != 0.) ? 1./sec : 9.99e9;
  return sec * pow(6.*tol / (omega()*omega()), 0.33333333);
}

double
BbrHelixTraj::curvature( double ) const
{
  double cosd = cosDip();
  return cosd*cosd*fabs(omega());
}

void
BbrHelixTraj::getInfo(double fltLen, HepPoint& pos, Hep3Vector& dir) const
{
  double ang = tanAngle(fltLen);
  double cang = cos(ang);
  double sang = sin(ang);
  double sphi0 = sin(phi0());
  double cphi0 = cos(phi0());
  double cDip = cosDip();
  double sDip = tanDip() * cDip;

  double xt = (sang - sphi0)/omega() + xOff();
  double yt = -(cang - cphi0)/omega() + yOff();
  double zt = fltLen*sinDip() + origin().z();
  pos = HepPoint(xt, yt, zt);

  dir = Hep3Vector( cang * cDip,
                    sang * cDip,
                    sDip );
}

void
BbrHelixTraj::getInfo(double fltLen, HepPoint& pos, Hep3Vector& dir,
                                     Hep3Vector& delDir) const
{
  double ang = tanAngle(fltLen);
  double cang = cos(ang);
  double sang = sin(ang);
  double sphi0 = sin(phi0());
  double cphi0 = cos(phi0());
  double cDip = cosDip();
  double sDip = tanDip() * cDip;

  double xt = (sang - sphi0)/omega() + xOff();
  double yt = -(cang - cphi0)/omega() + yOff();
  double zt = fltLen*sinDip() + origin().z();
  pos = HepPoint(xt, yt, zt);

  dir = Hep3Vector( cang * cDip,
                    sang * cDip,
                    sDip );

  double delX = -omega() * cDip * cDip * sang;
  double delY =  omega() * cDip * cDip * cang;
  delDir = Hep3Vector(delX, delY, 0.0);

}

void
BbrHelixTraj::accept(TrkGeomTrajVisitor& visitor) const
{
  //  visitor.visitHelix(this);
}

double
BbrHelixTraj::cosDip() const
{
  return 1./sqrt( 1.+tanDip()*tanDip() );
}

double
BbrHelixTraj::sinDip() const
{
  return tanDip() * cosDip();
}

double
BbrHelixTraj::tanAngle(double flt) const
{
  return BbrAngle(phi0() + omega() * cosDip() * flt);
}

double
BbrHelixTraj::phi0() const
{
  int whichDir = (omega() > 0.) ? 1 : 1;  // FIXME!!!
  return phiStart() + whichDir * 0.5 * Constants::pi;
}

double
BbrHelixTraj::xOff() const
{
  return cos(phiStart())/omega() + origin().x();
}

double
BbrHelixTraj::yOff() const
{
  return sin(phiStart())/omega() + origin().y();
}

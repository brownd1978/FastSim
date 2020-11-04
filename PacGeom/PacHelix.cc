// $Id: PacHelix.cc,v 1.7 2004/09/30 13:39:06 hulsberg Exp $

#include "BaBar/BaBar.hh"
#include <assert.h>
#include <math.h>
#include <limits.h>

#include "BaBar/Constants.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Utilities/CLHEP.h"
#include "PacGeom/PacHelix.hh"
#include "BbrGeom/TrkGeomTrajVisitor.hh"
#include "BbrGeom/BbrAngle.hh"
#include "BField/BField.hh"
#include <assert.h>
using std::endl;
using std::ostream;
//  Fix for some machines
//
#ifndef M_2PI
#define M_2PI 2*M_PI
#endif

PacHelix::PacHelix(double pars[nHelixPars],double lowlim, double hilim) : 
  TrkGeomTraj(lowlim,hilim), _mom(0.0), _bnom(0.0)
{
	for(int ipar=0;ipar<nHelixPars;ipar++){
		_pars[ipar]=pars[ipar];
	}
  setCache();
}

PacHelix::PacHelix(const HepVector& pars,double lowlim, double hilim) :
  TrkGeomTraj(lowlim,hilim), _mom(0.0), _bnom(0.0)
{
	assert(pars.num_row() == nHelixPars);
	for(int ipar=0;ipar<nHelixPars;ipar++){
		_pars[ipar]=pars[ipar];
	}
  setCache();
}


PacHelix::PacHelix( const PacHelix& other )
	: TrkGeomTraj(other.lowRange(),other.hiRange()), _mom(other._mom), _bnom(other._bnom)
{
	for(int ipar=0;ipar<nHelixPars;ipar++){
		_pars[ipar]=other._pars[ipar];
	}
  setCache();
}

PacHelix*
PacHelix::clone() const
{
  return new PacHelix(*this);
}

PacHelix&
PacHelix::operator=(const PacHelix& other)
{
  if( &other != this ){
    _mom = other._mom;
    _bnom = other._bnom;
    Trajectory::operator=(other);
		for(int ipar=0;ipar<nHelixPars;ipar++){
			_pars[ipar]=other._pars[ipar];
		}
  }
  setCache();
  return *this;
}

PacHelix::~PacHelix()
{
}

void
PacHelix::setCache() {
// set values for commonly-accessed immutable variables
  _cosdip = 1./sqrt(1.+sqr(tanDip()));
  _dip =  atan(tanDip());
  _sindip = tanDip()*cosDip();
  _sinphi0 = sin(phi0());
  _cosphi0 = cos(phi0());
}

HepPoint
PacHelix::position( double f) const
{
  double cDip = cosDip();
  double sDip = tanDip() * cDip;
  double ang = phi0() + cDip*f*omega();
  double cang = cos(ang);
  double sang = sin(ang);

  return HepPoint((sang - _sinphi0)/omega() - d0()*_sinphi0,
                 -(cang - _cosphi0)/omega() + d0()*_cosphi0,
                 z0() + f*sDip                       );
}

Hep3Vector
PacHelix::direction( double f) const
{
  // Angle formed by tangent vector after
  // being rotated 'arclength' around orbit.
  double alpha = angle( f );
  // Construct 3-D tangent vector of unit magnitude.
  double cDip = cosDip();
  return Hep3Vector ( cos(alpha)*cDip,
                      sin(alpha)*cDip,
                      cDip*tanDip() );
}

Hep3Vector
PacHelix::delDirect( double fltLen ) const
{
  double ang = angle(fltLen);
  double cDip = cosDip();
  double delX = -omega() * cDip * cDip * sin(ang);
  double delY =  omega() * cDip * cDip * cos(ang);
  return Hep3Vector(delX, delY, 0.0);
}

double
PacHelix::distTo1stError(double s, double tol, int pathDir) const
{
  return sqrt(2.*tol/fabs(omega())*(1.+sqr(tanDip())));
}

double
PacHelix::distTo2ndError(double s, double tol, int pathDir) const
{
  return sqrt(1.+sqr(tanDip()))*cbrt(6.*tol/sqr(omega()));
}

void
PacHelix::getInfo(double fltLen, HepPoint& pos, Hep3Vector& dir,
                   Hep3Vector& delDir) const
{
  //  double ang = angle(fltLen);
  double cDip = cosDip();
  double sDip = tanDip() * cDip;
  double ang = phi0() + cDip*fltLen*omega(); 
  double cang = cos(ang);
  double sang = sin(ang);

	double xt = (sang - _sinphi0)/omega() - d0()*_sinphi0;
	double yt = -(cang - _cosphi0)/omega() + d0()*_cosphi0;
	double zt = z0() + fltLen*sDip;
  pos.setX(xt);
  pos.setY(yt);
  pos.setZ(zt);

  dir.setX(cang * cDip);
  dir.setY(sang * cDip);
  dir.setZ(sDip);

  double delX = -omega() * cDip * cDip * sang;
  double delY =  omega() * cDip * cDip * cang;
  delDir.setX(delX);
  delDir.setY(delY);
  delDir.setZ(0.0);
}

void
PacHelix::getInfo( double fltLen, HepPoint& pos, Hep3Vector& dir ) const
{
  //  double ang = angle(fltLen);
  double cDip = cosDip();
  double sDip = tanDip() * cDip;
  double ang = phi0() + cDip*fltLen*omega(); 
  double cang = cos(ang);
  double sang = sin(ang);

	double xt = (sang - _sinphi0)/omega() - d0()*_sinphi0;
	double yt = -(cang - _cosphi0)/omega() + d0()*_cosphi0;
	double zt = z0() + fltLen*sDip;
  pos.setX(xt);
  pos.setY(yt);
  pos.setZ(zt);

  dir.setX(cang * cDip);
  dir.setY(sang * cDip);
  dir.setZ(sDip);
}

double
PacHelix::curvature(double ) const
{
//  Compute the curvature as the magnitude of the 2nd derivative
//  of the position function with respect to the 3-d flight distance
//
  double cosd = cosDip();
  return sqr(cosd)*fabs(omega());
}

void
PacHelix::accept(TrkGeomTrajVisitor& vis) const
{
}


double
PacHelix::angle(const double& f) const
{
  return BbrAngle(phi0() + arc(f));
}

HepVector
PacHelix::parameters() const {
	HepVector retval(nHelixPars);
	retval[d0Index] = d0();
	retval[phi0Index] = phi0();
	retval[omegaIndex] = omega();
	retval[z0Index] = z0();
	retval[tanDipIndex] = tanDip();
	return retval;
}


void
PacHelix::printAll(ostream& os) const
{
  os  << "PacHelix with range "
      << lowRange() <<" to " << hiRange() << " and parameters " << endl
      << "d0= " << d0() << " phi0= "
      << phi0() << " omega= "
      << omega() << " z0 = "
      << z0() << " tanDip= "
      << tanDip() << endl;
}

void
PacHelix::print(ostream& os) const
{
  Trajectory::print(os << "PacHelix" );
}

void
PacHelix::setReference(double bnom, double mom) {
  _mom = mom;
  _bnom = bnom;
}

double
PacHelix::charge() const {
  if(_bnom != 0){
    return _bnom*omega() > 0 ? -1.0 : 1.0;
  } else
    return 0.0;
}

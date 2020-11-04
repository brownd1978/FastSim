//--------------------------------------------------------------------------
// Name:
//   PacHelix
// Description:
//   Copy of TrkBase/Helix, but removing all the 'diff' stuff.
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#ifndef PacHelix_HH
#define PacHelix_HH
#include "BbrGeom/TrkGeomTraj.hh"
#include "CLHEP/Utilities/CLHEP.h"

class TrkVisitor;

class PacHelix : public TrkGeomTraj {
public:
//  Define the parameters 
	enum ParIndex {d0Index=0, phi0Index, omegaIndex, z0Index, tanDipIndex,nHelixPars};
  PacHelix(double pars[nHelixPars], double lowlim=-99999.,double hilim=99999.);
  PacHelix(const HepVector& pars, double lowlim=-99999.,double hilim=99999.);
  PacHelix( const PacHelix&  );  
  PacHelix* clone() const;
  virtual ~PacHelix();

// operators
  PacHelix& operator=(const PacHelix&);
// accessors
  virtual HepPoint   position(double fltLen)  const;
  virtual Hep3Vector direction(double fltLen) const;
  virtual Hep3Vector delDirect(double)        const;
  virtual void       getInfo(double fltLen, HepPoint& pos, 
			     Hep3Vector& dir) const;
  virtual void       getInfo(double fltLen, HepPoint& , Hep3Vector& dir, 
			     Hep3Vector& delDir) const;

  // How far can you go using given approximation before error > tolerance, 
  //   in direction pathDir?
  virtual double distTo1stError(double s, double tol, int pathDir) const;
  virtual double distTo2ndError(double s, double tol, int pathDir) const;

//  Real versions of the base class derrivative functions
	double curvature( double ) const;
  // Helix-specific accessors
	double d0() const {return _pars[d0Index];}
	double phi0() const { return _pars[phi0Index];}
	double omega() const {return _pars[omegaIndex]; }
	double z0() const {return _pars[z0Index]; }
	double tanDip() const {  return _pars[tanDipIndex]; }
	const double* params() const { return _pars; }
	HepVector parameters() const;
// efficient access of 1 coordinate
	double z(const double& fltlen) const { return z0() + fltlen*sinDip();  }
  //--------------------------------------------------
  // Visitor access
  //--------------------------------------------------

  virtual void accept(TrkGeomTrajVisitor& ) const;

  virtual void               print(std::ostream& os) const;
  virtual                    void printAll(std::ostream& os) const;

  double dip() const {return _dip;}
  double cosDip() const {return _cosdip; }
  double sinDip() const {return _sindip; }
  double translen(const double& f) const {return cosDip()*f;}
  double arc( const double& f) const {return translen(f)*omega();}
  double angle(const double& f) const;
// field stuff
  double bnom() const { return _bnom;}
  double mom() const { return _mom; }
  void setReference(double bnom, double mom);
// charge can be computed knowing the sign of Bz and the curvature
  double charge() const;
private:
  void setCache();
  
	double _pars[nHelixPars];
// record the nominal (Bz) field and the momentum magnitude
  double _mom;
	double _bnom;
  
// cache some variables for efficiency
  mutable double _cosdip;
  mutable double _sindip;
  mutable double _dip;
  mutable double _sinphi0;
  mutable double _cosphi0;
};
#endif


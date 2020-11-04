//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkCircleTraj.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//    Trajectory subclass that implements a circle in the x-y plane, suitable
//    for use in a track.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//------------------------------------------------------------------------
#ifndef TRKCIRCLETRAJ_HH
#define TRKCIRCLETRAJ_HH
#include "TrkBase/TrkSimpTraj.hh"

class TrkVisitor;
#include "CLHEP/Matrix/Vector.h"
class TrkExchangePar;

class TrkCircleTraj : public TrkSimpTraj
{

public:
  TrkCircleTraj(const HepVector&,const HepSymMatrix&, double lowlim=-99999.,
	    double hilim=99999., const HepPoint& refpoint = _theOrigin);
  TrkCircleTraj(const TrkExchangePar&, double lowlim=-99999.,
	    double hilim=99999., const HepPoint& refpoint = _theOrigin);
  TrkCircleTraj( const TrkCircleTraj&  );   // copy ctor
  TrkCircleTraj* clone() const;
  ~TrkCircleTraj();
  TrkCircleTraj& operator=(const TrkCircleTraj&);


  virtual HepPoint   position(double fltLen) const;
  virtual Hep3Vector direction(double fltLen) const;
  virtual Hep3Vector delDirect(double) const;
  virtual void       getInfo(double fltLen, HepPoint& pos, 
			      Hep3Vector& dir) const;
  virtual void       getInfo(double fltLen, HepPoint&, Hep3Vector& dir, 
			Hep3Vector& delDir) const;
  virtual void       getDFInfo(double fltLen, DifPoint&, DifVector& dir, 
			DifVector& delDir) const;
  virtual void       getDFInfo2(double fltLen, DifPoint& pos, DifVector& 
			       dir) const;
  virtual double     curvature( double fltLen) const;

  // How far can you go using given approximation before error > tolerance, 
  //   in direction pathDir?
  virtual double distTo1stError(double flt, double tol, int pathDir) const;
  virtual double distTo2ndError(double flt, double tol, int pathDir) const;

//  Real versions of the base class derivative functions
  HepMatrix derivDeflect(double fltlen,deflectDirection) const;
  HepMatrix derivDisplace(double fltlen,deflectDirection) const;
  HepMatrix derivPFract(double fltlen) const;
//  PointTraj functions
  TranslateParams paramFunction() const { return TrkCircleTraj::paramFunc; }
  // Invert the parameter.  Returns true in flags if the inversion
  // requires a change of sign in the covariance matrix.
  void invertParams(TrkParams* params, std::vector<bool>& flags) const;

  //--------------------------------------------------
  // Visitor access
  //--------------------------------------------------

  virtual void visitAccept(TrkVisitor* vis) const;

//  Define the parameters 
private:
  enum {d0Ind=0, phi0Ind, omegaInd}; 
  enum {NCIRPAR = 3};
public:
  static int d0Index()               {return d0Ind;}
  static int phi0Index()             {return phi0Ind;}
  static int omegaIndex()            {return omegaInd;}
  static int nCirPrm()               {return NCIRPAR;}

  // circle-specific accessors
  double d0() const {return parameters()->parameter()[d0Index()];}
  double phi0() const;
  double omega() const {return parameters()->parameter()[omegaIndex()]; }


private:
//  Private functions (data members are part of the base class)

  double x( const double& ) const;
  double y( const double& ) const;
  inline double arc( const double& f) const {return f*omega();}
         double angle(const double& f) const; 
// the real point translation function
  static void paramFunc(const HepPoint& oldpoint,const HepPoint& newpoint,
			const HepVector& oldpar,const HepSymMatrix& oldcov,
			HepVector& newpar,HepSymMatrix& newcov,
			double fltlen);
};
#endif





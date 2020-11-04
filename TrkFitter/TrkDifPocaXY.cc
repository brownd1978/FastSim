//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkDifPocaXY.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     
//       Poca of TrkDifTraj wrt a point or  between two TrkDifTraj's in
//       the XY plane
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Selenia Dittongo             Univ. Ferrara
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "TrkFitter/TrkDifPocaXY.hh"
#include "TrkFitter/TrkParVisitor.hh"
#include "TrkBase/TrkMomVisitor.hh"
#include "TrajGeom/TrkLineTraj.hh"

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Geometry/HepPoint.h"

#include "TrkBase/TrkErrCode.hh"
#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkPoca.hh"

#include "difAlgebra/DifNumber.hh"

#include <iostream>
#include <math.h>

// Constructor -- traj and traj
//------------------------------------------------------------------------
TrkDifPocaXY::TrkDifPocaXY(const TrkDifTraj& traj1, const double& flt1,
			   const TrkDifTraj& traj2, const double& flt2)
//------------------------------------------------------------------------
{
  //The local trajectories for the two trajectories
   
  double locFltl1, locFltl2;  

  const TrkSimpTraj* locTraj1 = traj1.localTrajectory(flt1, locFltl1);
  const TrkSimpTraj* locTraj2 = traj2.localTrajectory(flt2, locFltl2);

  //The parameters for the two trajectories

  TrkParVisitor pVisitor1(*locTraj1), pVisitor2(*locTraj2);

  const TrkExchangePar par1 = pVisitor1.parameters();
  const TrkExchangePar par2 = pVisitor2.parameters();

  //What kind of trajectory?

  TrkMomVisitor mVis1(*locTraj1), mVis2(*locTraj2);

  if ((mVis1.helix() || mVis1.circle()) &&
      (mVis2.helix() || mVis2.circle())) {// in the xy plane, both 
                                          // trajectories are circles
    pocaTwoCircles(par1, par2);
  }	

  if ((mVis1.helix() || mVis1.circle()) &&
      (mVis2.neut() || mVis2.line())) {// in the xy plane, the first 
                                         // trajectory is a circle and the 
                                         // second is a line
     pocaLineCircle(par2, par1);
  }	

  if ((mVis1.neut() || mVis1.line()) &&
      (mVis2.helix() || mVis2.circle())) {// in the xy plane, the first 
                                            // trajectory is a line and the
                                            // second is a line
    pocaLineCircle(par1, par2);
  }	

  if ((mVis1.neut() || mVis1.line()) &&
      (mVis2.neut() || mVis2.line())) {// in the xy plane, both trajectories
                                         // are lines
    pocaTwoLines(par1, par2);
  }	

  // Line going trhough the the poca for the two trajecories and parallel to 
  // z axis

  HepPoint thePoint11(_xpoca1.number(),_ypoca1.number(),-200.);
  HepPoint thePoint12(_xpoca1.number(),_ypoca1.number(),200.);

  TrkLineTraj theLine1(thePoint11, thePoint12);

  HepPoint thePoint21(_xpoca2.number(),_ypoca2.number(),-200.);
  HepPoint thePoint22(_xpoca2.number(),_ypoca2.number(),200.);

  TrkLineTraj theLine2(thePoint21, thePoint22);
  
  TrkPoca firstPoca3D(traj1, flt1, theLine1, 200.);
  TrkPoca secondPoca3D(traj2, flt2, theLine2, 200.);
  
  _fltl1 = firstPoca3D.flt1();
  _fltl2 = secondPoca3D.flt1();
  
  return;

}


// Constructor -- traj and point
//------------------------------------------------------------------------
TrkDifPocaXY::TrkDifPocaXY(const TrkDifTraj& traj, const double& fltl, 
                     const HepPoint& pt)
  //------------------------------------------------------------------------
{
  //The local trajectory for the DifTrajectory
   
  double locFltl;  
  const TrkSimpTraj* locTraj = traj.localTrajectory(fltl, locFltl);

  //The parameters for the trajectory

  TrkParVisitor pVisitor(*locTraj);
  const TrkExchangePar par = pVisitor.parameters();

  // The poca in the plane xy

  pocaTrajPoint(par,pt);
  	
  // Line going trhough the the poca and parallel to z axis

  HepPoint thePoint1(_xpoca1.number(),_ypoca1.number(),-200.);
  HepPoint thePoint2(_xpoca1.number(),_ypoca1.number(),200.);

  TrkLineTraj theLine(thePoint1, thePoint2);
  
  TrkPoca poca3D(traj, fltl, theLine, 200.);
  
  _fltl1 = poca3D.flt1();
  
  return;
}

 
//------------------------------------------------------------------------
void TrkDifPocaXY::pocaTwoCircles
(const TrkExchangePar& par1, const TrkExchangePar& par2) {
  //------------------------------------------------------------------------

  DifNumber d1(par1.d0(),TrkExchangePar::ex_d0+1,TrkExchangePar::nParam);
  DifNumber phi01(par1.phi0(),TrkExchangePar::ex_phi0+1,TrkExchangePar::nParam);
  DifNumber omega1(par1.omega(),TrkExchangePar::ex_omega+1,TrkExchangePar::nParam);
  DifIndepPar p1(params(par1));
  d1.setIndepPar(&p1);
  phi01.setIndepPar(&p1);
  omega1.setIndepPar(&p1);
  // parameters of the two circles
  DifNumber d2(par2.d0(),TrkExchangePar::ex_d0+1,TrkExchangePar::nParam);
  DifNumber phi02(par2.phi0(),TrkExchangePar::ex_phi0+1,TrkExchangePar::nParam);
  DifNumber omega2(par2.omega(),TrkExchangePar::ex_omega+1,TrkExchangePar::nParam);
  DifIndepPar p2(params(par2));
  d2.setIndepPar(&p2);
  phi02.setIndepPar(&p2);
  omega2.setIndepPar(&p2);


  //coordinates of the centers for the two circles

  DifNumber xc1 = -(d1+1/omega1)*sin(phi01);
  DifNumber yc1 = (d1+1/omega1)*cos(phi01);

  DifNumber xc2 = -(d2+1/omega2)*sin(phi02);
  DifNumber yc2 = (d2+1/omega2)*cos(phi02);

  //distance between the centers of the two circles

  DifNumber cdist = sqrt((xc1-xc2)*(xc1-xc2)+(yc1-yc2)*(yc1-yc2));
  DifNumber r1 = fabs(1/omega1);//fabsolute radius
  DifNumber r2 = fabs(1/omega2);

  // there are 4 possible cases:
  
  // a- coincident centers

  if (fabs(cdist) < 1.e-12 ) {   
    //the algorithm fails because the points have all
    //the same distance
    _status.setFailure(12, "TrkPocaXY:: the two circles have the same center...");
    return;
  }

  // b - intersection
 
  if ( (fabs(r1-r2) < cdist) && (cdist < r1+r2 ) ) { 

     DifNumber xint1, xint2, yint1, yint2;

     interTwoCircles(xc1,yc1,r1,xc2,yc2,r2,xint1,yint1,xint2,yint2);

     // There are two intersection points; in this case, I choose 
     // the point on the tracks the closest to the origin in the xy plane

     DifNumber dist1 = xint1*xint1 + yint1*yint1;  
     DifNumber dist2 = xint2*xint2 + yint2*yint2;

     if (dist1<dist2) {
       _xpoca1 = xint1;
       _ypoca1 = yint1;
       _xpoca2 = _xpoca1;
       _ypoca2 = _ypoca1;

     }
     else {
       _xpoca1 = xint2;
       _ypoca1 = yint2;
       _xpoca2 = _xpoca1;
       _ypoca2 = _ypoca1;
     }  

     _docaxy = sqrt((_xpoca1-_xpoca2)*(_xpoca1-_xpoca2) + 
                    (_ypoca1-_ypoca2)*(_ypoca1-_ypoca2));
  }

  // c - nested circles; d - separated circles

  if ( (fabs(r1-r2) > cdist) ||  // nested circles
       ( cdist > (r1+r2) )) {    // separated circles

    // line going through the centers of the two circles

    DifNumber m = (yc1-yc2)/(xc1-xc2);  // y = m*x+q
    DifNumber q = yc1 - m*xc1;

    // intersection points between the line and the two circles

    DifNumber xint1, yint1, xint2, yint2;
    interLineCircle(m, q, xc1, yc1, r1, xint1, yint1, xint2, yint2);

    DifNumber xint3, yint3, xint4, yint4;
    interLineCircle(m, q, xc2, yc2, r2, xint3, yint3, xint4, yint4);

    if (fabs(r1-r2) > cdist) { // nested circles
      DifNumber absdoca = fabs(r1-r2)-cdist;

      if (omega1*omega2>0) _docaxy = -absdoca;
      if (omega1*omega2<0) _docaxy = absdoca;

      DifNumber dist1_3 = pow((xint1-xint3),2.) + pow((yint1-yint3),2.);
      DifNumber dist2_4 = pow((xint2-xint4),2.) + pow((yint2-yint4),2.);

      if (dist1_3<dist2_4){
        _xpoca1 = xint1;
        _ypoca1 = yint1;
        _xpoca2 = xint3;
        _ypoca2 = yint3;
      }
      else {
        _xpoca1 = xint2;
        _ypoca1 = yint2;
        _xpoca2 = xint4;
        _ypoca2 = yint4;
      }
 
    }
    if( cdist > (r1+r2) ) { //separated circles
      DifNumber absdoca = cdist - (r1+r2);

      if (omega1*omega2>0) _docaxy = -absdoca;
      if (omega1*omega2<0) _docaxy = absdoca;

      DifNumber dist2_3 = pow((xint2-xint3),2.) + pow((yint2-yint3),2.);
      DifNumber dist1_4 = pow((xint1-xint4),2.) + pow((yint1-yint4),2.);

      if (dist2_3<dist1_4){
        _xpoca1 = xint2;
        _ypoca1 = yint2;
        _xpoca2 = xint3;
        _ypoca2 = yint3;
      }
      else {
        _xpoca1 = xint1;
        _ypoca1 = yint1;
        _xpoca2 = xint4;
        _ypoca2 = yint4;
      }

    }
  }

return;

}

//------------------------------------------------------------------------
void TrkDifPocaXY::pocaLineCircle
(const TrkExchangePar& par1, const TrkExchangePar& par2) {
//------------------------------------------------------------------------

  DifNumber d1(par1.d0(),TrkExchangePar::ex_d0+1,TrkExchangePar::nParam);
  DifNumber phi01(par1.phi0(),TrkExchangePar::ex_phi0+1,TrkExchangePar::nParam);
  DifIndepPar p1(params(par1));
  d1.setIndepPar(&p1);
  phi01.setIndepPar(&p1);

  DifNumber d2(par2.d0(),TrkExchangePar::ex_d0+1,TrkExchangePar::nParam);
  DifNumber phi02(par2.phi0(),TrkExchangePar::ex_phi0+1,TrkExchangePar::nParam);
  DifNumber omega2(par2.omega(),TrkExchangePar::ex_omega+1,TrkExchangePar::nParam);
  
  DifIndepPar p(params(par2));
  d2.setIndepPar(&p);
  phi02.setIndepPar(&p);
  omega2.setIndepPar(&p);
  
  DifNumber radius = fabs(1/omega2);

  //coordinates of the circle center

  DifNumber xc = -(d2+1/omega2)*sin(phi02);
  DifNumber yc = (d2+1/omega2)*cos(phi02);

  // parameters for the line (traj1) in the xy plane

  DifNumber m = tan(phi01);       //    y = m*x + q
  DifNumber q = d1 / cos(phi01);  

  // distance between the line and the circle center

  DifNumber dist = fabs((m*xc-yc+q)/sqrt(1+m*m));

  // there are 3 different cases

  if (dist <= radius) { // 2 or 1 intersection points

    // the intersections points

    DifNumber xint1,yint1,xint2,yint2;
    interLineCircle(m, q, xc, yc, radius, xint1, yint1, xint2, yint2);

    // selection of the intersection point the closest to the origin in the 
    // xy plane

    _docaxy = sqrt((xint1-xint2)*(xint1-xint2) +
                   (yint1-yint2)*(yint1-yint2));

    DifNumber dist1 = xint1*xint1 + yint1*yint1;   
    DifNumber dist2 = xint2*xint2 + yint2*yint2; 

    if (dist1<dist2) {
      _xpoca1 = xint1;
      _ypoca1 = yint1;
      _xpoca2 = _xpoca1;
      _ypoca2 = _ypoca1;
    }
    else {
      _xpoca1 = xint2;
      _ypoca1 = yint2;
      _xpoca2 = _xpoca1;
      _ypoca2 = _ypoca1;
    }  

  }
  else { // no intersection points
    
    // line going through the circle center and perpendicular to traj1

    DifNumber mperp = -1./m;
    DifNumber qperp = yc - mperp*xc;

    // intersection between this line and the two trajectories

    DifNumber xint1,yint1,xint2,yint2;
    interLineCircle(mperp, qperp, xc, yc, radius, xint1, yint1, xint2, yint2);

    DifNumber xint3,yint3;
    interTwoLines(m, q, mperp, qperp, xint3, yint3);

    _xpoca1 = xint3;
    _ypoca1 = yint3;

    DifNumber dist1_3 = pow((xint1-xint3),2.) + pow((yint1-yint3),2.);
    DifNumber dist2_3 = pow((xint2-xint3),2.) + pow((yint2-yint3),2.);

    if (dist1_3<dist2_3) {
      _xpoca2 = xint1;
      _ypoca2 = yint1;
    }
    else {
      _xpoca2 = xint2;
      _ypoca2 = yint2;
    }
 
    DifNumber absdoca = sqrt((_xpoca1-_xpoca2)*(_xpoca1-_xpoca2)+
                          (_ypoca1-_ypoca2)*(_ypoca1-_ypoca2));

    if (d1*d2>0) _docaxy = absdoca;
    if (d1*d2<0) _docaxy = -absdoca;
  }

return;
}

//------------------------------------------------------------------------
void TrkDifPocaXY::pocaTwoLines
(const TrkExchangePar& par1, const TrkExchangePar& par2) {
  //------------------------------------------------------------------------

  DifNumber d1(par1.d0(),TrkExchangePar::ex_d0+1,TrkExchangePar::nParam);
  DifNumber phi01(par1.phi0(),TrkExchangePar::ex_phi0+1,TrkExchangePar::nParam);

  
  DifIndepPar p1(params(par1));
  d1.setIndepPar(&p1);
  phi01.setIndepPar(&p1);

  DifNumber d2(par2.d0(),TrkExchangePar::ex_d0+1,TrkExchangePar::nParam);
  DifNumber phi02(par2.phi0(),TrkExchangePar::ex_phi0+1,TrkExchangePar::nParam);
  
  DifIndepPar p2(params(par2));
  d2.setIndepPar(&p2);
  phi02.setIndepPar(&p2);


  // angular coefficients

  DifNumber m1 = tan(phi01);
  DifNumber m2 = tan(phi02);

  if (fabs(m1-m2) < 1.e-12) {  // parallel lines

    //the algorithm fails because the points have all the same distance

    _status.setFailure(13, "TrkPocaXY:: the two lines are parallel...");
    return;
  }

  DifNumber q1 = d1/cos(phi01);
  DifNumber q2 = d2/cos(phi02);

  // intersection point

  DifNumber xint,yint;
  interTwoLines(m1, q1, m2, q2, xint, yint);

  _xpoca1 = xint;
  _ypoca1 = yint;
  _xpoca2 = _xpoca1;
  _ypoca2 = _ypoca2;
 
  _docaxy = sqrt((_xpoca1-_xpoca2)*(_xpoca1-_xpoca2) +
		 (_ypoca1-_ypoca2)*(_ypoca1-_ypoca2));

  return;
}


//------------------------------------------------------------------------
void
TrkDifPocaXY::pocaTrajPoint(const TrkExchangePar& par, const HepPoint& pt)
  //------------------------------------------------------------------------
{
  // parameters for the trajectory
  
  
  DifNumber d0(par.d0(),TrkExchangePar::ex_d0+1,TrkExchangePar::nParam);
  DifNumber phi0(par.phi0(),TrkExchangePar::ex_phi0+1,TrkExchangePar::nParam);
  DifNumber omega(par.omega(),TrkExchangePar::ex_omega+1,TrkExchangePar::nParam);
  
  DifIndepPar p(params(par));
  d0.setIndepPar(&p);
  phi0.setIndepPar(&p);
  omega.setIndepPar(&p);

  // some quantities to find the poca in the plane xy

  DifNumber deltax = -omega*pt.x()/(1+d0*omega);
  DifNumber deltay = -omega*pt.y()/(1+d0*omega);

  DifNumber phi = atan2(sin(phi0)-deltax,cos(phi0)+deltay);

  // the doca in the plane xy

  _docaxy = d0+pt.x()*sin(phi0)-pt.y()*cos(phi0)
            +(pt.x()*cos(phi0)+pt.y()*sin(phi0))*tan((phi-phi0)/2.);
  
  // the poca in the plane xy

  _xpoca1 = pt.x() - _docaxy*sin(phi);
  _ypoca1 = pt.y() + _docaxy*cos(phi);
  
  
  return;
}

//------------------------------------------------------------------------
void	
TrkDifPocaXY::interLineCircle(const DifNumber& m, const DifNumber& q, 
		    const DifNumber& xc, const DifNumber& yc, 
		    const DifNumber& r, DifNumber& xint1,  DifNumber& yint1,  
                    DifNumber& xint2, DifNumber& yint2)
 //-------------------------------------------------------------------------
{

 DifNumber alpha = 1 + m*m;
 
 DifNumber beta = -xc +m*(q-yc);

 DifNumber gamma = xc*xc + (q-yc)*(q-yc) -r*r;

 DifNumber Delta = beta*beta - alpha*gamma;

 if (Delta < 0) {

    _status.setFailure(14, "TrkPocaXY:: the line and the circle heve no intersections...");
    return;

  }
  else if (fabs(Delta) <1.e-12) {

  xint1 = -beta/alpha;
  xint2 = xint1;

  }
  else {

    DifNumber xPlus = -beta/alpha + sqrt(beta*beta - alpha*gamma)/alpha;
    DifNumber xMinus = -beta/alpha - sqrt(beta*beta - alpha*gamma)/alpha;

    if (xPlus > xMinus) {
      xint1 = xMinus;
      xint2 = xPlus;
    }
    else {
      xint1 = xPlus;
      xint2 = xMinus;
    }
  }
 yint1 = m*xint1 + q;
 yint2 = m*xint2 + q;

 return;
}


//------------------------------------------------------------------------
void	
TrkDifPocaXY::interTwoCircles
(const DifNumber& xc1, const DifNumber& yc1, const DifNumber& r1,
 const DifNumber& xc2, const DifNumber& yc2, const DifNumber& r2,
 DifNumber& xint1,  DifNumber& yint1, DifNumber& xint2,  DifNumber& yint2)
  //-------------------------------------------------------------------------
{

  DifNumber A = (xc1*xc1 + yc1*yc1 - r1*r1) - (xc2*xc2 + yc2*yc2 - r2*r2);
 
  DifNumber B = -xc1 + xc2;

  DifNumber C = -yc1 + yc2;

  DifNumber alpha = 1 + (B*B)/(C*C);

  DifNumber beta = -xc1 + B/C*(yc1+A/(2*C));

  DifNumber gamma = xc1*xc1 + (yc1+A/(2*C))*(yc1+A/(2*C)) - r1*r1;

  DifNumber Delta = beta*beta - alpha*gamma;

  if (Delta < 0) {

    _status.setFailure(14, "TrkPocaXY:: the two circles have no intersections...");
    return;

  }
  else if (fabs(Delta) <1.e-12) {

    xint1 = -beta/alpha;
    xint2 = xint1;

  }
  else {

    DifNumber xPlus = -beta/alpha + sqrt(beta*beta - alpha*gamma)/alpha;
    DifNumber xMinus = -beta/alpha - sqrt(beta*beta - alpha*gamma)/alpha;

    if (xPlus > xMinus) {
      xint1 = xMinus;
      xint2 = xPlus;
    }
    else {
      xint1 = xPlus;
      xint2 = xMinus;
    }

  }
 
  yint1 = -(A+2*B*xint1)/(2*C);
  yint2 = -(A+2*B*xint2)/(2*C);


  return;
}

double 
TrkDifPocaXY::docaXYCov() const{
  HepSymMatrix cov; cov.assign(_docaxy.indepPar()->covariance());
  return cov.similarity(_docaxy.derivatives());
}

//------------------------------------------------------------------------
void	
TrkDifPocaXY::interTwoLines
(const DifNumber& m1, const DifNumber& q1, const DifNumber& m2, 
 const DifNumber& q2, DifNumber& xint, DifNumber& yint)
 //-------------------------------------------------------------------------
{

  if (fabs(m1-m2) < 1.e-12) {  // parallel lines

    //the algorithm fails because the points have all the same distance

    _status.setFailure(13, "TrkPocaXY:: the two lines are parallel...");
    return;
  }
  else { // the lines have an intersection point

    xint = (q2-q1)/(m1-m2);
    yint = m1*xint + q1;
  }

return;
}

DifIndepPar
TrkDifPocaXY::params(const TrkExchangePar& par)const{
  HepVector p(TrkExchangePar::nParam);
  p[TrkExchangePar::ex_d0] = par.d0();
  p[TrkExchangePar::ex_phi0] = par.phi0();
  p[TrkExchangePar::ex_z0] = par.z0();
  p[TrkExchangePar::ex_tanDip] = par.tanDip();
  p[TrkExchangePar::ex_omega] = par.omega();
  return DifIndepPar(p,par.covariance());

}


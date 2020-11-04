//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkDifPocaXY.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Calculate the point of closest approach between two trajectories 
//     (TrkDifTraj only) or between a trajectory and a point in the XY plane. 
//     Calculates (in ctor) the distance and the flight lengths along
//     the trajectory or trajectories;
//     The input flightlengths are used as a starting point; 
//     (A good starting point also reduces CPU time.)
//     Note that distance is a signed quantity for two trajectories.
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Selenia Dittongo                    Univ. Ferrara
//------------------------------------------------------------------------
#ifndef TRKDIFPOCAXY_HH
#define TRKDIFPOCAXY_HH

#include "TrkBase/TrkErrCode.hh"
#include "difAlgebra/DifNumber.hh"
using CLHEP::HepVector;

class TrkExchangePar;
class TrkDifTraj;
class HepPoint;


class TrkDifPocaXY {
public:
  
  TrkDifPocaXY(const TrkDifTraj& traj1, const double& fltl1, 
	       const TrkDifTraj& traj2, const double& fltl2);

  TrkDifPocaXY(const TrkDifTraj& traj, const double& fltl, const HepPoint& pt);

  ~TrkDifPocaXY() {};
  
  inline DifNumber difDocaXY() const;         // distance of closest approach 
                                              // with errors
  inline double docaXY() const;               // distance of closest approach
  inline const HepVector derivs() const;      // derivs of doca w/r/t track 
    
  inline const TrkErrCode& status() const;    // did the calculation succeed?
  inline double fltl1() const;                // path length on traj 1 @ poca
  inline double fltl2() const;                // path length on traj 2 @ poca
  double docaXYCov() const;                     // error on doca
            
private:
	
  DifNumber _docaxy;
  DifNumber _xpoca1, _ypoca1, _xpoca2, _ypoca2;
  double _fltl1, _fltl2;
  TrkErrCode _status;

  void pocaTwoCircles 
  (const TrkExchangePar& par1, const TrkExchangePar& par2);

  void pocaTwoLines 
  (const TrkExchangePar& par1, const TrkExchangePar& par2);

  void pocaLineCircle
  (const TrkExchangePar& par1, const TrkExchangePar& par2);

  void pocaTrajPoint
  (const TrkExchangePar& par, const HepPoint& pt);

  void interLineCircle
  (const DifNumber& m, const DifNumber& q, 
   const DifNumber& xc, const DifNumber& yc, const DifNumber& radius,
   DifNumber& xint1, DifNumber& yint1, DifNumber& xint2, DifNumber& yint2);

  void interTwoLines
  (const DifNumber& m1, const DifNumber& q1, const DifNumber& m2, 
   const DifNumber& q2, DifNumber& xint, DifNumber& yint);

  void interTwoCircles
  (const DifNumber& xc1, const DifNumber& yc1, const DifNumber& r1,
   const DifNumber& xc2, const DifNumber& yc2, const DifNumber& r2,
   DifNumber& xint1, DifNumber& yint1, DifNumber& xint2, DifNumber& yint2);

  DifIndepPar params(const TrkExchangePar& par)const;

  
};

// Inlined functions

DifNumber TrkDifPocaXY::difDocaXY() const    {return _docaxy;}
double TrkDifPocaXY::docaXY() const          {return _docaxy.number();}
const HepVector TrkDifPocaXY::derivs() const {return _docaxy.derivatives();}
const TrkErrCode& TrkDifPocaXY::status() const  {return _status;}
double TrkDifPocaXY::fltl1() const           {return _fltl1;}
double TrkDifPocaXY::fltl2() const           {return _fltl2;}


#endif


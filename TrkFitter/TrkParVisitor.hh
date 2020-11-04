//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkParVisitor.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:  TrkParVisitor is an implementation of the Visitor 
//               pattern for use in finding the 5 helix parameters
//               for different types of trajectories.  
//  
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Selenia Dittongo         Univ. Ferrara
//
//------------------------------------------------------------------------
#ifndef TRKPARVISITOR_HH
#define TRKPARVISITOR_HH

#include "TrkBase/TrkVisitor.hh"
#include "TrkBase/TrkExchangePar.hh"

class TrkSimpTraj;
class HelixTraj;
class TrkCircleTraj;
class NeutTraj;
class TrkDifLineTraj;

class TrkParVisitor : public TrkVisitor {
public:

  TrkParVisitor(const TrkSimpTraj&);
 
  ~TrkParVisitor();

  // ******************
  // data member access
  // ******************

  inline const TrkExchangePar parameters();

  //********************************
  // The visitor functions
  //********************************

  void trkVisitHelixTraj(const HelixTraj*);
  void trkVisitCircleTraj(const TrkCircleTraj*);
  void trkVisitNeutTraj(const NeutTraj*);
  void trkVisitLineTraj(const TrkDifLineTraj*); 

private:	

  TrkExchangePar _par;
};

const TrkExchangePar TrkParVisitor::parameters() {return _par;};

#endif


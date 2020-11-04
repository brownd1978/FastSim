//--------------------------------------------------------------------------
// File and Version Information:
//  $Id: TrkParVisitor.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:  TrkParVisitor is an implementation of the Visitor 
//               pattern for use in finding the 5 helix parameters
//               for different types of trajectories.  
//
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Selenia Dittongo         Univ. Ferrara
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "TrkFitter/TrkParVisitor.hh"

#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkExchangePar.hh" 
#include "CLHEP/Matrix/SymMatrix.h"
#include "TrkBase/TrkMomVisitor.hh"
#include "TrkBase/NeutParams.hh"
#include "TrkBase/HelixTraj.hh"
#include "TrkBase/NeutTraj.hh"
#include "TrkFitter/TrkCircleTraj.hh"
#include "TrkFitter/TrkDifLineTraj.hh"

//------------------------------------------------------------------------
TrkParVisitor::~TrkParVisitor() {
//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
TrkParVisitor::TrkParVisitor(const TrkSimpTraj& theTraj): 
  _par(-9999., -9999., -9999.,-9999., -9999.) {
//------------------------------------------------------------------------

    theTraj.visitAccept(this);

}

//------------------------------------------------------------------------
void
TrkParVisitor::trkVisitHelixTraj(const HelixTraj* theTraj) {
//------------------------------------------------------------------------
// set the helix parameters

  TrkExchangePar thePar(theTraj->d0(), theTraj->phi0(), theTraj->omega(),
			theTraj->z0(), theTraj->tanDip());
  thePar.setError(theTraj->parameters()->covariance());
  _par = thePar;
  
}

//------------------------------------------------------------------------
void
TrkParVisitor::trkVisitCircleTraj(const TrkCircleTraj* theTraj) {
//------------------------------------------------------------------------
// set the helix parameters

  TrkExchangePar thePar(theTraj->d0(), theTraj->phi0(), theTraj->omega(),
			0., 0.);
  thePar.setError(theTraj->parameters()->covariance());

  _par = thePar;
}

//------------------------------------------------------------------------
void
TrkParVisitor::trkVisitNeutTraj(const NeutTraj* theTraj) {
//------------------------------------------------------------------------
// set the helix parameters
 
  TrkExchangePar thePar(theTraj->params().d0(), theTraj->params().phi0(), 0.0,
			theTraj->params().z0(),theTraj->params().tanDip());
  thePar.setError(theTraj->params().covariance().sub(1,5));
  _par = thePar;
}

//------------------------------------------------------------------------
void
TrkParVisitor::trkVisitLineTraj(const TrkDifLineTraj* theTraj) {
//------------------------------------------------------------------------
// set the helix parameters

  TrkExchangePar thePar(theTraj->d0(), theTraj->phi0(),0.,
			theTraj->z0(), theTraj->tanDip());
  thePar.setError(theTraj->parameters()->covariance());
  _par = thePar;
}

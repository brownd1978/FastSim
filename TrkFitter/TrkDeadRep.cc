//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkDeadRep.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Authors: Steve Schaffner
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include <iostream>
#include "TrkFitter/TrkDeadRep.hh"
#include "TrkBase/TrkHotListEmpty.hh"
#include "PDT/Pdt.hh"
#include "TrkBase/TrkErrCode.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "TrkFitter/TrkCircleTraj.hh"
#include "ProxyDict/IfdIntKey.hh"
#include "CLHEP/Matrix/Matrix.h"
using std::ostream;


// Ctor
TrkDeadRep::TrkDeadRep(TrkRecoTrk* trk, PdtPid::PidType hypo) :
  TrkRep(trk, hypo,0,0,0,-99999.0,99999.0)
{
  setValid(false);
}

// copy ctor -- same as ctor.  Everything looks the same when you're dead.
TrkDeadRep::TrkDeadRep(const TrkDeadRep& oldRep, TrkRecoTrk* trk,
		       PdtPid::PidType hypo) :
  TrkRep(trk, hypo, 0, 0, 0, -99999.0,99999.0)
{
  setValid(false);
}

TrkDeadRep*
TrkDeadRep::clone(TrkRecoTrk* tk) const
{
  return new TrkDeadRep(*this, tk, this->particleType());
}


TrkDeadRep::~TrkDeadRep()
{ }

bool
TrkDeadRep::operator== (const TrkDeadRep& rhs)
{
  return (&rhs == this);
}

void
TrkDeadRep::addHot(TrkHitOnTrk *newHot)
{}


void
TrkDeadRep::removeHot(TrkHitOnTrk *theHot)
{}


void
TrkDeadRep::activateHot(TrkHitOnTrk* )
{}

void
TrkDeadRep::deactivateHot(TrkHitOnTrk* )
{}

HepPoint
TrkDeadRep::position(double fltL) const
{
  return HepPoint(-9999., -9999., -9999.);
}

Hep3Vector
TrkDeadRep::direction(double fltL) const
{
  return Hep3Vector(-9999., -9999., -9999.);
}

Hep3Vector
TrkDeadRep::momentum(double fltL) const
{
  return Hep3Vector(-9999., -9999., -9999.);
}

double
TrkDeadRep::pt(double fltL) const
{
  return -9999.;
}

double
TrkDeadRep::arrivalTime(double fltL) const
{
  return -9999.;
}

BbrPointErr
TrkDeadRep::positionErr(double fltL) const
{
  HepPoint point(-9999., -9999., -9999.);
  return BbrPointErr(point);
}

BbrVectorErr
TrkDeadRep::directionErr(double fltL) const
{
  Hep3Vector dir(-9999., -9999., -9999.);
  return BbrVectorErr(dir);
}

BbrVectorErr
TrkDeadRep::momentumErr(double fltL) const
{
  Hep3Vector dir(-9999., -9999., -9999.);
  return BbrVectorErr(dir);
}


const IfdKey&
TrkDeadRep::myKey() const
{
  static IfdIntKey _theKey(0);
  return _theKey;
}

TrkErrCode
TrkDeadRep::fit()
{
  return TrkErrCode(TrkErrCode::fail);
}

int
TrkDeadRep::nDof() const
{return 0;}

double
TrkDeadRep::chisq() const
{
  return -9999.;
}

int
TrkDeadRep::charge() const
{
  return 0;
}

TrkDeadRep*
TrkDeadRep::cloneNewHypo(PdtPid::PidType)
{
  return 0;
}

TrkDifTraj&
TrkDeadRep::traj()
{
  static TrkExchangePar circParams(1.0, 1.0, 1.0, 0.0, 0.0);
  static TrkCircleTraj dummy(circParams);
  return dummy;
}

const TrkDifTraj&
TrkDeadRep::traj() const
{
  return const_cast<TrkDeadRep*>(this)->traj();
}

TrkExchangePar
TrkDeadRep::helix(double fltLen) const
{
  return TrkExchangePar(0., 0., 0., 0., 0.);
}

void
TrkDeadRep::printAll(ostream& ostr) const
{print(ostr);}

void
TrkDeadRep::print(ostream& ostr) const
{printType(ostr);}

HepMatrix
TrkDeadRep::posmomCov(double) const
{
  return HepMatrix();
}

void
TrkDeadRep::getAllCovs(double,HepSymMatrix&,HepSymMatrix&,HepMatrix&) const
{}

void
TrkDeadRep::getAllWeights(double,HepVector&,HepVector&,HepSymMatrix&,
  HepSymMatrix&,HepMatrix&) const
{}

void
TrkDeadRep::getAllWeights(const HepPoint& pt,
			  HepVector& pos,
			  HepVector& mom,
			  HepSymMatrix& xxWeight,
			  HepSymMatrix& ppWeight,
			  HepMatrix&    xpWeight)const
{}

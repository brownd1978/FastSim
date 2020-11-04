// File and Version Information:
//      $Id: TrkSimpleRep.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//      implementation for TrkSimpleHelix
//
// Author(s): Steve Schaffner
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkSimpleRep.hh"
#include "TrkBase/TrkHitOnTrk.hh"
#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkParams.hh"
#include "TrkBase/TrkPoca.hh"
#include "TrkBase/TrkMomCalculator.hh"
#include "TrkBase/TrkRecoTrk.hh"
class BField;
#include "TrkBase/TrkErrCode.hh"
#include "BbrGeom/BbrVectorErr.hh"
#include "BbrGeom/BbrPointErr.hh"
#include "CLHEP/Vector/ThreeVector.h"

TrkSimpleRep::TrkSimpleRep(TrkRecoTrk* trk, PdtPid::PidType hypo, bool createHotList) :
  TrkRep(trk, hypo,createHotList)
{
  setChisq(-1.);
}

TrkSimpleRep::TrkSimpleRep(const TrkHotList& inHots,
                           TrkRecoTrk* trk, PdtPid::PidType hypo ) :
  TrkRep(inHots, trk, hypo )
{
  setChisq(-1.);
}

TrkSimpleRep::TrkSimpleRep(TrkHotList& inHots,
                           TrkRecoTrk* trk, PdtPid::PidType hypo, bool stealHots ) :
  TrkRep(inHots, trk, hypo, stealHots )
{
  setChisq(-1.);
}

TrkSimpleRep::TrkSimpleRep(TrkRecoTrk* trk, PdtPid::PidType hypo, int nact,
                           int nsv, int ndc, double chi2,
                           double stFndRng, double endFndRng) :
  TrkRep(trk, hypo, nact, nsv, ndc, stFndRng, endFndRng)
{
  setChisq(chi2);
}

// copy ctor
TrkSimpleRep::TrkSimpleRep(const TrkSimpleRep& right,
                           TrkRecoTrk* trk, PdtPid::PidType hypo)  :
  TrkRep(right, trk, hypo), _fitter(right._fitter), _chisq(right._chisq)
{
  _hotList.reset( right.hotList()->clone(this) );
}

TrkSimpleRep::~TrkSimpleRep()
{
}

TrkSimpleRep&
TrkSimpleRep::operator=(const TrkSimpleRep &right)
{
  _chisq = right._chisq;
  TrkRep::operator=(right);
  return *this;
}

TrkErrCode
TrkSimpleRep::fit()
{
  if (!hitCapable()) {
    setValid(false);
    return TrkErrCode(TrkErrCode::fail,100,
                      "Tried to fit without a list of hits.");
  }

  TrkErrCode error = _fitter.fit(*hotList(), simpTraj());
  if (error.failure()) {
    setValid(false);
  } else {
    setValid(true);
    setCurrent(true);
  }
  setChisq(_fitter.lastChisq());
  return error;
}

int
TrkSimpleRep::nDof() const
{
  return nActive() - simpTraj().parameters()->nPar();
}

double
TrkSimpleRep::chisq() const
{
  // Note possible inefficiency in calling virtual simpTraj(); implementation 
  //    could be moved to derived classes instead.  
  return _chisq;
  //  return simpTraj().parameters()->chisquared();
}

void
TrkSimpleRep::setChisq(double c)
{
  // See inefficiency comment above.
  //  simpTraj().parameters()->setChisquared(c);
  _chisq = c;
}

Hep3Vector
TrkSimpleRep::momentum(double fltL) const
{
  const BField& theField = parentTrack()->bField();
  return TrkMomCalculator::vecMom(simpTraj(), theField, fltL);
}

double
TrkSimpleRep::pt(double fltL) const
{
  const BField& theField = parentTrack()->bField();
  return TrkMomCalculator::ptMom(simpTraj(), theField, fltL);
}

int
TrkSimpleRep::charge() const
{
  const BField& theField = parentTrack()->bField();
  return TrkMomCalculator::charge(simpTraj(), theField, 0.);
}

BbrVectorErr
TrkSimpleRep::momentumErr(double fltL) const
{
  const BField& theField = parentTrack()->bField();
  return TrkMomCalculator::errMom(simpTraj(), theField, fltL);
}

/*
 * added on 7/17/98 to accomplish vertexing interface (M.Bondioli)
 * implementation temporarily needed to assure no compile crashes
 * elsewhere.
 */

HepMatrix
TrkSimpleRep::posmomCov(double fltL) const
{
  const BField& theField = parentTrack()->bField();
  return TrkMomCalculator::posmomCov(simpTraj(), theField, fltL);
}

void
TrkSimpleRep::getAllCovs(double fltL,
                        HepSymMatrix& xxCov,
                        HepSymMatrix& ppCov,
                        HepMatrix&    xpCov) const
{
  const BField& theField = parentTrack()->bField();
  TrkMomCalculator::getAllCovs(simpTraj(), theField, fltL,
                               xxCov,ppCov,xpCov);
}

void TrkSimpleRep::getAllWeights(double fltL,
                                 HepVector& pos,
                                 HepVector& mom,
                                 HepSymMatrix& xxWeight,
                                 HepSymMatrix& ppWeight,
                                 HepMatrix&    xpWeight) const
{
  const BField& theField = parentTrack()->bField();
  TrkMomCalculator::getAllWeights(simpTraj(), theField, fltL,
                                   pos,mom,xxWeight,ppWeight,xpWeight);
}

void TrkSimpleRep::getAllWeights(const HepPoint& pt,
                                 HepVector& pos,
                                 HepVector& mom,
                                 HepSymMatrix& xxWeight,
                                 HepSymMatrix& ppWeight,
                                 HepMatrix&    xpWeight)const
{
  double fltL=0;
  TrkPoca poca(traj(),fltL, pt);
  fltL = poca.flt1();
  getAllWeights(fltL,pos,mom,xxWeight,ppWeight,xpWeight);
}

// File and Version Information:
//      $Id: TrkCircleRep.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//      Implements circle track representation 
//
// Author(s): Steve Schaffner
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkCircleRep.hh"
#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "BbrGeom/BbrAngle.hh"
using std::endl;
using std::ostream;
TrkCircleRep::TrkCircleRep(const TrkExchangePar& inPar,
                         TrkRecoTrk* trk,
                         PdtPid::PidType hypo, bool createHotList) :
  TrkSimpleRep(trk, hypo,createHotList),
  _traj(inPar)
{}


// Construct from exchange paramters
TrkCircleRep::TrkCircleRep(const TrkExchangePar& inPar,
                           TrkRecoTrk* trk,
                           PdtPid::PidType hypo,
                           const TrkHotList &inHots):
  TrkSimpleRep(inHots, trk, hypo),
  _traj(inPar)
{
  set2d(true);
}

// Construct from exchange paramters, no hits
TrkCircleRep::TrkCircleRep(const TrkExchangePar& inPar, TrkRecoTrk* trk,
                           PdtPid::PidType hypo, int nact, int nsv, int ndc, double chi2,
                           double stFndRng, double endFndRng) :
  TrkSimpleRep(trk, hypo, nact, nsv, ndc, chi2, stFndRng, endFndRng),
  _traj(inPar)
{
  set2d(true);
}

// copy ctor
TrkCircleRep::TrkCircleRep(const TrkCircleRep& right, TrkRecoTrk* trk,
                           PdtPid::PidType hypo) :
  TrkSimpleRep(right, trk, hypo), _traj(right._traj) 
{
    set2d(true);
}

TrkCircleRep::~TrkCircleRep()
{}

TrkCircleRep*
TrkCircleRep::clone(TrkRecoTrk* theTrack) const
{
  TrkCircleRep* newRep = new TrkCircleRep(*this, theTrack, this->particleType());
  newRep->setValid(fitValid());
  newRep->setCurrent(fitCurrent());
  return newRep;
}

TrkCircleRep*
TrkCircleRep::cloneNewHypo(PdtPid::PidType hypo)
{
  TrkCircleRep* newRep = new TrkCircleRep(*this, parentTrack(), hypo);
  newRep->setValid(fitValid());
  newRep->setCurrent(fitCurrent());  // true for circles, anyway
  return newRep;
}

TrkExchangePar
TrkCircleRep::helix(double ) const
{
  TrkExchangePar localPar(_traj.d0(), BbrAngle(_traj.phi0()).rad(), 
                          _traj.omega(), 0.0, 0.0);
  HepSymMatrix& theCov = localPar.covariance();
  for (int i = 1; i < 4; i++) {
    for (int j = 1; j < 4; j++) {
      theCov(i,j) = _traj.parameters()->covariance()(i,j);
    }
  }
  return localPar;
}

void 
TrkCircleRep::print(ostream& ostr) const
{
  ostr << "phi0: " << BbrAngle(_traj.phi0()).rad()
       << " om: "<<_traj.omega()
       << " d0: " << _traj.d0() << endl;
}

void
TrkCircleRep::printAll(ostream& ostr) const
{
  print(ostr);
}

TrkSimpTraj&
TrkCircleRep::simpTraj()
{
  return _traj;
}

const TrkSimpTraj&
TrkCircleRep::simpTraj() const
{
  return _traj;
}

TrkDifTraj&
TrkCircleRep::traj()
{
  return _traj;
}
const TrkDifTraj&
TrkCircleRep::traj() const
{
  return _traj;
}

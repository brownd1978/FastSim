// File and Version Information:
//      $Id
//
// Description:
//      Implements circle track representation 
//
// Author(s): Steve Schaffner
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkLineRep.hh"
#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "BbrGeom/BbrAngle.hh"
using std::endl;
using std::ostream;

// Construct from exchange paramters

TrkLineRep::TrkLineRep(const TrkExchangePar& inPar,
                       TrkRecoTrk* trk,
                       PdtPid::PidType hypo, bool createHotList ) :
  TrkSimpleRep( trk, hypo, createHotList),
  _traj(inPar)
  
{
}


TrkLineRep::TrkLineRep(const TrkExchangePar& inPar,
                       TrkRecoTrk* trk,
                       PdtPid::PidType hypo,
                       const TrkHotList& inHots) :
  TrkSimpleRep(inHots, trk, hypo),
  _traj(inPar)
  
{
}

// Construct from exchange paramters, no hits
TrkLineRep::TrkLineRep(const TrkExchangePar& inPar, TrkRecoTrk* trk,
  PdtPid::PidType hypo, int nact, int nsv, int ndc, double chi2,
			  double stFndRng, double endFndRng) :
  TrkSimpleRep(trk, hypo, nact, nsv, ndc, chi2, stFndRng, endFndRng),
  _traj(inPar)
{
}

// copy ctor
TrkLineRep::TrkLineRep(const TrkLineRep& right, TrkRecoTrk* trk, 
			   PdtPid::PidType hypo) :
  TrkSimpleRep(right, trk, hypo), _traj(right._traj)
{
}

TrkLineRep::~TrkLineRep()
{
}

TrkLineRep*
TrkLineRep::clone(TrkRecoTrk* theTrack) const
{
  TrkLineRep* newRep = new TrkLineRep(*this, theTrack, this->particleType());
  newRep->setValid(fitValid());
  newRep->setCurrent(fitCurrent());
  return newRep;
}

TrkLineRep*
TrkLineRep::cloneNewHypo(PdtPid::PidType hypo)
{
  TrkLineRep* newRep = new TrkLineRep(*this, parentTrack(), hypo);
  newRep->setValid(fitValid());
  newRep->setCurrent(fitCurrent());  // true for lines, anyway
  return newRep;
}

TrkExchangePar 
TrkLineRep::helix(double ) const
{
  TrkExchangePar localPar(_traj.d0(), BbrAngle(_traj.phi0()).rad(), 
			  0.0, _traj.z0(), _traj.tanDip());
  const HepSymMatrix& lineCov = _traj.parameters()->covariance();
  HepSymMatrix& theCov = localPar.covariance();
  int ex_d0        = TrkExchangePar::ex_d0 + 1;
  int ex_phi0      = TrkExchangePar::ex_phi0 + 1;
  int ex_z0        = TrkExchangePar::ex_z0 + 1;
  int ex_tD        = TrkExchangePar::ex_tanDip + 1;

  int ln_d0        = TrkDifLineTraj::d0Ind + 1;
  int ln_phi0      = TrkDifLineTraj::phi0Ind + 1;
  int ln_z0        = TrkDifLineTraj::z0Ind + 1;
  int ln_tD        = TrkDifLineTraj::tanDipInd +1;

  theCov(ex_d0, ex_d0) =     lineCov(ln_d0, ln_d0);
  theCov(ex_phi0, ex_d0) =   lineCov(ln_phi0, ln_d0);
  theCov(ex_phi0, ex_phi0) = lineCov(ln_phi0, ln_phi0);
  theCov(ex_z0, ex_d0) =     lineCov(ln_z0, ln_d0);
  theCov(ex_z0, ex_phi0) =   lineCov(ln_z0, ln_phi0);
  theCov(ex_z0, ex_z0) =     lineCov(ln_z0, ln_z0);
  theCov(ex_tD, ex_d0) =     lineCov(ln_tD, ln_d0);
  theCov(ex_tD, ex_phi0) =   lineCov(ln_tD, ln_phi0);
  theCov(ex_tD, ex_z0) =     lineCov(ln_tD, ln_z0);
  theCov(ex_tD, ex_tD) =     lineCov(ln_tD, ln_tD);

  return localPar;
}

void
TrkLineRep::print(ostream& ostr) const
{
  ostr << "phi0: " << BbrAngle(_traj.phi0()).rad()
       << " om: " << 0.0  
       << " d0: " << _traj.d0() 
       << " z0: " << _traj.z0() 
       << " tnDip: " << _traj.tanDip() 
       << endl;
}

void
TrkLineRep::printAll(ostream& ostr) const
{
  print(ostr);
}

TrkSimpTraj&
TrkLineRep::simpTraj()
{
  return _traj;
}

const TrkSimpTraj&
TrkLineRep::simpTraj() const
{
  return _traj;
}

TrkDifTraj&
TrkLineRep::traj()
{
  return _traj;
}

const TrkDifTraj&
TrkLineRep::traj() const
{
  return _traj;
}

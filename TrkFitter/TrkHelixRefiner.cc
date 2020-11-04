//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkHelixRefiner.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "BbrStdUtils/BbrCollectionUtils.hh"
#include "CLHEP/Matrix/SymMatrix.h"
#include "ProxyDict/IfdIntKey.hh"
#include "TrkFitter/TrkHelixRefiner.hh"
#include "TrkFitter/TrkHelixRep.hh"
#include "TrkBase/TrkSimpTraj.hh"
#include "TrkBase/TrkHitOnTrk.hh"

TrkHelixRefiner::TrkHelixRefiner() :
  _errorMatrix(0)
{
  invalidate();
}

TrkHelixRefiner::~TrkHelixRefiner() 
{
  std::for_each(_derivs.begin(),_derivs.end(),babar::Collection::DeleteObject());
}

void 
TrkHelixRefiner::invalidate() 
{
  _cacheValid = false;  
  _errCode.setFailure(11,"No calculation");
}

const IfdKey& 
TrkHelixRefiner::myKey() const 
{
  static IfdIntKey _theKey(3133);
  return _theKey;
}

TrkHelixRep& 
TrkHelixRefiner::helixRep() 
{
  assert (isAttached());
  return (TrkHelixRep&) (*myRep());   // cast -- I know it's the right rep.
}

bool 
TrkHelixRefiner::attach(TrkRep* rep) 
{
  bool success = TrkExtInterface::attach(rep);
  if (success) {
    invalidate();
  }
  return success;

}

// The existence of this function is puzzling . . .
bool 
TrkHelixRefiner::attach(const TrkRep* rep) 
{
  bool success = TrkExtInterface::attach(rep);
  if (success) invalidate();
  return success;
}

void
TrkHelixRefiner::updateErrorMatrix()
{
  _cacheValid = true;
  std::for_each(_derivs.begin(),_derivs.end(),babar::Collection::DeleteObject());
  _derivs.clear();

  TrkSimpTraj& theTraj = helixRep().simpTraj();

  TrkParams &params = *(theTraj.parameters());
  int npar = params.nPar();
  _errorMatrix.reset( new HepSymMatrix(npar,0) );

  TrkErrCode calcResult;
  const TrkHotList* hitlist = helixRep().hotList();
  for (TrkHotList::hot_iterator ihit = hitlist->begin(); 
                                ihit != hitlist->end(); ++ihit) {
    double deltaChiNew;
    HepVector* der = new HepVector(npar);
    _derivs.push_back(der);
    HepVector& derRef = *der;

    calcResult = ihit->getFitStuff(derRef, deltaChiNew);
    if (calcResult.failure()) continue;
    if (!ihit->isActive()) continue;

    for (int i = 0; i < npar; i++) {
      for (int j = 0; j < i+1; j++) {
         _errorMatrix->fast(i+1,j+1) += derRef[i] * derRef[j];
      }
    }
  }
  int ierr;
  _errorMatrix->invert(ierr);
  if (ierr) {
    _errCode.setFailure(12, "Matrix inversion failed in TrkHelixFitter");
  } else {
    _errCode.setSuccess(1);
  }
}

const HepSymMatrix&
TrkHelixRefiner::newErrorMatrix()
{
  if (!cacheValid()) updateErrorMatrix();
  return *_errorMatrix;
}

const std::vector<HepVector*>&
TrkHelixRefiner::newDeriv()
{
  if (!cacheValid()) updateErrorMatrix();
  return _derivs;
}

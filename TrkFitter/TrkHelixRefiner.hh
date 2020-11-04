//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkHelixRefiner.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//   Calculates updated error matrix and derivatives, based on fitted 
// values in HelixRep.  The derivatives are returned as a RW vector of 
// HepVectors, in the same order as the HoTs on the track.  The 
// calculation is done when you call either newErrorMatrix() or 
// newDerivs().  The status of the calculation (success, failure, hasn't 
// been done yet) can be obtained vis errorStatus().
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKHELIXREFINER_HH
#define TRKHELIXREFINER_HH
#include <vector>
#include <memory>
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "TrkBase/TrkErrCode.hh"
#include "TrkBase/TrkExtInterface.hh"
using CLHEP::HepVector;
using CLHEP::HepSymMatrix;

class TrkHelixRep;

// Class interface //
class TrkHelixRefiner : public TrkExtInterface {

public:
  TrkHelixRefiner();
  virtual ~TrkHelixRefiner();
  virtual const IfdKey& myKey() const;
  virtual bool attach(TrkRep*);
  virtual bool attach(const TrkRep*);

  const HepSymMatrix& newErrorMatrix();
  const std::vector<HepVector*>& newDeriv();
  const TrkErrCode& errorStatus() const                     {return _errCode;}

protected:
  TrkHelixRep& helixRep();
  bool cacheValid()                                    {return _cacheValid;}
  void updateErrorMatrix();
  HepSymMatrix& errorMatrix();
  void invalidate();

private:
  bool _cacheValid;
  std::auto_ptr<HepSymMatrix> _errorMatrix;
  std::vector<HepVector*> _derivs;
  TrkErrCode _errCode;

  // Preempt 
  TrkHelixRefiner&   operator= (const TrkHelixRefiner&);
  TrkHelixRefiner(const TrkHelixRefiner &);
};


#endif

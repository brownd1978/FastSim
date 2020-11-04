//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkBmSpotOnTrk.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//   Defines interface between Svt hit and track, and stores information
//       about fit.  Used by fitters
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Authors: Steve Schaffner
//
//      Copyright (C) 1996      Stanford Linear Accelerator Center
//------------------------------------------------------------------------

#ifndef TRKBMSPOTONTRK_HH
#define TRKBMSPOTONTRK_HH

#include "TrkBase/TrkHitOnTrk.hh"
#include "TrkBase/TrkEnums.hh"
#include "TrajGeom/TrkLineTraj.hh"

#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Matrix/SymMatrix.h"


// Class interface //
class TrkBmSpotOnTrk : public TrkHitOnTrk 
{
public:
  TrkBmSpotOnTrk(const HepPoint &ip, const HepSymMatrix &size );
  ~TrkBmSpotOnTrk();

  TrkBmSpotOnTrk* clone(TrkRep*, const TrkDifTraj *t =0) const;
  virtual TrkEnums::TrkViewInfo whatView()     const;
  virtual unsigned layerNumber() const { return 0; }

  const Trajectory*   hitTraj() const;
  const HepPoint&     ip() const;

// this hot has no timing information
  virtual bool timeResid(double& t,double& tErr) const { return false; }
  virtual bool timeAbsolute(double& t,double& tErr) const { return false; }

protected:
  TrkBmSpotOnTrk(const TrkBmSpotOnTrk &hitToBeCopied,TrkRep *newRep, const TrkDifTraj *trkTraj=0);
  virtual TrkErrCode updateMeasurement(const TrkDifTraj* traj, bool maintainAmbiguity);
  
  static const TrkLineTraj FindBeamTrajectory( const HepPoint &point, const HepSymMatrix &error );
  double GetRms();

private:
  // Make sure these aren't used by mistake:
  TrkBmSpotOnTrk&   operator= (const TrkBmSpotOnTrk&);
  TrkBmSpotOnTrk(const TrkBmSpotOnTrk&);

  const TrkLineTraj _beamTraj;
  const HepPoint _ip;
  const HepSymMatrix _size;
};

#endif

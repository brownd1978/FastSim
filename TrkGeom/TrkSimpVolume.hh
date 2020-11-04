//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TrkSimpVolume.hh 105 2010-01-15 12:14:11Z stroili $
//
// Description:
//	TrkSimpVolume Class - Simple Implementation of TrkVolume.
//         The tracking volume is defined by two cylinders along z
//         and two planes perpendicular to z.
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Gautier Hamel de Monchenault - CEN Saclay & Lawrence Berkeley Lab
//
// History (add to end):
//      Gautier   May 6, 1997  - creation
//
// Copyright Information:
//	Copyright (C) 1997		Lawrence Berkeley Laboratory
//	Copyright (C) 1997	       CEA - Centre d'Etude de Saclay
//
//------------------------------------------------------------------------

#ifndef TRKSIMPVOLUME_HH
#define TRKSIMPVOLUME_HH

class DetSurface;

#include <memory>
#include "TrkBase/TrkVolume.hh"


class TrkSimpVolume : public TrkVolume {

public:
// Constructors
  TrkSimpVolume( double rmin=0.,    double rmax=100.,
                 double zmin=-200., double zmax=200.);
  TrkSimpVolume( const char* name,
                 double rmin=0.,    double rmax=100.,
                 double zmin=-200., double zmax=200.);
// Destructor
  virtual ~TrkSimpVolume();
// Extend the flight range of the trajectory through the volume
  virtual bool extendThrough( const Trajectory* theTraj,
                              double& theFlightDist,
                              trkDirection theDirection=trkOut,
                              double* theStartingFlightDist=0) const;
// returns false if the point is outside the volume 
  virtual bool isInside( const HepPoint& ) const;

protected:
  double _rmin, _rmax;
  double _zmin, _zmax;
  std::auto_ptr<DetSurface> _surf[4];
//prohibit
  TrkSimpVolume& operator = (const TrkSimpVolume&);
  TrkSimpVolume(const TrkSimpVolume&);
};
#endif

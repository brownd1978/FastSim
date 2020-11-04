//--------------------------------------------------------------------------
// Name:
//   PacRecHitInfo
// Description:  Simple struct storing everything needed to create a reconstructed hit
//
// Environment:
//   Software developed for PACRAT / SuperB
//
//  Copyright Information:
//      Copyright (C) 2010
//
//  Authors: Dave Brown, Lawrence Berkeley National Lab
//-----------------------------------------------------------------------
#ifndef PacRecHitInfo_HH
#define PacRecHitInfo_HH

#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/HepPoint.h"

struct PacRecHitInfo {
  HepPoint _simpos; // true position of this hit
  HepPoint _hitpos; // 'reconstructed' position of this hit (including resolution smearing)
  Hep3Vector _wdir; // direction vector of the 'wire' against which this measurment was made
  Hep3Vector _mdir; // direction vector of the measurment
  Hep3Vector _pdir; // direction vector of POCA to the track  
  double _sres; // resolution used when smearing hit position
  double _herr; // reconstruction error to assign this hit, = smeared RMS 
  double _flentrue; // true pathlength along the global trajectory
};

#endif

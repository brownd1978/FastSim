//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TrkEnv.hh 103 2010-01-15 12:12:27Z stroili $
//
// Description:
//	Class TrkEnv
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David Brown
//
// Copyright Information:
//	Copyright (C) 1997		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef TRKENV_HH
#define TRKENV_HH

//---------------
// C++ Headers --
//---------------

#include <iostream>

//----------------------
// Base Class Headers --
//----------------------

#include "AbsEnv/AbsDetEnv.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "BaBar/BaBar.hh"
#include "TrkEnv/TrkVolumeHandle.hh"
#include "TrkEnv/KalContext.hh"
#include "CLHEP/Geometry/HepPoint.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class DetSet;
class BField;
class TrkVolume;
class TrkTimeAdjuster;
class AbsParmString;
class TrkPassive;
class TrkIdManager;
//		---------------------
// 		-- Class Interface --
//		---------------------
class TrkEnv: public AbsDetEnv
{
//--------------------
// Instance Members --
//--------------------

public:
  // Constructors
  TrkEnv();
  // Destructor
  virtual ~TrkEnv( );


  const BField* magneticField(const char* fieldname="Default") const;
  
  // This latter function is just to provide cross checking of the above
  // two and should not b in general use. 
  // const BField* magneticFieldOld() const;

  //  Return the DetectorModel top-level set
  const DetSet* trkDetSet() const;
  
  //  Find the track volume, given the enum detector value
  const TrkVolume* findTrkVolume( TrkVolumeHandle::trkvolumes ) const;

  //  list volumes
  void listTrkVolumes(std::ostream& os = std::cout) const;
  
  //  Tracking origin
  const HepPoint& trkOrigin() const 
    { return _theOrigin; }

  void setToUseNominalField(double nominalFieldValue=1.5);
  
  // KalContext 
  const KalContext* kalContext() const
    {return _theKalContext; }



  //  Static functions to define which subsystems to use
  enum detectors { Passive=0,SvtDetector,DchDetector,EmcDetector,
		  DrcDetector,IfrDetector,nDetectors};

  static bool detectorUsed(TrkEnv::detectors thedet);
  static void useDetector(TrkEnv::detectors thedet);

  // Howto convert 'raw' times in one subsystem to another.

  const TrkTimeAdjuster* getTrkTimeAdjuster(const char* keyname=0) const;

  // passive material in the tracking volume

  const TrkPassive* getTrkPassive(const char* keyname=0) const;

  const TrkIdManager* idManager(const char* name="Default") const;
private:

  // tracking origin
  HepPoint _theOrigin;
  // KalContext
  KalContext* _theKalContext;

  // If we have an Objy resident field, which one do we want?
  bool _useNominalField;
  double _nominalFieldValue;

  //  Dummy copy constructor and assignment operator, to preempt
  //  the compiler version of these.
  TrkEnv(const TrkEnv&);
  TrkEnv &operator=(const TrkEnv&);

  friend class TrkBuildEnv;
  friend class TrkBuildCoreEnv;
  
  // statics
  static bool _usedets[nDetectors];
};

#endif // TRKENV_HH

// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkPassive.hh 105 2010-01-15 12:14:11Z stroili $
//
//  Description:
//  Functions for building up the beampipe material model.  This defines
//  the master object which owns the full detector description heirarchy for
//  the beampipe.  The single (default) constructor for this class creates the object,
//  and builds the whole geometry/material tree for the beampipe.  
//
// Copyright Information:
//	Copyright (C) 1996	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 10/11/96
//           Johan Blouw, 07/10/2001
// ------------------------------------------------------------------------------
#ifndef TRKBPDETECTOR_HH
#define TRKBPDETECTOR_HH
#include <iostream>
#include <fstream>
#include "DetectorModel/DetSet.hh"
#include "DetectorModel/DetSetHash.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "TrkGeom/TrkCentralVolume.hh"
#include <vector>
#include <memory>
class TrkBPLayout;
class TrkSTLayout;
class TrkSvtSLayout;
class DetSurfaceType;

//
//  Define the main class
//
class TrkPassive {
public:
//  construct from the compact layout description object
  TrkPassive(const TrkBPLayout&, 
             const TrkSTLayout&,
             const TrkSvtSLayout&); // constructor
// Destructor
  ~TrkPassive();
  const TrkVolume& trkVolume() const { return *_trkBPVolume; }
  const DetSet& trkSet() const { return _trkset;}
  void print(std::ostream& os = std::cout) const; //  basic printout
  void printAll(std::ostream& os = std::cout) const; //  basic printout
//  void gnuplotOutline(const char*,int*,int*,int*) const; /* plot the outline of the material according
//   to the described limits in layer, phi, and half */
private:
  DetSet _trkset; // full Passive material set.
  std::vector<DetType*> _types; // list of types used/owned by TrkPassive
  std::auto_ptr<TrkCentralVolume> _trkBPVolume; // beampipe volume
  std::auto_ptr<TrkCentralVolume> _trkSTVolume; // support tube volume
  // premept
  TrkPassive(const TrkPassive&);
  TrkPassive& operator = (const TrkPassive& other);
// utility
  void createST();
  void createSvtShield();

  friend bool testCdb(const TrkPassive*, const TrkPassive*);

};

#endif

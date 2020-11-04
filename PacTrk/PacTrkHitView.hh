// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkHitView; Base class describing a single view (dimension) of a hit measurement.
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------
#ifndef PacTrkHitView_HH
#define PacTrkHitView_HH

#include "TrkBase/TrkEnums.hh"
#include "CLHEP/Vector/ThreeVector.h"
#include "PacTrk/PacRecHitInfo.hh"
#include <vector>

class PacSimHit;
class PacHitOnTrk;

class PacTrkHitView {
public:
  PacTrkHitView(TrkEnums::TrkViewInfo view);
  virtual ~PacTrkHitView() = 0;

  // get the (reco) hit information from the sim hit.  This includes
  // the (smeared) position, the hit measurement direction, and the
  // (predicted) resolution.
  virtual void getHitInfo(const PacSimHit& simhit,std::vector<PacRecHitInfo>&) const = 0;

  // efficiency for this view.  This can be correlated between views (ie for pixels).  Can depend on position
  // default implementation is 100% efficiency
  virtual double efficiency(const PacSimHit& simhit) const;
  // angle of the measurement direction WRT surface coordinate '0', = x for planes, phi for cylinders
  virtual double measurementAngle() const = 0;
  // rough measure of how far apart active elements are in the measurement direction
  virtual double sensorSeparation() const = 0;
  // rough measure of sensor size perpendicular to measurement direction;
  virtual double sensorSize() const = 0;
  
  // compatibility with BaBar view dfn.
  TrkEnums::TrkViewInfo whatView() const { return _view;}

private:
  TrkEnums::TrkViewInfo _view;
};

#endif

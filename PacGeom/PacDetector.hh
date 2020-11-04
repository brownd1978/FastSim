/* Class: PacDetector
 *
 * Base class for simple detector models, used in Pacrat testing. */

#ifndef PacDetector_HH
#define PacDetector_HH

class DetIntersection;
class Trajectory;
class TrkHotList;
class DetectorModel;
class DetSet;
class TrkVolume;
class TrkGeomTraj;

#include "CLHEP/Random/RandomEngine.h"
using CLHEP::HepRandomEngine;

class PacDetector {
public:
    PacDetector();
    virtual ~PacDetector();
// PravdaMC interface
	virtual const DetSet* detectorModel() const = 0;
  virtual const TrkVolume* detectorVolume() const = 0;
// navigation
  virtual bool findNextInter(const TrkGeomTraj* traj,DetIntersection& dinter) const =0;
// update the random engine as needed
  virtual void setRandomEngine(HepRandomEngine* engine) = 0;
private:
};

#endif // PacDetector_HH

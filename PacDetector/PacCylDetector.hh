/* Class: PacCylDetector
 *
 * A detector model consisting of a series of concentric cylinders, each with
 * the same thickness and single point resolution. */
#ifndef PacCylDetector_HH
#define PacCylDetector_HH

#include "PacGeom/PacDetector.hh"

#include <vector>
#include <set>

class DetElem;
class DetType;
class DetSet;
class PacMeasurement;
class PacCylVoxelSet;
class TrkGeomTraj;

class PacCylDetector : public PacDetector {
public:
//    PacCylDetector();
    PacCylDetector();
    virtual ~PacCylDetector();
// PacDetector interface
	virtual const DetSet* detectorModel() const { return _detset; }
  virtual const TrkVolume* detectorVolume() const { return _volume;}
  virtual void setRandomEngine(HepRandomEngine* engine);
  virtual bool findNextInter(const TrkGeomTraj* traj,DetIntersection& dinter) const;
// Specific interface
	const std::vector<double>& getRadii() const { return _radii;}
  const std::vector<PacCylVoxelSet*>& voxelSets() const { return _cvsets; }
protected:
    std::vector<double> _radii;
    std::vector<DetElem*> _elements; // ownership of all element objects
		std::set<DetType*> _types; // type ownership:  REMOVE ME!!!
		std::set<PacMeasurement*> _measures; // measurement ownership for this detector
    DetSet* _detset; // global set for the entire detector
    std::vector<PacCylVoxelSet*> _cvsets;  // voxel sets for each volume
    std::vector<DetSet*> _volsets;  // organization of elements into volumes
    TrkVolume* _volume; // tracking volume, defined for the entire active volume
// cache: current voxel set to speed up navigation
    mutable const PacCylVoxelSet* _cvset;
};

#endif // PacCylDetector_HH

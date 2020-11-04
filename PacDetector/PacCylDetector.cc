/*******************************************************************************
* PacCylDetector
*******************************************************************************/

#include "BaBar/BaBar.hh"
#include "PacDetector/PacCylDetector.hh"
#include "PacDetector/PacElemFactory.hh"
#include "PacGeom/PacDetElem.hh"
#include "PacGeom/PacDetSet.hh"
#include "PacGeom/PacMeasurement.hh"
#include "PacGeom/PacCylVoxelSet.hh"
#include "DetectorModel/DetElem.hh"
#include "DetectorModel/DetSet.hh"
#include "PacEnv/EdmlDetector.hh"
#include "PacEnv/EdmlDetVolume.hh"
#include "PacEnv/PacConfig.hh"
#include "TrkGeom/TrkCentralVolume.hh"
#include "BbrGeom/TrkGeomTraj.hh"
#include <iostream>
#include <vector>
#include <limits>
#include <assert.h>

using namespace std;

PacCylDetector::PacCylDetector() : _cvset(0) {

    // Check if the EDML detector description is available. If so then use
    // it in the constructor. Otherwise resort to the legacy implemenation.
    //
  const EdmlDetector* edml = gconfig.edml_detector();
  if(edml) {

    // construct top-level detector set
    unsigned setindex(1);
    _detset = new DetSet(edml->name().c_str(),setindex++);
    assert(_detset != 0);

    // Go through EDML volumes and get sub-detector elements from each.

    vector<const EdmlDetVolume* > volumes;
    edml->volumes(volumes);

    for(vector<const EdmlDetVolume* >::const_iterator vItr=volumes.begin();
    vItr!=volumes.end();++vItr) {

      const EdmlDetVolume* evol = *vItr;
    // Create a volume-level DetectorSet
      PacDetSet* volset = new PacDetSet(evol,setindex++);
      assert(volset != 0);
    // add this to the main detset and the (owned) list of volume sets
      *_detset += *volset;
      _volsets.push_back(volset);
    // get det element configurations for this volume

      vector<const EdmlDetElement* > elements;
      std::vector<PacDetElem*> velems;
      evol->elements(elements);

      for(vector<const EdmlDetElement* >::const_iterator eItr=elements.begin();
      eItr!=elements.end();++eItr) {

    // Use the factory to build a subdetector of the given type
        DetElem* elem = PacElemFactory::get(*eItr);
        PacDetElem* pelem = dynamic_cast<PacDetElem*>(elem);
    // all elements must be PacDetElems
        assert(pelem != 0);
        _elements.push_back(elem);
        velems.push_back(pelem);
        _types.insert(const_cast<DetType*>(elem->detectorType()));
    // find the associated measurement (if there is any)
        const std::vector<const PacMeasurement*>& mdevs = pelem->measurementDevices();
        for(std::vector<const PacMeasurement*>::const_iterator imdev=mdevs.begin();imdev!=mdevs.end();imdev++){
          PacMeasurement* meas = const_cast<PacMeasurement*>(*imdev);
          if(meas != 0)
            _measures.insert(meas);
        }
    // load the DetectorModel element into its volume set
        volset->append(elem);
      }
// construct voxel set for this volume and fill with elements
      std::string vname = volset->setName();
      vname += ".";
      
    	PacConfig vconfig = gconfig.getconfig(vname.c_str());
      std::vector<double> rbounds;
      rbounds = vconfig.getvector("rbounds",rbounds);
      std::vector<double> zbounds;
      zbounds = vconfig.getvector("zbounds",zbounds);
      std::vector<unsigned> nfvoxels;
      nfvoxels = vconfig.getvector("nphivoxels",nfvoxels);
      double phioffset = vconfig.getdouble("phioffset",0.0);
// check
      if(rbounds.size() == 0 || zbounds.size() == 0 || nfvoxels.size() == 0)
        ErrMsg(fatal) << "Voxel set parameters undefined for volume " << volset->setName() << endmsg;
      PacCylVoxelSet* cvset = new PacCylVoxelSet(rbounds,zbounds,nfvoxels,phioffset);
      assert(cvset != 0);
      _cvsets.push_back(cvset);
      for( std::vector<PacDetElem*>::iterator ielem = velems.begin();ielem != velems.end(); ielem++){
        cvset->insert(*ielem);
      }
    }
// construct the overall detector volume
    double rmax = gconfig.getdouble("DetectorVolume.rmax",321.0);
    double zmax = gconfig.getdouble("DetectorVolume.zmax",381.0);
    double zmin = gconfig.getdouble("DetectorVolume.zmin",-301.0);
    _volume = new TrkCentralVolume(edml->name().c_str(),rmax,zmin,zmax);
  } else {
    ErrMsg(fatal) << "failed to create edml-based detector, check for earlier parse errors" << endmsg;
  }
}

PacCylDetector::~PacCylDetector() {
	delete _detset;
	for(unsigned int ielem = 0; ielem < _elements.size(); ielem++)
		delete _elements[ielem];
  std::set<DetType*>::iterator itype = _types.begin();
	while( itype != _types.end()){
    delete *itype;
    itype++;
	}
  std::set<PacMeasurement*>::iterator imeas = _measures.begin();
	while( imeas != _measures.end()){
    delete *imeas;
    imeas++;
	}
  for(unsigned int i=0; i < _volsets.size(); i++)
    delete _volsets[i];

  for(std::vector<PacCylVoxelSet*>::iterator ivs = _cvsets.begin();ivs!=_cvsets.end();ivs++){
    delete *ivs;
  }
}

void
PacCylDetector::setRandomEngine(HepRandomEngine* engine){
// recur to all my measurments
  std::set<PacMeasurement*>::iterator imeas = _measures.begin();
  while(imeas != _measures.end()){
    (*imeas)->setRandomEngine(engine);
    imeas++;
  }
}

bool
PacCylDetector::findNextInter(const TrkGeomTraj* traj,DetIntersection& dinter) const {
  bool retval(false);
// try the most recently-checked volume first; if we're still inside it, keep iterating over those elements
  if(_cvset != 0 && _cvset->isInside(traj->position(dinter.pathlen))){
    DetIntersection dtest = dinter;
    double exitflt = dinter.pathlen;
    if(_cvset->findExit(traj,exitflt) && _cvset->findNextInter(traj,dtest) && dtest.pathlen < exitflt){
      dinter = dtest;
      retval =  true;
    }
  }
  if(!retval){
// otherwise, search all the volumes for the nearest entrance
    _cvset = 0;
    DetIntersection dbest = dinter;
    dbest.pathlen = std::numeric_limits<double>::max();
    for(std::vector<PacCylVoxelSet*>::const_iterator ivs=_cvsets.begin();ivs!=_cvsets.end();ivs++){
      DetIntersection dtest = dinter;
      if( (*ivs)->findNextInter(traj,dtest)  && dtest.pathlen < dbest.pathlen ){
        dbest = dtest;
        _cvset = *ivs;
      }
    }
    if(_cvset != 0){
      retval = true;
      dinter = dbest;
    }
  }
  return retval;
}


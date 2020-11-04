//-----------------------------------------------------------------------
// File and Version Information:
//     PacEmcModel
//
// Copyright Information:
//      Copyright (C) 2008      Caltech
//
//  Authors:  Chih-hsiang Cheng    2008/07/22
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"
#include "ErrLogger/ErrLog.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcAbsCalibrator.hh"
#include "PacEmc/PacEmcCalibrator.hh"
#include "PacEmc/PacEmcCalibratorShift.hh"
#include "PacEnv/EdmlDetElement.hh"
#include "PacEnv/EdmlDetector.hh"
#include "PacEnv/EdmlDetVolume.hh"
#include "PacEnv/EdmlCylDetElement.hh"
#include "PacEnv/EdmlRingDetElement.hh"
#include "PacEnv/EdmlConeDetElement.hh"

#include "AbsEnv/TwoCoordIndex.hh"
#include "PacEnv/PacConfig.hh"
#include "PacEnv/pstring.hh"
#include <TStopwatch.h>
#include <cmath>
#define DBL_LARGE  1e32

using namespace std;

PacEmcModel::PacEmcModel() :
  _initialized(false)
  , _nfwdLayers(0), _nbarrelLayers(0), _nbwdLayers(0)
  , _barrelBegin(0), _barrelEnd(0), _barrelNRings(0), _barrelNPhis(0)
  , _etaBarrelBegin(0), _etaBarrelEnd(0)
  , _deltaBarrelEta(0), _deltaBarrelPhi(0)
  , _fwdBegin(0), _fwdEnd(0), _fwdNRings(0), _deltaFwdTheta(0)
  , _bwdBegin(0), _bwdEnd(0), _bwdNRings(0), _bwdNPhis(0)
  , _deltaBwdTheta(0), _deltaBwdPhi(0)
  , _nCrysFwd(0), _nCrysBarrel(0), _nCrysBwd(0)
  , _firstFwdRing(0), _firstBarrelRing(0), _firstBwdRing(0)
  , _nCrysBeforeFwd(0), _nCrysBeforeBarrel(0), _nCrysBeforeBwd(0)
  , _nbinsPhiSimTrkKE(0), _nbinsThetaSimTrkKE(0)
  , _tciList( new std::vector<TwoCoordIndex*> )
  , _zshift(0), _foundPulseMax(false)
{
}

PacEmcModel::~PacEmcModel()
{}


//===================================================
PacEmcModel* aModel(0);

PacEmcModel* PacEmcModel::createNewModel() {

  ErrMsg(routine) << "Creating a new EMC model " << endmsg;
  if ( aModel ) {
    delete aModel;
  }

  aModel= new PacEmcModel();
  return aModel;
}

PacEmcModel* PacEmcModel::getModel() {

  if ( 0== aModel ) {
    ErrMsg(fatal) << "The EMC model hasn't been created yet." << endmsg;
  }
  
  if ( !aModel->initialized() ) {
    ErrMsg(fatal) << "The EMC model hasn't been initialized yet." << endmsg;
  }

  return aModel;
}

//===================================================

void PacEmcModel::init(const EdmlDetector* detector ) {

  if ( _initialized ) {
    return;
  }

  ErrMsg(routine) << "Initializing EMC model" << endmsg;
  TStopwatch timer;
  timer.Start();

  // Get detector volumes and find the z shift
  bool foundvolume= false;
  std::vector<const EdmlDetVolume* > volumes;
  detector->volumes(volumes);
  for ( std::vector<const EdmlDetVolume* >::const_iterator vitr=volumes.begin(); 
	vitr!=volumes.end();++vitr) {
    if ( (*vitr)->name().find("Calorimeter") != string::npos ) {
      double zshift= (*vitr)->zShift();
      if ( foundvolume ) {
	if ( zshift != _zshift ) {
	  ErrMsg(fatal) << "The Calorimeter volume " << (*vitr)->name() 
			<< " has a z shift= " << zshift 
			<< " that is different from the previous one= "
			<< _zshift << endmsg;
	}
      }
      _zshift= zshift;
      foundvolume= true;
    }
  }
  _origin= Hep3Vector(0,0,_zshift);

  // Get sub-detector elements. Look for EMC and set up geometry
  std::vector<const EdmlDetElement* > elements;
  detector->elements(elements);
  

  // We're interested in EMC sub-detector elements only. To be specific,
  // we'll be looking for one barrel and two endcap elements.
  //
  for(std::vector<const EdmlDetElement* >::const_iterator itr=elements.begin(); itr!=elements.end();++itr) {
    
    if((*itr)->type().find("Cylinder") != string::npos) {
      
      const EdmlCylDetElement* edml_element = dynamic_cast<const EdmlCylDetElement*>(*itr);
      if( edml_element->device() == "Emc" ) {
	// Cylindrical EMC must be a barrel
        this->addBarrel(edml_element->radius(), edml_element->zmin(),
          edml_element->zmax(), edml_element->thick());
      }

    } else if((*itr)->type().find("Ring") != string::npos) {
      const EdmlRingDetElement* edml_element = dynamic_cast<const EdmlRingDetElement*>(*itr);
      if( edml_element->device() == "Emc" ) {
    // Ring EMC can either be forward or backward
        if( edml_element->z() > 0 ) {
          this->addForward(edml_element->lowradius(),edml_element->hiradius(),
            edml_element->z(),edml_element->thick());
        } else {
          this->addBackward(edml_element->lowradius(),edml_element->hiradius(),
            edml_element->z(),edml_element->thick());
        }
      } // if( edml_element->device() == "Emc" )

    } else if((*itr)->type().find("Cone") != string::npos) {
      const EdmlConeDetElement* edml_element = dynamic_cast<const EdmlConeDetElement*>(*itr);
      if( edml_element->device() == "Emc" ) {
    // Cone EMC can either be forward or backward
        if ( edml_element->zvert() > 0 ) {
          this->addForward(edml_element->rmin(), edml_element->rmax(),edml_element->tanth(),
            edml_element->zvert(), edml_element->thick());
        } else {
          this->addBackward(edml_element->rmin(), edml_element->rmax(),edml_element->tanth(),
            edml_element->zvert(), edml_element->thick());
        }
      }// if( edml_element->device() == "Emc" )

    } else {
      ErrMsg(fatal) << "PacCylDetector::PacCylDetector(): unsupported EDML detector type: '" << (*itr)->type() << "'" << endmsg;
      return;
    } 
  }


  // === Calculate the polar angle ranges from the geometry and total thickness ===
  setupThetaRange();

  // Read from config file
  readConfig();


  // Sanity check
  if ( _nbarrelLayers!= _barrelR.size() ||
       _nbarrelLayers!= _barrelZmin.size() ||
       _nbarrelLayers!= _barrelZmax.size() ||
       _nbarrelLayers!= _barrelThick.size() ) {
    ErrMsg(fatal) << "Numbers of Barrel layers do not match!" << endmsg;
  }
  if ( _nfwdLayers!= _fwdRmin.size() ||
       _nfwdLayers!= _fwdRmax.size() ||
       _nfwdLayers!= _fwdTanth.size() ||
       _nfwdLayers!= _fwdZv.size() ||
       _nfwdLayers!= _fwdThick.size() ) {
    ErrMsg(fatal) << "Numbers of FWD layers do not match!" << endmsg;
  }
  if ( _nbwdLayers!= _bwdRmin.size() ||
       _nbwdLayers!= _bwdRmax.size() ||
       _nbwdLayers!= _bwdTanth.size() ||
       _nbwdLayers!= _bwdZv.size() ||
       _nbwdLayers!= _bwdThick.size() ) {
    ErrMsg(fatal) << "Numbers of BWD layers do not match!" << endmsg;
  }


  if ( _barrelNRings <= 0 || _barrelNPhis <= 0 ) {
    ErrMsg(fatal) << "PacEmcModel::init():  Error. emcBarrelNRings = " << _barrelNRings 
		  << ", emcBarrelNPhis = " << _barrelNPhis << ". Doesn't make sense. "
		  << endmsg;
  }

  if ( _fwdNRings <= 0 ) {
    ErrMsg(fatal) << "PacEmcModel::init():  Error. emcFWDNRings = " << _fwdNRings 
		  << ". Doesn't make sense. "
		  << endmsg;
  }
  
  if ( _fwdNRings != _fwdNPhis.size() ) {
    ErrMsg(fatal) << "The size of emcFWDNPhis does not match emcFWDNRings" << endmsg;
  }

  // Numbers of crystals in each sector
  for (unsigned i=0; i< _fwdNRings; i++) {
    _nCrysFwd+= _fwdNPhis[i];
    _fwdNPhisAccum.push_back(_nCrysFwd);
    _deltaFwdPhi.push_back( Constants::twoPi/_fwdNPhis[i] );
  }
  _nCrysBarrel=  _barrelNRings* _barrelNPhis;
  _nCrysBwd=  _bwdNRings* _bwdNPhis;
  
  _nCrysBeforeFwd=0;
  _nCrysBeforeBarrel= _nCrysBeforeFwd + _nCrysFwd;
  _nCrysBeforeBwd= _nCrysBeforeBarrel + _nCrysBarrel;


  // First ring index
  _firstFwdRing = 0;
  _firstBarrelRing = _firstFwdRing + _fwdNRings;
  _firstBwdRing = _firstBarrelRing + _barrelNRings;

  
  // The barrel segmentation in theta is in such a way that the interval in
  //  "pseudo-rapidity" is constant.
  _etaBarrelBegin = log( tan(_barrelBegin/2.));
  _etaBarrelEnd =   log( tan(_barrelEnd/2.));
  _deltaBarrelEta = (_etaBarrelEnd-_etaBarrelBegin)/_barrelNRings;

  _deltaBarrelPhi= Constants::twoPi/_barrelNPhis;
  _deltaFwdTheta= (_fwdEnd-_fwdBegin)/_fwdNRings;
  
  if ( _bwdNPhis > 0 ) _deltaBwdPhi = Constants::twoPi/_bwdNPhis;
  if ( _bwdNRings > 0 ) _deltaBwdTheta = (_bwdEnd-_bwdBegin)/_bwdNRings;

  for ( unsigned i=0; i<_barrelNRings; i++) {
    double eta= _etaBarrelBegin + i*_deltaBarrelEta;
    double th0=  2* atan(exp(eta));
    double th1=  2* atan(exp(eta+_deltaBarrelEta));
    _deltaBarrelTheta.push_back(th1-th0);
  }
				 

  // ====== Build TCI list ============
  unsigned index(0);
  for ( unsigned i= _firstFwdRing; i< _firstFwdRing+_fwdNRings; ++i) {
    for ( unsigned j=0; j< _fwdNPhis[i-_firstFwdRing]; ++j) {
      _tciList->push_back(new TwoCoordIndex(i,j,index));
      ++index;
    }
  }
  for ( unsigned i= _firstBarrelRing; i< _firstBarrelRing+_barrelNRings; ++i) {
    for ( unsigned j=0; j< _barrelNPhis; ++j) {
      _tciList->push_back(new TwoCoordIndex(i,j,index));
      ++index;
    }
  }
  for ( unsigned i= _firstBwdRing; i < _firstBwdRing+_bwdNRings; ++i ) {
    for ( unsigned j=0; j< _bwdNPhis; ++j) {
      _tciList->push_back(new TwoCoordIndex(i,j,index));
      ++index;
    }
  }

  // Add the neighbors to each tci
  for ( unsigned i=0; i< _tciList->size(); ++i) {
    TwoCoordIndex* tci= (*_tciList)[i];
    long ix= tci->itsXCoord();
    long iy= tci->itsYCoord();

    // Add the two neighbors in the same ring
    tci->addToNeighbourList( getTci(ix, iy+1) );
    tci->addToNeighbourList( getTci(ix, iy-1) );

    // shift iy for the neighboring rings if necessary
    if ( ix >= _firstFwdRing && ix < _firstFwdRing+_fwdNRings ) {  // Forward Endcap

      if ( ix > _firstFwdRing ) {
	// Add neighbors in the ring in front
	double ratio = _deltaFwdPhi[ix-_firstFwdRing]/_deltaFwdPhi[ix-_firstFwdRing-1];
	// iy2 starts from an integer >= iy*ratio - 1 and 
	//  ends at an integer <= (iy+1)*ratio
	for (long iy2= (long)(iy*ratio-1+0.99); iy2<= (iy+1)*ratio; iy2++ ) {
	  tci->addToNeighbourList( getTci(ix-1, iy2) );
	}
      }
      if ( ix < _firstFwdRing+ _fwdNRings-1 ) {
	// Add neighbors in the ring in back
	double ratio = _deltaFwdPhi[ix-_firstFwdRing]/_deltaFwdPhi[ix-_firstFwdRing+1];
	for (long iy2= (long)(iy*ratio-1+0.99); iy2<= (iy+1)*ratio; iy2++ ) {
	  tci->addToNeighbourList( getTci(ix+1, iy2) );
	}
      }
      if ( ix == _firstFwdRing+ _fwdNRings-1 && _barrelR.size() > 0 ) {
	// Last FWD ring; find neighbor in barrel
	double ratio = _deltaFwdPhi[ix-_firstFwdRing]/_deltaBarrelPhi;
	for (long iy2= (long)(iy*ratio-1+0.99); iy2<= (iy+1)*ratio; iy2++ ) {
	  tci->addToNeighbourList( getTci(ix+1, iy2) );
	}
      }

    } else if ( ix >= _firstBarrelRing && ix < _firstBarrelRing + _barrelNRings ) {  // Barrel
      
      // Add neighbors in the ring in front
      if ( ix == _firstBarrelRing && _fwdRmin.size()>0 ) { // first barrel ring
	double ratio = _deltaBarrelPhi/_deltaFwdPhi[ix-_firstFwdRing-1];
	for (long iy2= (long)(iy*ratio-1+0.99); iy2<= (iy+1)*ratio; iy2++ ) {
	  tci->addToNeighbourList( getTci(ix-1, iy2) );
	}
      } else {
	tci->addToNeighbourList( getTci(ix-1, iy-1) );
	tci->addToNeighbourList( getTci(ix-1, iy) );
	tci->addToNeighbourList( getTci(ix-1, iy+1) );
      }

      // Add neighbors in the ring in back
      if ( ix < _firstBarrelRing + _barrelNRings -1) {  // not the last ring
        tci->addToNeighbourList( getTci(ix+1, iy-1) );
        tci->addToNeighbourList( getTci(ix+1, iy) );
        tci->addToNeighbourList( getTci(ix+1, iy+1) );
      } else if (  _bwdRmin.size() > 0 )  {  // The last ring in barrel; and BWD exists
	//   There is a big gap between barrel and bacward endcap.
	//     Do not treat them as neighbors
	//double ratio = _deltaBarrelPhi/_deltaBwdPhi;
	//for (long iy2= (long)(iy*ratio-1+0.99); iy2<= (iy+1)*ratio; iy2++ ) {
	//  tci->addToNeighbourList( getTci(ix+1, iy2) );
	//}
      }

    } else if (ix >= _firstBwdRing && ix < _firstBwdRing+ _bwdNRings ) { // BWD
      // Add neighbors in the ring in front
      if ( ix == _firstBwdRing && _barrelR.size()>0 ) { // first bwd ring
	//   There is a big gap between barrel and bacward endcap.
	//     Do not treat them as neighbors
	//double ratio = _deltaBwdPhi/_deltaBarrelPhi;
	//for (long iy2= (long)(iy*ratio-1+0.99); iy2<= (iy+1)*ratio; iy2++ ) {
	//  tci->addToNeighbourList( getTci(ix-1, iy2) );
	//}
      } else {
	tci->addToNeighbourList( getTci(ix-1, iy-1) );
	tci->addToNeighbourList( getTci(ix-1, iy) );
	tci->addToNeighbourList( getTci(ix-1, iy+1) );
      }

      // Add neighbors in the ring in back
      if ( ix < _firstBwdRing+ _bwdNRings-1 ) {// not the last
	tci->addToNeighbourList( getTci(ix+1, iy-1) );
        tci->addToNeighbourList( getTci(ix+1, iy) );
        tci->addToNeighbourList( getTci(ix+1, iy+1) );
      }

    }

  }  // End adding neighbors
 
  // Build energy calibrator
  buildCalibrator();

  if ( !_foundPulseMax ) {
    findPulseMax();
  }


  ErrMsg(routine) << "Done PacEmcModel::init()" << endmsg;

  _initialized = true;
  timer.Stop();
  if(ErrLogging(routine)){
    timer.Print();
    this->print(std::cout);
  }
}

// Energy calibrator
void PacEmcModel::buildCalibrator() {

  PacEmcAbsCalibrator *fwdcalib(0), *barcalib(0), *bwdcalib(0);

  if ( 0== _calibType.compare("Shift") ) {
    fwdcalib= new PacEmcCalibratorShift("FWD");
    barcalib= new PacEmcCalibratorShift("Barrel");
    bwdcalib= new PacEmcCalibratorShift("BWD");
  } else if ( 0== _calibType.compare("AdHoc") ) {
    fwdcalib= new PacEmcCalibrator("FWD");
    barcalib= new PacEmcCalibrator("Barrel");
    bwdcalib= new PacEmcCalibrator("BWD");
  } else {
    ErrMsg(fatal) << "Unknown PacEmc calibration type " << _calibType << "." << endmsg;
  }
  
  fwdcalib->setParams(_calibParsFWD);
  barcalib->setParams(_calibParsBarrel);
  bwdcalib->setParams(_calibParsBWD);
  _calibrators.push_back(fwdcalib);
  _calibrators.push_back(barcalib);
  _calibrators.push_back(bwdcalib);

}

const PacEmcAbsCalibrator* PacEmcModel::getCalibrator(EmcRegion region) const {
  return _calibrators[region];
}


void
PacEmcModel::addBarrel(const double R, const double zmin, const double zmax, const double thick) {
  _barrelR.push_back(R);
  _barrelZmin.push_back(zmin);
  _barrelZmax.push_back(zmax);
  _barrelThick.push_back(thick);
  ++_nbarrelLayers;
}

void
PacEmcModel::addForward(const double Rmin, const double Rmax, const double tanth, const double zv, const double thick) {
  _fwdRmin.push_back(Rmin);
  _fwdRmax.push_back(Rmax);
  _fwdTanth.push_back(tanth);
  _fwdZv.push_back(zv);
  _fwdThick.push_back(thick);
  ++_nfwdLayers;
}
void
PacEmcModel::addForward(const double Rmin, const double Rmax, const double zv, const double thick) {
  _fwdRmin.push_back(Rmin);
  _fwdRmax.push_back(Rmax);
  _fwdTanth.push_back(DBL_LARGE);
  _fwdZv.push_back(zv);
  _fwdThick.push_back(thick);
  ++_nfwdLayers;
}

void
PacEmcModel::addBackward(const double Rmin, const double Rmax, const double tanth, const double zv, const double thick) {
  _bwdRmin.push_back(Rmin);
  _bwdRmax.push_back(Rmax);
  _bwdTanth.push_back(tanth);
  _bwdZv.push_back(zv);
  _bwdThick.push_back(thick);
  ++_nbwdLayers;
}
void
PacEmcModel::addBackward(const double Rmin, const double Rmax, const double zv, const double thick) {
  _bwdRmin.push_back(Rmin);
  _bwdRmax.push_back(Rmax);
  _bwdTanth.push_back(DBL_LARGE);
  _bwdZv.push_back(zv);
  _bwdThick.push_back(thick);
  ++_nbwdLayers;
}


void
PacEmcModel::setupThetaRange() {

  bool hasFwd(false), hasBarrel(false), hasBwd(false);
  // === Calculate the polar angle ranges from the geometry and total thickness ===
  double costh(0),sinth(0);
  for ( unsigned i=0; i<3; ++i) _thickness.push_back(0.);

  //  - Barrel 
  for (unsigned i=0; i<_barrelR.size(); ++i) {
    double th1= sAtan(_barrelR[i]/_barrelZmax[i]);
    double th2= sAtan(_barrelR[i]/_barrelZmin[i]);
    if ( th1 > th2 ) {
      double tmp= th1;
      th1=th2;
      th2=tmp;
    }
    if ( i == 0 ) {
      _barrelBegin= th1;
      _barrelEnd= th2;
    } else {
      if ( th1 < _barrelBegin ) _barrelBegin= th1;
      if ( th2 > _barrelEnd )   _barrelEnd= th2;
    }
    _thickness[Barrel] += _barrelThick[i];
    hasBarrel= true;
  }

  //  - Forward
  for (unsigned i=0; i<_fwdRmin.size(); ++i) {
    if ( _fwdTanth[i] < DBL_LARGE ) {
      sinth= fabs(_fwdTanth[i])/sqrt(1+ _fwdTanth[i]* _fwdTanth[i]);
      costh= sinth/_fwdTanth[i];
    } else {
      sinth= 1.;
      costh= 0.;
    }
    double th1= sAtan( _fwdRmin[i]*sinth / (_fwdZv[i]+ _fwdRmin[i] * costh) );
    double th2= sAtan( _fwdRmax[i]*sinth / (_fwdZv[i]+ _fwdRmax[i] * costh) );
    if ( th1 > th2 ) {
      double tmp= th1;
      th1=th2;
      th2=tmp;
    }

    if ( i == 0 ) {
      _fwdBegin = th1;
      _fwdEnd = th2;
    } else {
      if ( th1 < _fwdBegin ) _fwdBegin= th1;
      if ( th2 > _fwdEnd ) _fwdEnd= th2;
    }
    _thickness[FWD] += _fwdThick[i];
    hasFwd= true;
  }

  //  - Backward
  for (unsigned i=0; i<_bwdRmin.size(); ++i) {
    if ( _bwdTanth[i] < DBL_LARGE ) {
      sinth= fabs(_bwdTanth[i])/sqrt(1+ _bwdTanth[i]* _bwdTanth[i]);
      costh= sinth/_bwdTanth[i];
    } else {
      sinth= 1.;
      costh= 0.;
    }
    double th1= sAtan( _bwdRmax[i]*sinth / (_bwdZv[i]+ _bwdRmax[i] * costh) );
    double th2= sAtan( _bwdRmin[i]*sinth / (_bwdZv[i]+ _bwdRmin[i] * costh) );
    if ( th1 > th2 ) {
      double tmp= th1;
      th1=th2;
      th2=tmp;
    }

    if ( i == 0 ) {
      _bwdBegin = th1;
      _bwdEnd = th2;
    } else {
      if ( th1 < _bwdBegin ) _bwdBegin= th1;
      if ( th2 > _bwdEnd ) _bwdEnd= th2;
    }
    _thickness[BWD] += _bwdThick[i];
    hasBwd= true;
  }

  // Some sanity checks
  if ( hasBarrel && _barrelBegin > _barrelEnd ) {
    ErrMsg(fatal) << "Barrel begin > Barrel end" << endmsg;
  }
  if ( hasFwd && _fwdBegin > _fwdEnd ) {
    ErrMsg(fatal) << "Forward begin > Forward end" << endmsg;
  }
  if ( hasBwd && _bwdBegin > _bwdEnd ) {
    ErrMsg(fatal) << "Backward begin > Backward end" << endmsg;
  }

  if ( hasFwd && hasBarrel &&_fwdEnd > _barrelBegin ) {
    ErrMsg(fatal) << "Forward and barrel overlap (forwardEnd= " << _fwdEnd
		  << "  barrelBegin= " << _barrelBegin << ")" << endmsg;
  }
  if ( hasBwd && hasBarrel && _barrelEnd > _bwdBegin ) {
    ErrMsg(fatal) << "Backward and barrel overlap (backwardEnd= " << _fwdEnd
		  << "  barrelBegin= " << _barrelBegin << ")" << endmsg;
  }

}

void
PacEmcModel::readConfig() {

  _barrelNRings = gconfig.getint("PacEmcGeom.barrelNRings");
  _barrelNPhis =  gconfig.getint("PacEmcGeom.barrelNPhis");

  _fwdNRings = gconfig.getint("PacEmcGeom.fwdNRings",0);
  _fwdNPhis =  gconfig.getvector("PacEmcGeom.fwdNPhis",_fwdNPhis);

  _bwdNRings = gconfig.getint("PacEmcGeom.bwdNRings",0);
  _bwdNPhis =  gconfig.getint("PacEmcGeom.bwdNPhis",0); 

  _nbinsPhiSimTrkKE = gconfig.getint("PacEmcGeom.nbinsPhiSimTrkKE",120);
  _nbinsThetaSimTrkKE = gconfig.getint("PacEmcGeom.nbinsThetaSimTrkKE",80);
  _simTrkKEThreshold = gconfig.getdouble("PacEmcGeom.simTrkKEThreshold", 0.010);

  _rMoliere = gconfig.getvector("PacEmcGeom.rMoliere",_rMoliere);
  _rMoliereFluc = gconfig.getvector("PacEmcGeom.rMoliereFluc",_rMoliereFluc);
  
  _crystalGap = gconfig.getvector("PacEmcGeom.crystalGap", _crystalGap);

  _eResCoef = gconfig.getvector("PacEmcGeom.EnergyResCoef", _eResCoef);
  _eResConst = gconfig.getvector("PacEmcGeom.EnergyResConst", _eResConst);
  _eResEPower = gconfig.getvector("PacEmcGeom.EnergyResEPower", _eResEPower);
  _eResExpCoef = gconfig.getvector("PacEmcGeom.EnergyResExpCoef", _eResExpCoef);
  _eResExpConst = gconfig.getvector("PacEmcGeom.EnergyResExpConst", _eResExpConst);
  _eResExpEPower = gconfig.getvector("PacEmcGeom.EnergyResExpEPower", _eResExpEPower);

  _thetaResolution = gconfig.getvector("PacEmcGeom.thetaResolution", _thetaResolution);
  _phiResolution = gconfig.getvector("PacEmcGeom.phiResolution", _phiResolution);
  _radialResolution = gconfig.getvector("PacEmcGeom.radialResolution", _radialResolution);

  _hadShowerSigma =  gconfig.getvector("PacEmcGeom.hadShowerSigma", _hadShowerSigma);
  _hadShowerMaxR =   gconfig.getvector("PacEmcGeom.hadShowerMaxR", _hadShowerMaxR);
  _hadShowerQuanta = gconfig.getvector("PacEmcGeom.hadShowerQuanta", _hadShowerQuanta);
  _hadShowerSmear =  gconfig.getvector("PacEmcGeom.hadShowerSmear", _hadShowerSmear);
  _hadShowerMinE =   gconfig.getvector("PacEmcGeom.hadShowerMinE", _hadShowerMinE);
  _ecutoff =         gconfig.getvector("PacEmcGeom.ecutoff", _ecutoff);
  _digiThreshold =   gconfig.getvector("PacEmcGeom.digiThreshold", _digiThreshold);

  _emEssentricity = gconfig.getvector("PacEmcGeom.emEssentricity", _emEssentricity);
  _emEssenRMS = gconfig.getvector("PacEmcGeom.emEssenRMS", _emEssenRMS);
  _emAsymmetry = gconfig.getvector("PacEmcGeom.emAsymmetry", _emAsymmetry);

  _noiseOccupancy = gconfig.getvector("PacEmcGeom.noiseOccupancy", _noiseOccupancy);
  _noiseOccSigma = gconfig.getvector("PacEmcGeom.noiseOccSigma", _noiseOccSigma);
  _noiseExpConst = gconfig.getvector("PacEmcGeom.noiseExpConst", _noiseExpConst);


  _emShowerEMfrac = gconfig.getdouble("PacEmcGeom.emShowerEMfrac",1);
  _emShowerEMfracSigma = gconfig.getdouble("PacEmcGeom.emShowerEMfracSigma",0);
  _hadShowerEMfrac = gconfig.getdouble("PacEmcGeom.hadShowerEMfrac",0);
  _hadShowerEMfracSigma = gconfig.getdouble("PacEmcGeom.hadShowerEMfracSigma",0);

  _showerDepth = gconfig.getvector("PacEmcGeom.showerDepth", _showerDepth);

  _calibType = gconfig.getcstr("PacEmcGeom.calibType", "");
  _calibType.erase(_calibType.find_last_not_of(" ")+1 );
  _calibParsFWD = gconfig.getvector("PacEmcGeom.calibParsFWD", _calibParsFWD);
  _calibParsBarrel = gconfig.getvector("PacEmcGeom.calibParsBarrel", _calibParsBarrel);
  _calibParsBWD = gconfig.getvector("PacEmcGeom.calibParsBWD", _calibParsBWD);

//   _calibP0 = gconfig.getvector("PacEmcGeom.calibP0", _calibP0);
//   _calibP1 = gconfig.getvector("PacEmcGeom.calibP1", _calibP1);
//   _calibP2 = gconfig.getvector("PacEmcGeom.calibP2", _calibP2);
//   _calibP3 = gconfig.getvector("PacEmcGeom.calibP3", _calibP3);

  // signal timing window
  _signalTLo = gconfig.getvector("PacEmcGeom.signalWindowTLow", _signalTLo);
  _signalTHi = gconfig.getvector("PacEmcGeom.signalWindowTHigh", _signalTHi);
  // 
  _acceptWindowLo = gconfig.getvector("PacEmcGeom.acceptedWindowTLow", _acceptWindowLo);
  // allowed time window for background events, must be smaller than signalTLo
  _bkgWindowLo = gconfig.getvector("PacEmcGeom.backgroundWindowTLow", _bkgWindowLo);

  // signal shape parameter
  _sigF1 = gconfig.getvector("PacEmcGeom.sigLifeF1", _sigF1);
  _sigTau1 = gconfig.getvector("PacEmcGeom.sigTau1", _sigTau1);
  _sigTau2 = gconfig.getvector("PacEmcGeom.sigTau2", _sigTau2);
  _sigFilterT1 = gconfig.getvector("PacEmcGeom.sigFilterT1", _sigFilterT1);
  _sigFilterT2 = gconfig.getvector("PacEmcGeom.sigFilterT2", _sigFilterT2);

  // hit time resolution: gaussian sigma
  _timeResolution = gconfig.getvector("PacEmcGeom.timeResolution", _timeResolution);

}

const long
PacEmcModel::getTciIndex(const long ix0, const long iy0) const {
  long retval(-1);
  
  if ( ix0 < 0 ) return retval;

  long ix(ix0), iy(iy0);
  
  retval=0;

  EmcRegion theRegion= region(ix);

  switch ( theRegion ) {
    
  case PacEmcModel::FWD :
    if ( ix > _firstFwdRing) retval += _fwdNPhisAccum[ix-_firstFwdRing-1];

    iy = (long) fmod((double)iy, (double)_fwdNPhis[ix]);
    if ( iy < 0 ) iy+= _fwdNPhis[ix];

    retval+= iy;
    break;

  case PacEmcModel::Barrel :
    iy = (long) fmod((double)iy, (double)_barrelNPhis);
    if ( iy < 0 ) iy+= _barrelNPhis;

    retval= _nCrysFwd + (ix - _fwdNRings) * _barrelNPhis + iy;
    break;

  case PacEmcModel::BWD :
    iy = (long) fmod((double)iy, (double)_bwdNPhis);
    if ( iy < 0 ) iy+= _bwdNPhis;

    retval= _nCrysFwd+_nCrysBarrel + (ix-_fwdNRings-_barrelNRings)*_bwdNPhis + iy;
    break;

  default:
    break;
  }
  
  return retval;
  
}


const TwoCoordIndex*
PacEmcModel::getTci(long index) const {

  if ( index <0 || index >= _tciList->size() ) {
    return 0;
  }
  return (*_tciList)[index];

}

const TwoCoordIndex*
PacEmcModel::getTci(long ix, long iy) const {

  const long index = getTciIndex(ix, iy);
  if ( index < 0 || index >= _tciList->size() ) return 0;

  return (*_tciList)[index];

}


const TwoCoordIndex* 
PacEmcModel::realCoordToTci(const HepPoint &pos) const {
  return localCoordToTci(pos-_origin);
}

const TwoCoordIndex* 
PacEmcModel::localCoordToTci(const HepPoint &pos) const {
  
  const double theta= pos.theta();
  const double phi= pos.phi();

  double eta= log(tan(theta/2.));
  long iTheta(-1);
  long iPhi(-1);      // Both theta and phi indices start with 0

  // map phi from (-pi,pi) to (0,2*pi)
  double myphi= phi;
  myphi = fmod( myphi, Constants::twoPi);

  if ( myphi < 0 ) {
    myphi+= Constants::twoPi;
  }

  EmcRegion theRegion= regionLocal(theta);

  switch ( theRegion ) {
  case PacEmcModel::FWD :
    iTheta= (long)(( theta - _fwdBegin )/ _deltaFwdTheta) + _firstFwdRing;
    iPhi = (long)(myphi/_deltaFwdPhi[iTheta]);
    break;
  case PacEmcModel::Barrel :
    iTheta= (long)(( eta - _etaBarrelBegin )/_deltaBarrelEta) + _firstBarrelRing;
    iPhi= (long)(myphi / _deltaBarrelPhi);
    break;
  case PacEmcModel::BWD :
    iTheta= (long)(( theta - _bwdBegin )/ _deltaBwdTheta) + _firstBwdRing;
    iPhi = (long)(myphi/_deltaBwdPhi);
    break;
  default:
    break;
  }

  //   cout << "PacEmcModel::realCoordToTci    theta= " << theta
  //        << "  phi= " << phi << endl;
  //   cout << "iTheta = " << iTheta << "    iPhi = " << iPhi << endl;

  long theindex= getTciIndex(iTheta,iPhi);
  //  cout << "index= " << theindex << endl;

  if ( theindex < 0 || theindex >= _tciList->size() )  return 0;
  return (*_tciList)[ theindex ];
}


const double 
PacEmcModel::tciToRealThetaLocal(const TwoCoordIndex& tci) const {
  
  double retval(0), eta(0);
  long itheta= tci.itsXCoord();

  EmcRegion theRegion= region(itheta);
  if ( theRegion== PacEmcModel::Unknown ) {
    tci.print(std::cout);
    ErrMsg(fatal) << "Unknown region at the tci shown above." << endmsg;
  }

  switch( theRegion ) {
  case PacEmcModel::FWD :
    retval= (itheta- _firstFwdRing+ 0.5) * _deltaFwdTheta + _fwdBegin;
    break;
  case PacEmcModel::Barrel :
    eta= (itheta-_firstBarrelRing+ 0.5)*_deltaBarrelEta+_etaBarrelBegin;
    retval = 2* atan(exp(eta));
    break;
  case PacEmcModel::BWD :
    retval= (itheta-_firstBwdRing+ 0.5) * _deltaBwdTheta + _bwdBegin;
    break;
  default:
    break;
  }

  return retval;
}


const double 
PacEmcModel::tciToRealPhiLocal(const TwoCoordIndex& tci) const {

  double retval(0);
  long itheta= tci.itsXCoord();
  long iphi= tci.itsYCoord();

  EmcRegion theRegion= region(itheta);
  if ( theRegion== PacEmcModel::Unknown ) {
    tci.print(std::cout);
    ErrMsg(fatal) << "Unknown region at the tci shown above." << endmsg;
  }

  switch( theRegion ) {
  case PacEmcModel::FWD :
    retval= (iphi+0.5)* _deltaFwdPhi[itheta];
    break;
  case PacEmcModel::Barrel :
    retval= (iphi+0.5)* _deltaBarrelPhi;
    break;
  case PacEmcModel::BWD :
    retval= (iphi+0.5)* _deltaBwdPhi;
    break;
  default:
    break;
  }

  // map to (-pi, pi)
  retval = fmod(retval, Constants::twoPi);

  if ( retval > Constants::pi ) {
    retval-= Constants::twoPi;
  }
  if ( retval < -Constants::pi ) {
    retval+= Constants::twoPi;
  }

  return retval;

}

const HepPoint
PacEmcModel::where(const TwoCoordIndex& tci, const int eid ) const {
  return whereLocal(tci,eid)+ _origin;
}

const HepPoint
PacEmcModel::whereLocal(const TwoCoordIndex& tci, const int eid ) const {
  double phi= tciToRealPhiLocal(tci);
  double theta= tciToRealThetaLocal(tci);
  return whereLocal(theta,phi,eid);
}

const HepPoint 
PacEmcModel::whereLocal(const double theta, const double phi, const int eid) const {
  // If eid<0 (default), return the location at the EMC using certain shower depth.
  // Otherwise, return the location at the _center_ of the element #eid.

  HepPoint retval(0,0,0);
  int id(0);
  double thick(0), depth(0);
  EmcRegion theRegion= regionLocal(theta);
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "Unknown region at theta= " << theta << endmsg;
  }
  if ( eid>=0 ) { // check if eid is within range
    int eidmax(0);
    switch( theRegion ) {
    case PacEmcModel::FWD :
      eidmax= _nfwdLayers-1;
      break;
    case PacEmcModel::Barrel :
      eidmax= _nbarrelLayers-1;
      break;
    case PacEmcModel::BWD :
      eidmax= _nbwdLayers-1;
      break;
    default:
      eidmax=0;
      break;
    }
    if ( eid > eidmax ) {
      id= eidmax;
    } else {
      id= eid;
    }
  } else {
    switch( theRegion ) {
    case PacEmcModel::FWD :
      thick= _fwdThick[0];
      break;
    case PacEmcModel::Barrel :
      thick= _barrelThick[0];
      break;
    case PacEmcModel::BWD :
      thick= _bwdThick[0];
      break;
    default:
      break;
    }
    depth= _showerDepth[theRegion];
    id=0;
  }

  double theZ(0), theR(0);
  double tantheta= tan(theta);

  switch( theRegion ) {
  case PacEmcModel::FWD :
    if ( _fwdTanth[id] < DBL_LARGE ) { // Cone
      double sinth= fabs(_fwdTanth[id])/sqrt(1+_fwdTanth[id]*_fwdTanth[id]);
      theZ = (_fwdZv[id]- 0.5*thick/sinth )/( 1- tantheta/_fwdTanth[id]);
    } else { // Ring
      theZ = _fwdZv[id]-0.5*thick;
    }
    theZ+= depth*cos(theta);
    theR = theZ * tantheta;
    break;
  case PacEmcModel::Barrel :
    theR = _barrelR[id]-0.5*thick + depth*sin(theta);
    theZ = theR/tantheta;
    break;
  case PacEmcModel::BWD :
    if ( _bwdTanth[id] < DBL_LARGE ) { // Cone
      double sinth= fabs(_bwdTanth[id])/sqrt(1+_bwdTanth[id]*_bwdTanth[id]);
      theZ = (_bwdZv[id]+ 0.5*thick/sinth )/( 1- tantheta/_bwdTanth[id]);
    } else { // Ring
      theZ = _bwdZv[id]+ 0.5*thick;
    }
    theZ+= depth*cos(theta);
    theR = theZ * tantheta;
    break;
  default:
    break;
  }

  retval.setX( theR * cos(phi) );
  retval.setY( theR * sin(phi) );
  retval.setZ( theZ );

  return retval;
}


PacEmcModel::EmcRegion
PacEmcModel::region(const long i) const {
  if ( i < _firstFwdRing ) return PacEmcModel::Unknown;
  if ( i < _firstBarrelRing )  return PacEmcModel::FWD;
  else if ( i < _firstBwdRing ) return PacEmcModel::Barrel;
  else if ( i < _firstBwdRing+_bwdNRings ) return PacEmcModel::BWD; 
  return PacEmcModel::Unknown;
}

// PacEmcModel::EmcRegion
// PacEmcModel::region(const TwoCoordIndex* tci) const {
//   long i= tci->itsXCoord();
//   return region(i);
// }

PacEmcModel::EmcRegion
PacEmcModel::region(const HepPoint &point) const {
  return regionLocal((point-this->origin()).theta());
}

PacEmcModel::EmcRegion
PacEmcModel::regionLocal(const double &theta) const {
  
  if ( theta >= _fwdBegin && theta < _fwdEnd )       return PacEmcModel::FWD;
  if ( theta >= _barrelBegin && theta < _barrelEnd ) return PacEmcModel::Barrel;
  if ( theta >= _bwdBegin && theta < _bwdEnd )       return PacEmcModel::BWD;
  
  return PacEmcModel::Unknown;
}


const double
PacEmcModel::deltaFwdPhi(const long i) const {
  if ( region(i) ==  PacEmcModel::FWD ) return _deltaFwdPhi[i];
  
  return 0;
}

const double
PacEmcModel::deltaBarrelTheta(const long i) const {
  if ( region(i) == PacEmcModel::Barrel ) {
    return _deltaBarrelTheta[i- _firstBarrelRing];
  } 

  return 0.f;
}

const double
PacEmcModel::deltaTheta(const long itheta) const {

  double retval(0);

  switch ( region(itheta) ) {
  case PacEmcModel::FWD :
    retval = deltaFwdTheta() ;
    break;
  case PacEmcModel::Barrel :
    retval = deltaBarrelTheta(itheta);
    break;
  case PacEmcModel::BWD :
    retval = deltaBwdTheta();
    break;
  default:
    ErrMsg(fatal) << "PacEmcModel::deltaTheta  : Error, unknown region" << endmsg;
    break;
  }

  return retval;
}


const double
PacEmcModel::deltaTheta(const TwoCoordIndex& tci) const {
  return deltaTheta( tci.itsXCoord() );
}


const double 
PacEmcModel::deltaPhi(const long itheta) const {
  double retval(0);

  switch ( region(itheta) ) {
  case PacEmcModel::FWD :
    retval = deltaFwdPhi(itheta) ;
    break;
  case PacEmcModel::Barrel :
    retval = deltaBarrelPhi();
    break;
  case PacEmcModel::BWD :
    retval = deltaBwdPhi();
    break;
  default:
    ErrMsg(fatal) << "PacEmcModel::deltaPhi  : Error, unknown region" << endmsg;
    break;
  }

  return retval;
}

const double 
PacEmcModel::deltaPhi(const TwoCoordIndex& tci) const {
  return deltaPhi( tci.itsXCoord() );
}

const double
PacEmcModel::rMoliere( const TwoCoordIndex& tci) const {

  EmcRegion theRegion= region(tci.itsXCoord());

  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::rMoliere  : Error, unknown region" << endmsg;
  }

  return _rMoliere[theRegion];

}
  
const double
PacEmcModel::rMoliereFluc( const TwoCoordIndex& tci) const {
  
  EmcRegion theRegion= region(tci.itsXCoord());
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::rMoliereFluc  : Error, unknown region" << endmsg;
  }
  return _rMoliereFluc[theRegion];

}

const double
PacEmcModel::crystalGap( const TwoCoordIndex& tci) const {
  
  EmcRegion theRegion= region(tci.itsXCoord());
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::crystalGap  : Error, unknown region" << endmsg;
  }
  return _crystalGap[theRegion];

}

const double
PacEmcModel::energyResCoef( const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
  return energyResCoef(theRegion);
}

const double
PacEmcModel::energyResCoef( EmcRegion theRegion) const {

  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::energyResCoef  : Error, unknown region" << endmsg;
  }

  return _eResCoef[theRegion];
}

const double
PacEmcModel::energyResConst( const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
  return energyResConst(theRegion);
}

const double
PacEmcModel::energyResConst( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::energyResConst  : Error, unknown region" << endmsg;
  }
  
  return _eResConst[theRegion];
}

const double
PacEmcModel::energyResEPower( const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
  return energyResEPower(theRegion);
}

const double
PacEmcModel::energyResEPower( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::energyResEPower  : Error, unknown region" << endmsg;
  }  
  return _eResEPower[theRegion];
}

const double
PacEmcModel::energyResExpCoef( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::energyResExpCoef  : Error, unknown region" << endmsg;
  }  
  return _eResExpCoef[theRegion];
}

const double
PacEmcModel::energyResExpConst( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::energyResExpConst  : Error, unknown region" << endmsg;
  }  
  return _eResExpConst[theRegion];
}

const double
PacEmcModel::energyResExpEPower( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::energyResExpEPower  : Error, unknown region" << endmsg;
  }  
  return _eResExpEPower[theRegion];
}


const double
PacEmcModel::thetaResolution( const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
  return thetaResolution(theRegion);
}

const double
PacEmcModel::thetaResolution( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::thetaResolution  : Error, unknown region" << endmsg;
  }
  
  return _thetaResolution[theRegion];
}

const double
PacEmcModel::phiResolution( const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
  return phiResolution(theRegion);
}

const double
PacEmcModel::phiResolution( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::phiResolution  : Error, unknown region" << endmsg;
  }
  
  return _phiResolution[theRegion];
}

const double
PacEmcModel::radialResolution( const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
  return radialResolution(theRegion);
}

const double
PacEmcModel::radialResolution( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::radialResolution  : Error, unknown region" << endmsg;
  }
  
  return _radialResolution[theRegion];
}

const double
PacEmcModel::radialResolution(const long itheta) const {
  EmcRegion theRegion= region(itheta);
  return radialResolution(theRegion);
}

const double
PacEmcModel::thickness( const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
  return thickness(theRegion);
}
const double
PacEmcModel::thickness( EmcRegion theRegion) const {
  
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::thickness  : Error, unknown region" << endmsg;
  }
  return _thickness[theRegion];
}


const int
PacEmcModel::nPhis(const long theta) const {
  EmcRegion theRegion= region(theta);

  int retval(0);
  switch ( theRegion ) {
    
  case PacEmcModel::FWD :
    retval= _fwdNPhis[theta-_firstFwdRing];
    break;
  case PacEmcModel::Barrel :
    retval= barrelNPhis();
    break;
  case PacEmcModel::BWD :
    retval= bwdNPhis();
    break;
  default:
    break;
  }

  return retval;
}




////////
const double
PacEmcModel::hadShowerSigma(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::hadShowerSigma  : Error, unknown region" << endmsg;
  }
  
  return _hadShowerSigma[theRegion];
}

const double
PacEmcModel::hadShowerMaxR(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::hadShowerMaxR  : Error, unknown region" << endmsg;
  }
  
  return _hadShowerMaxR[theRegion];
}

const double
PacEmcModel::hadShowerQuanta(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::hadShowerQuanta  : Error, unknown region" << endmsg;
  }
  
  return _hadShowerQuanta[theRegion];
}

const double
PacEmcModel::hadShowerSmear(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::hadShowerSmear  : Error, unknown region" << endmsg;
  }
  
  return _hadShowerSmear[theRegion];
}

const double
PacEmcModel::hadShowerMinE(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::hadShowerMinE  : Error, unknown region" << endmsg;
  }
  
  return _hadShowerMinE[theRegion];
}

const double
PacEmcModel::ecutoff(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::ecutoff  : Error, unknown region" << endmsg;
  }
  
  return _ecutoff[theRegion];
}

const double
PacEmcModel::digiThreshold(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::digiThreshold  : Error, unknown region" << endmsg;
  }
  
  return _digiThreshold[theRegion];
}

const double
PacEmcModel::emEssentricity(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::emEssentricity  : Error, unknown region" << endmsg;
  }
  
  return _emEssentricity[theRegion];
}

const double
PacEmcModel::emEssenRMS(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::emEssenRMS  : Error, unknown region" << endmsg;
  }
  
  return _emEssenRMS[theRegion];
}

const double
PacEmcModel::emAsymmetry(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::emAsymmetry  : Error, unknown region" << endmsg;
  }
  
  return _emAsymmetry[theRegion];
}

const double
PacEmcModel::noiseOccupancy(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::noiseOccupancy  : Error, unknown region" << endmsg;
  }
  
  return _noiseOccupancy[theRegion];
}

const double
PacEmcModel::noiseOccSigma(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::noiseOccSigma  : Error, unknown region" << endmsg;
  }
  
  return _noiseOccSigma[theRegion];
}

const double
PacEmcModel::noiseExpConst(const TwoCoordIndex& tci) const {
 
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::noiseExpConst  : Error, unknown region" << endmsg;
  }
  
  return _noiseExpConst[theRegion];
}


const double
PacEmcModel::showerDepth(const TwoCoordIndex& tci) const {
  EmcRegion theRegion= region(tci.itsXCoord());
 
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::showerDepth  : Error, unknown region" << endmsg;
  }
  return _showerDepth[theRegion];

}

const double
PacEmcModel::timeResolution(const PacEmcModel::EmcRegion theRegion) const {
  if ( theRegion == PacEmcModel::Unknown ) {
    ErrMsg(fatal) << "PacEmcModel::timeResolution  : Error, unknown region" << endmsg;
  }
  return _timeResolution[theRegion];

}

const double
PacEmcModel::signalFraction(const double& t0, const EmcRegion &theRegion) const {

  // t0 = simhit time
  if ( (t0 < _signalTLo[theRegion] && t0 < _bkgWindowLo[theRegion] ) ||
       t0 > _signalTHi[theRegion] ) {
    return 0;
  }


  // The pulse height (max=1) at expected signal maximum time
  return  pulseShapeNorm(_pulseTshift[theRegion],t0,theRegion);
}





const double
PacEmcModel::pulseShape(const double t, const double t0, const PacEmcModel::EmcRegion re) const {

  double x= t-t0;
  if ( x<0 ) return 0;

  double sf= _sigF1[re]/_sigTau1[re] * exp(-x/_sigTau1[re]);
  if ( _sigTau2[re]>0 ) {
    sf+= (1-_sigF1[re])/_sigTau2[re] * exp(-x/_sigTau2[re]);
  }

  double filt= 0;
  if ( _sigFilterT1[re] == _sigFilterT2[re] ) {
    filt= x * exp(-x/_sigFilterT1[re]) / _sigFilterT1[re];
  } else {
    filt=  _sigFilterT1[re]*(exp(-x/_sigFilterT1[re])-exp(-x/_sigFilterT2[re])) / (_sigFilterT1[re]-_sigFilterT2[re]);
  }

  return sf*filt;
}

const double
PacEmcModel::pulseShapeNorm(const double t, const double t0, const PacEmcModel::EmcRegion re) const {
  assert(re<_pulseShapeMax.size());
  return pulseShape(t,t0,re)/_pulseShapeMax[re];
}

void
PacEmcModel::findPulseMax() {
  for ( int re= FWD; re< NRegions; re++) {
    double tm(0), pm(0);
    double tup= 1.1* ( _sigFilterT1[re]+_sigFilterT2[re])/2.;
    double dt= tup/400.;
    for ( int j=1; j< 400; j++) {
      double tp= j*dt;
      double ph= pulseShape(tp,0,(PacEmcModel::EmcRegion)re);
      if ( ph > pm ) {
	tm= tp;
	pm= ph;
      } else {
	break;
      }
    }

    _pulseShapeMax.push_back(pm);
    _pulseTshift.push_back(tm);

  }
  _foundPulseMax= true;
}


void
PacEmcModel::print( ostream& o ) {

  o << "The EMC Model : \n";
  o << "  origin = " << _origin << "\n";
  o << " Barrel \n" ;
  if ( _barrelR.size() == 0 ) {
    o << " IS NOT CONFIGURED! ********* \n";
  }
  for (unsigned i=0; i<_barrelR.size(); i++) {
    o << " R= " << _barrelR[i] << "  Zmin= " << _barrelZmin[i]
      << "  Zmax= " << _barrelZmax[i] << "  Thick= " << _barrelThick[i]
      << "\n";
  }

  o << " Forward endcap\n";
  if ( _fwdRmin.size() == 0 ) {
    o << " IS NOT CONFIGURED! ********* \n";
  }
  for (unsigned i=0; i<_fwdRmin.size(); i++) {
    o << " lowR= " << _fwdRmin[i] << "  hiR= " << _fwdRmax[i]
      << "  z= " << _fwdZv[i];
    if ( _fwdTanth[i] < DBL_LARGE ) {
      o << "  tanth= " << _fwdTanth[i] ;
    }
    o << "  Thick= " << _fwdThick[i] << "\n";
  }

  o << " Backward endcap\n" ;
  if ( _bwdRmin.size() == 0 ) {
    o << " IS NOT CONFIGURED! ********* \n";
  }
  for (unsigned i=0; i<_bwdRmin.size(); i++) {
    o << " lowR= " << _bwdRmin[i] << "  hiR= " << _bwdRmax[i]
      << "  z= " << _bwdZv[i];
    if ( _bwdTanth[i] < DBL_LARGE ) {
      o << "  tanth= " << _bwdTanth[i] ;
    }
    o << "  Thick= " << _bwdThick[i] << "\n";
  }

  o << "\n";
  o << "  Forward endcap begins at  " << _fwdBegin  << "  rad.\n";
  o << "  Forward endcap ends at    " << _fwdEnd  << "  rad.\n";
  o << "  Barrel begins at          " << _barrelBegin << "  rad.\n";
  o << "  Barrel ends at            " << _barrelEnd << "  rad.\n";
  o << "  Backward endcap begins at " << _bwdBegin  << "  rad.\n";
  o << "  Backward endcap ends at   " << _bwdEnd  << "  rad.\n";

  o << "  Forward endcap has " << _fwdNRings << " rings.\n";
  o << "    which have ";
  for ( unsigned i=0; i< _fwdNRings ; i++ ) {
    o << _fwdNPhis[i] << "  ";
  }
  o << " crystals.\n";
  o << "  Barrel has " << _barrelNRings << " rings.\n";
  o << "   each has " << _barrelNPhis << " crystals.\n";
  o << "  Backward endcap has " << _bwdNRings << " rings.\n";
  o << "   each has " << _bwdNPhis << " crystals or sections.\n";

  o << "  There are total of " << _nCrysFwd << " crystals in the forward endcap.\n";
  o << "  There are total of " << _nCrysBarrel << " crystals in the barrel.\n";
  o << "  There are total of " << _nCrysBwd << " sections in the backward endcap.\n\n";
  o << "  Moliere radius:  FWD = " << _rMoliere[PacEmcModel::FWD] << " cm"
    << "  Barrel = " << _rMoliere[PacEmcModel::Barrel] << " cm"
    << "  BWD = " << _rMoliere[PacEmcModel::BWD] << " cm\n";
  o << "  RM fluctuation (fraction):  FWD = " 
    << _rMoliereFluc[PacEmcModel::FWD]
    << "   Barrel = " << _rMoliereFluc[PacEmcModel::Barrel]
    << "   BWD = " << _rMoliereFluc[PacEmcModel::BWD] << "\n\n";
  o << "  Gap between crystals: FWD = "
    <<  _crystalGap[PacEmcModel::FWD]
    << "   Barrel = " << _crystalGap[PacEmcModel::Barrel]
    << "   BWD = " << _crystalGap[PacEmcModel::BWD] << "\n\n";
  o << "  Energy resolution coefficient\n";
  o << "     FWD :    " << _eResCoef[PacEmcModel::FWD]
    << "   " << _eResConst[PacEmcModel::FWD] << "\n";
  o << "     Barrel : " << _eResCoef[PacEmcModel::Barrel]
    << "   " << _eResConst[PacEmcModel::Barrel] << "\n";
  o << "     BWD :   " << _eResCoef[PacEmcModel::BWD]
    << "   " << _eResConst[PacEmcModel::BWD] << "\n";
  o << "  Pulse shape model: \n";
  o << "    height " << _pulseShapeMax[PacEmcModel::FWD]
    << " , " << _pulseShapeMax[PacEmcModel::Barrel]
    << " , " << _pulseShapeMax[PacEmcModel::BWD] << "\n"
    << "    t peak " << _pulseTshift[PacEmcModel::FWD]
    << " , "  << _pulseTshift[PacEmcModel::Barrel]
    << " , "  << _pulseTshift[PacEmcModel::BWD] << "\n";
  o << endl;
}

// Standard lib atan(theta) returns theta between [-pi/2, +pi/2].
// We want [0, +pi]
double PacEmcModel::sAtan(const double &at) {
  double retval= atan(at);
  if ( retval < 0 ) retval += Constants::pi;
  return retval;
}

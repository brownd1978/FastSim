//------------------------------------------------------------------------
// File
//   $Id$
//   PacEmcCalibration
//
//    Chih-hsiang Cheng @ Caltech   2009/06/06
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "PacEmc/PacEmcCalibration.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
//---------------
// C++ Headers --
//---------------
#include <iostream>
//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsCalo/AbsRecoCalo.hh"
#include "AbsEnv/AbsEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "AbsEvent/getTmpAList.hh"

#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcAbsCalibrator.hh"

//----------------
// Constructors --
//----------------
PacEmcCalibration::PacEmcCalibration ( const char* const theName, 
				       const char* const theDescription )
  : AppModule( theName, theDescription )
  ,_recoCaloRawListName("recoCaloRawListName", this, "PmcRecoCalosRaw")
  ,_recoCaloListName("recoCaloListName", this, "PmcRecoCalos")
  ,_recoCaloECut("recoCaloECut", this, 0.020)
  ,_maxCrystalEmin("maxCrystalEmin", this, 0.010)
  ,_disableFluctuation("disableFluctuation", this, false)
  ,_removeBadDigiTime("removeBadDigiTime", this, true)
  ,_model(0)
{
  commands()->append(&_recoCaloRawListName);
  commands()->append(&_recoCaloListName);
  commands()->append(&_recoCaloECut);
  commands()->append(&_maxCrystalEmin);
  commands()->append(&_disableFluctuation);
  commands()->append(&_removeBadDigiTime);
}

//--------------
// Destructor --
//--------------
PacEmcCalibration::~PacEmcCalibration ()
{}

//--------------
// Operations --
//--------------
AppResult
PacEmcCalibration::beginJob(AbsEvent* anEvent )
{
  ErrMsg(routine) << name() << "  Begin job" << endmsg;

  _model= PacEmcModel::getModel();
  
  return AppResult::OK;
}

AppResult
PacEmcCalibration::endJob(AbsEvent* anEvent ) {
  ErrMsg(routine) << name() << "  End job" << endmsg;
  return AppResult::OK;
}


AppResult
PacEmcCalibration::event(AbsEvent* anEvent ) {


  // Get a list of raw RecoCalo
  HepAList<AbsRecoCalo>* caloList= Ifd< HepAList<AbsRecoCalo> >::get(anEvent, _recoCaloRawListName.value());
  assert(caloList!=0);

  // Create a new list after calibration and energy cut
  HepAList<AbsRecoCalo>* caloList2= new HepAList< AbsRecoCalo>;
  assert (caloList2!=0);
  putData<HepAList<AbsRecoCalo> >(anEvent,caloList2,_recoCaloListName.value());


  HepAListIterator<AbsRecoCalo> itcalo(*caloList);
  AbsRecoCalo* calo(0);
  while ( calo = itcalo() ) {
    PacEmcCluster *cluster= dynamic_cast<PacEmcCluster*>(calo);
 
    if ( _removeBadDigiTime.value() ) {
      cluster->removeNoise(PacEmcCluster::BadTiming);
    }

    //=== Recale the cluster energy based on the resolution model 
    //=== for the entire cluster.                           ===//
    if (! _disableFluctuation.value() ) {
      cluster->fluctuateCluster();
    }

    // Add some electronic noise
    cluster->addNoise(); // add some noise (all above threshold)

    if (cluster->numberOfDigis()==0) {
      //ErrMsg(warning) << "this cluster has no digi"<< endmsg;
      continue;
    }

    // Calibration
    const PacEmcDigi* maxdigi= cluster->maxima();
    if ( !maxdigi ) {
      ErrMsg(warning) << "Can't find max digi? " << endmsg;
      cluster->print(std::cout);
      continue;
    }
    const PacEmcAbsCalibrator* calib= _model->getCalibrator(_model->region(maxdigi->theta()));

    if ( calib ) {
      calib->calibrate(cluster);
      if ( _verbose.value() ) {
	ErrMsg(routine) << "Cluster :" << endmsg;
	cluster->print(std::cout);
	ErrMsg(routine) << "  calibration factor " << cluster->calibFactor() 
			<< endmsg;
      }

    } else {
      ErrMsg(fatal) << "Can't get a calibrator" << endmsg;
    }


    if ( cluster->maxima()->energy() >= _maxCrystalEmin.value() && 
	 cluster->energy() >= _recoCaloECut.value() ) {
      caloList2->append(calo);
    }

  }

  return AppResult::OK;
}


template <class A>
void
PacEmcCalibration::putData (AbsEvent* anEvent, A* evtobj, const IfdKey& key) {
// put the object into the event
  IfdDataProxy< A >* proxy =  new IfdDataProxy< A >(evtobj);
  assert(proxy != 0);
  if (Ifd< A >::put(anEvent,proxy,key) == 0){
    ErrMsg(error) << "Cant put proxy into event " << endmsg;
    delete proxy;
    delete evtobj;
  }
}

//-----------------------------------------------------------------------
// File and Version Information:
//   PacEmcCalibration
//     A module for calibrating AbsRecoCalo energy
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2009/06/06)
//
//-----------------------------------------------------------------------
#ifndef PACEMCCALIBRATION
#define PACEMCCALIBRATION


//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class PacEmcModel;

class PacEmcCalibration : public AppModule {

public:

  PacEmcCalibration ( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~PacEmcCalibration();

  // Operations
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );
  
private:

  AbsParmIfdStrKey _recoCaloRawListName;
  AbsParmIfdStrKey _recoCaloListName;
  AbsParmDouble    _recoCaloECut, _maxCrystalEmin;
  AbsParmBool      _disableFluctuation;
  AbsParmBool      _removeBadDigiTime;

  PacEmcModel *_model;

  // utility
  template <class A> 
  static void putData (AbsEvent* anEvent,A* evtobj,const IfdKey& key);

};
#endif

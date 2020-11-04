//--------------------------------------------------------------------
// PacEmcMakeIntersections
//
// Class PacEmcMakeIntersections. Module to create a map that maps tracks to
// the intersection at the EMC
//
//  Chih-hsiang Cheng @ Caltech   2009/12/22
//------------------------------------------------------------------------
#ifndef PACEMCMAKEINTERSECTIONS_HH
#define PACEMCMAKEINTERSECTIONS_HH

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmDouble.hh"

class PacDetector;
class DetElem;

class PacEmcMakeIntersections : public AppModule {

public:

  // Constructor
  PacEmcMakeIntersections( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~PacEmcMakeIntersections();

  // Operations
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );
  
private:

  AbsParmIfdStrKey _recoTrkListName;
  AbsParmIfdStrKey _emcInterKey;

  PacDetector *_detector;
  DetElem *_emcfwd, *_emcbarrel, *_emcbwd;

};
#endif

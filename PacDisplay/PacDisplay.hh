#ifndef PACDISPLAY_HH
#define PACDISPLAY_HH

#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include "AbsEvent/AbsEvent.hh"
#include "Framework/AppModule.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmString.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "PacDisplay/PacEvtDisplay.hh"
#include "CLHEP/HepPoint.h"

class AbsRecoCalo;
class TrkRecoTrk;
class GTrack;
class BField;
class PacSimTrack;
class Trajectory;
class DetMaterial;
class TObjString;

class PacDisplay : public AppModule {

public:
  // Constructors
  PacDisplay( const char* const theName, const char* const theDescription );
  
  // Destructor
  virtual ~PacDisplay();
  
  // Operations
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );

protected:
	
private:
	AbsParmBool _saveGen; 
	AbsParmBool _saveSim;
	AbsParmBool _saveRec;
	AbsParmBool _saveDet;
	AbsParmBool _saveVox;
	AbsParmBool _saveHit;
	AbsParmBool _saveEmc;
	AbsParmGeneral<int> _resolution;
	AbsParmString _fileName; 
	AbsParmIfdStrKey _recoCaloListName;
	AbsParmIfdStrKey _simTrackListName;
  AbsParmIfdStrKey _recoTrackListName;
	int _event;
  PacEvtDisplay _display;
};

#endif

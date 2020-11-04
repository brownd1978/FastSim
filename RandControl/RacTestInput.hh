//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//	class RacTestInput
//	Input module that does nothing.
//

// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Anders Ryd 		        Original Author
//
// Copyright Information:
//	Copyright (C) 1996		R.T.Y.R. 
//
//------------------------------------------------------------------------

#ifndef RACTESTINPUT_HH
#define RACTESTINPUT_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppFileInput.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh"

class AbsEvent;


//		---------------------
// 		-- Class Interface --
//		---------------------
 
class RacTestInput : public AppFileInput {

  //--------------------
  // Instance Members --
  //--------------------

public:

  // Constructors
  RacTestInput();
  // const char* const theName, const char* const theDescription );
  
  virtual ~RacTestInput( );
  
  
  virtual AppResult beginJob   ( AbsEvent*  anEvent );
  virtual AppResult inputEvent ( AbsEvent*& anEvent );
  virtual AppResult inputEvent( AbsEvent*& anEvent, 
                                AppStopType& theDispatchStopType );
  virtual AppResult endJob     ( AbsEvent*  anEvent );

private:

  AbsParmGeneral<int> _runNumber ;
  AbsParmGeneral<int> _eventNumberCmd ;
  AbsParmIfdStrKey _geventList ;
  int _eventNumber ;
};

#endif





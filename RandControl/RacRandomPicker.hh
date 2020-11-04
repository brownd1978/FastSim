//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRandomPicker.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRandomPicker.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A.Salnikov
//
// Copyright Information:
//	Copyright (C) 1997	<Institution>
//
//------------------------------------------------------------------------

#ifndef RACRANDOMPICKER_HH
#define RACRANDOMPICKER_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <string>

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmGeneral.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class RacRandomPicker : public AppModule {


public:

  // Constructors
  // theName must be one of the "Primary", "Simulation", "Digis", "Reco" -
  //  see the RacRunConfig class
  RacRandomPicker ( const char* theName,
		    const char* theDescription = "Picker of the random engines" ) ;
    
  // Destructor
  virtual ~RacRandomPicker( );

  // Operators
    
  // Selectors (const)

  // Modifiers
  virtual AppResult beginJob ( AbsEvent* );
  virtual AppResult event    ( AbsEvent* );
  virtual AppResult endJob   ( AbsEvent* );

protected:

  // Helper functions

private:

  // Friends

  // Data members
  std::string _name ;

  AbsParmIfdStrKey _inputList ;
  AbsParmIfdStrKey _randomControlList ;
  AbsParmIfdStrKey _outputList ;

  // Note: if your class needs a copy constructor or an assignment
  // operator, make one of the following public and implement it.
  RacRandomPicker( const RacRandomPicker& );       // Copy Constructor
  RacRandomPicker&       operator= ( const RacRandomPicker& );  // Assignment op

};

#endif // RACRANDOMPICKER_HH

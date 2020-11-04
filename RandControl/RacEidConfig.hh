//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacEidConfig.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacEidConfig.
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

#ifndef RACEIDCONFIG_HH
#define RACEIDCONFIG_HH

//-------------
// C Headers --
//-------------
extern "C" {
#include <stddef.h>
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BaBar/BaBarODMGTypes.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class RacAbsSkipper ;
class AbsEventID ;


//		---------------------
// 		-- Class Interface --
//		---------------------

class RacEidConfig {

public:

  struct ProcessingStage {
    const char* stageName ;
    unsigned long consumption ;
  } ;

public:

  // Constructors
  RacEidConfig () {}
    
  // Destructor
  ~RacEidConfig () {}

  // Operators
    
  // Selectors (const)

  // number of the processing stages per event
  int nStages() const ;

  const char* stageName( int stage ) const ;
  size_t randomsPerStagePerEvent( int stage ) const ;
  int stageIndex( const char* name ) const ; // find a stage from its name

  // makes skipper to go the beginning of the stage. 
  // skipper must be in the state (run=0,event=0) before doing this
  // and it will be in the state (run=run,event=event,stage=stage) after this 
  void skip( RacAbsSkipper &skipper, 
	     const AbsEventID* eid,
	     const char* stage ) const;

  void skip( RacAbsSkipper &skipper, 
	     const AbsEventID* eid,
	     int stageIndex ) const;

  // Modifiers

protected:

  // Helper functions

private:

  // Friends

  // Data members

  // Note: if your class needs a copy constructor or an assignment
  // operator, make one of the following public and implement it.
  RacEidConfig( const RacEidConfig& );       // Copy Constructor
  RacEidConfig&       operator= ( const RacEidConfig& );  // Assignment op


  //------------------
  // Static Members --
  //------------------

public:

  // Selectors (const)

  // Modifiers

private:

  static d_ULong brev32 ( d_ULong a ) ; /** reverses bits in 32-bit word */

  // Data members
  static const ProcessingStage _stages [] ;

  static d_ULong _commonSkip[3] ;

};

#endif // RACEIDCONFIG_HH

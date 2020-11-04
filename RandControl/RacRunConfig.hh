//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRunConfig.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRunConfig.
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

#ifndef RACRUNCONFIG_HH
#define RACRUNCONFIG_HH

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

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class RacAbsSkipper ;

//		---------------------
// 		-- Class Interface --
//		---------------------

class RacRunConfig {

public:

  enum { _nStages = 3 } ;

private:

  struct RunClass {
    size_t firstRunNo ;
    size_t lastRunNo ;
    size_t maxEventsPerRun ;
    size_t maxRandomsPerEvent ;
    size_t stagePercent[_nStages] ;
  } ;

public:

  // Constructors
  RacRunConfig () {}
    
  // Destructor
  ~RacRunConfig () {}

  // Operators
    
  // Selectors (const)
  // number of the processing stages per event
  int nStages() const { return _nStages ; }

  // Access to the runClass parameters:
  size_t firstRunNumber() const ;
  size_t lastRunNumber() const ;
  size_t maxEventsPerRun() const ;
  size_t maxRandomsPerEvent() const ;

  const char* stageName( int stage ) const ;
  size_t randomsPercent( int stage ) const ;
  size_t randomsPerStagePerEvent( int stage ) const ;
  int stageIndex( const char* name ) const ;     // find a stage from its name

  // Change runClass parameters:
  void setFirstRunNumber(int value) ;
  void setLastRunNumber(int value) ;
  void setMaxEventsPerRun(int value) ;
  void setMaxRandomsPerEvent(int value) ;
  void setRandomsPercent(int stage, int value ) ;

  // makes skipper to go the beginning of the stage. 
  // skipper must be in the state (run=0,event=0) before doing this
  // and it will be in the state (run=run,event=event,stage=stage) after this 
  void skip( RacAbsSkipper &skipper, 
	     const char* stage,
	     size_t run, 
	     size_t event ) const;
  void skip( RacAbsSkipper &skipper, 
	     size_t stage,
	     size_t run, 
	     size_t event ) const;

  // Modifiers

protected:

  // Helper functions

private:

  // Friends

  // Data members

  // Note: if your class needs a copy constructor or an assignment
  // operator, make one of the following public and implement it.
  RacRunConfig( const RacRunConfig& );       // Copy Constructor
  RacRunConfig&       operator= ( const RacRunConfig& );  // Assignment op

  //------------------
  // Static Members --
  //------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members
  static RunClass & getRunClass();
  static const char*    _stageNames[_nStages] ;

};

#endif // RACRUNCONFIG_HH

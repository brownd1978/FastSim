//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRunConfig.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRunConfig
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
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "RandControl/RacRunConfig.hh"

//-------------
// C Headers --
//-------------
extern "C" {
#include <assert.h>
#include <string.h>
}

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
#include "RandControl/RacAbsSkipper.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

// The run stages:
const char* RacRunConfig::_stageNames[RacRunConfig::_nStages] = {
  "Primary", "Simulation", "Digis"
};



//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

//--------------
// Destructor --
//--------------

//-------------
// Methods   --
//-------------
    
//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------

size_t 
RacRunConfig::firstRunNumber( ) const 
{
  return getRunClass().firstRunNo ;
}

size_t 
RacRunConfig::lastRunNumber( ) const 
{
  return getRunClass().lastRunNo ;
}

size_t 
RacRunConfig::maxEventsPerRun( ) const 
{
  return getRunClass().maxEventsPerRun ;
}

size_t 
RacRunConfig::maxRandomsPerEvent( ) const 
{
  return getRunClass().maxRandomsPerEvent ;
}

const char* 
RacRunConfig::stageName( int stage ) const 
{
  assert ( stage < nStages() ) ;

  return _stageNames[stage] ;
}

size_t 
RacRunConfig::randomsPercent(int stage ) const 
{
  assert ( stage < nStages() ) ;

  return getRunClass().stagePercent[stage] ;
}

size_t 
RacRunConfig::randomsPerStagePerEvent( int stage ) const 
{
  assert ( stage < nStages() ) ;

  return getRunClass().maxRandomsPerEvent / 100 * 
    getRunClass().stagePercent[stage] ;
}

int 
RacRunConfig::stageIndex( const char* stage ) const 
{
  for ( int i = 0 ; i < nStages() ; ++ i ) {
    if ( strcmp(_stageNames[i],stage) == 0 ) {
      return i ;
    }
  }
  return -1 ;
}

// makes skipper to go the beginning of the stage. 
// skipper must be in the state (run=0,event=0) before doing this
// and it will be in the state (run=run,event=event,stage=stage) after this 
void 
RacRunConfig::skip( RacAbsSkipper &skipper, 
		    const char* stage,
		    size_t run, 
		    size_t event ) const
{
  int index = stageIndex ( stage ) ;
  if ( index < 0 ) {
    ErrMsg(error) << "RacRunConfig::skip -- stage name is unknown: " << stage << '\n'
		  << " -- no skipping will bne done at all" << endmsg ;
    return ;
  }
  skip ( skipper, index, run, event ) ;
}

void 
RacRunConfig::skip( RacAbsSkipper &skipper, 
		    size_t stage,
		    size_t run, 
		    size_t event ) const
{
  // now can work only with "exact" skipper
  assert ( skipper.randomsPerSkip() == 1 ) ;

  if ( stage >= nStages() ) {
    ErrMsg(error) << "RacRunConfig::skip -- stage index is incorrect: " << stage << '\n'
		  << " -- no skipping will be done at all" << endmsg ;
    return ;
  }

  // ====== first skip to the run number ======

  // skip the required runs in this class
  size_t runs_to_skip = run - firstRunNumber() ;
  skipper.skip ( runs_to_skip, maxEventsPerRun(), maxRandomsPerEvent() ) ;


  // ====== now skip also some events ======
  
  skipper.skip ( event, maxRandomsPerEvent() ) ;


  // ====== next skip to the stage ======

  if ( stage > 0 ) {
    // loop over all preceeding stages and sum their randoms consumption
    size_t randoms = 0 ;
    for ( int is = 0 ; is < stage ; is ++ ) {
      randoms += randomsPerStagePerEvent(is) ;
    }
    skipper.skip ( randoms ) ;
  }


}


void RacRunConfig::setFirstRunNumber(int value) {
  getRunClass().firstRunNo = value;
}

void RacRunConfig::setLastRunNumber(int value) {
  getRunClass().lastRunNo = value;
}
  
void RacRunConfig::setMaxEventsPerRun(int value) {
  getRunClass().maxEventsPerRun = value;
}

void RacRunConfig::setMaxRandomsPerEvent(int value) {
  getRunClass().maxRandomsPerEvent = value;
}

void RacRunConfig::setRandomsPercent(int stage, int value ) {
  getRunClass().stagePercent[stage] = value;
}


// A holder for the static run class, ensuring that it's always available
// when needed:
RacRunConfig::RunClass & RacRunConfig::getRunClass() {
  static RunClass _runClass;
  static bool firstCall = true;
  if (true == firstCall){
    firstCall = false;
    _runClass.firstRunNo         = 0;
    _runClass.lastRunNo          = 166000000;
    _runClass.maxEventsPerRun    = 10000;
    _runClass.maxRandomsPerEvent = 3000000;

    _runClass.stagePercent[0] = 10;
    _runClass.stagePercent[1] = 50;
    _runClass.stagePercent[2] = 40;
  }

  return _runClass;
}

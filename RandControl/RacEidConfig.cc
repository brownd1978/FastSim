//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacEidConfig.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacEidConfig
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
#include "RandControl/RacEidConfig.hh"

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
#include "AbsEvent/AbsEventID.hh"
#include "EidData/EidEventTriplet.hh"
#include "OdfCommon/odfTime.hh"
#include "RandControl/RacAbsSkipper.hh"
#include "ErrLogger/ErrLog.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

// consumption of the primaries by each stage
const RacEidConfig::ProcessingStage RacEidConfig::_stages[] = {
  { "Reco",     200000 },
  { "Analysis", 100000 }
};

d_ULong RacEidConfig::_commonSkip[3] = {      // equal to 2^60-2^58 = 2^29*2^29*3
  1L << 29, 1L << 29, 3 
} ;

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
    
// number of the processing stages per event
int 
RacEidConfig::nStages() const 
{ 
  return sizeof _stages / sizeof _stages[0] ; 
}

const char* 
RacEidConfig::stageName( int stage ) const 
{
  assert ( stage < nStages() ) ;

  return _stages[stage].stageName ;
}


size_t 
RacEidConfig::randomsPerStagePerEvent( int stage ) const 
{
  assert ( stage < nStages() ) ;

  return _stages[stage].consumption ;
}

int  
RacEidConfig::stageIndex( const char* stage ) const 
{
  for ( int i = 0 ; i < nStages() ; i ++ ) {
    if ( strcmp(stageName(i),stage) == 0 ) {
      return i ;
    }
  }
  return -1 ;
}


// makes skipper to go the beginning of the stage. 
// skipper must be in the state (run=0,event=0) before doing this
// and it will be in the state (run=run,event=event,stage=stage) after this 
void 
RacEidConfig::skip( RacAbsSkipper &skipper,  
		    const AbsEventID* eid,
		    const char* stage ) const
{
  int st = stageIndex ( stage ) ;
  if ( st < 0 ) {
    ErrMsg(error) << "RacEidConfig::skip -- stage name is unknown: " << stage << '\n'
		  << " -- no skipping will be done at all" << endmsg ;
    return ;
  }
  skip ( skipper, eid, st ) ;
}

void 
RacEidConfig::skip( RacAbsSkipper &skipper,  
		    const AbsEventID* eid,
		    int stage ) const
{
  // now can work only with "exact" skipper
  assert ( skipper.randomsPerSkip() == 1 ) ;


  // ====== first skip to the "Eid" stages ======
  skipper.skip ( _commonSkip[0], _commonSkip[1], _commonSkip[2] ) ; 

  // ====== get the event time ======
  const odfBinTime& time =  eid->eventIdTriplet().timeStamp().binary();

  // reverse all bits
  d_ULong lower = brev32 ( time.upper ) ;
  d_ULong upper = brev32 ( time.lower ) ;
  
  // limit this number to 2^58 - throw lower 6=64-58 bits
  lower >>= 6 ;
  lower |= ( upper & 0x3F ) << 26 ;
  upper >>= 6 ;

  // need to skip ( upper*2^32 + lower ) = ( upper*2^31 + upper*2^31 + lower )
  skipper.skip ( lower ) ;
  skipper.skip ( upper, 1L << 31 ) ;
  skipper.skip ( upper, 1L << 31 ) ;


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

//-------------
// Modifiers --
//-------------

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------


d_ULong 
RacEidConfig::brev32 ( d_ULong a ) 
{
  a = ( ( a & 0xFFFF0000 ) >> 16 ) | ( ( a & 0x0000FFFF ) << 16 ) ;
  a = ( ( a & 0xFF00FF00 ) >> 8 ) | ( ( a & 0x00FF00FF ) << 8 ) ;
  a = ( ( a & 0xF0F0F0F0 ) >> 4 ) | ( ( a & 0x0F0F0F0F ) << 4 ) ;
  a = ( ( a & 0xCCCCCCCC ) >> 2 ) | ( ( a & 0x33333333 ) << 2 ) ;
  a = ( ( a & 0xAAAAAAAA ) >> 1 ) | ( ( a & 0x55555555 ) << 1 ) ;
  return a ;
}

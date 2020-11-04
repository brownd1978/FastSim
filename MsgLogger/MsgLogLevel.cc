//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgLogLevel.cc 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgLogLevel
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Andy Salnikov
//
// Copyright Information:
//      Copyright (C) 2005 SLAC
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "MsgLogger/MsgLogLevel.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

// get full printable name of Level level
const char* 
MsgLogLevel::levelName () const 
{
  switch ( _level ) {
    case debug:
      return "debug" ;
    case trace:
      return "trace" ;
    case info:
      return "info" ;
    case nolog:
    default:
      return "no-log" ;
  }
}

// get one-char Level code
char 
MsgLogLevel::levelLetter () const
{
  switch ( _level ) {
    case debug:
      return 'D' ;
    case trace:
      return 'T' ;
    case info:
      return 'I' ;
    case nolog:
    default:
      return '?' ;
  }
}

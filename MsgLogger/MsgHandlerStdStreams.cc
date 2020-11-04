//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgHandlerStdStreams.cc 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgHandlerStdStreams
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
#include "MsgLogger/MsgHandlerStdStreams.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "MsgLogger/MsgFormatter.hh"
#include "MsgLogger/MsgLogLevel.hh"
#include "MsgLogger/MsgLogRecord.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

// Constructor
MsgHandlerStdStreams::MsgHandlerStdStreams() 
  : MsgHandler()
{
}

// Destructor
MsgHandlerStdStreams::~MsgHandlerStdStreams() 
{
}

/// get the stream for the specified log level
bool 
MsgHandlerStdStreams::log ( const MsgLogRecord& record ) const
{
  if ( ! logging( record.level() ) ) {
    return false ;
  }

// commented-out stuff, because I have removed error levels.
//  if ( record.level() <= MsgLogLevel::info ) {
    formatter().format ( record, std::cout ) ;
    std::cout << std::endl ;
//  } else {
//    formatter().format ( record, std::cerr ) ;
//    std::cerr << std::endl ;
//  }
  return true ;
}

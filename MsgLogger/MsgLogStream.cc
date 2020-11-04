//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgLogStream.cc 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgLogStream
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
#include "MsgLogger/MsgLogStream.hh"

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
#include "MsgLogger/MsgLogger.hh"
#include "MsgLogger/MsgLogRecord.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

// Destructor
MsgLogStream::~MsgLogStream() 
{
}

// send my content to logger
void 
MsgLogStream::emit() 
{
  // check if we need to send it at all
  MsgLogger logger( _logger ) ;
  if ( logger.logging ( _sev ) ) {
    MsgLogRecord record ( _logger, _sev, _file, _lineNum, rdbuf() ) ;
    logger.log ( record ) ;
  }
}

/// manipulator which sends the message
std::ostream& 
endlog ( std::ostream& o ) 
{
  if ( MsgLogStream* str = dynamic_cast<MsgLogStream*> ( &o ) ) {
    str->emit() ;
  } else {
    // for other types of streams it's equivalent to endl
    o << std::endl ;
  }
  return o ;
}

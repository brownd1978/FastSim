//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppShowCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppShowCommand. Command handler for the "show" command. It
//	delegates the request to the show function member of its target.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Merged in TK interface 5/15/98
//      Marc Turcotte                   Improved error reporting to
//                                      name already in use. 7/1/98
//      Marc Turcotte                   Added wrap around TK calls. 8/24/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppShowCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "FrameUtil/AbsInterp.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppShowCommand::AppShowCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppShowCommand::~AppShowCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppShowCommand::handle( int argc, char** argv )
{
    target( )->show( argc, argv );
    return AbsInterp::OK;
}

std::string 
AppShowCommand::description( ) const
{
   return "Display the value of any parameters or statistics.";
}

void
AppShowCommand::show( ) const 
{
}
bool
AppShowCommand::isShowable( ) const 
{
   return false; 
}

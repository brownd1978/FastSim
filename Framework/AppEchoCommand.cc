//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppEchoCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppEchoCommand. Command handler for the "echo" command. This
//	command just prints its arguments to standard output.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppEchoCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "FrameUtil/AbsInterp.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//static const char rcsid[] = "$Id: AppEchoCommand.cc 509 2010-01-14 15:18:55Z stroili $";

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppEchoCommand::AppEchoCommand( 
    const char* const theCommand, 
    AppModule* theTarget )
    : AppCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppEchoCommand::~AppEchoCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppEchoCommand::handle( int argc, char* argv[] )
{
    int index = 1;
    while( index < argc ) {
	target( )->partialReport( argv[index] );
	target( )->partialReport( " " );
	index++;
    }
    target( )->fullReport( NULL );
    return AbsInterp::OK;
}

std::string 
AppEchoCommand::description( ) const
{
   return "Send text argument to stdout (useful in scripts).";
}

void
AppEchoCommand::show( ) const 
{
}
bool
AppEchoCommand::isShowable( ) const 
{
   return false;
}

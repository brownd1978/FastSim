//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppHelpCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppHelpCommand. Command handler for the "help" command. It
//	delegates the request to the help function member of its target.
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
#include "Framework/AppHelpCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "FrameUtil/AbsInterp.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//static const char rcsid[] = "$Id: AppHelpCommand.cc 509 2010-01-14 15:18:55Z stroili $";

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppHelpCommand::AppHelpCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppHelpCommand::~AppHelpCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppHelpCommand::handle( int argc, char** argv )
{
    target( )->help( argc, argv );
    return AbsInterp::OK;
}

std::string 
AppHelpCommand::description( ) const
{
   return "Bring up help text for the current context.";
}

void
AppHelpCommand::show( ) const 
{
}
bool
AppHelpCommand::isShowable  ( ) const 
{ 
   return false;
}

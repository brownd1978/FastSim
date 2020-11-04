//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppExitCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppExitCommand. Command handler for the "exit" command. If 
//	issued to the framework itself, this command will terminate the
//	current job. If issued to a module (i.e. during a "talkTo" session),
//	this command will return to the main framework.
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
#include "Framework/AppExitCommand.hh"

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

//static const char rcsid[] = "$Id: AppExitCommand.cc 509 2010-01-14 15:18:55Z stroili $";

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppExitCommand::AppExitCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppExitCommand::~AppExitCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppExitCommand::handle( int, char** )
{
    target( )->exit( );
    return AbsInterp::OK;
}

std::string 
AppExitCommand::description( ) const
{
   return "Leave the current menu, module, or process";
}

void
AppExitCommand::show( ) const 
{
}
bool
AppExitCommand::isShowable( ) const 
{
   return false; 
}

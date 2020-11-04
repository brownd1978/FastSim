//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFileOutputCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppFileOutputCommand implementation file. 
//      Command handler for the "output" command for the file output 
//      module. Valid subcommands that are added to the standard output commands are:
//
//      output file <stream> [<file>]         Bind stream to file
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//
// Copyright Information:
//      Copyright (C) 1999              Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppFileOutputCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppFileStream.hh"
#include "Framework/AppStreamsOutputModule.hh"
#include "FrameUtil/AbsInterp.hh"

//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//----------------
// Constructors --
//----------------

AppFileOutputCommand::AppFileOutputCommand( 
        const char* const theCommand, AppModule* theTarget )
    : AppOutputCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppFileOutputCommand::~AppFileOutputCommand( )
{
}

//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------

int 
AppFileOutputCommand::helpHandler( )
{
    int result = AppOutputCommand::helpHandler( );

    target()->fullReport( "" );
    target()->partialReport( "output create <stream> [<file>]         ");
    target()->fullReport("Create stream" );
    target()->partialReport( "output file <stream> [<file>]           ");
    target()->fullReport("Bind stream to file" );
    return result;
}

AppStream* 
AppFileOutputCommand::createStream( const char* theName, const char* theDestination )
{

    AppStream* result = new AppFileStream( theName, theDestination );
    return result;
}

const char*
AppFileOutputCommand::destinationString( ) const
{
    return "File";
}

int 
AppFileOutputCommand::handleCommand( int argc, char* argv[] )
{
    int result = AppOutputCommand::handleCommand( argc, argv );
    if ( result != AbsInterp::OK ) {
        if ( 0 == strcmp( argv[1], "file" ) || 
             0 == strcmp( argv[1], "fi"   ) ) {
            result = destinationHandler( );
        }
    }
    return result;
}

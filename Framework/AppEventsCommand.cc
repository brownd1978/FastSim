//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppEventsCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppEventsCommand. This command handler deals with the "events"
//	command which is used to control the looping over events. Subcommands
//	are:
//
//		begin    [-nev <n>]	 Begin processing
//		continue [-nev <n>] 	 Continue processing
//              dumpOnBegin <filename>   Dump tcl config on "ev begin"
//
//	Where "begin" resets the current input module to start a new\n
//	event processing run, whereas "continue" continues the current\n
//	sequence (presumably because it was terminated prematurely by\n
//	the optional qualifier "-nev" which specifies a number of events).\n
//	The "continue" command acts identically to the "begin" command\n
//	if no prior "begin" command had been issued.\n
//      dumpOnBegin dumps the tcl configuration on "begin" and does not process
//      further.
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
#include "Framework/AppEventsCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "FrameUtil/AbsInterp.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppEventsCommand::AppEventsCommand( 
				   const char* const theCommand, 
				   AppModule* theTarget )
  : AppCommand( theCommand, theTarget )
  , _dumpFile(0)
{
}

//--------------
// Destructor --
//--------------

AppEventsCommand::~AppEventsCommand( )
{
  if (_dumpFile != 0) delete _dumpFile;
}

//-----------
// Handler --
//-----------

int 
AppEventsCommand::handle( int argc, char* argv[] )
{
    AppFramework* theFrame = (AppFramework*)target( );
    int result = AbsInterp::ERROR;
    
    if ( 2 <= argc ) {
	if        ( 0 == strcmp( argv[1], "begin" ) ) {
	    result = beginHandler( argc, argv );
	} else if ( 0 == strcmp( argv[1], "beg"   ) ) {
	    result = beginHandler( argc, argv );
	} else if ( 0 == strcmp( argv[1], "continue" ) ) {
	    result = continueHandler( argc, argv );
	} else if ( 0 == strcmp( argv[1], "cont" ) ) { 
	    result = continueHandler( argc, argv );
	} else if ( 0 == strcmp( argv[1], "help" ) ) { 
	    result = helpHandler( );
	} else if ( 0 == strcmp( argv[1], "dumpOnBegin" ) ) { 
	    result = dumpOnBeginHandler( argc, argv );
	} else {
          std::string txtmsg("invalid command name: ");
          for (int index=0; index < argc; index++ ) {
            txtmsg += " ";
            txtmsg += argv[index];
          }
          theFrame->setError( txtmsg.c_str() );
	} 
    } else {
        theFrame->setError( "wrong # args" );
    }
    return result;
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

int 
AppEventsCommand::beginHandler( int argc, char* argv[] )
{
  AppFramework* theFrame = (AppFramework*)target( );
  AbsInterp* interp = AbsInterp::theInterpreter( );
  int nEvents = 0;
  int result  = AbsInterp::OK;
  
  if ( 3 <= argc ) {
    if ( 0 == strcmp( argv[2], "-nev" ) && 
	 0 != argv[3] ) {
      result = interp->ascii2Int( argv[3], &nEvents );
    }
    else {
      result = AbsInterp::ERROR;
    }
  }

  if ( AbsInterp::OK == result ) {
    if (_dumpFile == 0) {
      theFrame->beginHandler( nEvents );
    } else {
      beginConfigDump(argc, argv);
    }
  }
  else {
    theFrame->setError( "syntax error: int arg. invalid" ); 
  }

  return result;
}

int 
AppEventsCommand::continueHandler( int argc, char* argv[] )
{
  AppFramework* theFrame = (AppFramework*)target( );
  AbsInterp* interp = AbsInterp::theInterpreter( );
  int nEvents = 0;
  int result  = AbsInterp::OK;

  if ( 3 <= argc ) {
    if ( 0 == strcmp( argv[2], "-nev" ) &&
         0 != argv[3] ) {
      result = interp->ascii2Int( argv[3], &nEvents );
      if ( AbsInterp::OK != result ) {
	theFrame->setError( "syntax error: int arg. invalid" );
      }
    }
  }

  if ( AbsInterp::OK == result ) {
    if (_dumpFile == 0) {
      theFrame->continueHandler( nEvents );
    } else {
      beginConfigDump(argc, argv);
    }
  }
  return result;
}

int
AppEventsCommand::helpHandler() {

  target()->fullReport( "The following commands are available:"); 

  target()->partialReport( "  beg(in) [-nev <n>]                    " ); 
  target()->fullReport("Begin processing");

  target()->partialReport( "  cont(inue) [-nev <n>]                 " ); 
  target()->fullReport("Continue processing");

  target()->partialReport( "  dumpOnBegin <filename>                " ); 
  target()->fullReport("Writes config. to <filename> on begin. " );
  target()->partialReport( "                                        " ); 
  target()->fullReport(" Also write the begin and exit commands." );

  target()->partialReport( "  help                                  " ); 
  target()->fullReport("Show available commands");

  target()->fullReport(" ");
  target()->fullReport( "Where \"begin\" resets the current input module to start a new\n\
event processing run, whereas \"continue\" continues the current\n\
sequence (presumably because it was terminated prematurely by\n\
the optional qualifier \"-nev\" which specifies a number of events).\n\
The \"continue\" command acts identically to the \"begin\" command\n\
if no prior \"begin\" command had been issued." );

  return AbsInterp::OK;
}

std::string 
AppEventsCommand::description( ) const
{
   return "Command used to control the looping over events";
}


void
AppEventsCommand::show( ) const 
{
}

bool
AppEventsCommand::isShowable( ) const 
{
   return false; 
}

int 
AppEventsCommand::dumpOnBeginHandler( int argc, char* argv[] )
{
  int result = AbsInterp::ERROR;
  AppFramework* theFrame = (AppFramework*)target();

  if ( 3 <= argc ) {
    if (_dumpFile != 0) delete _dumpFile;
    _dumpFile = new std::string(argv[2]);
    result = AbsInterp::OK;
  } else {
    theFrame->setError( "wrong # args" );
  }

  return result;
}

int
AppEventsCommand::beginConfigDump( int argc, char* argv[] )
{
  AbsInterp* interp = AbsInterp::theInterpreter( );
  int result  = AbsInterp::OK;

  std::string txtmsg = "config dump ";
  txtmsg += *_dumpFile;
  txtmsg += " -cmd ";
  for (int index=0; index < argc; index++ ) {
    txtmsg += " ";
    txtmsg += argv[index];
  }
  txtmsg += " -cmd exit";
  interp->simulateInput(txtmsg);

  return result;
}

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppInputCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppInputCommand. Command handler for the "input" command for
//	the standard input module. Type "input help" at Framework prompt
//      for a list of available commands.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Removed dependency on DbiEvent 6/97
//      Marc Turcotte                   Merged in R.Kapur's code 5/18
//
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
#include "Framework/AppInputCommand.hh"

//-------------
// C Headers --
//-------------
#include <string.h>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppInputModule.hh"

#include "FrameUtil/AbsInterp.hh"

#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppInputCommand::AppInputCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppInputCommand::~AppInputCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppInputCommand::handle( int argc, char* argv[] )
{
    AppInputModule* theInput = dynamic_cast<AppInputModule*>(target());
    int result = AbsInterp::ERROR;

    setArgs( argc, argv );
    if ( 2 <= argc ) {
      if ( 0 == strcmp( argv[1], "add" ) ) {
	result = addHandler( );
      } else if ( 0 == strcmp( argv[1], "list" ) ) {
	result = listHandler( );
      } else if ( 0 == strcmp( argv[1], "nosilent" ) ) {
	theInput->setSilent( false );
	result = AbsInterp::OK;
      } else if ( 0 == strcmp( argv[1], "remove" ) ) { 
	result = removeHandler( );
      } else if ( 0 == strcmp( argv[1], "silent" ) ) {
	theInput->setSilent( true );
	result = AbsInterp::OK;
      } else if ( 0 == strcmp( argv[1], "wipe-out" ) ) { 
	result = wipeoutHandler( );
      } else if ( 0 == strcmp( argv[1], "wipe" ) ) { 
	result = wipeoutHandler( );
      } else if ( 0 == strcmp( argv[1], "help" ) ||
		  0 == strcmp( argv[1], "he"   ) ) {
	result = helpHandler( );
      } else {
	std::string txtmsg("invalid command name: ");
	for (int index=0; index < argc; index++ ) {
	  txtmsg += " ";
	  txtmsg += argv[index];
	}
	theInput->setError( txtmsg.c_str() );
      } 
    } else {
      theInput->setError( "wrong # args" );
    }
    return result;
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------


int 
AppInputCommand::removeHandler( )
{
    AppInputModule* theInput = dynamic_cast<AppInputModule*>(target());

    AppFramework*       theFrame = theInput->framework( );
    char* arg;
    int index  = 2;
    int result = AbsInterp::ERROR;

    while ( NULL != ( arg = getArgument( index ) ) ) {
      theInput->remove( arg );
      index++;
      theFrame->setBegun( false );
      result = AbsInterp::OK;
    }
    return result;
}

int
AppInputCommand::wipeoutHandler( )
{
    AppInputModule* theInput = dynamic_cast<AppInputModule*>(target());

    AppFramework*       theFrame = theInput->framework( );

    theInput->wipeout( );
    theFrame->setBegun( false );
    return AbsInterp::OK;
}

std::string 
AppInputCommand::description() const
{
  std::string descr;
  descr = "(Command) : Type '";
  descr += command();
  descr += " help' for details.";
  return descr;
}

void
AppInputCommand::show( ) const 
{

  target()->partialReport(command());
  target()->partialReport(" : (command); type '");
  target()->partialReport(command());
  target()->fullReport(" list' for settings.");

}

bool
AppInputCommand::isShowable( ) const 
{
   return true; 
}

void
AppInputCommand::restoreState( char* argv[] ) {
  int i=0;
  while(argv[i]){
    i++;
  }
  int argc = i;
  handle(argc, argv);
}

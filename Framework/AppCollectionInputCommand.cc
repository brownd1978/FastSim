//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppCollectionInputCommand.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppCollectionInputCommand. Command handler for the "input" 
//      command for the standard collection input module. Type "input help" 
//      at Framework prompt for a list of available commands.
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
#include "Framework/AppCollectionInputCommand.hh"

//-------------
// C Headers --
//-------------
#include <string.h>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppCollectionInput.hh" 

#include "FrameUtil/AbsInterp.hh"

#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppCollectionInputCommand::AppCollectionInputCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppInputCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppCollectionInputCommand::~AppCollectionInputCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppCollectionInputCommand::handle( int argc, char* argv[] )
{
    return AppInputCommand::handle ( argc, argv );
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

int 
AppCollectionInputCommand::addHandler( )
{
    char* arg;
    AppCollectionInput* theInput = (AppCollectionInput*)target();

    AppFramework*       theFrame = theInput->framework( );
    int index = 2;
    int result = AbsInterp::ERROR;
    if ( 2 <= _argc ) {
	while ( NULL != ( arg = getArgument( index ) ) ) {
	  theInput->append( arg );
	  index++;
	}
	theFrame->setBegun( false );
	result = AbsInterp::OK;
    }
    return result;
}

int 
AppCollectionInputCommand::listHandler( )
{
   AppCollectionInput* theInput = (AppCollectionInput*)target();
   char**          theName;
   
   target()->partialReport("Collections: ");
   target()->partialReport("       ");
   if ( ! theInput->collections( )->isEmpty( ) ) {
     target()->fullReport("");
      APPListIterator<char*> theIterator( *theInput->collections( ) );
      while ( ( theName = theIterator( ) ) ) {
	target()->partialReport("       ");
	target()->partialReport( (*theName) );;
	if ( theInput->isOpen( ) ) {
	  if ( 0 == strcmp( (*theName), theInput->currentCollection( ) ) ) {
	    target()->partialReport( " (open)" );
	    }
	 }
	target()->fullReport( NULL );
      }
   } else {
      target()->fullReport( "No input collections are defined" );
   }
   return AbsInterp::OK;
}


int
AppCollectionInputCommand::helpHandler( )
{

  target()->fullReport( "input add <collection1> [<collection2> ...] " );
  target()->partialReport( "                                        " );
  target()->fullReport("Add collection(s) to list" );

  target()->partialReport( "input list                              " );
  target()->fullReport("Lists the current collection list" );

  target()->partialReport( "input nosilent                          " );
  target()->fullReport("Sets verbosity level" );

  target()->fullReport( "input remove <collection1> [<collection2> ...]      " );
  target()->partialReport( "                                        " );
  target()->fullReport("Remove collection(s) from list" );

  target()->partialReport( "input silent                            " );
  target()->fullReport("Sets verbosity level" );

  target()->partialReport( "input wipe[-out]                        " );
  target()->fullReport("Empty the list" );

  return AbsInterp::OK;
}


void
AppCollectionInputCommand::dumpState( AppConfigRecorder* r) {

  AppCollectionInput* theInput = (AppCollectionInput*)target();

  if (theInput->collections( ) != 0) {
    const char* var[10];
    var[0] = "input";
    var[1] = "add";
    APPListIterator<char*> theIterator( *theInput->collections( ) );
    char** theName;
    while ( ( theName = theIterator( ) ) ) {
      var[2] = *theName;
      r->store(3, var);
    }
  }
}

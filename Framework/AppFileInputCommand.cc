//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFileInputCommand.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppFileInputCommand. Command handler for the "input" command for
//	the standard file input module. Type "input help" at Framework prompt
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
#include "Framework/AppFileInputCommand.hh"

//-------------
// C Headers --
//-------------
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppFileInput.hh" 

#include "FrameUtil/AbsInterp.hh"

#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppFileInputCommand::AppFileInputCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppInputCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppFileInputCommand::~AppFileInputCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppFileInputCommand::handle( int argc, char* argv[] )
{
    AppFileInput*     theInput = (AppFileInput*)target();
    int result = AbsInterp::ERROR;

    setArgs( argc, argv );

    // check numbr of arguments
    if ( argc < 2 ) {
      theInput->setError( "wrong # args" );
      return AbsInterp::ERROR ;
    }
    
    if ( 0 == strcmp( argv[1], "file" ) ) {
      result = addHandler( );
    } else {
      result = AppInputCommand::handle ( argc, argv ) ;
    }
    return result;
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

int 
AppFileInputCommand::addHandler( )
{
    char* arg;
    AppFileInput* theInput = (AppFileInput*)target();

    AppFramework*       theFrame = theInput->framework( );
    int index = 2;
    bool validate = true;
    bool ok;
    int result = AbsInterp::ERROR;
    if ( 2 <= _argc ) {
	while ( NULL != ( arg = getArgument( index ) ) ) {
	    if        ( 0 == strcmp( arg, "-v"          ) || 
			0 == strcmp( arg, "-val"        ) ||
			0 == strcmp( arg, "-validate"   ) ||
			0 == strcmp( arg, "-c"          ) ||
			0 == strcmp( arg, "-check"      ) ) {
		validate = true;
	    } else if ( 0 == strcmp( arg, "-nov"        ) ||
			0 == strcmp( arg, "-noval"      ) ||
			0 == strcmp( arg, "-novalidate" ) ||
			0 == strcmp( arg, "-noc"        ) ||
			0 == strcmp( arg, "-nocheck"    ) ) {
		validate = false;
	    } else {
	        ok = true;
		if ( validate ) {
		    AbsInterp* interp = AbsInterp::theInterpreter( );
		    ok = interp->fileOperation( arg, "exists" );
		}
		if ( ok ) {
		    theInput->append( arg );
		} else {
		    theInput->partialReport( "***** Input File non-existent: " );
		    theInput->fullReport( arg );
		}
	    }
	    index++;
	}
	theFrame->setBegun( false );
	result = AbsInterp::OK;
    }
    return result;
}

int 
AppFileInputCommand::listHandler( )
{
   AppFileInput* theInput = (AppFileInput*)target();
   
   char**          theName;
   
   target()->partialReport("Files: ");
   target()->partialReport("       ");
   if ( ! theInput->files( )->isEmpty( ) ) {
     target()->fullReport("");
      APPListIterator<char*> theIterator( *theInput->files( ) );
      while ( ( theName = theIterator( ) ) ) {
	target()->partialReport("       ");
	target()->partialReport( (*theName) );;
	if ( theInput->isOpen( ) ) {
	  if ( 0 == strcmp( (*theName), theInput->theFilename( ) ) ) {
	    target()->partialReport( " (open)" );
	    }
	 }
	target()->fullReport( NULL );
      }
   } else {
      target()->fullReport( "No input files are defined" );
   }
   return AbsInterp::OK;
}

int
AppFileInputCommand::helpHandler( )
{

  target()->fullReport( "input add [-[no]v(alidate)|-[no]c(heck)] <file1> [<file2> ...] " );
  target()->partialReport( "                                        " );
  target()->fullReport("Add file(s) to list" );

  target()->fullReport( "input file [-[no]v(alidate)|-[no]c(heck)] <file1> [<file2> ...] " );
  target()->partialReport( "                                        " );
  target()->fullReport("Synonym for input add" );

  target()->partialReport( "input list                              " );
  target()->fullReport("Lists the current file list" );

  target()->partialReport( "input nosilent                          " );
  target()->fullReport("Sets verbosity level" );

  target()->partialReport( "input remove <file1> [<file2> ...]      " );
  target()->fullReport("Remove file(s) from list" );

  target()->partialReport( "input silent                            " );
  target()->fullReport("Sets verbosity level" );

  target()->partialReport( "input wipe[-out]                        " );
  target()->fullReport("Empty the list" );

  return AbsInterp::OK;
}

void
AppFileInputCommand::dumpState( AppConfigRecorder* r) {

  AppFileInput* theInput = (AppFileInput*)target();

  if (theInput->files( ) != 0) {
    const char* var[10];
    var[0] = "input";
    var[1] = "add";
    APPListIterator<char*> theIterator( *theInput->files( ) );
    char** theName;
    while ( ( theName = theIterator( ) ) ) {
      var[2] = *theName;
      r->store(3, var);
    }
  }
}


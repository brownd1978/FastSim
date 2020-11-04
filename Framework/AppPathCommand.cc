//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppPathCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppPathCommand. Command handler for the "path" command for
//	the BaBar Application Framework. Valid subcommands are:
//
//	path append  <path> <mod1> [<seq1>..]	Append module(s)/sequence(s)
//	path create  <path> [<mod1> [<seq2> ..]	Create a new path
//	path delete  <path>              	Delete a path
//	path disable <path1> [<path2> ..]	Disable path(s)
//	path enable  <path1> [<path2> ..]	Enable path(s)
//	path help				Show help for this command
//	path insert  <path> <mod1> [<seq1>..]	Insert module(s)/sequence(s)
//	path list				List paths
//	path remove  <path> <mod1> [<seq1>..]	Remove module(s)/sequence(s)
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
#include "Framework/AppPathCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppPath.hh"
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

AppPathCommand::AppPathCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppSequenceCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppPathCommand::~AppPathCommand( )
{
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

int 
AppPathCommand::deleteHandler( )
{
    AppFramework* theFrame = (AppFramework*)target( );
    char*         theName;
    AppPath*      thePath;
    int index  = firstIndex;
    int result = AbsInterp::OK;
    std::string unknownPath;
    
    while ( NULL != ( theName = getArgument( index ) ) ) {
        if ( NULL != ( thePath = (AppPath*)fetchSeq( theFrame, theName ) ) ) {
	    theFrame->remove( thePath );
	    delete thePath; // ownership given up by AppFramework
	    result = AbsInterp::OK;
        } else {
	  unknownPath += theName;
	  unknownPath += " ";
	}
	index++;
    }

    if (unknownPath.size() != 0) {
      unknownPath += ": unknown name";
      theFrame->setError(unknownPath.c_str());
      result = AbsInterp::ERROR;
    }

    return result;
}

AppSequence* 
AppPathCommand::fetchSeq( 
    AppFramework* theFrame, 
    const char* const theName )
{
    return theFrame->fetchPath( theName );
}

AppSequence* 
AppPathCommand::newSeq( 
    AppFramework* theFrame, 
    const char* const theName )
{
    AppPath* result = NULL;
    if ( ! theFrame->has( theName ) ) {
	result = new AppPath( theName, NULL );
	theFrame->add( result );
    }
    return (AppSequence*)result;
}

APPList< AppSequence* >* 
AppPathCommand::fetchSeqs( AppFramework* theFrame )
{
    return (APPList< AppSequence* >*)theFrame->paths( );
}

std::string 
AppPathCommand::description( ) const
{
   return "Command used to create, modify and control paths";
}

void
AppPathCommand::show( ) const 
{
}
bool
AppPathCommand::isShowable( ) const 
{
   return false; 
}

int 
AppPathCommand::helpHandler( )
{
  AppBasicCommand::helpHandler( );
  
  target()->partialReport( "  append <path> [-a(fter) <mod/seq>] "); 
  target()->fullReport("<mod1/seq1> [<mod2/seq2> ..]");
  target()->partialReport( "                                        " ); 
  target()->fullReport("Append module(s)/sequence(s)");  

  target()->partialReport( "  create <path> [<mod1/seq1> ..]        " );
  target()->fullReport("Create a new path");
  
  target()->partialReport( "  delete <path1> [<path2> ..]           " );
  target()->fullReport("Delete path(s)");

  target()->partialReport( "  disable <path1> [<path2> ..]          " );
  target()->fullReport("Disable path(s)*");

  target()->partialReport( "  enable <path1> [<path2> ..]           " );
  target()->fullReport("Enable path(s)*");

  target()->partialReport( "  help                                  " );
  target()->fullReport("Show available commands");

  target()->partialReport( "  insert <path> [-b(efore) <mod/seq>] " );
  target()->fullReport("<mod1/seq1> [<mod2/seq2> ..]");
  target()->partialReport( "                                        " ); 
  target()->fullReport("Insert module(s)/sequence(s)");

  target()->fullReport( "  list [-b(rief)] -e(xpand) -l(evel) <level> ] " );
  target()->partialReport( "                                        " );
  target()->fullReport("List paths");

  target()->fullReport( "  listMembers <path> [-b(rief)] -e(xpand) -l(evel) <level> ] " );
  target()->partialReport( "                                        " );
  target()->fullReport("List members of a path");
   
  target()->fullReport( "  remove <path> <mod1/seq1> [<mod2/seq2> ..] " );
  target()->partialReport( "                                        " ); 
  target()->fullReport("Remove module(s)/sequence(s)");
   
  target()->fullReport(" ");
  target()->fullReport("* = 'all' can be used in place of <path1>.");

  return AbsInterp::OK;

}


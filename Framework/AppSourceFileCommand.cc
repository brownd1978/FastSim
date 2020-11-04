//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppSourceFileCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppSourceFileCommand.
//      Command handler for the "sourceFileFound" command.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Asoka De Silva          Original Author
//
// Copyright Information:
//
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppSourceFileCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "Framework/AppFramework.hh"
#include "FrameUtil/AbsInterp.hh"

#include "CommonUtils/ComPathNameSearch.hh"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppSourceFileCommand::AppSourceFileCommand( 
    const char* const theCommand, AppModule* theTarget )
    : AppCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppSourceFileCommand::~AppSourceFileCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppSourceFileCommand::handle( int argc, char* argv[] )
{
  AppFramework* theFrame = (AppFramework*)target( );
  AbsInterp* theInterp = AbsInterp::theInterpreter();
  int result = AbsInterp::ERROR;

  if ( 2 <= argc ) {
    std::string fileName( (const char*) ComPathNameSearch(argv[1]) );
    if (fileName.length() != 0) {
      const char* foundFile = (const char*) fileName.c_str();
      result = theInterp->runCommandFile( (const char*) foundFile);
      if (result != AbsInterp::OK) {
	std::string errmsg = "sourceFoundFile failed when running ";
	errmsg+=fileName;
	theFrame->setError(errmsg.c_str());
      }
    } 
  } else {
    theFrame->setError("wrong # args");
    theFrame->setError("The syntax is: sourceFoundFile <filename>");
  }

  return result;
}

void
AppSourceFileCommand::show( ) const 
{
}

bool
AppSourceFileCommand::isShowable( ) const 
{
   return false; 
}

std::string 
AppSourceFileCommand::description( ) const
{
   return "Source file in $BFDEFAULTSEARCHPATH or $BFSEARCHPATH.";
}









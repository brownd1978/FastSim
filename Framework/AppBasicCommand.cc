//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppBasicCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppBasicCommand. This is an abstract class that acts as the
//	parent for the "module", "sequence" and "path" commands. In particular
//	it provides default handlers for the common sub-commands.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Introduced Actions '97
//      Marc Turcotte                   Added R. Kapur's TK interface 5/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997, 1998        University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppBasicCommand.hh"

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
#include "Framework/AppNameNode.hh"
#include "FrameUtil/AbsInterp.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppBasicCommand::AppBasicCommand( 
   const char* const theCommand,  
   AppModule* theTarget )
    : AppCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppBasicCommand::~AppBasicCommand()
{
}

//-----------
// Handler --
//-----------

int 
AppBasicCommand::handle( int argc, char* argv[] )
{
    AppFramework* theFrame = (AppFramework*)target();
    int result = AbsInterp::OK;
    
    setArgs( argc, argv );

    if ( 2 <= argc ) 
      {
	if      ( 0 == strcmp( argv[1], "append" ) ) 
	  result = appendHandler();
	else if ( 0 == strcmp( argv[1], "clone" ) )  
	  result = cloneHandler();
	else if ( 0 == strcmp( argv[1], "create" ) ) 
	  result = createHandler();
	else if ( 0 == strcmp( argv[1], "delete" ) )  
	  result = deleteHandler();
	else if ( 0 == strcmp( argv[1], "disable" ) )  
	  result = disableHandler();
	else if ( 0 == strcmp( argv[1], "enable" ) )  
	  result = enableHandler();
	else if ( 0 == strcmp( argv[1], "help" ) )  
	  result = helpHandler();
	else if ( 0 == strcmp( argv[1], "insert" ) ) 
	  result = insertHandler();
	else if ( 0 == strcmp( argv[1], "list"   ) ) 
	  result = listHandler();
	else if ( 0 == strcmp( argv[1], "remove" ) ) 
	  result = removeHandler();
	else if ( (0 == strcmp( argv[1], "talk")) ||
		 (0 == strcmp( argv[1], "talkTo" )) ) 
	  result = talkToHandler();
	else if ( (0 == strcmp( argv[1], "input")) ||
		 (0 == strcmp( argv[1], "in" )) ) 
	  result = inputHandler();
	else if ( (0 == strcmp( argv[1], "output")) ||
		 (0 == strcmp( argv[1], "out" )) ) 
	  result = outputHandler();
	else if (0 == strcmp( argv[1], "update"))    // temporary
	  result = updateHandler();
	else {
	  std::string txtmsg("invalid command name:");
	  for (int index=0; index < argc; index++ ) {
	    txtmsg += " ";
	    txtmsg += argv[index];
	  }
	  theFrame->setError( txtmsg.c_str() );
	  result = AbsInterp::ERROR;
	} 
      } 
    else {
      theFrame->setError( "not enough arguments" );
      result = AbsInterp::ERROR;
    }
    return result;
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

// temporary
int AppBasicCommand::updateHandler(){
  return AbsInterp::OK;
}

int 
AppBasicCommand::appendHandler() {
  AppFramework* theFrame = (AppFramework*)target();
  theFrame->setError( "invalid command name: append" );
  return AbsInterp::ERROR; }


int 
AppBasicCommand::cloneHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: clone" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::createHandler() {
  AppFramework* theFrame = (AppFramework*)target();
  theFrame->setError( "invalid command name: create" );
  return  AbsInterp::ERROR; }

int 
AppBasicCommand::deleteHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: delete" );
  return  AbsInterp::ERROR; }

int 
AppBasicCommand::disableHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: disable" );
  return AbsInterp::ERROR; }

int
AppBasicCommand::enableHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: enable" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::helpHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->fullReport( "The following commands are available:" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::insertHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: insert" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::listHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: list" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::removeHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: remove" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::talkToHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: talk(To)" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::inputHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: in(put)" );
  return AbsInterp::ERROR; }

int 
AppBasicCommand::outputHandler() {
  AppFramework* theFrame = (AppFramework*)target();  
  theFrame->setError( "invalid command name: out(put)" );
  return AbsInterp::ERROR; }



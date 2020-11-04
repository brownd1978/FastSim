//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppActionCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppActionCommand. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Marc Turcotte			Original Author, 4/15/98
//
// Copyright Information:
//	Copyright (C) 1998 		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// This Class's Header
//
#include "Framework/AppActionCommand.hh"

//
// C Headers
//
#include <assert.h>
#include <string.h>

//
// Collaborating Class Headers
//
#include "Framework/AppModule.hh"
#include "FrameUtil/AbsInterp.hh"
#include "Framework/AppActionController.hh"
#include "Framework/AppAction.hh"
#include "Framework/AppFramework.hh"
#include "FrameUtil/APPListIterator.hh"

AppActionCommand::AppActionCommand( 
    const char* const theCommand, 
    AppModule* theTarget )
    : AppCommand( theCommand, theTarget ) {
}


AppActionCommand::~AppActionCommand( ) {
}

//
// Handler
//

int 
AppActionCommand::handle( int argc, char* argv[] )
{
    AppFramework* theFrame = (AppFramework*)target( );
    int result = AbsInterp::ERROR;
    
    setArgs( argc, argv );
    if ( 2 <= argc ) {
	if        ( 0 == strcmp( argv[1], "list" ) ) {
	    result = listHandler( argc, argv );
	} else if ( 0 == strcmp( argv[1], "enable" ) ) { 
	    result = enableHandler( argc, argv );
	} else if ( 0 == strcmp( argv[1], "disable" ) ) { 
	    result = disableHandler( argc, argv );
        } else if ( 0 == strcmp( argv[1], "help" ) ) {
            result = helpHandler();
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


int 
AppActionCommand::listHandler( int argc, char* argv[] ) {

  AppFramework* theFrame  = target()->framework();
            
  // List all Actions

  APPListIterator<AppActionController*> 
    iter2( *(theFrame->actionControllers()) );
  AppActionController** anActionController;
  while ( ( anActionController=iter2() ) ) {        
    target()->partialReport("Action ");
    target()->partialReport((*anActionController)->action()->name());
    target()->partialReport(" is ");
    if( (*anActionController)->doAction() ) {   
      target()->fullReport("Enabled.");
    } else {                               
      target()->fullReport("Disabled.");
    }  
  }	 
  
  return AbsInterp::OK;

}

int 
AppActionCommand::enableHandler( int argc, char* argv[] ) {

  AppFramework* theFrame  = target()->framework();
  int result              = AbsInterp::ERROR; 

  int index = 2;
  if ( 0==getArgument(index) ) { 
    // Neither "all" or a list of Action names supplied
    target()->partialReport("Warning: \"");
    target()->partialReport(getArgument(0));
    target()->partialReport(" ");
    target()->partialReport(getArgument(1));
    target()->fullReport( "\" - no action name supplied" );
    return result;
  }
  
  // Process "all" specifying "all Actions"
  if ( 0 == strcmp(getArgument(index),"all") ) { 
    APPListIterator<AppActionController*> 
      iter( *(theFrame->actionControllers()) );
    AppActionController** anActionController;
    while ( ( anActionController=iter() ) ) {  
      (*anActionController)->setDoAction(true);
    }
    result = AbsInterp::OK;
    return result; 
  }

  // Process list of individual Action names 
  while ( getArgument( index ) != 0 ) { 
    std::string actionName;
    actionName = getArgument(index); 
    APPListIterator<AppActionController*> 
      iter( *(theFrame->actionControllers()) );
    iter.rewind();
    AppActionController** anActionController;
    bool foundIt;
    foundIt = false;
    while ( ( anActionController=iter() ) ) { 
      if ( actionName==(*anActionController)->action()->name() ) {
	foundIt=true;
	(*anActionController)->setDoAction(true); 
      }
    }
    if (!foundIt) { // This Action does not exist
      target()->partialReport("Error: could not find this Action: ");
      target()->fullReport(actionName.c_str());
      return result; 
    }       
    index++;
  } 
    
  result = AbsInterp::OK;
  return result;
}

int 
AppActionCommand::disableHandler( int argc, char* argv[] ) {

  AppFramework* theFrame  = target()->framework();
  int result              = AbsInterp::ERROR; 

  int index = 2;
  if ( 0==getArgument(index) ) {
    // Neither "all" or a list of Action names supplied
    target()->partialReport("Warning: \"");
    target()->partialReport(getArgument(0));
    target()->partialReport(" ");
    target()->partialReport(getArgument(1));
    target()->fullReport( "\" - no action name supplied" );
    return result;
  }

  // Process "all" specifying "all Actions"
  if ( 0 == strcmp(getArgument(index),"all") ) { 
    APPListIterator<AppActionController*> 
      iter( *(theFrame->actionControllers()) );
    AppActionController** anActionController;
    while ( ( anActionController=iter() ) ) {  
      (*anActionController)->setDoAction(false);
    }
    result = AbsInterp::OK;
    return result; 
  }

  // Process list of individual Action names 
  while ( getArgument( index ) != 0 ) { 
    std::string actionName;
    actionName = getArgument(index); 
    APPListIterator<AppActionController*> 
      iter( *(theFrame->actionControllers()) );
    iter.rewind();
    AppActionController** anActionController;
    bool foundIt;
    foundIt = false;
    while ( ( anActionController=iter() ) ) { 
      if ( actionName==(*anActionController)->action()->name() ) {
        foundIt=true;
        (*anActionController)->setDoAction(false); 
      }
    }
    if (!foundIt) { // This Action does not exist
      target()->partialReport("Error: could not find this Action: ");
      target()->fullReport(actionName.c_str());
      return result; 
    }       
    index++;
  } 
    
  result = AbsInterp::OK;
  return result;
}

std::string 
AppActionCommand::description( ) const
{
   return "Command used to control module actions";
}

int 
AppActionCommand::helpHandler( )
{
 
  target()->fullReport( "The following commands are available:"); 

  target()->partialReport( "  disable <act1> [<act2> ..]            " ); 
  target()->fullReport("Disable action(s)*");

  target()->partialReport( "  enable <act1> [<act2> ..]             " );
  target()->fullReport("Enable action(s)*");

  target()->partialReport( "  help                                  " );
  target()->fullReport("Show available commands");

  target()->partialReport( "  list                                  " );
  target()->fullReport("List all defined actions");

  target()->fullReport(" ");
  target()->fullReport("* = 'all' may be used in place of <act1>.");

  return AbsInterp::OK;

}

void
AppActionCommand::show( ) const {
}

bool
AppActionCommand::isShowable( ) const {
   return false; 
}









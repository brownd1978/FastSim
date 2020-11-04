//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppModuleCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppModuleCommand. Command handler for the "module" command for
//	the BaBar Application Framework. Valid subcommands are in 
//      helpHandler().
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Introduced Actions  May '97
//      Marc Turcotte                   Removed dependence on RW
//                                      to allow sharing    June '97
//      Marc Turcotte                   Added "list" to actions command 4/10/98
//      Marc Turcotte                   Added checks on whether TK is up 6/18/98
//      Marc Turcotte                   Added error reporting to attempting to
//                                      talkto an unknown module. 7/1/98
//                                      Also made it NOT a TCL error to disable
//                                      an unknown module, as per Bob Jacobsen's
//                                      request. 7/1/98
//      Marc Turcotte                   Implemented clone command 8/6/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997. 1998        Unviersity of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppModuleCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppExecNode.hh"
#include "Framework/AppPath.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppInputModule.hh"
#include "Framework/AppOutputModule.hh"
#include "Framework/AppFramework.hh"
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "FrameUtil/AbsInterp.hh"
#include "Framework/AppUserBuild.hh"
#include "Framework/AppCloneRecorder.hh"
#include "Framework/AppReportAttributes.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

static const int firstIndex = 2;

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppModuleCommand::AppModuleCommand( 
   const char* const theCommand,  AppModule* theTarget )
    : AppBasicCommand( theCommand, theTarget )
{
}

//--------------
// Destructor --
//--------------

AppModuleCommand::~AppModuleCommand( )
{
}

//-----------
// Handler --
//-----------

int 
AppModuleCommand::handle( int argc, char* argv[] )
{
    int result = AbsInterp::ERROR;
    AppFramework* theFrame = (AppFramework*)target( );

    if ( AbsInterp::CONTINUE == (result = AppBasicCommand::handle( argc, argv )) ) {
       if ( 2 <= argc ) {
	  if        ( 0 == strcmp( argv[1], "input" ) )     {
	     result = inputHandler( );
	  } else if ( 0 == strcmp( argv[1], "in" ) )        {
	     result = inputHandler( );
	  } else if ( 0 == strcmp( argv[1], "output" ) )    {
	     result = outputHandler( );
	  } else if ( 0 == strcmp( argv[1], "out" ) )       {
	     result = outputHandler( );
	  } else if ( 0 == strcmp( argv[1], "talk" ) )      {
	     result = talkToHandler( );
	  } else if ( 0 == strcmp( argv[1], "talkTo" ) )    { 
	     result = talkToHandler( );
	  } else {
	    std::string txtmsg("invalid command name: ");
	    for (int index=0; index < argc; index++ ) {
	      txtmsg += " ";
	      txtmsg += argv[index];
	    }
	    theFrame->setError( txtmsg.c_str() );
	  }
       }
    }
    return result;
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

int AppModuleCommand::cloneHandler() { // Clone handler

  AppFramework*  theFrame = (AppFramework*)target( );
  char*          theName;
  const char*    theNewName;
  AppModule*    theMod;
  
  int secondIndex  = firstIndex+1;     // clone name
  int result       = AbsInterp::ERROR;
  
  theName    = getArgument(  firstIndex );
  theNewName = getArgument( secondIndex );

  if ( 0==theName ) { // no target module name supplied
    target()->setError( "no target or clone names were supplied" );
    return result;
  }  
  
  if ( 0==theNewName ) { // no clone name supplied
    target()->setError( "no clone name was supplied" );
    return result;
  } 
  
  if ( 0==strcmp(theName, theNewName) ) {
    target()->setError( "cloning a module to itself doesn't make sense;" 
			"choose a different name..." );
    return result;
  }

  // cloned module name already exists
  if ( 0 != theFrame->fetchModule( theNewName ) ) {
    std::string theError(theNewName);
    theError += " module already exists.  It will not be cloned.";
    target()->setError( theError.c_str() );
    return result;
  }
 
  theMod = theFrame->fetchModule( theName );
  if ( 0 != theMod) {
    AppModule* theClone = theMod->clone(theNewName);
    if ( theClone != 0 ) {
      theFrame->appBuilder()->insert(theClone,theMod); // Add clone to the Framework
      AppCloneRecorder* cloneRecorder = 
	new AppCloneRecorder(theName,theNewName);
      theFrame->add(cloneRecorder);
      return AbsInterp::OK;
    } else {
      return result;
    }
  }
  else {
    std::string message("cannot find user module by the name ");
    message += theName;
    theFrame->setError(message.c_str() );
  }

  return result;  

}

int 
AppModuleCommand::disableHandler( )
{
  AppFramework* theFrame = (AppFramework*)target( );
  char* theName;
  AppExecutable* theExec;
  int index  = firstIndex;
  int result = AbsInterp::OK;
  std::string unknownName;
    
  while ( NULL != ( theName = getArgument( index ) ) ) {

    if ( NULL != ( theExec = theFrame->fetchModule( theName )) ) {
      theExec->setEnabled( false );

    } else if ( 0 == strcmp(theName, "all") ) {
      
      APPList<AppModule*> allModules;      
      theFrame->buildModuleList( allModules, *(theFrame->modules()) );
      theFrame->buildModuleList( allModules, *(theFrame->inputModules()) );
      theFrame->buildModuleList( allModules, *(theFrame->outputModules()) );
      
      AppModule** theModule;
      APPListIterator<AppModule*> iter( allModules );
      while ( ( theModule = iter( ) ) ) {         
	(*theModule)->setEnabled( false );
      }	
      
    } else {

      unknownName += theName;
      unknownName += " ";
    }
    index++;
  }

  if (unknownName.size() != 0) {
    unknownName += ": unknown module";
    if (theFrame->strictModuleDisable() ) {
      theFrame->setError(unknownName.c_str());
      result = AbsInterp::ERROR;
    }
    else {
      target()->partialReport("Warning : ");
      target()->fullReport( unknownName.c_str( ) ); 
    }
  }

  if ( firstIndex == index ) {
    theFrame->setError( "no name supplied" );
    result = AbsInterp::ERROR;
  }
  return result;
}

int 
AppModuleCommand::enableHandler( )
{
  AppFramework* theFrame = (AppFramework*)target( );
  char* theName;
  AppExecutable* theExec;
  int index  = firstIndex;
  int result = AbsInterp::OK;
  std::string theModName;
  
  while ( NULL != ( theName = getArgument( index ) ) ) {
    
    if ( NULL != ( theExec = theFrame->fetchModule( theName )) ) {
      theExec->setEnabled( true );
      
    } else if ( 0 == strcmp(theName, "all") ) {
      
      APPList<AppModule*> allModules;      
      theFrame->buildModuleList( allModules, *(theFrame->modules()) );
      theFrame->buildModuleList( allModules, *(theFrame->inputModules()) );
      theFrame->buildModuleList( allModules, *(theFrame->outputModules()) );
      
      APPListIterator<AppModule*> iter( allModules );
      AppModule** theModule;
      while ( ( theModule = iter( ) ) ) {         
	(*theModule)->setEnabled( true );
      }
      
    } else if ( 0 == strcmp(getArgument( firstIndex ), "path") ) { 
      //enable modules in the given path
      
      AppPath** thePath;
      APPListIterator<AppPath*> iterPath( *theFrame->paths() );
      while ( ( thePath = iterPath() ) ) {
	if ( NULL != getArgument ( index + 1 ) ){ 
	  if ( strcmp((*thePath)->name(), getArgument(index+1)) == 0 ){ 
	    //this is the path
	    AppExecNode** theNode;
	    APPList<AppExecNode*> nodes;
	    //this actually builds a list modules for this path      
	    (*thePath)->buildNodeList( nodes, *( (*thePath)->nodes() )); 
	    APPListIterator<AppExecNode*> iterNode( nodes );
	    while ( ( theNode = iterNode() ) ) {         
	      ((AppModule*) (*theNode)->target() )->setEnabled( true );
	    }
	  }
	}
      }
      
    } else {
      theModName += theName;
      theModName += " ";
    }
    
    index++;
  } 

  // Generate meaningful error if unknown modules were in the args
  if (theModName.size() != 0) {
      theModName += ": unknown module(s)";
      theFrame->setError( theModName.c_str( ) );
      result = AbsInterp::ERROR;
  }
  else if ( firstIndex == index ) {
    theFrame->setError( "no name supplied" );
    result = AbsInterp::ERROR;
  }
  return result;
}
  
int 
AppModuleCommand::helpHandler( )
{
  
    AppBasicCommand::helpHandler( );

    target()->partialReport( "  clone <mod> <new-mod>                 " );
    target()->fullReport("Clones <mod> to <new-mod>");

    target()->partialReport( "  disable <mod1> [<mod2> ..]            " );
    target()->fullReport("Disable module(s)*");

    target()->partialReport( "  enable <mod1> [<mod2> ..]             " );
    target()->fullReport("Enable module(s)*");

    target()->partialReport( "  enable path <path1> [<path2> ..]      " );
    target()->fullReport("Enable modules along the path(s)");

    target()->partialReport( "  help                                  " );
    target()->fullReport("Show available commands");

    target()->partialReport( "  in(put) <mod>                         " );
    target()->fullReport("Specify the input module");

    target()->partialReport( "  list [-b(rief)]                       " );
    target()->fullReport("List the modules");

    target()->partialReport( "  out(put) <mod>                        " );
    target()->fullReport("Specify the output module");

    target()->partialReport( "  talk(To) <mod>                        " );
    target()->fullReport("Talk to module");

    target()->fullReport(" ");
    target()->fullReport("* = 'all' may be used in place of <mod1>");

    return AbsInterp::OK;
}


int 
AppModuleCommand::inputHandler( )
{
    AppFramework* theFrame = (AppFramework*)target( );
    char* theName;
    int result = AbsInterp::ERROR;
    
    theName = getArgument( firstIndex );
    if ( NULL != theName ) {
        if ( theFrame->setInputModule( theName ) ) {
	   theFrame->setBegun( false );
	   result = AbsInterp::OK;
        } else {
	   std::string theModName(theName);
	   std::string theTrailer(": unknown module");
	   std::string theMessage=theModName+theTrailer;
	   theFrame->setError( theMessage.c_str() );
	}
    } else {
       if (theFrame->theInputModule() != NULL)
       {
         std::string message("no arg supplied; input module is unchanged : ");
         message += theFrame->theInputModule()->name();
         theFrame->setError(message.c_str());
         result = AbsInterp::OK;
       }
       else
	 theFrame->setError("no arg supplied to module input");
    }
    return result;
}

int 
AppModuleCommand::listHandler( )
{

  AppFramework* theFrame = (AppFramework*)target( );
  AppReportAttributes attrib;
  char* theName = 0;
  int index = firstIndex;
  std::string unknownName;
  while ( 0 != ( theName = getArgument( index ) ) ) {
    if ( 0 == strcmp( theName, "-brief" ) ||
	 0 == strcmp( theName, "-b"      ) ) {
      attrib.setBrief(true);
    }
    // Not yet implemented !
    /*
    else if ( 0 == strcmp( theName, "-tcl" ) ||
	      0 == strcmp( theName, "-t"      ) ) {
      index++;
      char* theVarName = getArgument( index );
      if (theVarName != 0) {
	attrib.setTclOutput( theVarName );
      }
      else {
	unknownName += theName;
        unknownName += " is mising argument ";
      }
    }
    */
    else {
      unknownName += theName;
      unknownName += " ";
    }
    index++;
  }
  if (unknownName.size() != 0) {
    unknownName += ": unknown options";
    theFrame->setError(unknownName.c_str());
    return AbsInterp::ERROR;
  }

  AppModule** mod;
  
  if ( ! theFrame->modules( )->isEmpty( ) ) {
    APPListIterator<AppModule*> iter1( *theFrame->modules( ) );
    while ( ( mod = iter1( ) ) ) {
      (*mod)->report(&attrib, 0);
    }
  } else {
    theFrame->fullReport( "     No modules are defined" );
  }
  if ( ! attrib.listTclOutput() ) {
    theFrame->fullReport( NULL );
    theFrame->fullReport( "Input Modules" );
  }
  if ( ! theFrame->inputModules( )->isEmpty( ) ) {
    APPListIterator<AppInputModule*> iter2( *theFrame->inputModules( ) );
    while ( ( mod = (AppModule**)iter2( ) ) ) {
      (*mod)->report(&attrib, 0);
    }
  } else {
    theFrame->fullReport( "     No Input Modules are defined" );
  }
  if ( ! attrib.listTclOutput() ) {
    theFrame->fullReport( NULL );
    theFrame->fullReport( "Output Modules" );
  }
  if ( ! theFrame->outputModules( )->isEmpty( ) ) {
    APPListIterator<AppOutputModule*> iter3( *theFrame->outputModules( ) );
    while ( ( mod = (AppModule**)iter3( ) ) ) {
      (*mod)->report(&attrib, 0);
    }
  } else {
    theFrame->fullReport( "     No Output Modules are defined" );
  }
    
  return AbsInterp::OK;
}

int 
AppModuleCommand::outputHandler( )
{
    AppFramework* theFrame = (AppFramework*)target( );
    char* theName;
    int result = AbsInterp::ERROR;
    
    theName = getArgument( firstIndex );
    if ( NULL != theName ) {
        if ( theFrame->setOutputModule( theName ) ) {
	   result = AbsInterp::OK;
        } else {
	   std::string theModName(theName);
	   std::string theTrailer(": unknown module");
	   std::string theMessage=theModName+theTrailer;
	   theFrame->setError( theMessage.c_str() );
	}
    } else {
      std::string message("no arg supplied; output module is unchanged : ");
      message += theFrame->theOutputModule()->name();
      theFrame->setError( message.c_str());
      result = AbsInterp::OK;
    }
    return result;
}

int 
AppModuleCommand::talkToHandler( )
{
    AppFramework* theFrame = (AppFramework*)target( );
    char* theName;
    AppModule* theMod;
    int result = AbsInterp::ERROR;

    theName = getArgument( firstIndex );
    if ( NULL != theName ) {
        if ( NULL != ( theMod = theFrame->fetchModule( theName )) ) {
			theFrame->disableCommands( );
			theMod->talkTo           ( );
			result = AbsInterp::OK;
        } else {
	  std::string theModName(theName);
	  std::string theTrailer(": unknown module");
	  std::string theMessage=theModName+theTrailer;
	  theFrame->setError( theMessage.c_str() );
	}
    } else {
		theFrame->setError( "no name supplied" );
    }
    return result;
}
std::string
AppModuleCommand::description( ) const
{
   return "Command used to control and specify modules";
}

void
AppModuleCommand::show( ) const 
{
}
bool
AppModuleCommand::isShowable( ) const 
{
   return false; 
}

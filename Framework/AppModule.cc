// File and Version Information:
// 	$Id: AppModule.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppModule. This class describes modules within the BaBar
//	Application Framework.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Bob Jacobsen                    Brought into AbsEvent conformance
//      Bob Jacobsen                    _commands must be unowned list
//      P.Murat                         add handling of menu files
//      Marc Turcotte                   Added new style begin<Job/Run>
//                                      Added new style event.
//      Marc Turcotte                   Added execute member function
//                                      to support dynamic dispatch 3/98
//      Marc Turcotte                   Introduced AppStopType 3/20/98
//      Marc Turcotte                   Renamed execute to frame 3/26/98
//      Marc Turcotte                   Modified to pass frames 'round 4/7/98 
//      Marc Turcotte                   Modified to not barf if zero
//                                      frame passes thru meaning someone
//                                      forgot to line up a provider
//                                      in the time ordered series 4/8/98                                    
//      Marc Turcotte                   Added AppActionController 4/13/98
//      Marc Turcotte                   Modified setActionEnabled to set
//                                      Action Controllers. 4/15/98
//      Marc Turcotte                   Removed static init of _theFrame 4/30/98
//      Marc Turcotte                   Merged R.Kapur's TK interface 5/98
//      Marc Turcotte                   Modified for AppStatus 6/20/98
//      Marc Turcotte                   Moved dispatch from frame to here 6/22/98
//      Marc Turcotte                   Added input/output Frame 6/22/98
//      Marc Turcotte                   Added theDispatchStopType to
//                                      signatures 6/23/98
//      Marc Turcotte                   Changed AppStatus to AppResult 6/23/98
//      Marc Turcotte                   Modified for new AbsEvent sigs 6/24/98
//      Marc Turcotte                   Added clone command 8/6/98
//      Marc Turcotte                   Modified used of getStopTypeKey 
//                                      not to cause a leak. 8/21/98
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
#include "Framework/AppModule.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppEchoCommand.hh"
#include "Framework/AppExitCommand.hh"
#include "Framework/AppHelpCommand.hh"
#include "Framework/AppStopType.hh"
//? #include "Framework/AppRecord.hh"
#include "Framework/AppMethodBase.hh"
#include "Framework/AppMethod.hh"
class AppFrame;
#include "Framework/AppHash.hh"
#include "FrameUtil/AbsInterp.hh"

#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

//-----------------------
// Static Initializers --
//-----------------------

AppMenu   AppModule::moduleMenu;

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppModule::AppModule( 
		     const char* const theName, 
		     const char* const theDescription )
  : AppExecutable( theName, theDescription ), 
  _error   (NULL ),
  _errorLen( 0 ),
  _menuFileName(NULL ),
  _echo("echo", this),
  _exit("exit", this),
  _help("help", this),
  _show("show", this),
  _sourceFileCmd( "sourceFoundFile", this ),
  _verbose("verbose", this, false),
  _production("production", this, false), 
  _enableFrames("enableFrames", this, false)      

{
    _execType = APP_module;
    _isInitialized = false;

    
    _commands = new APPList< AppCommand* >;
    _commands->append( &_echo );
    _commands->append( &_exit );
    _commands->append( &_help );
    _commands->append( &_show );
    _commands->append( &_sourceFileCmd);     
    _commands->append( &_verbose );
    _commands->append( &_production );
    _commands->append( &_enableFrames );

    _prompt = new char[strlen( theName )+3];
    strcpy( _prompt, theName );
    strcat( _prompt, "> ");
    
    // Dynamic Dispach Map
    
    _theDynamicDispatchMap = new AppHash < AppMethodBase >;
    
}

//--------------
// Destructor --
//--------------

AppModule::~AppModule( )
{
    delete _commands;
    
    if ( NULL != _prompt ) {
        delete [] _prompt;
    }
    if ( NULL != _error ) {
        delete [] _error;
    }
    
    if ( NULL != _menuFileName ) {
        delete [] _menuFileName;
    }
  
    delete _theDynamicDispatchMap; // delete the dispatch map

}

//--------------
// Operations --
//--------------

AppResult
AppModule::beginJob( AbsEvent* anEvent ){
  return AppResult::OK;
}

AppResult
AppModule::beginRun( AbsEvent* anEvent)
{
  return AppResult::OK;
}

AppResult
AppModule::frame( AppFrame* aFrame,
                  const AppStopType& theDispatchStopType) {  // Dynamic dispatch 
  
  AppResult theResult = AppResult::OK;  
  
  if ( aFrame!=0 ) { // Zero frames go nowhere...
    
    string theStopTypeKey = theDispatchStopType.getStopTypeKey();     
    if((_theDynamicDispatchMap->has(theStopTypeKey.c_str()))==true) 
    {// there is there a binding
      AppMethodBase* theMethod = _theDynamicDispatchMap->fetch(theStopTypeKey.c_str());
      theResult=theMethod->execute(aFrame); // then just deliver the frame to the binding...
    }

  } else {
   
   fullReport("AppModule:: Error: You are moving a null Frame pointer.");
  
  }
  
  return theResult.value(); 

}

AppResult
AppModule::event( AbsEvent* anEvent )
{
    return AppResult::OK;
}

AppResult
AppModule::other( AbsEvent* anEvent )
{
    return AppResult::OK;
}

AppResult
AppModule::endRun( AbsEvent* anEvent) {
    return AppResult::OK;
}

AppResult
AppModule::endJob( AbsEvent* anEvent){
    return AppResult::OK;
}

AppResult
AppModule::abortJob( AbsEvent* anEvent ){
    return AppResult::OK;
}

void
AppModule::talkTo( )
{
    enableCommands( );
}

// for the Framework migration.  Will be removed soon.
void
AppModule::help( )
{
}
void 
AppModule::show()
{
}


void
AppModule::help( int argc, char** argv )
{
  // default behavior, unless overridden, is to list commands
  // available in this module
  partialReport( "Command(s) available in the \"");
  partialReport( name() );
  fullReport("\" module:\n" );

  AppCommand** command;
  APPListIterator<AppCommand*> theIterator( *commands( ) );
  while ( ( command = theIterator( ) ) ) 
  {
     if ( argc == 1 ) // Describe the whole list of commands
     {
	partialReport( "      " );
	partialReport((*command)->command() ) ;
	if ( strlen( (*command)->command() ) < 10 )
	{
	   partialReport("\t\t");
	}
	else
	{
	   partialReport("\t");	
	}
	fullReport((*command)->description( ).c_str( ));
     }
     else  // Describe the one they asked for
     {
	if ( strcmp( (*command)->command( ), argv[1] ) == 0 )
	{
	   partialReport( "      " );
	   partialReport((*command)->command() ) ;
	   partialReport("\t");
  	   fullReport((*command)->description( ).c_str( ));
	}
     }
  }
}

void
AppModule::show( int argc, char** argv ) const
{
  // default behavior, unless overridden, is to display the
  // current values of commands available in this module
  partialReport( "Current value of item(s) in the \"");
  partialReport( name() );
  fullReport("\" module:\n" );

  AppCommand** command;
  APPListIterator<AppCommand*> theIterator( *commands( ) );
  while ( ( command = theIterator( ) ) ) 
  {
     if ( argc == 1 ) // Show the whole list of showable commands
     {
	if ( (*command)->isShowable( ) )
	{
	   partialReport( "      " );
	   (*command)->show( );
	}
     }
     else  // Show the one they asked for
     {
	if ( strcmp( (*command)->command( ), argv[1] ) == 0 )
	{
	   partialReport( "      " );
	   (*command)->show( );
	}
     }
  }
}

AppModule* 
AppModule::clone(const char* cloneName) {
  partialReport("cloning to ");
  partialReport(cloneName);
  partialReport(" has not been implemented by ");
  partialReport(this->name());
  partialReport(" user module...\n");
  return 0;
}

bool
AppModule::verbose() const
{
  return _verbose.value();
}

bool
AppModule::production() const
{
  return _production.value();
}

bool
AppModule::isShowable( ) const 
{
   return true; 
}

void AppModule::menuHandler(char* menu, char* command, int argc, char** argv) {
  partialReport(" -------------------- menuHandler entered module : "); 
  fullReport(this->name());
  partialReport("menu    = ");
  fullReport(menu);
  partialReport("command = ");
  fullReport(command);
  partialReport(" N(argv) = ");
  ostringstream strbuff;
  strbuff << argc;
  fullReport( strbuff.str().c_str() );
  for (int i=0; i<argc; i++) {
    fullReport(argv[i]);
  }
}

void
AppModule::exit( )
{
    disableCommands( );
    framework( )->enableCommands( );
}

void
AppModule::disableCommands( )
{
   AppCommand** command;
   APPListIterator<AppCommand*> theIterator( *commands( ) );
   while ( ( command = theIterator( ) ) ) {
      (*command)->disable( );
   }
}

void
AppModule::enableCommands( )
{
    AppCommand** command;
    APPListIterator<AppCommand*> theIterator( *commands( ) );
    while ( ( command = theIterator( ) ) ) 
    {
      (*command)->enable( );
    }
    AbsInterp* interp = AbsInterp::theInterpreter( );
    interp->setCommandPrompt( prompt( ) );
}

//-------------
// Selectors --
//-------------
    
APPList< AppCommand* >* 
AppModule::commands ( ) const
{
    return _commands;
}

bool
AppModule::isInitialized( ) const
{
    return _isInitialized;
}

bool
AppModule::passed( ) const
{
    return true;
}

char*
AppModule::prompt( ) const
{
    return _prompt;
}

char* 
AppModule::menuFileName( ) const {
    return _menuFileName;
}

//-------------
// Modifiers --
//-------------

void 
AppModule::setError( const char* const theError )
{
	if ( NULL != theError ) {
		if ( _errorLen < strlen( theError )+1 ) {
			if ( NULL != _error ) {
				delete [] _error;
			}
			_errorLen = strlen( theError )+1;
			_error    = new char[_errorLen];
		}
		strcpy( _error, theError );
	} else {
		if ( NULL != _error ) {
			delete [] _error;
			_error    = NULL;
			_errorLen = 0;
		}
	}
	AbsInterp* interp = AbsInterp::theInterpreter( );
	interp->setResult( _error );
}

void 
AppModule::setInitialized( )
{
    _isInitialized = true;
}
    
void 
AppModule::setPrompt( const char* const thePrompt )
{
    if ( NULL != _prompt ) {
		delete [] _prompt;
		_prompt = NULL;
    }
    if ( NULL != thePrompt ) {
		_prompt = new char[strlen( thePrompt )+1];
		strcpy( _prompt, thePrompt );
    }
}
//
// Provide a means to bind a derived module's method to a particular frame state
//
void AppModule::bindMethodToState(const AppStopType& aStopType,
                                  const AppMethodBase* const aMethod) {
  // 
  // Make an entry in the hash table of AppMethodBase pointers indexed by the Frame State.
  //
  
  string aStopTypeKey = aStopType.getStopTypeKey();
  assert(_theDynamicDispatchMap->add(aStopTypeKey.c_str(),aMethod)==true); // cannot fail...    
  partialReport("AppModule: ");
  partialReport(name());
  partialReport(" added this hash key: ");
  fullReport(aStopTypeKey.c_str());
}

AppHash < AppMethodBase >* AppModule::dynamicDispatchMap() {
  return _theDynamicDispatchMap;
}

void AppModule::setMenuFileName(const char* const filename )
{
  if (! filename) return;
  if (_menuFileName) {
    delete _menuFileName;
  }
  _menuFileName = new char[strlen(filename)+1];
  strcpy(_menuFileName,filename);
}

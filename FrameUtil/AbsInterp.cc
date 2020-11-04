//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsInterp.cc 508 2010-01-14 15:18:18Z stroili $
//
// Description:
//	Class AbsInterp
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar/CDF Framework
//
// Author List:
//	Liz Sexton-Kennedy		        Original Author
//
// Copyright Information:
//	Copyright (C) 1997		        FNAL
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FrameUtil/AbsInterp.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include <stdlib.h>
#include <string.h>

#include <iostream>

#include "FrameUtil/TclInterp.hh"
#include "FrameUtil/FtclInterp.hh"
using std::cout;
using std::endl;
using std::flush;
//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

AbsInterp* AbsInterp::_theInterpreter = 0;

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AbsInterp::AbsInterp( ) :   
   _commandPrompt   ( NULL )
{
}

//--------------
// Destructor --
//--------------

AbsInterp::~AbsInterp( )
{
    delete [] _commandPrompt;
}

// members
AbsInterp* AbsInterp::theInterpreter( )
{

   if ( _theInterpreter == 0 )
   {
      const char* interpType = getenv( "INTERP_TYPE" );
      if ( ( interpType != 0 ) && ( strcmp( interpType, "tcl" ) == 0 ) )
      {
	 _theInterpreter = new TclInterp( );
      }
      else
      {
	 _theInterpreter = new FtclInterp( );
      }      
   }
   return _theInterpreter;
}

AbsInterp* AbsInterp::theInterpreter(Tcl_Interp* interp){
  if ( _theInterpreter == 0 ) {
    const char* interpType    = getenv( "INTERP_TYPE" );
    if ( ( interpType != 0 ) && ( strcmp( interpType, "tcl" ) == 0 ) ) {
      _theInterpreter = new TclInterp(interp);
    }
    else {
      _theInterpreter = new FtclInterp(interp);
    }      
  }
  return _theInterpreter;  
}

bool AbsInterp::isInitialized()
{
   return ( _theInterpreter != 0 );
}

void AbsInterp::sendPartialMessage( const char* const aMessage ) const
{
   if ( NULL != aMessage &&
	0    != strlen( aMessage ) ) 
   {
      cout << aMessage;
#if DEBUG
      cout << flush;
#endif
   }  
}

void AbsInterp::sendFullMessage( const char* const aMessage ) const
{
   if ( NULL != aMessage &&
	0    != strlen( aMessage ) ) 
   {
      cout << aMessage;
#if DEBUG
      cout << flush;
#endif
   } 
   cout << endl;
}

void AbsInterp::flushMessage( ) const
{
   cout << flush;
}

void AbsInterp::setCommandPrompt( const char* const thePrompt )
{
    if ( NULL != _commandPrompt ) {
        delete [] _commandPrompt;
		_commandPrompt = NULL;
    }
    if ( NULL != thePrompt ) {
		_commandPrompt = new char[strlen( thePrompt )+1];
		strcpy( _commandPrompt, thePrompt );
    }
}

char* AbsInterp::commandPrompt( ) const
{
   return _commandPrompt;
}

AbsInterp::StatusCode AbsInterp::parseArgs( const std::string formalCmd, int argc, char *argv[] ) const
{
    cout << "ERROR: This interpreter doesn't support parseArgs()." << endl;
    return ERROR;
}

bool AbsInterp::present( const std::string Key ) const
{
    cout << "ERROR: This interpreter doesn't support present()." << endl;
    return ERROR;
}

AbsInterp::StatusCode AbsInterp::getInt( const std::string intKey, int &intPtr ) const
{
    cout << "ERROR: This interpreter doesn't support getInt()." << endl;
    return ERROR;
}

AbsInterp::StatusCode AbsInterp::getUnsigned( const std::string uintKey, unsigned int &uintPtr ) const
{
    cout << "ERROR: This interpreter doesn't support getUnsigned()." << endl;
    return ERROR;
}

AbsInterp::StatusCode AbsInterp::getDouble( const std::string doubleKey, double &doublePtr ) const
{
    cout << "ERROR: This interpreter doesn't support getDouble()." << endl;
    return ERROR;
}

AbsInterp::StatusCode AbsInterp::getString( const std::string strKey, std::string* strPtr ) const
{
    cout << "ERROR: This interpreter doesn't support getString()." << endl;
    return ERROR;
}

AbsInterp::StatusCode AbsInterp::getCstring( const char* charKey, char* *charPtr ) const
{
    cout << "ERROR: This interpreter doesn't support getCstring()." << endl;
    return ERROR;
}

bool AbsInterp::fileOperation( const char* const fileName,
			       const char* const operation ) const
{
   cout << "ERROR: This interpreter doesn't support File operation.";
   return false;
}

void AbsInterp::setResult( const char* result ) const
{
   cout << "ERROR: This interpreter doesn't support setResult.";
}

void AbsInterp::appendResult( const char* result ) const
{
  cout << "ERROR: This interpreter doesn't support setResult.";
}

void AbsInterp::simulateInput( std::string strInput )
{
   cout << "ERROR: This interpreter doesn't support simulateInput.";
}

bool AbsInterp::existsCommand( const char* commandName  ) const
{
   cout << "ERROR: This interpreter doesn't support existsCommand.";
   return false;
}

void 
AbsInterp::setDoLoopInterp( bool stayInLoop) {
  _doLoopInterp = stayInLoop;
}

bool 
AbsInterp::getDoLoopInterp() const {
  return _doLoopInterp;
}



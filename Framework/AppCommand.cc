//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppCommand. The abstract parent of all command classes. Each
//	descendant class handles a single command, either for the framework
//	itself, or for one of its modules. A command has a target upon which
//	it acts, and can be enabled or disabled.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Merged R.Kapur's TK interface 5/98
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
#include "Framework/AppCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>
#include <stdlib.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppNameNode.hh"
#include "FrameUtil/AbsInterp.hh"

//              -----------------------------------------------
//              -- Static Data & Function Member Definitions --
//              -----------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppCommand::AppCommand( )
  : _enabled( false )
  , _isValid( false )
{
}

AppCommand::AppCommand( const char* const theCommand, AppModule* theTarget )
  : _target ( theTarget )
  , _enabled( false )
{

    assert( NULL != theTarget );
    assert( NULL != theCommand );
    
    _command = new char[strlen( theCommand )+1];
    strcpy( _command, theCommand );
    _isValid = true;
}

//--------------
// Destructor --
//--------------

AppCommand::~AppCommand( )
{
    if ( _isValid ) delete [] _command;
}

void
AppCommand::initialize( const char* const theCommand, AppModule* theTarget )
{
   assert( NULL != theTarget );
   assert( NULL != theCommand );

   _target = theTarget;
   _command = new char[strlen( theCommand )+1];
   strcpy( _command, theCommand );
   _isValid = true;
}
//-------------
// Selectors --
//-------------

const char*
AppCommand::command( ) const
{
   if ( _isValid ) 
   {
      return _command;
   }
   else
   {
      return 0;
   }
}

AppModule*
AppCommand::target( ) const
{   
   if ( _isValid ) 
   {
      return _target;
   }
   else
   {
      return 0;
   }
}

bool
AppCommand::isEnabled( ) const
{
      return _enabled;
}

//-------------
// Modifiers --
//-------------

void
AppCommand::disable( )
{

  if ( isEnabled( ) ) {
    AbsInterp* absInterp = AbsInterp::theInterpreter();
    absInterp->deleteCommand( command() );
    setEnabled( false );
  }
}

void
AppCommand::enable( )
{

  if ( ! isEnabled( ) ) {
    AbsInterp* absInterp = AbsInterp::theInterpreter();
    absInterp->createCommand( command(), this );
    setEnabled( true );
  }
}

//-----------
// Handler --
//-----------

int 
AppCommand::handle( int argc, char* argv[] )
{
  target()->partialReport(_command);
  target()->fullReport(" not implemented yet");
    return AbsInterp::OK;
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------
bool
AppCommand::isValid( )
{ 
   return _isValid;
}

void
AppCommand::setEnabled( bool mode ) 
{
   if ( _isValid ) 
   {
      _enabled = mode;
   }
}

char* 
AppCommand::getArgument( int index ) const
{
    char* result = NULL;
    
    if ( index <= _argc ) {
        result = _argv[index];
    }
    return result;
}

AppNameNode* 
AppCommand::getNameNode( int* index ) const
{
    AppFramework* theFrame = (AppFramework*)target( );
    AbsInterp* interp = AbsInterp::theInterpreter( );
    AppNameNode* result = NULL;
    char* name;
    int   mode   = 0;
    int   status = AbsInterp::OK;

    if ( *index < _argc ) {
        name = _argv[*index];
	if ( validateName( name ) ) {
	    (*index)++;
	    if ( *index+1 <= _argc ) {
		if ( 0 == strcmp( _argv[*index], "-mode" ) || 
		     0 == strcmp( _argv[*index], "-m"    ) ) {
		    (*index)++;
		    status = interp->ascii2Int( _argv[*index], &mode );
		    if ( AbsInterp::OK != status ) {
			theFrame->setError( "syntax error" );
		    }
		    (*index)++;
		}
	    }
	    result = new AppNameNode( name, mode );
	}
    }
    return result;
}

bool
AppCommand::validateName( const char* const theName ) const
{
    bool result = false;

    result = true;
    return result;
}

void
AppCommand::setArgs( int argc, char* argv[] )
{
    _argc = argc;
    _argv = argv;
}



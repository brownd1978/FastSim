//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppExecutable.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppExecutable. This is the abstract parent class for the
//	executable elements (modules, sequences & paths etc.) in the
//	BaBar Application Framework.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Introduced Actions '97
//      Marc Turcotte                   Initialized _framework to zero 4/98
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
#include "Framework/AppExecutable.hh"

//-------------
// C Headers --
//-------------
extern "C" {
#include <assert.h>
#include <stddef.h>
#include <string.h>
}

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "FrameUtil/AbsInterp.hh"
#include "Framework/AppReportAttributes.hh"

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

AppExecutable*
AppExecutable::narrow( AppExecutable* anExec, ExecType aType )
{
    AppExecutable* result = NULL;
    
    if ( aType == anExec->execType( ) ) {
	result = anExec;
    }
    return result;
}

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppExecutable::AppExecutable( )
    : _description( NULL ),
      _name( NULL ),
      _isEnabled( true ),
      _passed( true ),
      _framework ( 0 )
{
}

AppExecutable::AppExecutable( 
    const char* const theName, 
    const char* const theDescription )
    : _description( NULL ),
      _isEnabled  ( true ),
      _passed( true ),
      _framework ( 0 )
{
    assert( NULL != theName );

    _name = new char[strlen( theName )+1];
    strcpy( _name, theName );
    if ( NULL != theDescription ) {
	_description = new char[strlen( theDescription )+1];
	strcpy( _description, theDescription );
    }
}

//--------------
// Destructor --
//--------------

AppExecutable::~AppExecutable( )
{
    delete [] _name;
    if ( NULL != _description ) {
	delete [] _description;
    }
}

//-------------
// Selectors --
//-------------

void 
AppExecutable::fullReport( const char* const aMessage ) const
{
  AbsInterp* interp = AbsInterp::theInterpreter( );
  interp->sendFullMessage( aMessage );
}

void 
AppExecutable::partialReport( const char* const aMessage ) const
{
  AbsInterp* interp = AbsInterp::theInterpreter( );
  interp->sendPartialMessage( aMessage );
}

void 
AppExecutable::report( AppReportAttributes* attrib, int mode ) const
{
  if ( attrib->listTclOutput() ) {
    // Not yet implemented
  } 
  else {
    assert( NULL != framework( ) );    
    partialReport(attrib->getIndentation().c_str());
    framework()->printName( name( ), mode );
    if ( isEnabled( ) ) {
	partialReport(" (enabled) ");
    } else {
	partialReport(" (disabled)");
    }
    addToReport(attrib, mode);
    if ( NULL != description( ) & (! attrib->listBrief() ) ) {
	partialReport("\t");
	partialReport(description( ));
    }
    fullReport("");
  }
}

void
AppExecutable::flushReport( ) const
{
  AbsInterp* interp = AbsInterp::theInterpreter( );
  interp->flushMessage();
}







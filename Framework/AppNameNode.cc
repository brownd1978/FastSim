//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppNameNode.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppNameNode. Class to act as node in Commands. The node
//	contains the name of the executable and the execution mode.
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
#include "Framework/AppNameNode.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <stddef.h>
#include <string.h>

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppNameNode::AppNameNode( 
    const char* const theName, const int theMode )
    : _mode( theMode )
{
    assert( NULL != theName );

    _name = new char[strlen( theName )+1];
    strcpy( _name, theName );
}

//--------------
// Destructor --
//--------------

AppNameNode::~AppNameNode( )
{
    delete [] _name;
}

//-------------
// Selectors --
//-------------

int
AppNameNode::mode( ) const
{
    return _mode;
}

char*
AppNameNode::name( ) const
{
    return _name;
}

//-------------
// Modifiers --
//-------------

void 
AppNameNode::setMode( int newMode )
{
    _mode = newMode;
}

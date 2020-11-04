//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppFileStream.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppFileStream. The output stream class, implemented as a file.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie        Original Author
//
// Copyright Information:
//  Copyright (C) 1999          Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppFileStream.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>
using std::cout;
using std::endl;
using std::fstream;
using std::ios;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "colias/colias.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppFileStream::AppFileStream( 
    const char* const theName, 
    const char* const theFile,
    const char* const theDescription )
    : AppStream( theName, theFile, theDescription)
{
}

//--------------
// Destructor --
//--------------

AppFileStream::~AppFileStream( )
{
}

//-------------
// Selectors --
//-------------

const char*
AppFileStream::filename( ) const
{
    return destination( );
}

bool
AppFileStream::isOpen( ) const
{
    bool result = false;
    if ( NULL != _destination ) {
      if ( ((fstream*)&_file)->rdbuf()->is_open( ) ) {
	result = true;
      }
    }
    return result;
}

//-------------
// Modifiers --
//-------------

void
AppFileStream::setFilename( const char* const theName )
{
    setDestination( theName );
}

bool
AppFileStream::open( )
{
    bool result = false;
    if ( NULL != _destination) {
        _file.open( _destination, ios::out );   
        if ( isOpen( ) ) {
            result = true;
        } else {
            cout << name() <<  "::open Failed to open Output file" << endl;
        }
    }
    return result;
}

void
AppFileStream::close( )
{
    if ( isOpen( ) ) {
      _file.close( );
    }
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

bool 
AppFileStream::writeToDestination( AbsEvent* anEvent )
{
    bool result = true;    
    return result;
}

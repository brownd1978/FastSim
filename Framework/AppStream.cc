//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppStream.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppStream. The abstract output stream class.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//      Marc Turcotte                   Merged in R.Kapur's TK interface 5/98
//
// Copyright Information:
//      Copyright (C) 1994, 1995        Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppStream.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppPath.hh"
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppStream::AppStream( 
    const char* const theName, 
    const char* const theDestination,
    const char* const theDescription )
    : _paths      ( NULL ),
      _description( NULL ),
      _destination( NULL ),
      _isEnabled  ( true ),
      _events     (    0 )
      
{
    assert( NULL != theName );
    
    _paths = new APPList< AppPath* >;
    _name  = new char[strlen( theName )+1];
    strcpy( _name, theName );
    if ( NULL != theDestination ) {
        _destination = new char[strlen( theDestination )+1];
        strcpy( _destination, theDestination );
    }
    if ( NULL != theDescription ) {
        _description = new char[strlen( theDescription )+1];
        strcpy( _description, theDescription );
    }
}

//--------------
// Destructor --
//--------------

AppStream::~AppStream( )
{
    delete [] _name;
    if ( NULL != _destination ) delete [] _destination;
    if ( NULL != _description ) delete [] _description;
    // wipeout( ); 
    // _paths should just be deleted but not the paths themselves
    delete _paths;

}

//-------------
// Selectors --
//-------------

unsigned int
AppStream::events( ) const
{
    return _events;
}

const char*
AppStream::name( ) const
{
    return _name;
}

const char*
AppStream::description( ) const
{
    return _description;
}

const char*
AppStream::destination( ) const
{
    return _destination;
}

bool
AppStream::isEnabled( ) const
{
    return _isEnabled;
}

APPList< AppPath* >*
AppStream::paths( ) const
{
    return _paths;
}

AppPath*
AppStream::fetch( const char* const theName ) const
{
    AppPath* thePath = NULL;
    
    APPListIterator<AppPath*> theIterator( *_paths );
    AppPath** aPath;
    while ( ( aPath = theIterator( ) ) ) {
        if ( 0 == strcmp( (*aPath)->name( ), theName ) ) {
	    thePath = *aPath;
            break;
        }
    }
    return thePath;
}

bool
AppStream::has( const char* const theName ) const
{
    AppPath** thePath;
    bool result = false;
    
    APPListIterator<AppPath*> theIterator( *_paths );
    while ( ( thePath = theIterator( ) ) ) {
        if ( 0 == strcmp( (*thePath)->name( ), theName ) ) {
            result = true;
            break;
        }
    }
    return result;
}

//-------------
// Modifiers --
//-------------

void
AppStream::setDestination( const char* const theName )
{
    if ( NULL != _destination ) {
        close( );
        delete [] _destination;
        _destination = NULL;
    }
    if ( NULL != theName ) {
        _destination = new char[strlen( theName )+1];
        strcpy( _destination, theName );
    }
}

void
AppStream::setEnabled( bool mode )
{
    _isEnabled = mode;
}

//--------------
// Operations --
//--------------

bool 
AppStream::write( AbsEvent* anEvent )
{
    assert( NULL != anEvent );
    AppPath** thePath;
    bool result = true;
    
    if ( isEnabled( ) && isOpen( ) ) {
        APPListIterator<AppPath*> theIterator( *_paths );
        while ( ( thePath = theIterator( ) ) ) {
            if ( (*thePath)->isEnabled( ) && (*thePath)->passed( ) ) {
                bool status = writeToDestination( anEvent );
                if ( status ) {
                    _events++;
                }
		result = status;
		break;
            }
        }
    }
    // result will be true if there was no error
    return result;
}

void
AppStream::append( const AppPath* const thePath )
{
    AppPath** aPath;

    assert( NULL != thePath );

    APPListIterator<AppPath*> theIterator( *_paths );
    while ( ( aPath = theIterator( ) ) ) {
        if ( thePath == (*aPath) ) {
            break;
        }
    }
    if ( NULL == aPath ) {
        _paths->append( (AppPath*)thePath );
    }
}

void
AppStream::remove( const AppPath* const thePath )
{
    assert( NULL != thePath );
    
    _paths->remove( (AppPath*)thePath );
}

void
AppStream::remove( const char* const theName )
{
    AppPath** thePath;

    assert( NULL != theName );
    
    APPListIterator<AppPath*> theIterator( *_paths );
    while ( ( thePath = theIterator( ) ) ) {
        if ( 0 == strcmp( (*thePath)->name( ), theName ) ) {
            _paths->remove( *thePath );
            break;
        }
    }
}

void
AppStream::wipeout( ) {
    APPListDeleteAll( *_paths );
    // this function destroys the paths in the list
    // but they belong to the Framework 
}


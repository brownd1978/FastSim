//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppStreamsOutputModule.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppStreamsOutputModule. This is the abstract streams output module
//      for the BaBar framework.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//      Bob Jacobsen                    Started AbsEvent dbio mods
//      Marc Turcotte                   Modified for new style begin/end
//      Marc Turcotte                   Modified for new style event.
//      Marc Turcotte                   Merged in R.Kapur's TK interface
//      Marc Turcotte                   Modified for FNAL sigs 6/24/98
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
#include "Framework/AppStreamsOutputModule.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFileStream.hh"
#include "Framework/AppFileOutputCommand.hh"

class AbsEvent;
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

//----------------------------------
// Templated Function realization --
//----------------------------------
//template void F_createTree(DATFileNode*& newNode);

//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//----------------
// Constructors --
//----------------

AppStreamsOutputModule::AppStreamsOutputModule( const char* const theName,
                                                const char* const theDescription )
    : AppOutputModule( theName, theDescription ),
      _theEvent ( NULL ),
      _outputCmd( NULL ),
      _outCmd   ( NULL )
{
    _streams = new APPList< AppStream* >;
}

//--------------
// Destructor --
//--------------

AppStreamsOutputModule::~AppStreamsOutputModule( )
{
    APPListDeleteAll(*_streams);
    delete _streams;
    if ( NULL != _outputCmd ) {
        delete _outputCmd;
    }
    if ( NULL != _outCmd ) {
        delete _outCmd;
    }
}

//--------------
// Operations --
//--------------

AppResult
AppStreamsOutputModule::beginJob( AbsEvent* anEvent ) {
    return AppResult::OK;
}

AppResult
AppStreamsOutputModule::beginRun( AbsEvent* anEvent )
{
    AppStream** theStream;
    bool status;
    
    APPListIterator<AppStream*> theIterator( *streams( ) );
    while ( ( theStream = theIterator( ) ) ) {
        if ( (*theStream)->isEnabled( ) && ! (*theStream)->isOpen( ) ) {
            status = (*theStream)->open( );
        }
    }
    if ( NULL != anEvent ) {
        // Decide what to do when a real new run starts
    }
    
    return AppResult::OK;
    
}

AppResult
AppStreamsOutputModule::outputEvent( AbsEvent* anEvent ) 
{
    AppStream** theStream;
    bool status = true;
    
    APPListIterator<AppStream*> theIterator( *streams( ) );
    while ( ( theStream = theIterator( ) ) ) {
        status = status && (*theStream)->write( anEvent );
    }
    if (status)
      return AppResult::OK;

    return AppResult::ERROR;
}

AppResult
AppStreamsOutputModule::outputFrame( AppFrame* aFrame ) {
    return AppResult::OK;
}


AppResult
AppStreamsOutputModule::endRun( AbsEvent* anEvent ) {
    return AppResult::OK;
}

AppResult
AppStreamsOutputModule::endJob( AbsEvent* anEvent )
{
    AppStream** theStream;
    
    APPListIterator<AppStream*> theIterator( *streams( ) );
    while ( ( theStream = theIterator( ) ) ) {
        (*theStream)->close( );
    }
    return AppResult::OK;
}

AppResult
AppStreamsOutputModule::abortJob( AbsEvent* anEvent)
{
    AppStream** theStream;
    
    APPListIterator<AppStream*> theIterator( *streams( ) );
    while ( ( theStream = theIterator( ) ) ) {
        (*theStream)->close( );
    }
    return AppResult::OK;
}

void
AppStreamsOutputModule::help(int argc, char** argv  )
{
    AppOutputModule::help(argc, argv);
}

AppStream*
AppStreamsOutputModule::fetch( const char* const theName ) const
{
    AppStream** theStream = NULL;
    
    APPListIterator<AppStream*> theIterator( *streams( ) );
    while ( ( theStream = theIterator( ) ) ) {
        if ( 0 == strcmp( (*theStream)->name( ), theName ) ) {
            break;
        }
    }
    return *theStream;
}

bool
AppStreamsOutputModule::has( const char* const theName ) const
{
    AppStream** theStream;
    bool result = false;
    
    APPListIterator<AppStream*> theIterator( *streams( ) );
    while ( ( theStream = theIterator( ) ) ) {
        if ( 0 == strcmp( (*theStream)->name( ), theName ) ) {
            result = true;
            break;
        }
    }
    return result;
}

//-------------
// Selectors --
//-------------

APPList< AppStream* >*
AppStreamsOutputModule::streams( ) const
{
    return _streams;
}

//-------------
// Modifiers --
//-------------

void
AppStreamsOutputModule::append( const AppStream* const theStream )
{
    assert( NULL != theStream );
    
    _streams->append( (AppStream*)theStream );
}

void
AppStreamsOutputModule::remove( const AppStream* const theStream )
{
    assert( NULL != theStream );
    
    _streams->remove( (AppStream*)theStream );
}

void
AppStreamsOutputModule::remove( const char* const theName )
{
    AppStream** theStream;

    assert( NULL != theName );
    
    APPListIterator<AppStream*> theIterator( *streams( ) );
    while ( ( theStream = theIterator( ) ) ) {
        if ( 0 == strcmp( (*theStream)->name( ), theName ) ) {
            _streams->remove( *theStream );
            break;
        }
    }
}

void
AppStreamsOutputModule::wipeout( )
{
    APPListDeleteAll( *streams( ) ); 
}

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParm.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AbsParm
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//
// Copyright Information:
//	Copyright (C) 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AbsParm.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>
#include <iostream>
#include <set>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "FrameUtil/APPListIterator.hh"
#include "FrameUtil/AbsInterp.hh"
using std::endl;
using std::ostream;

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AbsParm::AbsParm( 
    const char* const theCommand, AppModule* theTarget)
   : AppCommand( theCommand, theTarget ), 
     _userDescription()
{
}

AbsParm::AbsParm( const AbsParm& theOther, AppModule* theTarget )
  : AppCommand( theOther.command(), theTarget )
  , _userDescription(theOther._userDescription)  
{
}


//--------------
// Destructor --
//--------------

AbsParm::~AbsParm( )
{
}

//-----------
// Handler --
//-----------

int 
AbsParm::handle( int argc, char* argv[] )
{
    AppModule* theInput = target( );
    int result = AbsInterp::ERROR;
    
    setArgs( argc, argv );
    if ( 2 <= argc ) {
	if        ( 0 == strcmp( argv[1], "set" ) ) {
	    result = setHandler( );
	} else if ( 0 == strcmp( argv[1], "list" ) ) {
	    result = listHandler( );
	} else if ( 0 == strcmp( argv[1], "get" ) ) {
	   result = getHandler();
	} else {
	  std::string txtmsg("invalid command name: ");
          for (int index=0; index < argc; index++ ) {
            txtmsg += " ";
            txtmsg += argv[index];
          }
          theInput->setError( txtmsg.c_str() );
	} 
    } else {
        theInput->setError( "wrong # args" );
    }
    return result;
}
void
AbsParm::show( ) const
{
   listHandler( );
}
bool
AbsParm::isShowable( ) const 
{
   return true; 
}
void
AbsParm::addDescription( const std::string& userString )
{
   _userDescription = userString;   
}

std::string
AbsParm::description() const
{
  return classDescription() + " " + _userDescription;
}

//		-------------------------------------------
// 		-- Friend Function Definitions --
//		-------------------------------------------

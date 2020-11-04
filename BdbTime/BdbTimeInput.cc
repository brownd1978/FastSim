//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BdbTimeInput.cc 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//	BdbTimeInput implementation
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Gregory Dubois-Felsmann  (originator)
//
// Copyright Information:
//	Copyright (C) 2003   California Institute of Technology
//
//------------------------------------------------------------------------

// Get the "non-zeroing" behavior of strptime() on Solaris.
// It's the default on Linux.
// 
// Note that Unix98 says that one should not rely on one or the other
// behavior, but that portable applications should do conversions as
// single unit and make no assumptions.  If this proves to be a problem
// in the future we could rewrite this code in a more complicated way
// to avoid relying on non-zeroing behavior.  In the mean time we do
// provide a regression test that checks this.                        FIXME
#ifdef __SUNPRO_CC
#define _STRPTIME_DONTZERO
#endif

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BdbTime/BdbTimeInput.hh"

//-----------------
// C/C++ Headers --
//-----------------
#include <string.h>
#include <iostream>
using std::cerr;
using std::endl;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

//--------------
// Destructor --
//--------------

//-------------
// Methods   --
//-------------
    
//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

BdbTimeInput::Status
BdbTimeInput::parseTime( const char* string, 
			 struct tm&  result )
{
  return BdbTimeInput::parseString( string, timeFormats_, result, debug_ );
}


BdbTimeInput::Status
BdbTimeInput::parseDate( const char* string, 
			 struct tm&  result )
{
  return BdbTimeInput::parseString( string, dateFormats_, result, debug_ );
}


BdbTimeInput::Status
BdbTimeInput::parseString( const char* string, 
			   const char* const formats[], 
			   struct tm&  result,
			   bool debug )
{
  // Find the end of the input string.  This is needed in order to be
  // sure that strptime has parsed all of it.
  if ( debug ) {
    cerr << "BdbTimeInput::parseString: strlen(\"" << string << "\") is "
	 << strlen(string) << endl;
  }
  const char* const stringEnd = string + strlen(string);

  BdbTimeInput::Status status = BdbTimeInput::NoMatch;
  for ( const char* const* fmt = formats; *fmt != 0; ++fmt ) {
    if ( debug ) {
      cerr << "Trying format \"" << *fmt << "\":";
    }
    const char* last = strptime( string, *fmt, &result );
    if ( last == stringEnd ) {
      status = BdbTimeInput::Success;
      if ( debug ) {
	cerr << " OK!" << endl;
      }
      break;
    }
    else if ( debug ) {
      if ( last == 0 ) {
	cerr << " returned null pointer; failure." << endl;
      }
      else {
	cerr << " stopped at position " << (last-string);
	if ( *last == '\0' ) {
	  cerr << " = '\0'; strange!" << endl;
	}
	else {
	  cerr << " = '" << *last << "'; failure." << endl;
	}
      }
    }
  }
  if ( debug && status != BdbTimeInput::Success ) {
    cerr << "Tried all patterns without success." << endl;
  }

  return status;
}

void
BdbTimeInput::setDebugging( bool debug ) 
{
  debug_ = debug;
}

const char*
BdbTimeInput::defaultTimeFormats_[] = 
  {
    "%T"     // hh:mm:ss
    , "%R"   // hh:mm
    , 0
  };

const char* const *
BdbTimeInput::timeFormats_ = BdbTimeInput::defaultTimeFormats_;

const char*
BdbTimeInput::defaultDateFormats_[] = 
  {
    "%D"     // mm/dd/yy
    , "%d%b%Y" // ddMmmyyyy
    , 0
  };

const char* const *
BdbTimeInput::dateFormats_ = BdbTimeInput::defaultDateFormats_;

bool
BdbTimeInput::debug_ = false;

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------

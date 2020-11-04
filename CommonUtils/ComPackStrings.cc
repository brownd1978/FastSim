//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ComPackStrings.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class ComPackStrings
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Stephen J. Gowdy	Originator
//
// Copyright Information:
//	Copyright (C) 2001	Lawrence Berkeley National Lab
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "CommonUtils/ComPackStrings.hh"

//-------------
// C Headers --
//-------------
extern "C" {
#include <assert.h>
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include <string>
using std::string;
#include <vector>


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
ComPackStrings::ComPackStrings()
  : _stringLengths( new std::vector< char > ),
    _strings( new string )
{
}

//--------------
// Destructor --
//--------------
ComPackStrings::~ComPackStrings()
{
  delete _stringLengths;
  delete _strings;
}

//-------------
// Methods   --
//-------------
    
//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------
bool
ComPackStrings::getString( int id, string& toGet ) const
{
  // Make sure id is valid (must have at least one more entry)
  assert( _stringLengths->size() > id );

  // Get start of string
  int start = 0, i=0;
  for ( i=0; i<id; i++ ) start += (*_stringLengths)[i];

  int strLen = (*_stringLengths)[id+1];
  string tempString( *_strings, start, strLen);

  toGet = tempString;

  return true;
}

int
ComPackStrings::getId( const string& aString ) const
{
  // Determine if the string is already stored
  size_t i=0, length = _stringLengths->size();
  int inSize = aString.length(), start=0;
  assert( inSize < 256 );
  for ( i=0; i<length; i++ )
    {
      int size = (*_stringLengths)[i];
      if ( size == inSize )
	{
	  string toCompare( &((*_strings)[start]), size );
	  if ( toCompare == aString ) return i;
	}
      start += size;
    }

  // String isn't found, return -1
  return -1;
}

//-------------
// Modifiers --
//-------------
int
ComPackStrings::addString( const string& aString )
{
  // Determine if the string is already stored
  int id = getId( aString );

  if ( id == -1 )
    {
      // We've not seen this before, add it
      (*_strings) += aString;
      _stringLengths->push_back( aString.length() );
      // verify added correctly
      id = getId( aString );
      assert( id != -1 );
    }

  return id;
}

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------

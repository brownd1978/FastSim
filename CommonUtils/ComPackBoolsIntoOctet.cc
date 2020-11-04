//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ComPackBoolsIntoOctet.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class ComPackBoolsIntoOctet
//      Do not use this for comPackBoolsIntoOctetd class (foo<T>).  use ComPackBoolsIntoOctetComPackBoolsIntoOctet.hh
//      instead.
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
#include "CommonUtils/ComPackBoolsIntoOctet.hh"

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

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
ComPackBoolsIntoOctet::ComPackBoolsIntoOctet()
  : _octet( 0 ),
    _index( 0 )
{
  reset();
}

//--------------
// Destructor --
//--------------
ComPackBoolsIntoOctet::~ComPackBoolsIntoOctet()
{
}

//-------------
// Methods   --
//-------------

//-------------
// Operators --
//-------------
bool
ComPackBoolsIntoOctet::operator()( unsigned int index ) const
{
  return _bools[ index ];
}

bool
ComPackBoolsIntoOctet::operator[]( unsigned int index ) const
{
  assert( index < 8 );
  return _bools[ index ];
}

//-------------
// Selectors --
//-------------
uint8
ComPackBoolsIntoOctet::pack( bool v1, bool v2, bool v3, bool v4,
			     bool v5, bool v6, bool v7, bool v8 ) const
{
  uint8 packed = 0;

  packed += v1;
  packed += v2 << 1;
  packed += v3 << 2;
  packed += v4 << 3;
  packed += v5 << 4;
  packed += v6 << 5;
  packed += v7 << 6;
  packed += v8 << 7;

  return packed;
}

//-------------
// Modifiers --
//-------------
void
ComPackBoolsIntoOctet::unpack( uint8 valueToUnpack )
{
  for ( unsigned int i=0; i<8; i++ )
    {
      uint8 mask = 1 << i;
      _bools[i] = ( ( valueToUnpack & mask ) == mask ) ? true : false;
    }
}

bool
ComPackBoolsIntoOctet::add( bool toAdd )
{
  assert( _index < 8 );

  _octet += toAdd << _index++;

  // If we've finished adding bools let the user know
  if ( _index == 8 ) return true;
  else return false;
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

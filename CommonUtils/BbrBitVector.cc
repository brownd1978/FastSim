//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BbrBitVector.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class BbrBitVector
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Stephen J. Gowdy	Originator
//
// Copyright Information:
//	Copyright (C) 2001	Stanford Linear Accelerator Center
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------
#include "CommonUtils/BbrBitVector.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
using std::istream;
using std::ostream;

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
bool&
BbrBitVector::operator[] ( unsigned int i )
{
  if ( i >= _size )
    {
      ErrMsg( fatal ) << "Requested bit (" << i
		      << ") is outside valid range (0 to " << (_size-1) << ")."
		      << endmsg;
      ::abort();
    }
  return _bits[i];
}

BbrBitVector&
BbrBitVector::operator= ( const BbrBitVector& v )
{
  if ( this == &v ) return *this;

  delete [] _bits;
  _size = v._size;
  _bits = new bool[ _size ];

  for ( int i=0; i<_size; i++ ) _bits[i] = v._bits[i];

  return *this;
}

//-------------
// Selectors --
//-------------
bool
BbrBitVector::operator[] ( unsigned int i ) const
{
  if ( i >= _size )
    {
      ErrMsg( fatal ) << "Requested bit (" << i
		      << ") is outside valid range (0 to " << (_size-1) << ")."
		      << endmsg;
      ::abort();
    }
  return _bits[i];
}

BbrBitVector
BbrBitVector::operator& ( const BbrBitVector& toCompare ) const
{
  if ( _size != toCompare._size )
    {
      ErrMsg( error ) << "Attempt to compare different sized objects, returning zeros..." << endmsg;
      return BbrBitVector( _size );
    }

  BbrBitVector newVec( _size );
  for ( unsigned int i=0; i<_size; i++ ) newVec[i] = _bits[i] && toCompare[i];

  return newVec;
}
   
bool
BbrBitVector::bitwiseAnd( const BbrBitVector& toCompare ) const
{
  if ( _size != toCompare._size )
    {
      ErrMsg( error ) << "Attempt to compare different sized objects, returning false..." << endmsg;
      return false;
    }

  for ( unsigned int i=0; i<_size; i++ )
    if ( _bits[i] && toCompare[i] ) return true;

  return false;
}

bool
BbrBitVector::operator==( const BbrBitVector& toCompare ) const
{
  if ( _size != toCompare._size ) return false;

  for ( unsigned int i=0; i<_size; i++ )
    if ( _bits[i] != toCompare._bits[i] ) return false;

  return true;
}

//-------------
// Modifiers --
//-------------
void
BbrBitVector::extend( unsigned int newSize )
{
  if ( newSize < _size )
    ErrMsg( fatal ) << "Attempt to shrink with extend method." << endmsg;

  bool* oldbits = _bits;
  bool* newbits = new bool[ newSize ];

  for ( unsigned int i=0; i<_size; i++ ) newbits[i] = oldbits[i];
  for ( unsigned int j=_size; j<newSize; j++ ) newbits[j] = false;

  // Delete old array and replace it with new one
  delete [] oldbits;
  _bits = newbits;
  _size = newSize;
}

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------
istream& operator>>( istream& input, BbrBitVector& toFill )
{
  input >> toFill._size;
  char temp;
  for ( unsigned int i=0; i<toFill._size; i++ )
    {
      input >> temp;
      toFill._bits[i] = ( (temp==1)?true:false );
    }
  return input;
}

ostream& operator<<( ostream& output, const BbrBitVector& toWrite )
{
  output << toWrite._size;
  for ( unsigned int i=0; i<toWrite._size; i++ )
    output << (char)toWrite._bits[i];
  return output;
}

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------

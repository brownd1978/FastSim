//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BbrBitVector.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class BbrBitVector.
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

#ifndef BBRBITVECTOR_HH
#define BBRBITVECTOR_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include <iosfwd>
#include <iosfwd>

//		---------------------
// 		-- Class Interface --
//		---------------------

class BbrBitVector {

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  BbrBitVector( unsigned int size = 64 )
    : _size( size ),
      _bits( new bool[size] )
  {
    for ( unsigned int i=0; i<_size; i++ ) _bits[i] = false;
  }
       // Copy Constructor
  BbrBitVector( const BbrBitVector& toCopy )
    : _size( toCopy._size ),
      _bits( new bool[toCopy._size] )
  {
    for ( unsigned int i=0; i<_size; i++ ) _bits[i] = toCopy._bits[i];
  }

  // Destructor
  virtual ~BbrBitVector()
  {
    delete [] _bits;
  }

  // Operators
  bool& operator() ( unsigned int i ) { return _bits[i]; }
  bool& operator[] ( unsigned int i );
  BbrBitVector& operator= ( const BbrBitVector& );  // Assignment op
    
  // Selectors (const)
  bool operator() ( unsigned int i ) const { return _bits[i]; }
  bool operator[] ( unsigned int i ) const;
  BbrBitVector operator& ( const BbrBitVector& toCompare ) const;
  bool operator==( const BbrBitVector& toCompare ) const;

  // Return true when two same place bits are set
  bool bitwiseAnd( const BbrBitVector& toCompare ) const;

  // Modifiers
  void extend( unsigned int newSize );

protected:

  // Helper functions

private:

  // Friends
  friend std::istream& operator>>( std::istream& input, BbrBitVector& toFill );
  friend std::ostream& operator<<( std::ostream& output, const BbrBitVector& toWrite );

  // Data members
  int _size;
  bool* _bits; // will be an array of bools

//------------------
// Static Members --
//------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members

};

std::istream& operator>>( std::istream& input, BbrBitVector& toFill );
std::ostream& operator<<( std::ostream& output, const BbrBitVector& toWrite );

#endif // BBRBITVECTOR_HH

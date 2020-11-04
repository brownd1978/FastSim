//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ComPackBoolsIntoOctet.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class ComPackBoolsIntoOctet. Used to assign 8 bool values into
//      an d_Octet.
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

#ifndef COMPACKBOOLSINTOOCTET_HH
#define COMPACKBOOLSINTOOCTET_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include "BaBar/BaBarODMGTypes.h"

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class ComPackBoolsIntoOctet
{

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  ComPackBoolsIntoOctet();

  // Destructor
  virtual ~ComPackBoolsIntoOctet();

  // Operators
  bool operator()( unsigned int index ) const;
  bool operator[]( unsigned int index ) const;

  // Selectors (const)
  uint8 pack( bool v1,
		bool v2 = false,
		bool v3 = false,
		bool v4 = false,
		bool v5 = false,
		bool v6 = false,
		bool v7 = false,
		bool v8 = false ) const;
  uint8 getPackedValue() const { return _octet; }

  // Modifiers
  void unpack( uint8 valueToUnpack );
  bool add( bool );
  void reset()
  {
    for ( unsigned int i=0; i<8; i++ ) _bools[i] = false;
    _octet = _index = 0;
  }

protected:

  // Helper functions

private:

  // Friends

  // Data members
  bool _bools[8]; // Used to unpack from Octet
  uint8 _octet;
  uint8 _index;

  //Note: if your class needs a copy constructor or an assignment operator, 
  //  make one of the following public and implement it.
  ComPackBoolsIntoOctet( const ComPackBoolsIntoOctet& );       // Copy Constructor
  ComPackBoolsIntoOctet& operator= ( const ComPackBoolsIntoOctet& );  // Assignment op

//------------------
// Static Members --
//------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members

};

#endif // COMPACKBOOLSINTOOCTET_HH

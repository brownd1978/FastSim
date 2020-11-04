//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ComPackStrings.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class ComPackStrings. Aggregates short (<256) strings into one string.
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
#ifndef COMPACKSTRINGS_HH
#define COMPACKSTRINGS_HH

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
#include <string>
#include <vector>

//		---------------------
// 		-- Class Interface --
//		---------------------

class ComPackStrings {

//--------------------
// Declarations     --
//--------------------

  // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  ComPackStrings();

  // Copy Constructor

  // Destructor
  virtual ~ComPackStrings();

  // Operators

  // Selectors (const)
  bool getString( int id, std::string& returnedString ) const;
  const std::string* getRawStrings() const { return _strings; }
  const std::vector< char >* getLengths() const
                                         { return _stringLengths; }
  int getId( const std::string& toFind ) const;

  // Modifiers
  int addString( const std::string& aString );

protected:

  // Helper functions

private:

  // Friends

  // Data members
  std::vector< char >* _stringLengths;
  std::string* _strings;

  // Forbidden
  ComPackStrings( const ComPackStrings& );

  //------------------
  // Static Members --
  //------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members

};

#endif // COMPACKSTRINGS_HH

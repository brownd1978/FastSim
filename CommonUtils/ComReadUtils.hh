//==========================================================================
// File and Version Information:
// 	$Id: ComReadUtils.hh 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	Class ComReadUtils is a place holder for utilities which perform 
// stream input tasks that could be useful for several users. Initially it
// is very lean, but presumably people will add useful things to it as they
// go along
//
//--------------------------------------------------------------------------
// Collaborating classes:
//
//--------------------------------------------------------------------------
// Sample User Code:
//
//--------------------------------------------------------------------------
// Compiler Default Member Functions:
//
//--------------------------------------------------------------------------
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//--------------------------------------------------------------------------
// Author List:
//	Abi Soffer              (Original author)
//
//--------------------------------------------------------------------------
// Copyright Information:
//	Copyright (C) 1998	Colorado State University
//
//==========================================================================

#ifndef ComReadUtils_HH
#define ComReadUtils_HH

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

//--------------------------------------------
// Collaborating Class Forward Declarations --
//--------------------------------------------
#include <iosfwd>

//		---------------------
// 		-- Class Interface --
//		---------------------

class ComReadUtils {

//------------------
// Static Members --
//------------------

public:
  // Static accessors:

  // Read a word from the stream, terminated by a space, and make sure
  // it is identical to match. Otherwise, complain in the name of caller
  // and assert(0):
  static void readWord(std::istream & stream, 
		       const char * match, 
		       const char * caller);
  
  
  // Static modifiers
  
private:
  
  // Static data members
  

//--------------------
// Instance Members --
//--------------------

public:
  // Constructors

  // Copy Constructor

  // Destructor

  // Assignment operator:

  // Accessors (const)
  
  // Modifiers
  
  // Globals:

protected:

  // Helper functions

private:

  // Friends
  
  // Data members

};

#endif

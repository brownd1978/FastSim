#ifndef BDBTIMEINPUT_HH
#define BDBTIMEINPUT_HH

//--------------------------------------------------------------------------
//
// $Id: BdbTimeInput.hh 496 2010-01-13 17:10:44Z stroili $
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 2003   California Institute of Technology
//
//------------------------------------------------------------------------

//-----------------
// C/C++ Headers --
//-----------------
#include <time.h>

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


/**
 *  This is a helper class used in the implementation of BdbTime.
 *  It is not intended to be supported as a public interface.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2003   California Institute of Technology
 *
 *  @version $Id: BdbTimeInput.hh 496 2010-01-13 17:10:44Z stroili $
 *
 *  @author Gregory Dubois-Felsmann  (originator)
 */

class BdbTimeInput {

//--------------------
// Declarations     --
//--------------------

    // Typedefs, consts, and enums

public:

  // Error codes match those of Unix98's getdate().
  enum Status { Success = 0, NoMatch = 7, BadValue = 8 };

//--------------------
// Instance Members --
//--------------------

public:

    // Constructors

    // Destructor

    // Operators
    
    // Selectors (const)

    // Modifiers

protected:

    // Helper functions

private:

    // Friends

    // Data members

    // Disable copy construction and assignment.
    BdbTimeInput( const BdbTimeInput& );
    BdbTimeInput& operator=( const BdbTimeInput& );

//------------------
// Static Members --
//------------------

public:

  static BdbTimeInput::Status parseTime( const char* string,
					 struct tm&  result );

  static BdbTimeInput::Status parseDate( const char* string,
					 struct tm&  result );

  static BdbTimeInput::Status parseString( const char* string,
					   const char* const formats[],
					   struct tm&  result,
					   bool debug );

  static void setDebugging( bool debug );

private:

    // Data members

  // Default table of time-only formats
  static const char* defaultTimeFormats_[];

  // Active table of time-only formats
  static const char* const * timeFormats_;

  // Default table of date-only formats
  static const char* defaultDateFormats_[];

  // Active table of date-only formats
  static const char* const * dateFormats_;

  // Debugging flag
  static bool debug_;
};

#endif // BDBTIMEINPUT_HH

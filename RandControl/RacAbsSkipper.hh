//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacAbsSkipper.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacAbsSkipper.
//	  Abstract interface to the "skipper" classes. "skipper" is an object
//	which knows somehow the state of the generator (seeds) and also can 
//	change quickly this state to the state which is identical to that 
//	after generation of some number of randoms.
//	  There are basically two types of the generators (or more). The first 
//	type allows skipping of the arbitrary number of randoms, one example
//	is l'Ecuyer's ranecu engine. The second type does not allow this but 
//	instead can have a mechanism for setting the seeds where each seed
//	value produces a separate random sequence of the big length. The example 
//	of this type is a Marsaglia-type generator which allows more than 
//	900.000.000 different seeds producing sequences of near 10^30 randoms each.
//	  I tried to make this interface usable for both types of the "skippable"
//	generators. The methods randomsPerSkip() should distinguish between them,
//	for the first type it will return 1, and for the second type - some big 
//	number (it's return type is size_t so it cannot return 10^30) which is 
//	not longer that the shortest sequence. The method skip(...) will do just
//	skipping, for the first type of generators it will skip exact number of 
//	randoms, and for the second type - number of seeds, or sequences, so that
//	the total distance skipped will be not smaller than 'number of skipped 
//	seeds multiplied by the randomsPerSkip() value'.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A.Salnikov
//
// Copyright Information:
//	Copyright (C) 1998	Saclay
//
//------------------------------------------------------------------------

#ifndef RACABSSKIPPER_HH
#define RACABSSKIPPER_HH

//-------------
// C Headers --
//-------------
extern "C" {
#include <stddef.h>    // for the size_t
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

//		---------------------
// 		-- Class Interface --
//		---------------------

class RacAbsSkipper {

public:

  // Destructor
  virtual ~RacAbsSkipper( ) {}

  // Operators
    
  // get the numbers of randoms to be skipped when skip(1) is called
  // The values allowed here 1-10^9
  virtual size_t randomsPerSkip() const = 0 ;

  // do (n1*n2*n3) skips. The total number of randoms skipped will be
  // (n1*n2*n3)*randomsPerSkip()
  // better way will be to make it with one parameter only but then it
  // must be at least 64-bits number, i.e. non-portable. 
  virtual void skip ( size_t n1, size_t n2=1, size_t n3=1 ) = 0 ;

protected:

  // Helper functions

private:

  // Friends

  // Data members

};

#endif // RACABSSKIPPER_HH

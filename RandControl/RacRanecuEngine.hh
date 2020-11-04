//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRanecuEngine.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRanecuEngine.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A.Salnikov
//
// Copyright Information:
//	Copyright (C) 1997	<Institution>
//
//------------------------------------------------------------------------

#ifndef RACRANECUENGINE_HH
#define RACRANECUENGINE_HH

//-------------
// C Headers --
//-------------
extern "C" {
#include <stddef.h>
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/config/CLHEP.h"
#include "RandControl/RacAbsSkipper.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class RacRanecuEngine : public HepRandomEngine, public RacAbsSkipper {

public:

  // Constructors
  RacRanecuEngine( int index=0, bool doCount=false ) ;
  RacRanecuEngine( const RacRanecuEngine& ) ;

  // Destructor
  virtual ~RacRanecuEngine( );

  // Operators
    
  //    virtual int operator==( const RacRanecuEngine& ) const;
  //            int operator!=( const RacRanecuEngine& ) const;

  // Selectors (const)
  size_t getCounter() const { return _counter; }

  // ======= HepRandomEngine interface ======
  virtual HepDouble flat() ;
  // Should return a pseudo random number between 0 and 1 
  // (excluding the end points)

  virtual void flatArray(const HepInt size, HepDouble* vect) ;
  // Fills an array "vect" of specified size with flat random values.

  virtual void setSeed(long seed, HepInt=0) ;
  // Should initialise the status of the algorithm according to seed.

  virtual void setSeeds(const long * seeds, HepInt=-1) ;
  // Should initialise the status of the algorithm according to the zero terminated
  // array of seeds. It is allowed to ignore one or many seeds in this array.

  virtual void saveStatus(const char*) const ;
  // Should save on a file specific to the instantiated engine in use
  // the current status.

  virtual void restoreStatus(const char*) ;
  // Should read from a file (specific to the instantiated engine in use)
  // and restore the last saved engine configuration.

  virtual void showStatus() const ;
  // Should dump the current engine status on the screen.

  // get the numbers of randoms to be skipped when skip(1) is called
  // The values allowed here 1-10^9
  virtual size_t randomsPerSkip() const ;

  // ======= RacAbsSkipper interface ======
  // do (n1*n2*n3) skips. The total number of randoms skipped will be
  // (n1*n2*n3)*randomsPerSkip()
  // better way will be to make it with one parameter only but then it
  // must be at least 64-bits number, i.e. non-portable. 
  virtual void skip ( size_t n1, size_t n2=1, size_t n3=1 ) ;

  // Modifiers
  void resetCounter() { _counter = 0 ; }

  virtual std::string name() const { return "RacRanecuEngine"; }
  // Engine name. This method is declared in newer versions of CERN CLHEP.
  // So we have to have it here. And its presence won't hurth anything for
  // older versions.

protected:

  // Helper functions

  // find new seed for one MLCG after skipping n=n1*n2 randoms. It does the 
  // following calculation
  //    seed' = ( a^n * seed ) mod m
  // this will be equal of course to
  //    ( a^n mod m * seed ) mod m
  // so that we need first calculate  a^n mod m
  static long skipN ( long seed, long a, long m, size_t n1, size_t n2, size_t n3 ) ;

  // returns ( a^(2^n) mod m )
  static long skipTwoToN ( long a, long m, size_t n ) ;

  // just the formula for the MLCG: (a*s) mod m
  static long mlcg ( long a, long s, long m ) ;


private:

  // Friends

  // Data members
  long _seeds[2] ;
  bool _doCount ;
  size_t _counter ;

  //Note: if your class needs a copy constructor or an assignment operator, 
  //  make one of the following public and implement it.
  RacRanecuEngine&       operator= ( const RacRanecuEngine& );  // Assignment op

//		------------------------------------------------
// 		-- Static Data & Function Member Declarations --
//		------------------------------------------------
private:
  static const HepInt a1, m1, q1, r1 ;
  static const HepInt a2, m2, q2, r2 ;
  static const HepDouble scale ;
};

#endif // RACRANECUENGINE_HH

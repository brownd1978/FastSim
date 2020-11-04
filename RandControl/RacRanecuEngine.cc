//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRanecuEngine.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRanecuEngine
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
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "RandControl/RacRanecuEngine.hh"

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
#include "CLHEP/Random/Random.h"
#include "ErrLogger/ErrLog.hh"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

const HepInt RacRanecuEngine::a1 = 40014 ;        // multiplier
const HepInt RacRanecuEngine::m1 = 2147483563 ;   // modulo
const HepInt RacRanecuEngine::q1 = 53668 ;        // [modulo/multiplier]
const HepInt RacRanecuEngine::r1 = 12211 ;        // modulo mod multiplier

const HepInt RacRanecuEngine::a2 = 40692 ;        // multiplier
const HepInt RacRanecuEngine::m2 = 2147483399 ;   // modulo
const HepInt RacRanecuEngine::q2 = 52774 ;        // [modulo/multiplier]
const HepInt RacRanecuEngine::r2 = 3791 ;         // modulo mod multiplier

const HepDouble RacRanecuEngine::scale = 4.656613057391769e-10 ;  // 1/m1

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
RacRanecuEngine::RacRanecuEngine( int index, bool doCount ) :
  _doCount(doCount),
  _counter(0)
{
  // copy the seeds from the table 
  HepRandom::getTheTableSeeds ( _seeds, index ) ;

  theSeed = index ;
  theSeeds = _seeds ;   // this will be left intact
}

RacRanecuEngine::RacRanecuEngine( const RacRanecuEngine& o) :
  _doCount(o._doCount),
  _counter(0)
{
  _seeds[0] = o._seeds[0] ;
  _seeds[1] = o._seeds[1] ;
  
  theSeed = o.theSeed ;
  theSeeds = _seeds ;
}

//--------------
// Destructor --
//--------------
RacRanecuEngine::~RacRanecuEngine()
{
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
HepDouble 
RacRanecuEngine::flat() 
{
  if ( _doCount ) _counter ++ ;

  register long seed1(_seeds[0]) ;
  register long seed2(_seeds[1]) ;

  register HepInt k1(seed1/q1);
  register HepInt k2(seed2/q2);
  
  seed1 = a1*(seed1-k1*q1)-k1*r1 ;
  if (seed1 < 0) seed1 += m1 ;
  seed2 = a2*(seed2-k2*q2)-k2*r2 ;
  if (seed2 < 0) seed2 += m2 ;
  
  _seeds[0] = seed1;
  _seeds[1] = seed2;
  
  long diff = seed1-seed2;
  
  if (diff <= 0) diff += (m1-1);
  return (HepDouble)(diff*scale);
}

void 
RacRanecuEngine::flatArray ( const HepInt size, HepDouble* vect )
{
  if ( _doCount ) _counter += size ;

  register long seed1(_seeds[0]) ;
  register long seed2(_seeds[1]) ;
  
  for ( register HepInt i=0; i<size; ++i ) {

    register HepInt k1(seed1/q1);
    register HepInt k2(seed2/q2);
    
    seed1 = a1*(seed1-k1*q1)-k1*r1 ;
    if (seed1 < 0) seed1 += m1 ;
    seed2 = a2*(seed2-k2*q2)-k2*r2 ;
    if (seed2 < 0) seed2 += m2 ;
    
    long diff = seed1-seed2;
    
    if (diff <= 0) diff += (m1-1);
    vect[i] = (HepDouble)(diff*scale);

  }

  _seeds[0] = seed1;
  _seeds[1] = seed2;
  
}
    
//-------------
// Modifiers --
//-------------
void 
RacRanecuEngine::setSeed(long index, HepInt)
{

  HepRandom::getTheTableSeeds ( _seeds, index ) ;
  theSeed = index ;
}

void 
RacRanecuEngine::setSeeds(const long* seeds, HepInt pos)
{
  if (pos != -1) {
    theSeed = pos ;
  }
  _seeds[0] = seeds[0] ;
  _seeds[1] = seeds[1] ;
}

void 
RacRanecuEngine::saveStatus(const char*) const
{
  ErrMsg(error) << "RacRanecuEngine::saveStatus() must not be used!" << endmsg ;
}

void 
RacRanecuEngine::restoreStatus(const char*)
{
  ErrMsg(error) << "RacRanecuEngine::restoreStatus() must not be used!" << endmsg ;
}

void 
RacRanecuEngine::showStatus() const
{
   cout << endl;
   cout << "--------- Ranecu engine status ---------" << endl;
   cout << " Initial seed (index) = " << theSeed << endl;
   cout << " Current couple of seeds = " << _seeds[0]
                                 << ", " << _seeds[1] << endl;
   cout << "----------------------------------------" << endl;
}

// ===============================================================
//	RacAbsSkipper interface implementation
// ===============================================================
size_t
RacRanecuEngine::randomsPerSkip() const
{
  return 1 ;
}
    
void
RacRanecuEngine::skip ( size_t n1, size_t n2, size_t n3 )
{
  // do it for each of the two generators
  _seeds[0] = skipN ( _seeds[0], a1, m1, n1, n2, n3 ) ;
  _seeds[1] = skipN ( _seeds[1], a2, m2, n1, n2, n3 ) ;
}

long
RacRanecuEngine::skipN ( long seed, long a, long m, size_t n1, size_t n2, size_t n3 ) 
{
  uint64_t n = (uint64_t)n1*(uint64_t)n2*(uint64_t)n3 ; 

  // we need to compute the ned seed as:
  //      seed' = ( a^n * seed ) mod m = ( a^n mod m * seed ) mod m
  // first we calculate a^n expressing n as a series of powers of 2:
  //      n = 2^n1 + 2^n2 + 2^n3 ...
  // Then a^n mod m will become
  //      ( a^(2^n1) mod m * a^(2^n2) mod m * a^(2^n3) mod m * ... ) mod m
  long a_new = 1 ;
  size_t degree = 0 ;
  while ( n != 0 ) {
    if ( n & 1 ) {
      // a' = a^(2^n) mod m
      long a_prime = skipTwoToN ( a, m, degree ) ;
      // a_new = ( a' * a_new ) mod m      
      a_new = mlcg ( a_prime, a_new, m ) ;
    }
    n >>= 1 ;
    degree ++ ;
  }

  // now we have a^n and can semply calculate ( a^n * s ) mod m
  return mlcg ( a_new, seed, m ) ;
}

long 
RacRanecuEngine::skipTwoToN ( long a, long m, size_t n )
{
  // calculate ( a^(2^n) mod m ) - simply taking a and squaring it n times
  // (and taking reminder each time)
  int64_t aa ( a ) ;
  int64_t mm ( m ) ;
  for ( size_t i = 0 ; i < n ; i ++ ) {
    aa = ( aa * aa ) % mm ;
  }
  return long(aa) ;
}

long 
RacRanecuEngine::mlcg ( long a, long s, long m )
{
  // here we need 64 bit calculations
  int64_t aa ( a ) ;
  int64_t ss ( s ) ;
  int64_t mm ( m ) ;
  return long( (aa*ss) % mm ) ;
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

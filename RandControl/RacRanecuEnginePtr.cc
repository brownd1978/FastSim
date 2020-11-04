//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRanecuEnginePtr.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRanecuEnginePtr
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
#include "RandControl/RacRanecuEnginePtr.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iomanip>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Random/RandomEngine.h"
#include "RandControl/RacRanecuEngine.hh"
using std::dec;
using std::endl;
using std::hex;
using std::ostream;
using std::setfill;
using std::setw;

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
RacRanecuEnginePtr::~RacRanecuEnginePtr()
{
  if ( _owning ) delete _engine ;
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

const std::string& 
RacRanecuEnginePtr::getName() const 
{
  return _name ;
}

HepRandomEngine* 
RacRanecuEnginePtr::getEngine() const 
{
  return _engine ;
}

RacAbsEnginePtr::EngineType 
RacRanecuEnginePtr::getType() const 
{
  return RacAbsEnginePtr::ranecu ;
}

void 
RacRanecuEnginePtr::print(ostream& o) const
{
  o << '[' << getName() << ":Ranecu:"
    << setfill('0') << hex
    << setw(8) << getEngine()
    << setfill(' ') << dec
    << ']' ;
}

void 
RacRanecuEnginePtr::printAll(ostream& o) const
{
  o << "RacRanecuEnginePtr -- name: "
    << getName()
    << " ptr: "
    << setfill('0') << hex
    << setw(8) << getEngine()
    << setfill(' ') << dec
    << endl ;
}
    
//-------------
// Modifiers --
//-------------

RacAbsEnginePtr*
RacRanecuEnginePtr::clone() const
{
  // make the _exact_ copy of random engine first
  RacRanecuEngine* engine = new RacRanecuEngine(*_engine) ;

  bool owning = true ;
  return new RacRanecuEnginePtr( getName(), engine, owning ) ;
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

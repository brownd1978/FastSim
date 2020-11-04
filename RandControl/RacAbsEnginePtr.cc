//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacAbsEnginePtr.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacAbsEnginePtr
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
#include "RandControl/RacAbsEnginePtr.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iomanip>
using std::dec;
using std::endl;
using std::hex;
using std::ostream;
using std::setfill;
using std::setw;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

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
RacAbsEnginePtr::~RacAbsEnginePtr()
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
void 
RacAbsEnginePtr::print(ostream& o) const
{
  o << '[' << getName() << ':'
    << setfill('0') << hex
    << setw(8) << getEngine()
    << setfill(' ') << dec
    << ']' ;
}

void 
RacAbsEnginePtr::printAll(ostream& o) const
{
  o << "RacAbsEnginePtr -- name: "
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

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsDetStructureIterator.cc 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsDetStructureIterator
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stephen J. Gowdy                University Of Edinburgh
//      Phil Strother                   Imperial College
//
// Copyright Information:
//	Copyright (C) 1997	University of Edinburgh
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsEnv/AbsDetStructureIterator.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsDetIndex.hh"
#include "AbsEnv/AbsDetStructure.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
AbsDetStructureIterator::AbsDetStructureIterator( const AbsDetStructure& 
						  theStructure)
{
  _theIterator = new AbsIndexStore::iterator;
  (*_theIterator) = (theStructure.itsChannels)->begin();
  _theStructure = theStructure.itsChannels;
}

//--------------
// Destructor --
//--------------
AbsDetStructureIterator::~AbsDetStructureIterator()
{
  delete _theIterator;
}

//-------------
// Methods   --
//-------------
    
//-------------
// Operators --
//-------------
const AbsDetIndex*
AbsDetStructureIterator::operator()()
{
  if ((*_theIterator) < (*_theStructure).end()) {
    AbsDetIndex *element = (*(*_theIterator));
    ++(*_theIterator);
    return element;
  }
  else
    return NULL;
}
    
//-------------
// Selectors --
//-------------
const AbsDetIndex*
AbsDetStructureIterator::current() const
{
  if ((*_theIterator) < (*_theStructure).end()) 
    return (*(*_theIterator));
  else
    return NULL;
}  
    
//-------------
// Modifiers --
//-------------
/* 
void
AbsDetStructureIterator::reset()
{
  _theIterator->reset();
}
*/
//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------
AbsIndexStore::iterator*
AbsDetStructureIterator::theIterator() const
{
  return _theIterator;
}

AbsIndexStore*
AbsDetStructureIterator::theStructure() const
{
  return _theStructure;
}

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------

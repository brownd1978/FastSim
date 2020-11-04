//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsDetStructureIterator.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsDetStructureIterator
//        Almost a carbon copy of EmcDetStructureIterator.
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

#ifndef ABSDETSTRUCTUREITERATOR_HH
#define ABSDETSTRUCTUREITERATOR_HH

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

#include "AbsEnv/AbsDetStructure.hh"
#include "AbsEnv/AbsEnvTypes.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsDetIndex;
class AbsDetStructure;

//		---------------------
// 		-- Class Interface --
//		---------------------

class AbsDetStructureIterator
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
  AbsDetStructureIterator(const AbsDetStructure &);

  // Destructor
  virtual ~AbsDetStructureIterator();

  // Operators
  const AbsDetIndex* operator()();

  // Selectors (const)
  const AbsDetIndex* current() const;

  // Modifiers
  /*
  virtual void reset(); // Resets to start
  */

protected:

  // Helper functions
  AbsIndexStore::iterator* theIterator() const;
  AbsIndexStore*           theStructure() const;

private:
  // Things which shouldn't be done
  AbsDetStructureIterator( const AbsDetStructureIterator& );
  AbsDetStructureIterator();
  AbsDetStructureIterator& operator=( const AbsDetStructureIterator& );

  // Friends

  // Data members
  AbsIndexStore::iterator* _theIterator;
  AbsIndexStore* _theStructure;

//------------------
// Static Members --
//------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members

};

// Inline implementations
// Note that use of .icc files is NOT recommended, and use of
// inline functions should be limited
//#include "AbsEnv/AbsDetStructureIterator.icc"

#endif // ABSDETSTRUCTUREITERATOR_HH

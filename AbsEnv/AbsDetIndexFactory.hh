//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsDetIndexFactory.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsDetIndexFactory
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	<Author1>		<originator/contributor etc.>
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1994	<Institution>
//
//------------------------------------------------------------------------

#ifndef ABSDETINDEXFACTORY_HH
#define ABSDETINDEXFACTORY_HH

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

#ifndef BABAR_HH
#include "BaBar/BaBar.hh"
#endif

#include "AbsEnv/AbsEnvTypes.hh"
#include "AbsEnv/AbsDetIndex.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class AbsDetIndex;

//		---------------------
// 		-- Class Interface --
//		---------------------

class AbsDetIndexFactory {

//--------------------
// Declarations     --
//--------------------

    // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AbsDetIndexFactory(){}

  // Copy Constructor

  AbsDetIndexFactory( const AbsDetIndexFactory& ){}
  
  // Destructor
  virtual ~AbsDetIndexFactory();

  // Operators
  
  //    AbsDetIndexFactory&       operator= ( const AbsDetIndexFactory& );
  //    virtual Boolean operator==( const AbsDetIndexFactory& ) const;
  //            Boolean operator!=( const AbsDetIndexFactory& ) const;

  // Selectors (const)
  
  virtual AbsDetIndex * generateNewIndex(long, AbsDetCoord *)const=0;
  
  // Modifiers

protected:

    // Helper functions

private:

    // Friends

    // Data members

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
//#include "AbsDetIndexFactory.icc"

#endif // ABSDETINDEXFACTORY_HH

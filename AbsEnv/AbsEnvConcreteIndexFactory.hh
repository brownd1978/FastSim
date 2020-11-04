//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsEnvConcreteIndexFactory.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsEnvConcreteIndexFactory.  Index factory for those 
//      types of indices found in AbsEnv.  This will be duplicated 
//      where (e.g. EmcEnv) for those types of indices found there.
//      This is a pain, but you want to reduce cross package dependency.
//      Alternatively somebody who knows how to program could come
//      up with a solution.
//
// NB!  This class has a non-virtual destructor. Why? The templating (or rather,
//      gcc 2.7.2's interpretation of it) requires that every function be declared 
//      inline.  DO NOT INHERIT FROM THIS CLASS. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Phil Strother                         Original Author
//
// Copyright Information:
//	Copyright (C) 1996	              Imperial College
//
//------------------------------------------------------------------------

#ifndef ABSENVCONCRETEINDEXFACTORY_HH
#define ABSENVCONCRETEINDEXFACTORY_HH

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

#include "AbsEnv/AbsDetIndexFactory.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "BaBar/BaBar.hh"
#include "AbsEnv/AbsEnvTypes.hh"
#include "AbsEnv/TwoCoordIndex.hh"
#include "AbsEnv/ThreeCoordIndex.hh"

// Have to include the header for every type you want.  YUK!

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

template <class T>
class AbsEnvConcreteIndexFactory:public AbsDetIndexFactory {

  //--------------------
  // Declarations     --
  //--------------------
  
  // Typedefs, consts, and enums
  

  //--------------------
  // Instance Members --
  //--------------------

public:

  // Constructors
  AbsEnvConcreteIndexFactory(){}

  // Copy Constructor

  AbsEnvConcreteIndexFactory( const AbsEnvConcreteIndexFactory< T >& ){}
  
  // Destructor
  ~AbsEnvConcreteIndexFactory(){};

  // Operators
  

  // Selectors (const)
  
  inline AbsDetIndex* generateNewIndex(long, AbsDetCoord *) const;
  
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
//#include "AbsEnvConcreteIndexFactory.icc"

template<class T>
inline AbsDetIndex*
AbsEnvConcreteIndexFactory<T>::generateNewIndex(long index , 
				     AbsDetCoord *initialValues) const
{
  T* theIndex = new T(index,initialValues);  
  return theIndex;
}

#endif // ABSENVCONCRETEINDEXFACTORY_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacAbsEnginePtr.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacAbsEnginePtr.
//      Pointer to the HepRandomEngine which can be saved in the AbsEvent.
//      This is the user's responsability to tell this pointer who owns the engine.
//	This is the abstrack class, the subclasses need to implement clone()
//	method. Also the enum must be extended each time a new engine type
//	is added.
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

#ifndef RACABSENGINEPTR_HH
#define RACABSENGINEPTR_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <string>

//----------------------
// Base Class Headers --
//----------------------
#include "AbsEvent/AbsEvtObj.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "CLHEP/Random/RandomEngine.h"

//		---------------------
// 		-- Class Interface --
//		---------------------

class RacAbsEnginePtr : public AbsEvtObj {

public:
  enum EngineType { ranecu, 
		    unknown=-1 } ;

public:

  // Constructors
  RacAbsEnginePtr() {}

  // Destructor
  virtual ~RacAbsEnginePtr( );

  // Operators
  //    virtual int operator==( const RacAbsEnginePtr& ) const;
  //            int operator!=( const RacAbsEnginePtr& ) const;

  // Selectors (const)
  virtual const std::string& getName() const = 0 ;
  virtual HepRandomEngine* getEngine() const = 0 ;

  // kind of RTTI interface. Some code, for example DB facilites,
  // still will need to know the exact type of the engine pointed.
  virtual EngineType getType() const = 0 ;

  // this method will produce a new RacAbsEnginePtr object pointing to
  // the exact _copy_ of the random engine. 
  virtual RacAbsEnginePtr* clone() const = 0 ;

  // printing methods from AbsEvtObj interface
  virtual void print(std::ostream& o) const;
  virtual void printAll(std::ostream& o) const;
  
  // Modifiers

protected:

  // Helper functions

  // Data members

private:

  // Friends

  // Data members

  // to make copy constructor and assignment operator work we need to
  // know a way how to clone engines in the case we own them.
  RacAbsEnginePtr( const RacAbsEnginePtr& o) ;
  RacAbsEnginePtr& operator= ( const RacAbsEnginePtr& o) ;
};

#endif // RACABSENGINEPTR_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRanecuEnginePtr.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRanecuEnginePtr.
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

#ifndef RACRANECUENGINEPTR_HH
#define RACRANECUENGINEPTR_HH

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
#include "RandControl/RacAbsEnginePtr.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "CLHEP/Random/RandomEngine.h"
class RacRanecuEngine ;

//		---------------------
// 		-- Class Interface --
//		---------------------

class RacRanecuEnginePtr : public RacAbsEnginePtr {

public:

  // Constructors
  RacRanecuEnginePtr( const std::string& name, 
		      RacRanecuEngine* engine, 
		      bool owning=false ) : 
    RacAbsEnginePtr(),
    _name(name),
    _engine(engine),
    _owning(owning) {}

  // Destructor
  virtual ~RacRanecuEnginePtr( );

  // Operators
  //    virtual int operator==( const RacRanecuEnginePtr& ) const;
  //            int operator!=( const RacRanecuEnginePtr& ) const;

  // Selectors (const)
  virtual const std::string& getName() const ;
  virtual HepRandomEngine* getEngine() const ;

  // kind of RTTI interface. Some code, for example DB facilites,
  // still will need to know the exact type of the engine pointed.
  virtual RacAbsEnginePtr::EngineType getType() const ;

  // this method will produce a new RacRanecuEnginePtr object pointing to
  // the exact _copy_ of the random engine. Note that this will be
  // "owning" pointer. The implementation is left to subclasses.
  virtual RacAbsEnginePtr* clone() const ;

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
  const std::string _name ;
  RacRanecuEngine* _engine ;
  bool _owning ;

  // to make copy constructor and assignment operator work we need to
  // know a way how to clone engines in the case we own them.
  RacRanecuEnginePtr( const RacRanecuEnginePtr& o) ;
  RacRanecuEnginePtr& operator= ( const RacRanecuEnginePtr& o) ;
};

#endif // RACRANECUENGINEPTR_HH

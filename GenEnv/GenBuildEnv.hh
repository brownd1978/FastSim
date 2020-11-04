//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: GenBuildEnv.hh 510 2010-01-14 15:28:09Z stroili $
//
// Description:
//	Class GenBuildEnv. 
//      This is the class which builds the Gen environment GenEnv.  It has 
//      memory managment repsonsiblities for GenEnv and ntupleManager.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//       Stephen J. Gowdy               University of Edinburgh
//
// Copyright Information:
//	Copyright (C) 1994		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef GENBUILDENV_HH
#define GENBUILDENV_HH

//-------------
// C Headers --
//-------------

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
struct GenBuildEnvData;

//		---------------------
// 		-- Class Interface --
//		---------------------

class GenBuildEnv : public AppModule
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
  GenBuildEnv( const char* const theName, 
	       const char* const theDescription );
  
  // Copy Constructor

  // Destructor
  virtual ~GenBuildEnv( );
  
  // Methods
  virtual AppResult           beginJob( AbsEvent* anEvent );
  virtual AppResult           event( AbsEvent* anEvent );
  virtual AppResult           other( AbsEvent* anOther );
  virtual AppResult           endJob  ( AbsEvent* anEvent );
  virtual AppResult           abortJob( AbsEvent* anEvent );
  virtual void           help( int argc, char** argv );
  virtual void           help( );
  
  // Operators

  // Selectors (const)

  // Modifiers

protected:

  // Helper functions

private:

  // Helper functions
  
  // Friends

  // Data members
  GenBuildEnvData* _privateData;

//------------------
// Static Members --
//------------------

public:

  // Selectors (const)

  // Modifiers

};

#endif // GENBUILDENV_HH


//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: PdtTestModule.hh 445 2010-01-15 08:53:50Z stroili $
//
// Description:
//	Class PdtTestModule
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David Lange                   Original Author
//
// Copyright Information:
//	Copyright (C) 2002	
//
//------------------------------------------------------------------------

#ifndef PDTTESTMODULE_HH
#define PDTTESTMODULE_HH


//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


//		---------------------
// 		-- Class Interface --
//		---------------------
 
class PdtTestModule : public AppModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  PdtTestModule( const char* const theName, const char* const theDescription );

  // Destructor
  virtual ~PdtTestModule( );

  // Operations

  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );
    
protected:
  


private:
};

#endif


//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppNoCloneModule.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppNoCloneModule. This is the abstract parent class for 
//	no-clone modules - modules that are wrappers for Fortran code and
//	therefore do not maintain their own independent state.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPNOCLONEMODULE_HH
#define APPNOCLONEMODULE_HH

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
 
class AppNoCloneModule : public AppModule {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppNoCloneModule( 
		   const char* const theName, 
		   const char* const theDescription 
		   );
  
  // Copy Constructor
  //    AppNoCloneModule( const AppNoCloneModule& );
  
  // Destructor
  virtual ~AppNoCloneModule( );
  
  // Selectors (const)
  
  bool isClonable( ) const;

private:
  // Not implemented.
  AppNoCloneModule( const AppNoCloneModule& );
  AppNoCloneModule& operator=( const AppNoCloneModule& );
    
};

#endif

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFilterModule.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppFilterModule. This is the abstract parent for filter
//	modules, modules that can terminate processing of a sequence
//	or path. Eventually these modules should also be able to
//	redirect processing to an alternative sequence, but this is not
//	yet implemented.
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

#ifndef APPFILTERMODULE_HH
#define APPFILTERMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppFilterModule : public AppModule {

  //--------------------
  // Instance Members --
  //--------------------
  
public:

  // Constructors
  AppFilterModule( 
		  const char* const theName, 
		  const char* const theDescription );
  
  // Copy Constructor
  //    AppFilterModule( const AppFilterModule& );
  
  // Destructor
  virtual ~AppFilterModule( );
  
  // Selectors (const)
  
  bool passed( ) const;
  
  // Modifiers
  
  void setPassed( bool flag );

private:

  // Not implemented.
  AppFilterModule( const AppFilterModule& );
  AppFilterModule& operator=( const AppFilterModule& );
    
};

typedef AppFilterModule AppFilterModule;
#endif

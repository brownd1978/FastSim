//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppUserBuild.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppUserBuild. This class must be provided by the user of
//	the framework in order to build an application. It must define
//	the modules that are to form the basis of the application.
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

#ifndef APPUSERBUILD_HH
#define APPUSERBUILD_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppBuild.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppUserBuild : public AppBuild {
  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppUserBuild( AppFramework* theFramework );
  
  // Destructor
  virtual ~AppUserBuild( );
  
  const char*  rcsId( ) const;

private:

  // Not implemented.
  AppUserBuild( const AppUserBuild& );
  AppUserBuild& operator=( const AppUserBuild& );

  
};

#endif

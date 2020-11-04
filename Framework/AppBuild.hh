//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppBuild.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppBuild. This is an abstract class that is designed to be
//	the parent for implementation build classes that specify which
//	modules are to be incorporated into the set of modules accessible
//	to an application.
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

#ifndef APPBUILD_HH
#define APPBUILD_HH

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class AppFramework;
class AppModule;
class AppInputModule;
class AppOutputModule;
class AppSequence;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppBuild {

//--------------------
// Instance Members --
//--------------------

public:
  
  // Constructors
  AppBuild( AppFramework* theFramework );
  
  // Destructor
  virtual ~AppBuild( );
  
  // Modifiers

  // add to the Framework ...
  virtual void add (AppModule* aModule);
  virtual void insert (AppModule* aModule, const AppModule* before);
  virtual void add (AppInputModule* aModule);
  virtual void add (AppOutputModule* aModule);
  virtual void add (AppSequence* aSequence);
  
protected:
  AppFramework* _theFramework;
};

#endif

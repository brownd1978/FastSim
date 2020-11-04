//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppMethod.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppMethod Class Definiton.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was designed and developed in cooperation with CLEO.
//
// Author List:
//	Marc Turcotte	                Original Author February '98
//      Marc Turcotte                   Changed for Record 3/26/98
//      Marc Turcotte                   Added support for parametrized
//                                      dispatch type  4/7/98
//      Marc Turcotte                   Modified for expanded use 6/19/98
//      Marc Turcotte                   Changed AppStatus for AppResult 6/23/98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPMETHOD_HH
#define APPMETHOD_HH

//
// Collaborating Class Headers
//

#include "Framework/AppMethodBase.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppResult.hh"
class AppFrame;

//
// Class Interface
//

template < class ModuleClass >
class AppMethod : public AppMethodBase {

private:
  ModuleClass* _targetModule;
  // Pointer to a (user) module of type "ModuleClass"
  
  AppResult (ModuleClass::* _targetMethod)(AppFrame* aFrame);
  // Pointer to a method of this module type with the (AppFrame*) 
  // signature and the return type "AppResult"

public:
  AppMethod( AppResult (ModuleClass::* aMethod)(AppFrame* aFrame), 
	     ModuleClass* aModule ) :
    AppMethodBase(false),
    _targetModule(aModule),
    _targetMethod(aMethod) {};
  virtual ~AppMethod();
  AppResult execute( AppFrame* aFrame );

private:

  // Not implemented.
  AppMethod( const AppMethod& );
  AppMethod& operator=( const AppMethod& );
    
};

// Inline implementation

#include "Framework/AppMethod.icc"

#endif







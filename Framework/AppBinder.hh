//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppBinder.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//
//	Header file for the AppBinder class
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was designed and developed in cooperation with CLEO.
//
// Author List:
//	Marc Turcotte	                Original Author February '98
//      Marc Turcotte                   Introduced AppStopType 3/20/98
//      Marc Turcotte                   Modified for AppRecord 3/26/98
//      Marc Turcotte                   Modified for AppFrame 4/7/98
//      Marc Turcotte                   Modified for expanded use 6/19/98
//      Marc Turcotte                   Changed AppStatus for AppResult 6/23/98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPBINDER_HH
#define APPBINDER_HH

class AppStopType;
class AppRecord;

#include "Framework/AppMethod.hh"
#include "Framework/AppFramework.hh"
#include "Framework/AppResult.hh"

//
// This class "incapsulates" a function that binds a predifined frame state to 
// a specifiable method in a user module. Why this is not a templated function
// is because cxx 5.5 rudely refused to compile it. The problem is that
// the template needs to use "T" as a parameter and the compiler
// did not recognize it as a class or a struct or a union. 
// So to avoid this problem, just use the following
// workaround for now. M. Turcotte
//

template < class ModuleClass > 
class AppBinder{
public:
  AppBinder();

  void 
  bind(const AppStopType& aStopType,
       AppResult (ModuleClass::* aMethod)(AppFrame* aFrame),    
       ModuleClass* aModule,
       AppFramework* theFramework);

private:

  // Not implemented.
  AppBinder( const AppBinder& );
  AppBinder& operator=( const AppBinder& );

};    

//
// Inline implementations
//

#include "Framework/AppBinder.icc"

#endif

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFilterModule.cc 509 2010-01-14 15:18:55Z stroili $
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
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppFilterModule.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppFilterModule::AppFilterModule( 
    const char* const theName, 
    const char* const theDescription )
    : AppModule( theName, theDescription )
{
    _execType  = APP_filter;
}

//--------------
// Destructor --
//--------------

AppFilterModule::~AppFilterModule( )
{
}

//-------------
// Selectors --
//-------------

bool
AppFilterModule::passed( ) const
{
    return _passed;
}

//-------------
// Modifiers --
//-------------

void
AppFilterModule::setPassed( bool flag )
{
    _passed = flag;
}
  

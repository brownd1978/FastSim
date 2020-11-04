//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppOutputModule.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppOutputModule. Abstract parent for output modules. Implementers
//	of such modules should inherit from this class rather than from the
//	AppModule class directly.
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
#include "Framework/AppOutputModule.hh"

//-------------
// C Headers --
//-------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppReportAttributes.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppOutputModule::AppOutputModule( 
    const char* const theName, 
    const char* const theDescription )
    : AppModule( theName, theDescription )
{
    _execType  = APP_output;
}

//--------------
// Destructor --
//--------------

AppOutputModule::~AppOutputModule( )
{
}
AppResult
AppOutputModule::outputEvent ( AbsEvent* anEvent ) {
  return AppResult::OK;
}

AppResult
AppOutputModule::outputFrame( AppFrame* aFrame ) { // Frame Output
  return AppResult::OK;
}

void
AppOutputModule::addToReport(AppReportAttributes* attrib, int mode) const
{
  if (framework()->theOutputModule() == this)
    partialReport(" (Active) ");
}

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppInputModule.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppInputModule. Abstract parent class for input modules.
//	Implementers of input modules should inherit from this class
//	rather than AppModule directly.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   9/22/98 Added default behavior
//                                      to inputEvent
//      A. De Silva                     1/29/99 inputEvent return value changed.
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998              The University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppInputModule.hh"

//-------------
// C Headers --
//-------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "Framework/AppFramework.hh"
#include "Framework/AppReportAttributes.hh"
#include <iostream>

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//static const char rcsid[] = "$Id: AppInputModule.cc 509 2010-01-14 15:18:55Z stroili $";

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppInputModule::AppInputModule( 
    const char* const theName, 
    const char* const theDescription )
    : AppModule( theName, theDescription )
{
    _execType = APP_input;
}

//--------------
// Destructor --
//--------------

AppInputModule::~AppInputModule( )
{
}

//-------------
// Modifiers --
//-------------

AppResult
AppInputModule::inputEvent( AbsEvent*& anEvent,
			    AppStopType& theDispatchStopType ) 
{   // Quiery for Input
    // if not overridden, then defer to the old, plain input method that
    // just fetches physics_event inputs

    theDispatchStopType = AppStopType::physics_event;
    return inputEvent( anEvent );
}

AppResult
AppInputModule::inputFrame( AppFrame*& aFrame,
                            AppStopType& theDispatchStopType ) { // Frame Input
  return AppResult::OK;
}

void AppInputModule::addToReport( AppReportAttributes* attrib, int mode ) const
{
  if (framework()->theInputModule() == this)
    partialReport(" (Active) ");
}

void 
AppInputModule::deleteEvent(AbsEvent*& anEvent) {
  if (anEvent != 0) {
    delete anEvent;
    anEvent = 0;
  }
}

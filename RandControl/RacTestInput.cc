//--------------------------------------------------------------------------
// File and Version Information:
//	$Id: RacTestInput.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacTestInput
//	Subclass of InputModule which builds the event containing only 
//	event info (GEvent data)
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A.Salnikov
//
// Copyright Information:
//	Copyright (C) 1998		Saclay
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "RandControl/RacTestInput.hh"

//-------------
// C Headers --  
//-------------

//---------------
// C++ Headers --
//---------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "G3Data/GEvent.hh"
#include "CLHEP/Alist/AList.h"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "CLHEP/Alist/AList.h"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


RacTestInput::RacTestInput( ) : 
  AppFileInput( "RacTestInput", "Dummy Input Module" ),
  _runNumber ( "run", this, 1 ),
  _eventNumberCmd("eventNumber", this, 1),  // first event #
  _geventList ( "geventList", this, "Default" )
{
  commands()->append ( &_runNumber ) ;
  commands()->append ( &_eventNumberCmd ) ;
  commands()->append ( &_geventList ) ;
}


//--------------
// Destructor --
//--------------

RacTestInput::~RacTestInput( )
{
}

//--------------
// Operations --
//--------------

AppResult
RacTestInput::beginJob ( AbsEvent* )
{
  if (_verbose.value()) {
    cout << name( ) << " -- begin job." << endl;
  }
  _eventNumber = _eventNumberCmd.value() - 1; // -1 to cancel ++ in event(..)
  return AppResult::OK;
}

AppResult
RacTestInput::inputEvent( AbsEvent*& anEvent, 
                                AppStopType& theDispatchStopType )
{
  // just invoke the base class method
  return AppFileInput::inputEvent(anEvent, theDispatchStopType);
}

AppResult
RacTestInput::inputEvent ( AbsEvent*& evt )
{
  _eventNumber ++ ;

  // check for existing event, delete if present
  if (0 != evt) {
    delete evt;
  }

  evt = new(AbsEvent);

  GEvent* einfo = new GEvent ;
  einfo->setIdrun( _runNumber.value() ) ;
  einfo->setIdevt( _eventNumber ) ;

  // create HepAQList to store event info
  HepAList<GEvent>* elist = new HepAList<GEvent>() ;
  elist->append( einfo ) ;  
  
  // store it in the event
  Ifd< HepAList<GEvent> >::put ( evt, 
				    new IfdHepAListProxy<GEvent>(elist),
				    "Default" ) ;
  
  return AppResult::OK;
}

AppResult
RacTestInput::endJob ( AbsEvent* )
{
  if ( _verbose.value() ) {
    cout << name( ) << " -- end job." << endl;
  }
  return AppResult::OK;
}


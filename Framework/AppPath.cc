//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppPath.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppPath. This class describes paths within the BaBar
//	Application Framework.
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
#include "Framework/AppPath.hh"

//-------------
// C Headers --
//-------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppPath::AppPath( const char* const theName, const char* const theDescription )
    : AppSequence( theName, theDescription )
{
    _execType  = APP_path;
}

//--------------
// Destructor --
//--------------

AppPath::~AppPath( )
{
}

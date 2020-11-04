//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFileOutputModule.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
///      Class AppFileOutputModule implementations file.
//
//      This is the standard file output module for the BaBar framework. 
//      It is designed as a replacement for the APPFileOutputModule class, 
//      inheriting from an abstract AppStreamsOutputModule class. 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	    David R. Quarrie		        Original Author
//
// Copyright Information:
//	    Copyright (C) 1999      	    Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppFileOutputModule.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFileOutputCommand.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppFileOutputModule::AppFileOutputModule( )
    : AppStreamsOutputModule( "FileOutput", "Standard File Output Module" )
{
    commands( )->append( _outputCmd = new AppFileOutputCommand( "output", this ) );
    commands( )->append( _outCmd    = new AppFileOutputCommand( "out"   , this ) );
}

//--------------
// Destructor --
//--------------

AppFileOutputModule::~AppFileOutputModule( )
{
}

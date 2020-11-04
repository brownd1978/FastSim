//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppBuild.cc 509 2010-01-14 15:18:55Z stroili $
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
//
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Made last added InputModule the
//                                      Framework's input module. 5/97
//      Marc Turcotte                   Merged R.Kapur's TK interface 5/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997, 1998        The University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppBuild.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"

//----------------
// Constructors --
//----------------

AppBuild::AppBuild(  AppFramework* theFramework )
    : _theFramework( theFramework )
{
}

//--------------
// Destructor --
//--------------

AppBuild::~AppBuild( )
{
}

//-------------
// Modifiers --
//-------------

void 
AppBuild::add( AppModule* aModule )
{
    assert( NULL != aModule );
    _theFramework->add( aModule );
}

void 
AppBuild::insert( AppModule* aModule, const AppModule* before )
{
    assert( NULL != aModule );
    _theFramework->insert( aModule, before );
}

void 
AppBuild::add( AppInputModule* aModule )
{
    assert( NULL != aModule );    

    _theFramework->add( aModule );
    _theFramework->setTheInputModule(aModule);

}

void 
AppBuild::add( AppOutputModule* aModule )
{
    assert( NULL != aModule );    

    _theFramework->add( aModule );
}

void 
AppBuild::add( AppSequence* aSequence )
{
    assert( NULL != aSequence );    

    _theFramework->add( aSequence );
}


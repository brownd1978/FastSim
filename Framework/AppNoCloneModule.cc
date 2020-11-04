//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppNoCloneModule.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppNoCloneModule
//
// Environment:
//	Class AppNoCloneModule. This is the abstract parent class for 
//	no-clone modules - modules that are wrappers for Fortran code and
//	therefore do not maintain their own independent state.
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
#include "Framework/AppNoCloneModule.hh"

//-------------
// C Headers --
//-------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppNoCloneModule::AppNoCloneModule( 
    const char* const theName, 
    const char* const theDescription )
    : AppModule( theName, theDescription )
{
    _execType  = APP_noclone;
}

//--------------
// Destructor --
//--------------

AppNoCloneModule::~AppNoCloneModule( )
{
}

//-------------
// Selectors --
//-------------

bool
AppNoCloneModule::isClonable( ) const
{
    return false;
}

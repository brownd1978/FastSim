//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppExecNode.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppExecNode. Class to act as node in Sequences & Paths. The
//	node contains the associated target (module or sequence) and the
//	execution mode.
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
#include "Framework/AppExecNode.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppExecutable.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//static const char rcsid[] = "$Id: AppExecNode.cc 509 2010-01-14 15:18:55Z stroili $";

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppExecNode::AppExecNode( 
    const AppExecutable* const theTarget, const int theMode )
    : _mode  ( theMode ),
      _target( theTarget )
{
}

//--------------
// Destructor --
//--------------

AppExecNode::~AppExecNode( )
{
}

//-------------
// Selectors --
//-------------


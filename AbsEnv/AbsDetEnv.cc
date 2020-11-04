//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsDetEnv.cc 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsDetEnv
//
//      Abstract class for detector envirnoments.  Individual
//      detectors' environment classes will inherit from this
//      class and define their own environment variables.  Access
//      to these variables is via an asbtract structure, which 
//      also contains neighbour information.  See EmcEnv.cc/hh for
//      an example. 
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		        Original Author
//      Stephen J. Gowdy                University Of Edinburgh
//      Phil Strother                   Imperial College
//
// Copyright Information:
//	Copyright (C) 1995, 1996	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsEnv/AbsDetEnv.hh"

//-------------
// C Headers --
//-------------
//#include <assert.h>
//#include <string.h>
#include <iostream>

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

//--------------
// Destructor --
//--------------

AbsDetEnv::~AbsDetEnv( )
{
}


// Selectors

const AbsDetStructure* AbsDetEnv::getDetStructure() const
{
  return _detStructure;
}

// Modifiers

void AbsDetEnv::setDetStructure(const AbsDetStructure* detStructure)
{
  //_detStructure=const_cast<AbsDetStructure*>(detStructure);
  _detStructure=(AbsDetStructure*)detStructure;
}		   














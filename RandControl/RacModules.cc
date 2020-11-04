//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacModules.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Function RacModules.
//	Creates all modules and adds them to the caller-AppUserBuild
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A.Salnikov
//
// Copyright Information:
//	Copyright (C) 1998
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppUserBuild.hh"
#include "RandControl/RacRandomPicker.hh"
#include "RandControl/RacRandomControl.hh"

//----------------------------------------------
// include files for example analysis modules --
//----------------------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

void 
RacModules ( AppUserBuild* forWho )
{
  // One RandomControl module
  forWho -> add( new RacRandomControl ) ;

  // and four RandomPicker modules
  forWho -> add( new RacRandomPicker( "Primary" ) ) ;
  forWho -> add( new RacRandomPicker( "Simulation" ) ) ;
  forWho -> add( new RacRandomPicker( "Digis" ) ) ;
  forWho -> add( new RacRandomPicker( "Reco" ) ) ;
  forWho -> add( new RacRandomPicker( "Analysis" ) ) ;
}


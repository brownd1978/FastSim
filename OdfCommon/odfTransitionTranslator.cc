//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: odfTransitionTranslator.cc 159 2010-01-15 08:50:21Z stroili $
//
// Description:
//	odfTransitionTranslator implementation
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      A.M. Eisner, Univ. Calif. Santa Cruz
//
// History:
//      18-Jul-2001, AME  - Initial version (adapted from gpdf's version
//                          of odfDamageTranslator).
//
// Copyright Information:
//	Copyright (C) 2001   California Institute of Technology
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

// no BaBar.hh permitted in dual-use code

//-----------------------
// This Class's Header --
//-----------------------
#include "OdfCommon/odfTransitionTranslator.hh"

//-------------
// C Headers --
//-------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

//--------------
// Destructor --
//--------------

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

const char*
odfTransitionTranslator::_transTrans[] = { 
  "Unknown",      // id  0
  "Reset",        // id  1
  "Shutdown",     // id  2
  "Map",          // id  3
  "Unmap",        // id  4
  "Configure",    // id  5
  "Unconfigure",  // id  6
  "BeginRun",     // id  7
  "EndRun",       // id  8
  "BeginMeta",    // id  9
  "EndMeta",      // id 10
  "Resume",       // id 11
  "Pause",        // id 12
  "BeginMacro",   // id 13
  "EndMacro",     // id 14
  "BeginMajor",   // id 15
  "EndMajor",     // id 16
  "BeginMinor",   // id 17
  "EndMinor",     // id 18
  "Enable",       // id 19
  "Disable",      // id 20
  "L1Accept",     // id 21
  "Crash",        // id 22
};


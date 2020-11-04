//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: odfDamageTranslator.cc 159 2010-01-15 08:50:21Z stroili $
//
// Description:
//	odfDamageTranslator implementation
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Gregory Dubois-Felsmann  (originator)
//
// Copyright Information:
//	Copyright (C) 2001   California Institute of Technology
//
//------------------------------------------------------------------------

// no BaBar.hh permitted in dual-use code

//-----------------------
// This Class's Header --
//-----------------------
#include "OdfCommon/odfDamageTranslator.hh"

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
odfDamageTranslator::_damageTrans[] = { 
  "PCI Parity Error",                         // bit  0
  "Dropped Contribution",		      // bit  1
  "PCI Target Abort",			      // bit  2
  "PCI Master Abort",			      // bit  3
  "Shutdown",				      // bit  4
  "i960 Bus Fault",			      // bit  5
  "i960 Memory Fault",			      // bit  6
  "Payload Truncated",			      // bit  7
  "Timeout",				      // bit  8
  "MUQ NAQ",				      // bit  9
  "Invalid Transition",			      // bit 10
  "Transition Timeout",			      // bit 11
  "Out of Order",			      // bit 12
  "Out of Synch",			      // bit 13
  "FEX Error",				      // bit 14
  "Incomplete Contribution",		      // bit 15
  "Clink Not Ready",			      // bit 16
  "PC Full",				      // bit 17
  "FEE Full",				      // bit 18
  "Clink B Not Ready",			      // bit 19
  "Clink A Not Ready",			      // bit 20
  "Write In Progress",			      // bit 21
  "Clink Not Enabled",			      // bit 22
  "Clink Throttled",			      // bit 23
  "Dlink A Not Ready / Flink A Timeout",      // bit 24
  "Dlink A Timeout / Flink B Timeout",	      // bit 25
  "Dlink A Overflow / Flink C Timeout",	      // bit 26
  "Dlink A No Start Bit / Flink A Not Ready", // bit 27
  "Dlink B Not Ready / Flink B Not Ready",    // bit 28
  "Dlink B Timeout / Flink C Not Ready",      // bit 29
  "Dlink B Overflow",			      // bit 30
  "Dlink B No Start Bit"		      // bit 31
};

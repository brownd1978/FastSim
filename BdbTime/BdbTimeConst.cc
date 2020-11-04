//-----------------------------------------------------------------------------
//
// File and Version Information:
//      $Id: BdbTimeConst.cc 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//      Class BdbTimeConst
//      This class contains time constants used by the BdbTime 
//      and BdbDuration classes.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      J. Ohnemus                      Original Author
//
// Copyright Information:
//      Copyright (C) 1994, 1995        Lawrence Berkeley Laboratory
//
//-----------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "BdbTime/BdbTimeConst.hh"
 
//---------------
// Definitions --
//---------------

const d_ULong BdbTimeConst::minusInfinity = 0U;           // 0
const d_ULong BdbTimeConst::plusInfinity  = 4294967295U;  // 2^32 - 1
const d_ULong BdbTimeConst::nsecInASec    = 1000000000U;  // ns in 1 sec 
const d_ULong BdbTimeConst::seconds_1901_to_1970 = 2177452800U;








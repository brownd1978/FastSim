//-----------------------------------------------------------------------------
//
// File and Version Information:
//      $Id: BdbTimeConst.hh 496 2010-01-13 17:10:44Z stroili $
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

#ifndef BDBTIMECONST_HH
#define BDBTIMECONST_HH

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBarODMGTypes.h"

class BdbTimeConst {

public:

//---------------
// Definitions --
//---------------

  static const d_ULong minusInfinity;   // -infinity = 0
  static const d_ULong plusInfinity;    // +infinity = 2^32 - 1
  static const d_ULong nsecInASec;      // # of nanoseconds in one second
  static const d_ULong seconds_1901_to_1970; // what it says
};

#endif








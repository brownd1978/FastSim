//-----------------------------------------------------------------------------
// File and Version Information
//      $Id: BdbMinimumInterval.hh 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//      BdbMinimumInterval.
//      Simple utility to find the minimum validity interval given
//      a vector of validity intervals. Assumes all input intervals
//      contain at least one common time point.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Jane Tinslay                
//
// Copyright Information:
//      Copyright (C) 2005          SLAC
//
//-----------------------------------------------------------------------------
#ifndef BDBMINIMUMINTERVAL
#define BDBMINIMUMINTERVAL

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include <vector>

using std::vector;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
class BdbIntervalBase;

namespace BdbMinimumInterval {

  BdbIntervalBase findInterval(const vector<BdbIntervalBase>& intervals);

}

#endif

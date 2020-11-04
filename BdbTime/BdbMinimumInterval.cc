//-----------------------------------------------------------------------------
// File and Version Information
//      $Id: BdbMinimumInterval.cc 496 2010-01-13 17:10:44Z stroili $
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
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BdbTime/BdbMinimumInterval.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BdbTime/BdbIntervalBase.hh"
#include "BdbTime/BdbTime.hh"

namespace BdbMinimumInterval {

  BdbIntervalBase findInterval(const vector<BdbIntervalBase>& intervals)
  {
    BdbIntervalBase result;

    BdbTime begin(BdbTime::minusInfinity);
    BdbTime end(BdbTime::plusInfinity);
    
    vector<BdbIntervalBase>::const_iterator iter = intervals.begin();
    
    while (iter != intervals.end()) {
      if (iter->beginTime() > begin) begin = iter->beginTime();
      if (iter->endTime() < end) end = iter->endTime();
      iter++;
    }
    
    result.setBeginTime(begin);
    result.setEndTime(end);

    return result;
  }
}

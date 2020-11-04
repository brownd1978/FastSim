// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkSTLayout.hh 105 2010-01-15 12:14:11Z stroili $
//
//  Description:
//  Support tube geometry primivitve interface definition
//
// Copyright Information:
//	Copyright (C) 2001	Colorado State University
//
//  Authors: Johan Blouw, July 29, 2001
//------------------------------------------------------------------------------
#ifndef TRKSTLAYOUT_HH
#define TRKSTLAYOUT_HH
#include "BaBar/BaBarODMGTypes.h"
#include <iostream>
#include "ErrLogger/ErrLog.hh"

class TrkSTLayout {
public:
  enum zends { lowZEnd=0,highZEnd,nEnds=2};
  TrkSTLayout(){;}
  virtual ~TrkSTLayout(){;}
// accessors: global
  virtual d_Double radius() const = 0;
// number of segments
  virtual d_ULong nSegments() const = 0;
// segment-dependent values
  virtual d_Double thickness(unsigned iseg) const = 0;
// material is referenced by name
  virtual const char* getSTMaterial(unsigned iseg) const = 0;
  virtual d_Double zSTEnds(unsigned iseg,TrkSTLayout::zends end) const = 0;
  
  virtual void print(std::ostream& os=std::cout) const {
    os << "Support tube layout with radius = " << radius()
       << " and "  << nSegments() << " segments, as follows: "<< endmsg;
    for(unsigned iseg=0;iseg<nSegments();iseg++){
      os << "Segment " << iseg 
	 << " has material " << getSTMaterial(iseg)
	 << " with thickness " << thickness(iseg)
	 << " , low Z end = " << zSTEnds(iseg,lowZEnd)
	 << " , high Z end = " << zSTEnds(iseg,highZEnd)
	 << endmsg;
    }
  }
};
#endif

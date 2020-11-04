// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkBPLayout.hh 105 2010-01-15 12:14:11Z stroili $
//
//  Description:
//  Pep beampipe geometry primivitve interface definition
//
// Copyright Information:
//	Copyright (C) 1996	Colorado State University
//
//  Authors: Johan Blouw, April 25, 2001
//------------------------------------------------------------------------------
#ifndef TRKBPLAYOUT_HH
#define TRKBPLAYOUT_HH
#include "BaBar/BaBarODMGTypes.h"
#include <iostream>
#include "ErrLogger/ErrLog.hh"

class TrkBPLayout {
public:
  enum zends { lowZEnd=0,highZEnd,nEnds=2};
  TrkBPLayout(){;}
  virtual ~TrkBPLayout(){;}
// accessors: global
  virtual d_Double radius() const = 0;
// number of segments
  virtual d_ULong nSegments() const = 0;
// segment-dependent values
  virtual d_Double thickness(unsigned iseg) const = 0;
// material is referenced by name
  virtual const char* getBPMaterial(unsigned iseg) const = 0;
  virtual d_Double zBPEnds(unsigned iseg,TrkBPLayout::zends end) const = 0;
  
  virtual void print(std::ostream& os=std::cout) const {
    os << "Beampipe layout with radius = " << radius()
       << " and "  << nSegments() << " segments, as follows: "<< endmsg;
    for(unsigned iseg=0;iseg<nSegments();iseg++){
      os << "Segment " << iseg 
	 << " has material " << getBPMaterial(iseg)
	 << " with thickness " << thickness(iseg)
	 << " , low Z end = " << zBPEnds(iseg,lowZEnd)
	 << " , high Z end = " << zBPEnds(iseg,highZEnd)
	 << endmsg;
    }
  }
};
#endif

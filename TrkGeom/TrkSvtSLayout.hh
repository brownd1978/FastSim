// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkSvtSLayout.hh 124 2010-05-11 10:41:40Z stroili $
//
//  Description:
//  Support tube geometry primivitve interface definition
//
// Copyright Information:
//	Copyright (C) 2001	Colorado State University
//
//  Authors: Johan Blouw, July 29, 2001
//------------------------------------------------------------------------------
#ifndef TRKSVTSLAYOUT_HH
#define TRKSVTSLAYOUT_HH
#include "BaBar/BaBarODMGTypes.h"
#include <iostream>
#include "ErrLogger/ErrLog.hh"

class TrkSvtSLayout {
public:
  enum zends { lowZEnd=0,highZEnd,nEnds=2};
  TrkSvtSLayout(){;}
  virtual ~TrkSvtSLayout(){;}
// accessors: global
// number of elements
  virtual d_ULong nElements() const = 0;
// number of segments
  virtual d_ULong nSegments() const = 0;
// radii for inner and outer shields
  virtual d_Double radii(unsigned ielem) const = 0;
// segment-dependent values
  virtual d_Double thickness(unsigned ielem,unsigned iseg) const = 0;
// material is referenced by name
  virtual const char* getSvtSMaterial(unsigned ielem,unsigned iseg) const = 0;
  virtual d_Double zSvtSZEnds(unsigned ielem,unsigned iseg,TrkSvtSLayout::zends end) const = 0;
  
  virtual void print(std::ostream& os=std::cout) const {
    const char *seg[2] = {"inner","outer"};
    os << " " << nElements() << " elements, consisting of: "<< endmsg;
    for(unsigned ielem=0;ielem<nElements();ielem++){
        os << "	  an " << seg[ielem] << " svt shield" << endmsg;
        os << "	  with radius " << radii(ielem) << endmsg;
        os << "	  and this element consists of " << nSegments() << " segments, as follows: "<< endmsg;
        for(unsigned iseg=0;iseg<nSegments();iseg++){
          os << "		Segment " << iseg << ": Svt " << seg[iseg] << "shield "
	     << "has material " << getSvtSMaterial(ielem,iseg)
	     << " with thickness " << thickness(ielem,iseg) << std::endl
	     << "		low z-end = " << zSvtSZEnds(ielem,iseg,lowZEnd) << std::endl
	     << "		high z-end = " << zSvtSZEnds(ielem,iseg,highZEnd)
	     << endmsg;
        }
    }
  }
};
#endif

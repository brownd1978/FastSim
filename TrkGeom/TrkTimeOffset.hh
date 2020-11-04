// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkTimeOffset.hh 105 2010-01-15 12:14:11Z stroili $
//
//  Authors: 
//      Andy Salnikov
//
// Copyright Information:
//	Copyright (C) 2006 SLAC
//
//------------------------------------------------------------------------------
#ifndef TRKTIMEOFFSET_HH
#define TRKTIMEOFFSET_HH

#include "BaBar/BaBarODMGTypes.h"

class TrkTimeOffset {
public:

  TrkTimeOffset() : _offset(0) {}

  TrkTimeOffset(double offset) : _offset(offset) {}

  ~TrkTimeOffset() {}

  d_Double timeOffset() const { return _offset; }

private:

  double _offset;
};

#endif // TRKTIMEOFFSET_HH

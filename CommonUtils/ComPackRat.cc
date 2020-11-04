//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackRat.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackRat.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Authors: 8/17/01 David Brown
//
//      Copyright (C) 2001 Lawrence Berkeley National Lab
//--------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "CommonUtils/ComPackRat.hh"
#include <math.h>
#include <assert.h>

ComPackRat::ComPackRat(unsigned denom, long minnum, long maxnum) :
  _denom(denom)
{
// check for stupidity or typo
  assert(minnum<maxnum);
// limits are easy
  _minVal = double(minnum)/_denom;
  _maxVal = double(maxnum)/_denom;
  _valRange = _maxVal-_minVal;
// scale factor for unpacking.
  _scale = 1.0/double(_denom);
// compute how many bits it takes to store this rational.  This comes
// from the number of possible numerators.
  static double invln2(1.0/log(2.0));
  unsigned irange = maxnum-minnum;
  _bitRange = unsigned(log(double(irange))*invln2+1.00001); // avoid roundoff error
  _bitMask = (1<<_bitRange)-1;
}

ComPackRat::~ComPackRat()
{} 

ComPackBase<float>::StatusCode
ComPackRat::pack (const float val, d_ULong & ival) const {
// truncate with a 1/2 value. this centers the bins on the range limits
  int jval = int(_denom*(val-_minVal)+0.5);
  if(jval >= 0 && jval <= _bitMask){
    ival = jval;
    return ComPackBaseBase::TAG_OK;
  } else if (jval < 0){
    ival = 0;
    return ComPackBaseBase::TAG_VAL_ROUND_UP;
  } else {
    ival = _bitMask;
    return ComPackBaseBase::TAG_VAL_ROUND_DOWN;
  }
}

ComPackBase<double>::StatusCode
ComPackRat::unpack (const d_ULong ival, float &val) const {
  val = double(ival)*_scale + _minVal;
  return ComPackBaseBase::TAG_OK;
}

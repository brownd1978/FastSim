//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackFlatFloatBroken.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackFlatFloatBroken - see .hh file for details
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      D.E.Azzopardi               Originator. 
//
// Copyright Information:
//      Copyright (C) 1999  DESVA Research & Design, for Hedgehog Concepts
//
// Additional information:
//      This class, and related classes, are in development right now...
//      so BEWARE!!!!
//   
// Quote:
//      "I have never let my schooling get in the way of my education."
//                              -Mark Twain 
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//
// C includes
//
#include <stddef.h> 
#include <math.h>
#include <stdlib.h>

//
// C++ includes
//
#include <iostream>

//
// Bass class includes
//
#include "CommonUtils/ComPackBase.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"

//
// This class
//
#include "CommonUtils/ComPackFlatFloatBroken.hh"
using std::cout;
using std::endl;

ComPackFlatFloatBroken::ComPackFlatFloatBroken(const double val_one, const double val_two, size_t bits)
{
  if (val_one>val_two) {
    _minVal=val_two; 
    _maxVal=val_one;
  } else {
    _minVal=val_one; 
    _maxVal=val_two;
  }

  const double epsilon = 0.0001;
  if (( _maxVal - _minVal )<=epsilon) {
    ErrMsg(fatal) << "Error : Range is zero!" << endmsg;
  }
  _valRange = _maxVal - _minVal;

  if (bits > _maxlongbits ) {
    bits = _maxlongbits;
    ErrMsg(fatal) << "Warning : Number of bits truncated! "
    << "Reason  : Number of bits too large for " << _maxlongbits << " bit packing operations! "
    << "This is probably caused by a serious typo somewhere!" << endmsg;
  }
  _bitRange = bits;
  _bitMask = (1<<_bitRange) - 1;
  if ( 0 == _bitMask) { // check for wrap around
    _bitMask--;
  }
}

ComPackFlatFloatBroken::~ComPackFlatFloatBroken(){}

ComPackBase<double>::StatusCode 
ComPackFlatFloatBroken::pack(const double theval, d_ULong & packedval) const
{
  double useVal (theval);
  StatusCode retval(TAG_OK);
  if (theval>_maxVal) {
    useVal = _maxVal; // clamp
    ErrMsg(warning) << "Error : Value is out of range - clamping" << endmsg;
    retval = TAG_VAL_ROUND_DOWN;
  }
  else if (theval<_minVal) {
    useVal = _minVal; // clamp
    ErrMsg(warning) << "Error : Value is out of range - clamping" << endmsg;
    retval = TAG_VAL_ROUND_UP;
  }
  useVal -= _minVal;

  //add half the error to useVal?
  double dbitrange  = double (_bitMask);
  //useVal += 0.5*_valRange/dbitrange;
  double fraction (useVal / _valRange );
  double result (fraction * dbitrange);
  packedval = (d_ULong) result;
  packedval &= _bitMask;
  return retval;
}

ComPackBase<double>::StatusCode
ComPackFlatFloatBroken::unpack(const d_ULong val, double & unpackedval ) const
{
  d_ULong useVal (val & _bitMask) ; // Make sure we have the lowest bits
  if (useVal==_bitMask) useVal--; // LW: avoid overflow
  double fraction((double(useVal)+0.5)/double(_bitMask)); // LW: added 0.5, i.e. unpack to bin center
  unpackedval = (_minVal+fraction*_valRange);
  return TAG_OK;
}

void
ComPackFlatFloatBroken::testMe (size_t numsteps, double & toterror)
{
  if ( 0 == numsteps) return;
  toterror = 0.;
  const double incstep = double (_valRange) / double (numsteps);
  for ( double i = _minVal; i<= _maxVal; i+=incstep)
    {
      d_ULong tagVal;
      pack ( i, tagVal );
      double unTagVal;
      unpack ( tagVal, unTagVal );
      cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << unTagVal << endl;
      toterror += fabs ( i-unTagVal );
    }
}


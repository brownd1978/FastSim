//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackInt.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackInt
//      - allows one to construct limited "short ints"
// To Use:
//      Construct with the minimum, and the maximum value that you would like
//      to store. The number of bits required are calculated and stored 
//      internally. Provides pack() and unpack() methods.
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
//      "If life is a highway, then it's a traffic jam. "
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BaBar/BaBarODMGTypes.h"

//
// C includes
//
#include <stddef.h> 
#include <stdlib.h>
#include <math.h>
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
#include "CommonUtils/ComPackInt.hh"
using std::cout;
using std::endl;

ComPackInt::ComPackInt(const int val_one, const int val_two) // bits doesn't make sense!
{
  if (val_one>val_two) {
    _minVal=val_two; 
    _maxVal=val_one;
  } else {
    _minVal=val_one; 
    _maxVal=val_two;
  }

  if ( _maxVal == _minVal ) 
    ErrMsg(fatal)   << "Error : Range is zero!" << endmsg;
  _valRange = _maxVal - _minVal; 
  const double invLog2 ( 1.0 / log(2.0) ); 
  double numBitsNeeded ( ceil ( invLog2*log ( (double) 1.0 + _valRange) ) );
  if (numBitsNeeded>(_maxlongbits-1)) // subtract one for sign bit - really only being this pedantic because sizeof(int) might be >4 in future...
    ErrMsg(fatal)   << "Error : Range too great!" << endmsg;
  _bitRange = ((size_t) numBitsNeeded);
  _bitMask = (0x1<<_bitRange) - 1;
}

ComPackInt::~ComPackInt(){}

ComPackBase<int>::StatusCode
ComPackInt::pack(const int theval, d_ULong & packedval) const
{
  int useVal = theval;
  StatusCode retval = TAG_OK;
  if (theval>_maxVal) {
    useVal = _maxVal; // clamp
//    ErrMsg(warning) << "Error : Value is out of range - clamping" << endmsg;
    retval = TAG_VAL_ROUND_DOWN;    
  }
  else if (theval<_minVal) {
    useVal = _minVal; // clamp
//    ErrMsg(warning)<< "Error : Value is out of range - clamping" << endmsg;
    retval = TAG_VAL_ROUND_UP;
  }
  packedval = ((useVal - _minVal)&_bitMask);
  return retval;
}

ComPackBase<int>::StatusCode
ComPackInt::unpack(const d_ULong val, int & unpackedval ) const
{
  unpackedval = (val&_bitMask) + _minVal;
  return TAG_OK;
}

void 
ComPackInt::testMe (size_t incstep, int & toterror)
{
  toterror = 0; // ... and should *stay* zero!
  cout << "Incremental step is :" << incstep << endl;
  cout << _minVal <<endl;
  cout << _bitMask << endl;
  for ( int i = _minVal; i<= _maxVal; i+= incstep )
    {
      d_ULong tagVal;
      pack ( i, tagVal );
      int unTagVal;
      unpack ( tagVal, unTagVal );
      cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << (float) unTagVal << endl;
      toterror += abs ( i-unTagVal );
    }
}

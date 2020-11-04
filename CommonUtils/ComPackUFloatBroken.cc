// --------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackUFloatBroken.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackUFloatBroken 
//      - allows one to construct limited precision "unsigned floats"
//        User may specify number of bits for storing mantissa and
//        exponent, subject to the restriction that the total number of 
//        bits is less than 32 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//      Used only in old "micro" written before end of Y2K 
//
// Author List:
//      D.E.Azzopardi           Originator of ComPackUFloat. 
//      D.Aston   cloned from ComPackUFloat in CommonUtils V00-03-02. 
//
// Copyright Information:
//      Copyright (C) 1999  DESVA Research & Design, for Hedgehog Concepts
//
// Additional information:
//      This class, and related classes, has bugs and is declared broken...
//      so BEWARE!!!!
//   
// Quote:
//      "And so it goes."
//
// --------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BaBar/BaBarODMGTypes.h"

//
// C includes
//
#include <stddef.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>

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
#include "CommonUtils/ComPackUFloatBroken.hh"
using std::cout;
using std::endl;

ComPackUFloatBroken::ComPackUFloatBroken(const size_t fraclen, const size_t explen, const int bias)
{
  if (fraclen<FRACLENLOWERLIMIT) {
    ErrMsg(fatal) << "Error : fraclen too small to be useful! - Perhaps you require ComPackFlatFloat?" << endmsg;
  }
  if (explen<EXPLENLOWERLIMIT) {
    ErrMsg(fatal) << "Error : explen too small to be useful! - Perhaps you require ComPackFlatFloat?" << endmsg;
  }
  if (explen>EXPLENUPPERLIMIT) {
    ErrMsg(fatal) << "Error : explen too large to be useful! - Perhaps you require a long double?" << endmsg;
  }
  if (fraclen>FRACLENUPPERLIMIT) {
    ErrMsg(fatal) << "Error : fraclen too large to be useful! - Perhaps you require a long double?" << endmsg;
  }

  _fracBits=fraclen;
  _expBits=explen;
  _bias = bias;
  
  size_t bits = _fracBits + _expBits;
  if (bits>_maxlongbits) ErrMsg(fatal) << "Error : too large!" << endmsg;
  _bitRange = bits;
  _bitMask = (1<<_bitRange) - 1;

  _fracMask = (0x1<<_fracBits);
  _fracRange = 2.0 * (double) _fracMask;
  _fracMask --;   

  _expMax = 0x1<<_expBits;
  _expMask = _expMax - 1;
  _expMax>>=1;            //Top bit is used for sign, so maximum value is halved
  _expMask <<= _fracBits;
  _minVal = ldexp(0.5,-_expMax+_bias);
  double maxFrac = 0.5 + (((double)_fracMask - 1.0)/ (double) _fracRange);
  _maxVal = ldexp(maxFrac,_expMax+_bias-1);
  _valRange = _maxVal-_minVal;

}

ComPackUFloatBroken::~ComPackUFloatBroken(){}

ComPackBase<double>::StatusCode 
ComPackUFloatBroken::pack(const double theval, d_ULong & packedval) const
{
  double useval = theval;
  if (theval < 0.0) {
    useval = -theval;
    ErrMsg(warning) << "Error : ComPackUFloatBroken given negative value..." << endmsg;
  } else if (theval==0.0) {
    packedval = _fracMask;
    return TAG_OK;
  }
  StatusCode retval(TAG_OK);

  if (useval>_maxVal) {
    useval = _maxVal; // clamp
    ErrMsg(warning) << "Error : Overflow - setting to maximum representable value" << endmsg;
    retval = TAG_VAL_ROUND_DOWN;
  } else if (useval<_minVal) {
    useval = _minVal; 
    retval = TAG_VAL_ROUND_UP;
  }
  
  int index;
  double fraction = frexp ( useval, &index );

  // fraction is in [.5,1), such that absval = fraction*pow(2,index) 
  // fraction can never be 1.0, so we use this to represent zero

  // Stored as:
  // EEEEEEEEFFFFFFF
  // E - small integer holding exponent
  // F - positive integer holding representation of fraction

  fraction -= 0.5; // Now in range 0.0 - 0.5
  fraction *= _fracRange;
  index += _expMax - _bias; // subtract bias, and map to a positive range
  d_ULong tempPackedVal = (index<<_fracBits);
  d_ULong tempPackedFrac = (unsigned long) fraction;
  tempPackedVal |= tempPackedFrac;
  packedval = tempPackedVal;
  return retval;
}

ComPackBase<double>::StatusCode
ComPackUFloatBroken::unpack(const d_ULong val, double & unpackedval ) const
{
  if ( val == _fracMask) {
    unpackedval = 0.0;
  } else {
    unpackedval = ldexp(getFractionalPart(val),getExponentialPart(val));
  }
  return TAG_OK;
}

double 
ComPackUFloatBroken::getFractionalPart ( d_ULong input ) const
  //
  // Takes as input the word containing the packed float
  // Returns the fraction 0.5-1.0
{
  return 0.5+(double(input&_fracMask)+0.5)/_fracRange;
  //                                   ^-- LW: added 0.5 -> unpack to the bin center
}

int
ComPackUFloatBroken::getExponentialPart ( d_ULong input ) const
  //
  // Takes as input the word containing the packed float
  // Returns the exponent with the bias taken into account
{
  return ((input&_expMask)>>_fracBits) + _bias - _expMax;
}


void
ComPackUFloatBroken::testMe (size_t numsteps, double & toterror)
{
  if ( 0 == numsteps) return;
  toterror = 0.;
  const double incstep = double (1.0) / double (numsteps);
  cout << "Range is : " << _valRange << " and incstep is : " << incstep << endl;
  cout << "** Sweaping up from -(minimum value) :" << endl;
  double i = 0.0;
  for ( i = -_minVal; i<= _minVal*numsteps; i+=_minVal)
    {
      d_ULong tagVal;
      pack ( i, tagVal );
      double unTagVal;
      unpack ( tagVal, unTagVal );
      cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << unTagVal << endl;
      toterror += fabs ( i-unTagVal );
    }
  //  for ( i = -_minVal; i<= _minVal*numsteps; i+=_minVal)
  cout << "** Sweaping across +/- maximum value :" << endl;
  for ( i = -_maxVal; i<= _maxVal; i+=0.5*_maxVal)
      {
	  d_ULong tagVal;
	  pack ( i, tagVal );
	  double unTagVal;
	  unpack ( tagVal, unTagVal );
	  cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << unTagVal << endl;
	  toterror += fabs ( i-unTagVal );
      }
  cout << "** Sweaping - 1.0 - 1.0 :" << endl;
  for ( i = -1.0; i<= 1.0; i+=0.2)
      {
	  d_ULong tagVal;
	  pack ( i, tagVal );
	  double unTagVal;
	  unpack ( tagVal, unTagVal );
	  cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << unTagVal << endl;
	  toterror += fabs ( i-unTagVal );
      }
  d_ULong zerotagVal, onetagVal;
  pack ( 0.0, zerotagVal );
  pack ( 1.0, onetagVal);
  double unTagVal;
  unpack ( zerotagVal, unTagVal );
  cout << "Zero retrieved as " << unTagVal << endl;
  unpack ( onetagVal, unTagVal );
  cout << "One retreived as " << unTagVal << endl; 
  cout << "Range : " << _valRange << endl;
  cout << "Minimum value storable: " << _minVal << endl;
}


// --------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackFloat.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackFloat 
//      - allows one to construct limited precision "floats"
//        User may specify number of bits for storing mantissa and
//        exponent, subject to the restriction that the total number of 
//        bits is less than 31 (one bit is needed to store the sign, 
//        and a 32 bit type is what will be used in the tag DB).
//        In addition, specifying a mantissa of greater precision than found 
//        in a float is pointless, so the constructor detects this
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      D.E.Azzopardi               Originator. 
//                     Nov 1 1999   Activated test function, removed timing
//
// Copyright Information:
//      Copyright (C) 1999  DESVA Research & Design, for Hedgehog Concepts
//
// Additional information:
//      This class, and related classes, are in development right now...
//      so BEWARE!!!!
//   
// Quote:
//      "It's the cheese"
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
#include "CommonUtils/ComPackFloat.hh"
using std::cout;
using std::endl;
using std::hex;

ComPackFloat::ComPackFloat(const size_t fraclen, const size_t explen, const int bias)
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
  
  // 1 extra bit is used for the overall sign
  size_t bits = 1+ _fracBits + _expBits;
  if (bits>_maxlongbits) ErrMsg(fatal) << "Error : too large!" << endmsg;
  _bitRange = bits;
  _bitMask = (1<<_bitRange) - 1;

  _fracMask = (0x1<<_fracBits);
  _fracRange = 2.0 * (double) _fracMask;
  _fracMask --;   

  _expMax = 0x1<<_expBits;
  _expMask = _expMax - 1;
  _expMax>>=1;            //this is a red herring. Nothing to do with sign bit
  _expMask <<= _fracBits;
  _minVal = ldexp(0.5,-_expMax+_bias+1)/_fracRange;
  double maxFrac = 0.5 + (((double)_fracMask )/ (double) _fracRange);
  _maxVal = ldexp(maxFrac,_expMax+_bias-2);
  _valRange = _maxVal-_minVal;

}

ComPackFloat::~ComPackFloat(){}

ComPackBase<double>::StatusCode 
ComPackFloat::pack(const double theval,d_ULong & packedval) const
{
  double absVal;
  d_ULong negVal;
  if (theval < 0.0) {
    absVal = -theval;
    negVal = 0x1<<(_fracBits+_expBits);
  } else if (theval>0.0) {
    absVal = theval;
    negVal = 0x0;
  } else { // is zero!
    packedval = 0;
    return TAG_OK;
  }
  StatusCode retval(TAG_OK);

  if (absVal>_maxVal) {
    absVal = _maxVal; // clamp
    ErrMsg(warning) << "Error : Overflow - setting to maximum representable value" << endmsg;
    retval = TAG_VAL_ROUND_DOWN;
  } else if (absVal<_minVal) {
    absVal = _minVal; 
    retval = TAG_VAL_ROUND_UP;
  }
  
  int index;
  double fraction = frexp ( absVal, &index );
  // fraction is in [.5,1), such that absval = fraction*pow(2,index) 
  // fraction can never be 1.0, so we use this to represent zero
  // Stored as:
  // SEEEEEEEEFFFFFFF
  // S - sign bit 0 for positive, 1 for negative
  // E - small integer holding exponent
  // F - positive integer holding representation of fraction
  // E=0 has special treatment to bridge the gap nicely down to zero

  index += _expMax + 1 - _bias; // subtract bias, and map to a positive range
  if (index < 1) 	{
     index= 0;
     fraction= absVal/_minVal + 0.5;
  } else {
     fraction -= 0.5; // Now in range 0.0 - 0.5
     fraction *= _fracRange;
     fraction += 0.5; // Correct bin centre on WRITE
  }
  d_ULong tempPackedFrac = (unsigned long) fraction;
  if (tempPackedFrac > _fracMask) 	{
      tempPackedFrac= 0;
	  index++;
  }
  d_ULong tempPackedVal = (index<<_fracBits);
  tempPackedVal |= tempPackedFrac;
  tempPackedVal |= negVal;
  packedval = tempPackedVal;
  return retval;
}

ComPackBase<double>::StatusCode
ComPackFloat::unpack(const d_ULong val, double & unpackedval ) const
{
  if ( val == 0) {
    unpackedval = 0.0;
  } else if ( (val&_expMask) == 0) {
    unpackedval = (val&_fracMask)*_minVal ;
  } else {
    unpackedval = ldexp(getFractionalPart(val),getExponentialPart(val));
  }
  if ( val > (_fracMask+_expMask) ) unpackedval=-unpackedval;
 
  return TAG_OK;
}

double 
ComPackFloat::getFractionalPart ( d_ULong input ) const
  //
  // Takes as input the word containing the packed float
  // Returns the fraction 0.5-1.0
{
  return 0.5+(double(input&_fracMask))/_fracRange;
}

int
ComPackFloat::getExponentialPart ( d_ULong input ) const
  //
  // Takes as input the word containing the packed float
  // Returns the exponent with the bias taken into account
{
  return ((input&_expMask)>>_fracBits) + _bias - _expMax - 1;
}

void
ComPackFloat::testMe (size_t numsteps, double & toterror)
{
  if ( 0 == numsteps) return;
  toterror = 0.;
  double error= 0.;
  const double incstep = 2.*_minVal;
  cout << "Range is : " << _valRange << " and incstep is : " << incstep << endl;
  cout << "** Sweeping through zero :" << endl;
  double i = 0.0;
  for (i = -_minVal*(numsteps+1); i<= incstep*numsteps; i+=incstep)
    {
      d_ULong tagVal;
      pack ( i, tagVal );
      double unTagVal;
      unpack ( tagVal, unTagVal );
      cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << unTagVal << endl;
      error += fabs ( i-unTagVal );
    }
  cout << "Total error from this part: " << error << endl;
  toterror += error;

  cout << "** Sweeping across +/- maximum value :" << endl;
  error= 0.;
  for ( i = -_maxVal; i<= _maxVal; i+=0.03125*_maxVal)
      {
	  d_ULong tagVal;
	  pack ( i, tagVal );
	  double unTagVal;
	  unpack ( tagVal, unTagVal );
	  cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << unTagVal << endl;
	  error += fabs ( i-unTagVal );
      }
  cout << "Total error from this part: " << error << endl;
  toterror += error;

  cout << "** Sweeping - 1.0 - 1.0 :" << endl;
  error= 0.;
  for ( i = -1.0; i<= 1.0; i+=0.01)
      {
	  d_ULong tagVal;
	  pack ( i, tagVal );
	  double unTagVal;
	  unpack ( tagVal, unTagVal );
	  cout << i << " is converted to :" << tagVal << ". Upon unpacking :" << unTagVal << endl;
	  error += fabs ( i-unTagVal );
      }
  cout << "Total error from this part: " << error << endl;
  toterror += error;

  d_ULong zerotagVal, onetagVal;
  pack ( 0.0, zerotagVal );
  pack ( 1.0, onetagVal);
  double unTagVal;
  unpack ( zerotagVal, unTagVal );
  cout << "Zero retrieved as " << unTagVal << endl;
  unpack ( onetagVal, unTagVal );
  cout << "One retrieved as " << unTagVal << endl;
  cout << "Range : " << _valRange << endl;
  cout << "Minimum value storable: " << _minVal << endl;

  cout << "Test packed values near zero incrementally: " << endl;
  error= 0.;
  d_ULong tagVal = 0x1<<(_fracBits+_expBits);
  for ( d_ULong j = 0; j<=numsteps; j++,tagVal++ )  // with & without sign bit
    {
      double unTagVal;
      unpack ( j, unTagVal );
      cout << j << " unpacks as :" << unTagVal << endl;
      double unTagVal2;
      unpack ( tagVal, unTagVal2 );
      cout << hex << tagVal << " unpacks as :" << unTagVal2 << endl;
      error += fabs ( unTagVal+unTagVal2 );
    }
  cout << "Total error from this part: " << error << endl;
  toterror += error;
}


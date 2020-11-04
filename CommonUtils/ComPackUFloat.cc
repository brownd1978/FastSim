// --------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackUFloat.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackUFloat 
//      - allows one to construct limited precision "unsigned floats"
//        User may specify number of bits for storing mantissa and
//        exponent, subject to the restriction that the total number of 
//        bits is less than 32 
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
//      ""
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
#include "CommonUtils/ComPackUFloat.hh"
using std::cout;
using std::endl;

ComPackUFloat::ComPackUFloat(const size_t fraclen, const size_t explen, const int bias)
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
  _expMax>>=1;            //this is a red herring. Nothing to do with sign bit
  _expMask <<= _fracBits;
  _minVal = ldexp(0.5,-_expMax+_bias+1)/_fracRange;
  double maxFrac = 0.5 + (((double)_fracMask )/ (double) _fracRange);
  _maxVal = ldexp(maxFrac,_expMax+_bias-2);
  _valRange = _maxVal-_minVal;

}

ComPackUFloat::~ComPackUFloat(){}

ComPackBase<double>::StatusCode 
ComPackUFloat::pack(const double theval, d_ULong & packedval) const
{
  double useval = theval;
  if (theval < 0.0) {
    useval = -theval;
    ErrMsg(warning) << "Error : ComPackUFloat given negative value..." << endmsg;
  } else if (theval==0.0) {
    packedval = 0;
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
  // Stored as:
  // EEEEEEEEFFFFFFF
  // E - small integer holding exponent
  // F - positive integer holding representation of fraction
  // E=0 has special treatment to bridge the gap nicely down to zero

  index += _expMax + 1 - _bias; // subtract bias, and map to a positive range
  if (index < 1) 	{
     index= 0;
     fraction= useval/_minVal + 0.5;
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
  packedval = tempPackedVal;
  return retval;
}

ComPackBase<double>::StatusCode
ComPackUFloat::unpack(const d_ULong val, double & unpackedval ) const
{
  if ( val == 0) {
    unpackedval = 0.0;
  } else if ( val <= _fracMask) {
    unpackedval = val*_minVal ;
  } else {
    unpackedval = ldexp(getFractionalPart(val),getExponentialPart(val));
  }
  return TAG_OK;
}

double 
ComPackUFloat::getFractionalPart ( d_ULong input ) const
  //
  // Takes as input the word containing the packed float
  // Returns the fraction 0.5-1.0
{
  return 0.5+(double(input&_fracMask))/_fracRange;
}

int
ComPackUFloat::getExponentialPart ( d_ULong input ) const
  //
  // Takes as input the word containing the packed float
  // Returns the exponent with the bias taken into account
{
  return ((input&_expMask)>>_fracBits) + _bias - _expMax - 1;
}


void
ComPackUFloat::testMe (size_t numsteps, double & toterror)
{
  if ( 0 == numsteps) return;
  toterror = 0.;
  double error= 0.;
  const double incstep = 3.*_minVal;
  cout << "Range is : " << _valRange << " and incstep is : " << incstep << endl;
  cout << "** Sweeping up from -(minimum value) :" << endl;
  double i = 0.0;
  for ( i = -_minVal; i<= incstep*numsteps; i+=incstep)
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

  cout << "** Sweeping across zero - maximum value :" << endl;
  error= 0.;
  for ( i = -_minVal; i<= _maxVal; i+=0.03125*_maxVal)
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

  cout << "** Sweeping - zero - 1.0 :" << endl;
  error= 0.;
  for ( i = -_minVal; i<= 1.0; i+=0.01)
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
  for ( d_ULong j = 0; j<=numsteps; j++)
    {
      double unTagVal;
      unpack ( j, unTagVal );
      cout << j << " unpacks as :" << unTagVal << endl;
    }
}


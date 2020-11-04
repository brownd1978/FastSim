//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackFloat.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      ComPackFloat interface
//      - See class for more information
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
// Quote:
//      "Like, whatever. "
//
//--------------------------------------------------------------------------

#ifndef COMPACKFLOAT_HH
#define COMPACKFLOAT_HH

//
// Bass class includes
//
#include "CommonUtils/ComPackBase.hh"
#include "BaBar/BaBarODMGTypes.h"

class ComPackFloat:public ComPackBase<double> {

public:
  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackFloat(const size_t, const size_t, const int); // linearly packed float

  ///////////////////
  // Destructor:   //
  ///////////////////
  virtual ~ComPackFloat();

  ///////////////////
  // Packers:      //
  ///////////////////
  StatusCode pack (const double, d_ULong &) const;

  ///////////////////
  // Unpackers:    //
  ///////////////////
  StatusCode unpack (const d_ULong, double &) const;

  ///////////////////
  // Accessors:    //
  ///////////////////

  ///////////////////
  // Diagnostic:   //
  ///////////////////
  void testMe ( size_t, double & );

private:
  // Limits
  enum {
    FRACLENLOWERLIMIT=3,
    FRACLENUPPERLIMIT=23,
    EXPLENLOWERLIMIT=1,
    EXPLENUPPERLIMIT=8
  };
  
  double getFractionalPart ( d_ULong ) const;
  int getExponentialPart ( d_ULong ) const;
  size_t _fracBits; // number of bits devoted to the fractional part
  size_t _expBits; // number of bits devoted to the exponential part
  int _expMax; // Maximum positive value of exponent...
  int _bias;
  double _fracRange;
  d_ULong _fracMask;
  d_ULong _expMask;
};

#endif

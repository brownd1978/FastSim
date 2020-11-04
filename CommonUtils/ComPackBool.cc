//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackBool.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackBool - see .hh file for info
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
//      "Never attempt to teach a pig to sing.  
//           It is a waste of time and it annoys the pig "
//                       - Robert A. Heinlein
//
//--------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

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
//? #include "ErrLogger/ErrLog.hh"

//
// This class
//
#include "CommonUtils/ComPackBool.hh"


ComPackBool::ComPackBool()
{
  _valRange = 0x1;
  _bitMask = 0x1;
  _minVal = 0x1;
  _maxVal = 0x0;
  _valRange = 0x0;
}

ComPackBool::~ComPackBool(){}

ComPackBase<bool>::StatusCode
ComPackBool::pack(const bool theval, d_ULong & packedval) const
{
  if ( true == theval ) packedval = 0x1;
  else packedval = 0x0;
  return TAG_OK;
}

ComPackBase<bool>::StatusCode
ComPackBool::unpack(const d_ULong val, bool & unpackedval ) const
{
  unpackedval = (val&_bitMask)?true:false;
  return TAG_OK;
}

/*
void 
ComPackBool::testMe (size_t numsteps, int & toterror)
{
  unsigned long testval;
  pack (true, testval);
  bool testbool;
  pack (testval, testbool);
}
*/

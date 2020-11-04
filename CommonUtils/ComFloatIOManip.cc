//==========================================================================
// File and Version Information:
// 	$Id: ComFloatIOManip.cc 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	See ComFloatIOManip.hh
//
//--------------------------------------------------------------------------
// Sample User Code:
//
//--------------------------------------------------------------------------
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//--------------------------------------------------------------------------
// Author List:
//	Abi Soffer              (Original author)
//
//--------------------------------------------------------------------------
// Copyright Information:
//	Copyright (C) 1998	Colorado State University
//
//==========================================================================
#include "BaBar/BaBar.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>


//-----------------------
// This Class's Header --
//-----------------------

#include "CommonUtils/ComFloatIOManip.hh"
using std::ios;
using std::istream;
using std::ostream;
using std::resetiosflags;
using std::setiosflags;
using std::setprecision;
using std::setw;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//-----------------------------------------------
//-- Static Data & Function Member Definitions --
//-----------------------------------------------

//----------------------------------------
//-- Public Function Member Definitions --
//----------------------------------------

//----------------
// Constructors --
//----------------
ComFloatIOManip::ComFloatIOManip(int precision, int width, 
				 ComFloatIOManip::Format format) {
  setPrecision(precision);
  setWidth(width);
  setFormat(format);
}

//-------------------------------------------------------------
ComFloatIOManip::ComFloatIOManip(ostream & stream) {
  setPrecision(stream.precision());
  setWidth(stream.width());

  // Give preferance to scientific if both bits are set:
  if (stream.flags() & ios::scientific){
    setFormat(SCIENTIFIC);
  }
  else if (stream.flags() & ios::fixed){
    setFormat(FIXED);
  }
  else {
    setFormat(NEITHER);
  }    
}    

//-------------------------------------------------------------
ComFloatIOManip::ComFloatIOManip(istream & stream) {
  setPrecision(stream.precision());
  setWidth(stream.width());

  // Give preferance to scientific if both bits are set:
  if (stream.flags() & ios::scientific){
    setFormat(SCIENTIFIC);
  }
  else if (stream.flags() & ios::fixed){
    setFormat(FIXED);
  }
  else {
    setFormat(NEITHER);
  }    
}    

//--------------
// Destructor --
//--------------
ComFloatIOManip::~ComFloatIOManip() {}

//-------------
// Operators --
//-------------
ComIOManip * 
ComFloatIOManip::clone() const {
  return new ComFloatIOManip(*this);
}

//---------------------------------------------------------------
ostream & 
ComFloatIOManip::oManipulate(ostream & stream) const {
  if (_precision > IGNORE){
    stream << setprecision(_precision);
  }

  if (_width > IGNORE) {
    stream << setw(_width);
  }

  switch (_format) {
  case FIXED: 
    stream << resetiosflags(ios::scientific) << setiosflags(ios::fixed);
    break;
  case SCIENTIFIC:
    stream << resetiosflags(ios::fixed) << setiosflags(ios::scientific);
    break;
  case NEITHER:
    stream << resetiosflags(ios::scientific) << resetiosflags(ios::fixed);
    break;
  default:
    break;
  }

  return stream;
}
  
//---------------------------------------------------------------
istream & 
ComFloatIOManip::iManipulate(istream & stream) const {
  if (_precision > IGNORE){
    stream >> setprecision(_precision);
  }

  if (_width > IGNORE) {
    stream >> setw(_width);
  }

  switch (_format) {
  case FIXED: 
    stream >> resetiosflags(ios::scientific) >> setiosflags(ios::fixed);
    break;
  case SCIENTIFIC:
    stream >> resetiosflags(ios::fixed) >> setiosflags(ios::scientific);
    break;
  case NEITHER:
    stream >> resetiosflags(ios::scientific) >> resetiosflags(ios::fixed);
    break;
  default:
    break;
  }

  return stream;
}
  
//-----------
// Modifiers:
//-----------
ComFloatIOManip & 
ComFloatIOManip::setPrecision(int p) {
  _precision = p;
  return *this;
}

//---------------------------------------------------------------
ComFloatIOManip & 
ComFloatIOManip::setWidth(int w) {
  _width = w;
  return *this;
}

//---------------------------------------------------------------
ComFloatIOManip & 
ComFloatIOManip::setFormat(ComFloatIOManip::Format f) {
  _format = f;
  return *this;
}

//---------------------------------------------------------------
ComFloatIOManip &
ComFloatIOManip::setDefaults() {
  setPrecision(IGNORE);
  setWidth(IGNORE);
  setFormat(IGNORE);
  return *this;
}


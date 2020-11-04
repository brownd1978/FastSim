//==========================================================================
// File and Version Information:
// 	$Id: Com3State.cc 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	See Com3State.hh
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
#include <stdlib.h>


//-----------------------
// This Class's Header --
//-----------------------
#include "CommonUtils/Com3State.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/String/Strings.h"
#include "ErrLogger/ErrLog.hh"

using std::istream;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------------------------------
//-- Public Function Member Definitions --
//----------------------------------------

//----------------
// Constructors --
//----------------

Com3State::Com3State() {
  *this = Com3Neutral;
}  

//-------------------------------------------------------------
Com3State::Com3State(const Com3State & s) {
  *this = s;
}

//-------------------------------------------------------------
Com3State::Com3State(bool b, int ignored) {
  *this = b;
}
  
//-------------------------------------------------------------
Com3State::Com3State(Com3Value v) {
  _state = v;
}

//--------------
// Destructor --
//--------------
Com3State::~Com3State() {}

//------------------------
// Assignment Operators --
//------------------------
Com3State & 
Com3State::operator=(const Com3State & s) {
  _state = s._state;
  return *this;
}

//-------------------------------------------------------------
Com3State & 
Com3State::operator=(bool b) {
  if (true == b){
    *this = Com3True;
  }
  else {
    *this = Com3False;
  }
  return *this;
}

//-----------
// Accessors:
//-----------

ostream &
Com3State::print(ostream & stream) const {
  switch (_state){
  case Com3State::TRUE3:
    stream << "Com3True";
    break;
  case Com3State::FALSE3: 
    stream << "Com3False";
    break;
  case Com3State::NEUTRAL3: 
    stream << "Com3Neutral";
    break;
  }
  return stream;
}

//-----------
// Modifiers:
//-----------

istream & 
Com3State::read(istream & stream) {
  HepString value;
  stream >> value;

  // Do nothing if no input:
  if (true == value.isEmpty()){
    return stream;
  }

  const HepString lower = value.lower();

  if (lower == "com3true" || lower == "true" || lower == "t") {
    *this = Com3True;
  }
  else if (lower == "com3false" || lower == "false" || lower == "f") {
    *this = Com3False;
  }
  else if (lower == "com3neutral" || lower == "neutral" || lower == "n") {
    *this = Com3Neutral;
  }
  else {
    ErrMsg(fatal) << "ERROR: operator>>(istream & stream, Com3State & c):\n"
	          << "       Unrecognized input \"" << value << "\"." << endmsg;
  }
    
  return stream;
}


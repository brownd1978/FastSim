//==========================================================================
// File and Version Information:
// 	$Id: ComIOManip.cc 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	See ComIOManip.hh
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

#include "CommonUtils/ComIOManip.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrStream.hh"
using std::istream;
using std::ostream;

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

//--------------
// Destructor --
//--------------
ComIOManip::~ComIOManip() {}

//-------------
// Operators --
//-------------
ostream & operator<<(ostream & stream, const ComIOManip & manip) {
  return manip.oManipulate(stream);
}

//-----------------------------------------------------------------
istream & operator<<(istream & stream, const ComIOManip & manip) {
  return manip.iManipulate(stream);
}

//-----------------------------------------------------------------
ErrStream & operator<<(ErrStream & stream, const ComIOManip & manip) {
  manip.oManipulate(stream);
  return stream;
}

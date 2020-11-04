//==========================================================================
// File and Version Information:
// 	$Id: ComReadUtils.cc 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	See ComReadUtils.hh
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
#include <assert.h>


//-----------------------
// This Class's Header --
//-----------------------
#include "CommonUtils/ComReadUtils.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
#include "CLHEP/String/Strings.h"
using std::endl;
using std::istream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//-----------------------------------------------
//-- Static Data & Function Member Definitions --
//-----------------------------------------------

void 
ComReadUtils::readWord(istream & stream, 
		       const char * match, 
		       const char * caller) {
  HepString string;
  stream >> string;
  if (string != match){
    ErrMsg(fatal) 
      << "ERROR: " << caller << ": Read \"" << string << "\"" << endl
      << "       when expecting \"" << match << "\"" << endmsg;
    assert(0);
  }
}

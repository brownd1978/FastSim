//==========================================================================
// File and Version Information:
// 	$Id: ComIOManip.hh 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	Class ComIOManip is an abstract base class for smart iostream 
// manipulators, which enables the user to package several manipulators with
// their arguments in a class. It is used in BtaPrintTree to allow flexible 
// printing format, and might be used in BtaReadTree too.
//
//--------------------------------------------------------------------------
// Collaborating classes:
//
//--------------------------------------------------------------------------
// Sample User Code:
//
//--------------------------------------------------------------------------
// Compiler Default Member Functions:
//   Copy constructor and assignment operator
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

#ifndef ComIOManip_HH
#define ComIOManip_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//--------------------------------------------
// Collaborating Class Forward Declarations --
//--------------------------------------------
#include <iosfwd>
#include <iosfwd>
class ErrStream;

//		---------------------
// 		-- Class Interface --
//		---------------------

class ComIOManip {
public:
  // Constructors
  ComIOManip() {}

  // Destructor
  virtual ~ComIOManip();

  // Cloning:
  virtual ComIOManip * clone() const = 0;

  // The functions that actually manipulate the streams:
  virtual std::ostream & oManipulate(std::ostream & stream) const = 0;
  virtual std::istream & iManipulate(std::istream & stream) const = 0;
};

// globals:
// Applys iManipulate(stream):
std::istream & operator>>(std::istream & stream, const ComIOManip & manip);

// Apply oManipulate(stream):
ErrStream & operator<<(ErrStream & stream, const ComIOManip & manip);
std::ostream & operator<<(std::ostream & stream, const ComIOManip & manip);

#endif

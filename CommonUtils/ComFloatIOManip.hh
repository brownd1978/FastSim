//==========================================================================
// File and Version Information:
// 	$Id: ComFloatIOManip.hh 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	Class ComFloatIOManip is a simple ComIOManip which applies
// manipulators common in floating point format. These are the iomanip 
// functions 
// setprecision(precision):
//   sets the minimal number of significant digits printed,
// setw(width): 
//   sets the minimal number of characters printed,
// (re)setiosflags(ios::scientific):
//   determines whether to force use of scientific notation (1.2e01, etc.),
// (re)setiosflags(ios::fixed):
//   determines whether to force use of non-scientific notation
//
// The diferent settings are not totally independent. For example, width
// and precision can do slightly different things depending on whether 
// the fixed and/or scientific bits are set. Different operating systems
// may also behave differently, so some experimentation is always needed. 
// Also, having the fixed and scientific bits set at the same time leads to
// an undefined state, which some compilers don't handle gracefully. Therefore,
// The user of this class can set only one of these bits, or unset them both,
// using the function
//    setFormat(ComFloatIOManip::Format f);
//
// Here, the Format can be one of
//   IGNORE (do not set the fixed or scientific bits)
//   FIXED (set the fixed bit, unset the scientific bit)
//   SCIENTIFIC (set the scientific bit, set the fixed bit)
//   NEITHER (unset both the fixed and scientific bits)
//
// The precision and width are set using the functions
//   setPrecision(int);
//   setWidth(int);
//
// Setting the width or precision to IGNORE means that setw(..) and
// setprecision(..) are not called, leaving the stream settings as they are.
// The precision, width and format can also be specified in the constructor.
//
// The setting of a stream can be preserved in a ComFloatIOManip using the 
// constructors 
//   ComFloatIOManip(ostream & stream); 
//   ComFloatIOManip(istream & stream); 
//
// This is useful for restoring the stream settings to their original values 
// after manipulations.
//
// Manipulation of the stream is done using the base class operators
//   istream & operator>>(istream & stream, const ComIOManip & manip);
//   ostream & operator<<(ostream & stream, const ComIOManip & manip);
//   ErrStream & operator<<(ErrStream & stream, const ComIOManip & manip);
//
// These operators call the iManipulate(istream&) and oManipulate(ostream&) 
// functions.
//
// See also the man pages for manip, ostream, istream and ios.
//
//--------------------------------------------------------------------------
// Collaborating classes:
//
//--------------------------------------------------------------------------
// Sample User Code:
//      See testCommonUtils.cc
//
//--------------------------------------------------------------------------
// Compiler Default Member Functions:
//   copy constructor and assignment operator
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

#ifndef ComFloatIOManip_HH
#define ComFloatIOManip_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <iomanip>

//----------------------
// Base Class Headers --
//----------------------
#include "CommonUtils/ComIOManip.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//--------------------------------------------
// Collaborating Class Forward Declarations --
//--------------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class ComFloatIOManip : public ComIOManip {
  // ---------------
  // Static Members:
  //----------------
public:
  // if width or precision are set to IGNORE, the width or precision of 
  // the stream are not manipulated. If the format is set to IGNORE, it is 
  // not manipulated. Otherwise you can choose between FIXED format, SCIENTIFIC
  // format, or NEITHER, which sets both the fixed and scientific bits off:

  enum Format {IGNORE = -1, FIXED = 0, SCIENTIFIC = 1, NEITHER = 2};

  //------------------
  // Instance Members:
  //------------------
public:
  // Constructor: By default, arguments are set to IGNORE, 
  // so no stream manipulation is done:
  ComFloatIOManip(int precision = IGNORE, int width = IGNORE,
		  Format f = IGNORE);

  // These constructors take the current parameters of the stream, which
  // is useful for restoring the original settings:
  ComFloatIOManip(std::ostream & stream); 
  ComFloatIOManip(std::istream & stream); 

  // Destructor
  virtual ~ComFloatIOManip();

  // Cloning:
  virtual ComIOManip * clone() const;

  // The functions that actually manipulate the streams:
  virtual std::ostream & oManipulate(std::ostream & stream) const;
  virtual std::istream & iManipulate(std::istream & stream) const;

  // Accessors:
  int precision() const {return _precision;}
  int width() const {return _width;}
  Format format() const {return _format;}

  // Modifiers: All return *this, so that their insertion in the stream 
  // implies an immediate operator<< or operator>>:
  ComFloatIOManip & setPrecision(int p);
  ComFloatIOManip & setWidth(int w);
  ComFloatIOManip & setFormat(Format f);

  ComFloatIOManip & setDefaults();   // No stream manipulation will occur


private:
  // Data members
  int _precision;
  int _width;
  Format _format;
};

// Inline implementations
/// #include "ComFloatIOManip.icc"

#endif

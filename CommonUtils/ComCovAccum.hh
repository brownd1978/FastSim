//==========================================================================
// File and Version Information:
// 	$Id: ComCovAccum.hh 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	Class ComCovAccum accumulates means, covariances and correlation
// coefficients for a given number of variables.
//
//--------------------------------------------------------------------------
// Collaborating classes:
//
//--------------------------------------------------------------------------
// Sample User Code:
// Instantiate a ComCovAccum for 5 variables:
//
//   ComCovAccum accum(5);
//
// Accumulate events:
//
//   for (...){
//     HepVector vector(5);
//     vector(1) = var1;
//     vector(2) = var2;
//     vector(3) = var3;
//     vector(4) = var4;
//     vector(5) = var5;
//     accum.accumulate(vector);   (see additional accumulate functions below)
//   }
//
// Print out summary of means, covariance and correlation matrices:
//
//   accum.printSummary(cout);
//
//--------------------------------------------------------------------------
// Compiler Default Member Functions:
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
//	Copyright (C) 1999	Colorado State University
//
//==========================================================================

#ifndef ComCovAccum_HH
#define ComCovAccum_HH

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
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Alist/AList.h"
using CLHEP::HepSymMatrix;
using CLHEP::HepVector;

//--------------------------------------------
// Collaborating Class Forward Declarations --
//--------------------------------------------
#include <iosfwd>


//		---------------------
// 		-- Class Interface --
//		---------------------

class ComCovAccum{
public:
  // Constructors
  ComCovAccum(int numVariables = 1);

  // Destructor
  virtual ~ComCovAccum();
  
  // Accessors. 
  int numVariables() const {return _numVariables;}

  // # of accumulate(..) calls:
  int numEntries() const {return _numEntries;}

  // The accum... functions return the sum of products and variable
  // values over accumulate(..) calls:
  const HepSymMatrix & accumProducts() const {return _accumProducts;}
  const HepVector & accumValues() const {return _accumValues;}

  bool isModified() const {return _isModified;}

  // These accessors change the _covariance and _means, which are
  // mutable.  These variables are mutable that only const access is
  // required in order to information from the object.

  // Summarize the covariance. All following accessors call this function:
  void summarize() const;

  // The summary quantities:
  const HepVector & means() const;
  const HepSymMatrix & covariance() const;
  const HepSymMatrix & correlation() const;

  // Print out the covariance, means and correlations.
  std::ostream & printSummary(std::ostream & stream) const; 
  
  // Modifiers
  // Erase all accumulations and reset with a given number of variables:
  void reset(int numVariables);

  // Accumulate data using different forms of containers:
  void accumulate(double * values);
  void accumulate(const HepAList<double> & alist);
  void accumulate(const HepVector & entryr);
  
protected:
  // Helpers:
  void privateSummarize();  


private:
  // Data members
  int _numVariables;
  int _numEntries;
  bool _isModified;

  HepVector _entry;

  HepVector _accumValues;
  HepSymMatrix _accumProducts;

  /* mutable */ HepVector _means;
  /* mutable */ HepSymMatrix _covariance;
  /* mutable */ HepSymMatrix _correlation;

};

// Globals:
// Calls printSummary(stream) and also prints the _acumValues and
// _accumProducts
std::ostream & operator<<(std::ostream & stream, const ComCovAccum & cca);

// Inline implementations
/// #include "ComCovAccum.icc"

#endif

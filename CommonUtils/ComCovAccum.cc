//==========================================================================
// File and Version Information:
// 	$Id: ComCovAccum.cc 443 2010-01-14 12:24:42Z stroili $
//
//--------------------------------------------------------------------------
// Description:
//	See ComCovAccum.hh
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
//	Copyright (C) 1999	Colorado State University
//
//==========================================================================

//----------------
// BaBar Header --
//----------------
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
#include "CommonUtils/ComCovAccum.hh"
using std::ostream;

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
ComCovAccum::ComCovAccum(int numVariables) :
  _isModified(false) 
{	      
  reset(numVariables);
}

//--------------
// Destructor --
//--------------
ComCovAccum::~ComCovAccum() {}

//-------------
// Operators --
//-------------
    
//-------------
// Accessors --
//-------------
void 
ComCovAccum::summarize() const {
  // Cast to non-const in order to modify mutables:
  ((ComCovAccum&)(*this)).privateSummarize();
}

//--------------------------------------------------------------------
const HepVector &
ComCovAccum::means() const {
  summarize();
  return _means;
}

//--------------------------------------------------------------------
const HepSymMatrix & 
ComCovAccum::covariance() const {
  summarize();
  return _covariance;
}

//--------------------------------------------------------------------
const HepSymMatrix & 
ComCovAccum::correlation() const {
  summarize();
  return _correlation;
}

//--------------------------------------------------------------------
ostream & 
ComCovAccum::printSummary(ostream & stream) const {
  summarize();
  stream << "  numEntries = " << numEntries() << '\n'
	 << "  means = " << _means
	 << "  covariance = " << _covariance
	 << "  correlation = " << _correlation;
  
  return stream;
}
    
//-------------
// Modifiers --
//-------------
void 
ComCovAccum::reset(int numVariables) {
  _numVariables = numVariables;
  _numEntries = 0;

  _entry =         HepVector(numVariables, 0);
  _accumValues =   HepVector(numVariables, 0);
  _means =         HepVector(numVariables, 0);
  _accumProducts = HepSymMatrix(numVariables, 0);
  _covariance =    HepSymMatrix(numVariables, 0);
  _correlation =   HepSymMatrix(numVariables, 0);
}

//--------------------------------------------------------------------
void 
ComCovAccum::accumulate(double * values) {
  for (int row = 1; row <= numVariables(); ++row){
    _entry(row) = values[row - 1];
  }
  accumulate(_entry);
}

//--------------------------------------------------------------------
void 
ComCovAccum::accumulate(const HepAList<double> & alist) {
  for (int row = 1; row <= numVariables(); ++row){
    _entry(row) = *(alist[row - 1]);
  }
  accumulate(_entry);
}


//--------------------------------------------------------------------
void 
ComCovAccum::accumulate(const HepVector & entry) {
  _isModified = true;
  ++_numEntries;

  int row, col;
  for (row = 1; row <= numVariables(); ++row){
    _accumValues(row) += entry(row);
    for (col = row; col <= numVariables(); ++col){
      _accumProducts(row, col) += entry(row) * entry(col);

    }
  }
}

//-----------
// Globals --
//-----------
ostream & 
operator<<(ostream & stream, const ComCovAccum & cca) {
  // Print summary:
  cca.printSummary(stream);

  // Print additional information:
  stream << "  accumValues = " << cca.accumValues()
	 << "  accumProducts = " << cca.accumProducts();
  
  return stream;
}

//-------------------------------------------
//-- Protected Function Member Definitions --
//-------------------------------------------
void 
ComCovAccum::privateSummarize() {
  if (isModified() == true){
    _isModified = false;

    // Calculate means and normalize covariance:
    int row;
    int col;
    for (row = 1; row <= numVariables(); ++row){
      _means(row) = _accumValues(row) / numEntries();
      for (col = row; col <= numVariables(); ++col) {
	_covariance(row, col) =  _accumProducts(row, col) / numEntries();
      }
    }
      
    // Calculate covariance:
    for (row = 1; row <= numVariables(); ++row){
      for (col = row; col <= numVariables(); ++col) {
	_covariance(row, col) -= _means(row) * _means(col);
      }
    }

    // Calculate correlations:
    for (row = 1; row <= numVariables(); ++row){
      for (col = row; col <= numVariables(); ++col){
	const double denominator = 
	  sqrt(_covariance(row, row) * _covariance(col, col));

	_correlation(row, col) = 0.0;
	if (denominator > 0) {
	  _correlation(row, col) = _covariance(row, col) / denominator;
	}
      }
    }  
  }
}
      


//-----------------------------------------
//-- Private Function Member Definitions --
//-----------------------------------------

//-----------------------------------
//-- Internal Function Definitions --
//-----------------------------------

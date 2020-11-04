//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComExtendedArgValues.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComExtendedArgValues
//      This class keeps track of both individual values and range of values
//      that are specified with an extended syntax
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      A. De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "CommonUtils/ComExtendedArgValues.hh"
#include "ErrLogger/ErrLog.hh"

#include <algorithm>
#include <functional>
#include <iostream>
using std::ostream;
using namespace std::placeholders;

ComExtendedArgValues::ComExtendedArgValues() 
  : _hasUIntValues(false)
  , _hasStrValues(false)
{
}


ComExtendedArgValues::~ComExtendedArgValues() {
}


bool
ComExtendedArgValues::addRangeOfValues(const unsigned int lowerVal, 
				       const unsigned int upperVal){

  _hasUIntValues = true;
  // check that args are consistent
  if (lowerVal > upperVal) {
    ErrMsg(fatal) << "Range of values in extended collection syntax "
		  << lowerVal << " to " << upperVal 
		  << " is in reverse order ! " << endmsg;
    return false;
  }

  unsigned int tmpUpperVal = upperVal + 1;
  unsigned int tmpLowerVal = lowerVal - 1;

  // find the upper boundary that will contain the new lower value
  std::vector<unsigned int>::iterator positionHigh = 
    std::find_if(_upperUIntBoundary.begin(), _upperUIntBoundary.end(), 
		 std::bind(std::greater_equal<unsigned int>(),_1,tmpLowerVal));
  if (positionHigh == _upperUIntBoundary.end()) {
    // This is a new value not in the container
    _upperUIntBoundary.push_back(upperVal);
    _lowerUIntBoundary.push_back(lowerVal);
    return true;
  }

  int numPos =
  std::distance(_upperUIntBoundary.begin(), positionHigh);
  std::vector<unsigned int>::iterator positionLow = _lowerUIntBoundary.begin();
  std::advance(positionLow, numPos);
  // Corresponding lower boundary found.  Merge new range if it overlaps
  if ( ! ( tmpUpperVal < *positionLow || tmpLowerVal > *positionHigh ) ) {
    if (lowerVal < *positionLow) *positionLow = lowerVal;
    if (upperVal > *positionHigh) *positionHigh = upperVal;
    mergeUIntRanges();
  } 
  else {
    // This is a new value not in the container
    _upperUIntBoundary.insert(positionHigh,upperVal);
    _lowerUIntBoundary.insert(positionLow,lowerVal);
  }

  return true;
}


bool
ComExtendedArgValues::addIndividualValues(const unsigned int theValue) {
  return addRangeOfValues(theValue, theValue);
}


bool
ComExtendedArgValues::addRangeOfValues(const std::string& lowerVal, 
				       const std::string& upperVal){
  _hasStrValues = true;
  _upperStrBoundary.push_back(upperVal);
  _lowerStrBoundary.push_back(lowerVal);
  return true;
}


bool
ComExtendedArgValues::addIndividualValues(const std::string& theValue) {
  return addRangeOfValues(theValue, theValue);
}


bool
ComExtendedArgValues::findInContainers(const unsigned int theValue) const 
{
  std::vector<unsigned int>::const_iterator positionHigh = 
    std::find_if(_upperUIntBoundary.begin(), _upperUIntBoundary.end(), 
		 std::bind(std::greater_equal<unsigned int>(),_1,theValue));
  if (positionHigh == _upperUIntBoundary.end()) {
    // not found
    return false;
  }
  else if (*positionHigh == theValue) {
    // individual match on boundary
    return true;
  } 
  else {
    // is in the range ?
    int numPos =
    std::distance(_upperUIntBoundary.begin(),positionHigh);
    std::vector<unsigned int>::const_iterator positionLow = _lowerUIntBoundary.begin();
    std::advance(positionLow, numPos);
    if ( (theValue >= *positionLow) &&
	 (theValue <= *positionHigh) ) return true;
  }
  
  return false;

}


bool
ComExtendedArgValues::nextUpperBoundary(const unsigned int theValue, 
					unsigned int& nextUpperBoundary) const
{
  std::vector<unsigned int>::const_iterator positionHigh = 
    std::find_if(_upperUIntBoundary.begin(), _upperUIntBoundary.end(), 
		 std::bind(std::greater_equal<unsigned int>(),_1,theValue));
  if (positionHigh != _upperUIntBoundary.end()) {
    nextUpperBoundary = *positionHigh;
    return true;
  }
  return false;
}


bool
ComExtendedArgValues::nextLowerBoundary(const unsigned int theValue, 
					unsigned int& nextLowerBoundary) const
{
  std::vector<unsigned int>::const_iterator positionLow = 
    std::find_if(_lowerUIntBoundary.begin(), _lowerUIntBoundary.end(), 
		 std::bind(std::greater_equal<unsigned int>(),_1,theValue));
  if (positionLow != _lowerUIntBoundary.end()) {
    nextLowerBoundary = *positionLow;
    return true;
  }
  return false;
}


void
ComExtendedArgValues::print(ostream& theStream) const {
  int containerSize = _upperUIntBoundary.size();
  int ctr(0);

  while (ctr < containerSize) {
    if (ctr != 0) {
      theStream << ",";
    } 
    unsigned int upper = _upperUIntBoundary[ctr];
    unsigned int lower = _lowerUIntBoundary[ctr];
    if (upper == lower) {
      theStream << lower;
    }
    else {
      theStream << lower <<  "-" << upper;
    }
    ctr++;
  }

  containerSize = _upperStrBoundary.size();
  if ((containerSize > 0) && hasUIntValues()) theStream << ",";
  ctr = 0;
  while (ctr < containerSize) {
    if (ctr != 0) {
      theStream << ",";
    } 
    const std::string& upper = _upperStrBoundary[ctr];
    const std::string& lower = _lowerStrBoundary[ctr];
    if (upper == lower) {
      theStream << lower;
    }
    else {
      theStream << lower <<  "-" << upper;
    }
    ctr++;
  }

}

void
ComExtendedArgValues::mergeUIntRanges() 
{
  int containerSize(_upperUIntBoundary.size());
  int ctr(0);

  while (ctr < containerSize-1) {
    unsigned int high = _upperUIntBoundary[ctr];
    unsigned int nextLow = _lowerUIntBoundary[ctr+1];
    if (nextLow <= high+1) {
      unsigned int nextHigh = _upperUIntBoundary[ctr+1];
      if (nextHigh > high) _upperUIntBoundary[ctr] = nextHigh;
      std::vector<unsigned int>::iterator iter;
      iter = _upperUIntBoundary.begin();
      std::advance(iter, ctr+1);
      _upperUIntBoundary.erase(iter);
      iter = _lowerUIntBoundary.begin();
      std::advance(iter, ctr+1);
      _lowerUIntBoundary.erase(iter);
      containerSize = containerSize - 1;
    }
    else {
      ctr++;
    }
  }

}







//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComExtendedArgValues.hh 443 2010-01-14 12:24:42Z stroili $
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

#ifndef COMEXTENDEDARGVALUES_HH
#define COMEXTENDEDARGVALUES_HH

#include <vector>
#include <string>

#include <iosfwd>

class ComExtendedArgValues {

public:

  ComExtendedArgValues();
  ~ComExtendedArgValues();

  // Note: individual values are those specified as 
  // ?key=value1, value2, ...
  // whereas range of values are those specified in the extended syntax as
  // ?key=value1-value2, ...

  // Store the values in the containers.
  // Some checks are done for uniqueness and to merge overlapping ranges.
  bool addRangeOfValues(const unsigned int lowerVal, 
			const unsigned int upperVal);
  bool addIndividualValues(const unsigned int theValue);

  // Store string values in the containers.
  // No checks are done and the list of boundary values are accessible 
  // for clients to determine what ranges mean and to do their own searches.
  bool addRangeOfValues(const std::string& lowerVal, 
			const std::string& upperVal);
  bool addIndividualValues(const std::string& theValue);

  // Look for the value in the containers.
  // Returns true if it is found; false otherwise.
  bool findInContainers(const unsigned int theValue) const;

  // Look for the nextUpperBoundary and returns true if found.
  // if the value is on the boundary, it returns true.
  bool nextUpperBoundary(const unsigned int theValue, 
			 unsigned int& nextUpperBoundary) const;

  // Look for the nextLowerBoundary and true if found
  // if the value is on the boundary, it returns true.
  bool nextLowerBoundary(const unsigned int theValue, 
			 unsigned int& nextLowerBoundary) const;

  // Print for diagnostics
  void print(std::ostream& theStream) const;

  // Get list of boundary values
  const std::vector<std::string>& getUpperStrBoundaries() const {
    return _upperStrBoundary;
  }
  const std::vector<std::string>& getLowerStrBoundaries() const {
    return _lowerStrBoundary;
  }

  // What type of values are stored here ?
  bool hasUIntValues() const {return _hasUIntValues;}
  bool hasStrValues() const {return _hasStrValues;}

private:

  // Not implemented.
  ComExtendedArgValues( const ComExtendedArgValues& );
  ComExtendedArgValues& operator=( const ComExtendedArgValues& );

  // Merge any overlapping ranges in the containers.   
  void mergeUIntRanges();

  // These contain the upper/lower boundaries of the range pairs or
  // individual values (in which case, it upper = lower boundary).
  // These will be in ascending order and contain no duplicates/overlaps.
  std::vector<unsigned int> _upperUIntBoundary;
  std::vector<unsigned int> _lowerUIntBoundary;

  // These are not sorted or checked for overlaps. 
  std::vector<std::string> _upperStrBoundary;
  std::vector<std::string> _lowerStrBoundary;

  bool _hasUIntValues;
  bool _hasStrValues;

};

#endif

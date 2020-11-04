//--------------------------------------------------------------------------
// File and Version Information:
//
// $Id: RegExp.cc 494 2010-01-13 17:08:01Z stroili $
//
// Description:
//      The functions, within this namespace, are needed by BaBar to replace
//      those lost in the rw to STL string migration.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//       A. De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "BbrStdUtils/RegExp.hh"

#include <string>

#include "boost/cregex.hpp"

std::string 
babar::String::regExSearch(const std::string& str, boost::RegEx& re) {
  bool retVal = re.Search(str.c_str(), boost::match_default);
  return ( (retVal) ? re.What(0) : "" );
}

std::string
babar::String::regExSearch(const std::string& str, boost::RegEx& re, 
			  size_t pos) {
  bool retVal = re.Search( str.c_str()+pos, boost::match_default ) ;
  return ( (retVal) ? re.What(0) : "" );
}


size_t 
babar::String::regExIndex(const std::string& str, boost::RegEx& re) {
  bool retVal = re.Search(str.c_str(), boost::match_default);
  return ( (retVal) ? re.Position(0) : boost::RegEx::npos);
}

size_t
babar::String::regExIndex(const std::string& str, boost::RegEx& re,
                          size_t pos) {
  bool found = re.Search( str.c_str()+pos, boost::match_default ) ;
  if ( found ) {
    size_t index = re.Position(0);
    return ( index != boost::RegEx::npos ? index+pos : index );
  }
  return boost::RegEx::npos;
}



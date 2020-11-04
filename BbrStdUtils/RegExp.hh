//--------------------------------------------------------------------------
// File and Version Information:
//
// $Id: RegExp.hh 494 2010-01-13 17:08:01Z stroili $
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

#ifndef BABAR_REGEXP_HH
#define BABAR_REGEXP_HH

#include <string>
#include "boost/cregex.hpp"

namespace babar {
  namespace String {
    
    // Regular Expression 

    // will return null string if not found
    std::string regExSearch(const std::string& str, boost::RegEx& re);
    std::string regExSearch(const std::string& str, boost::RegEx& re, 
			    size_t pos);

    // will return boost::RegEx::npos if not found
    size_t regExIndex(const std::string& str, boost::RegEx& re);
    size_t regExIndex(const std::string& str, boost::RegEx& re, size_t pos);

  }
}

#endif



//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: FjrAbsJobReport.cc 506 2010-01-14 15:16:53Z stroili $
//
// Description:
//      Class FjrAbsJobReport
//      Abstract class for the Framework job reporting classes.
//      There can only be one instance of this in Framework hence this class
//      is a singleton.
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

#include "FrameJobReport/FjrAbsJobReport.hh"

#include "BbrStdUtils/String.hh"
#include "BbrStdUtils/RegExp.hh" 
#include "boost/cregex.hpp"
#include "ErrLogger/ErrLog.hh"

#include <algorithm>

using namespace babar::String;

FjrAbsJobReport::FjrAbsJobReport() 
  : _enabled(false)
{
}

FjrAbsJobReport::~FjrAbsJobReport()
{
}

bool 
FjrAbsJobReport::validateHierarchy(const std::string& hierarchy) {
  bool status = true;

  // allow only expressions such as 'word' or 'word1::word2::...::wordN'
  // For some reason, \n are passing the filter so we do 2 searches 
  static boost::RegEx expression ("^[a-zA-Z0-9_]+(:{2}[a-zA-Z0-9_]+)*$");
  static boost::RegEx excludeCh("\n");
  if ( (regExIndex(hierarchy,expression) == boost::RegEx::npos) 
       || (regExIndex(hierarchy,excludeCh) != boost::RegEx::npos) ) {
    ErrMsg(error) 
      << "Job Report hierarchy syntax is incorrect : "
      << "'" << hierarchy << "'" 
      << endmsg;
    status = false;
    return status;
  }

  // Check if this hierarchy already exists and remember if it is new
  std::set<std::string>::iterator found = _hierarchyNames.find(hierarchy);
  if (found == _hierarchyNames.end() ) {
    // If there is more than one level in the hierarchy (seperated by '::')
    // the higher levels must already be defined.  
    int posNewHierarchy = hierarchy.rfind("::");
    if (posNewHierarchy != std::string::npos) {
      std::string upHierarchy = hierarchy.substr(0,posNewHierarchy);    
      found = _hierarchyNames.find(upHierarchy);
      if (found == _hierarchyNames.end() ) {
	// upper level hierarchy does not exist !
	ErrMsg(error) 
	  << "Job Report upper level hierarchy undefined : "
	  << "'" << upHierarchy << "'" 
	  << endmsg;
	status = false;
      } 
    }          
    if (status) _hierarchyNames.insert(hierarchy);
  }

  return status;
}

bool 
FjrAbsJobReport::validateValueMeaning(const std::string& valueMeaning) {
  bool status = true;

  // Only normal characters (a-z and numeric) and underscore word allowed
  // For some reason, \n are passing the filter so we do 2 searches 
  static boost::RegEx excludeCh("\n");
  static boost::RegEx expression("^[a-zA-Z0-9_]+$");
  if ( (regExIndex(valueMeaning,expression) == boost::RegEx::npos) 
       || (regExIndex(valueMeaning,excludeCh) != boost::RegEx::npos) ){
    ErrMsg(error) 
      << "Job Report valueMeaning syntax is incorrect : "
      << "'" << valueMeaning << "'" 
      << endmsg;
    status = false;
  }

  return status;
}

bool 
FjrAbsJobReport::validateValue(const std::string& value) {
  bool status = true;

  // anything is fine here except for newline
  boost::RegEx excludeCh("\n");
  if (regExIndex(value,excludeCh) != boost::RegEx::npos) {
    ErrMsg(error) 
      << "Job Report value syntax is incorrect : "
      << "'" << value << "'" 
      << endmsg;
    status = false;
  }

  return status;
}

const char*
FjrAbsJobReport::getJobReportOwnerStr(JobReportOwner who) const {
  
  switch (who) {
  case FjrAbsJobReport::Bbr :
    return "Bbr";
    break;
  case FjrAbsJobReport::Usr :
    return "Usr";
    break;
  default :
    break;
  }
  return "Unknown";

}

bool
FjrAbsJobReport::existsHierarchy(const std::string& hierarchy) const 
{
  std::set<std::string>::const_iterator found
    = _hierarchyNames.find(hierarchy);
  return ( found != _hierarchyNames.end() );
};

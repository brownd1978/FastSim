//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: FjrJobReportFile.cc 506 2010-01-14 15:16:53Z stroili $
//
// Description:
//      Class FjrJobReportFile
//      Job summary report that writes to a file.
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

#include "FrameJobReport/FjrJobReportFile.hh"

#include "ErrLogger/ErrLog.hh"

#include <string>
#include <fstream>
#include <iostream>
using std::endl;
using std::fstream;
using std::ios;
using std::ofstream;

FjrJobReportFile::FjrJobReportFile() 
  : FjrAbsJobReport()
  , _outputStream(0)
{
}

FjrJobReportFile::~FjrJobReportFile() {
  if ( _outputStream != 0) close();
}
  
bool
FjrJobReportFile::open(const std::string& fileName) {
  bool status = true;
  if (_outputStream == 0) {
    _outputStream = new ofstream(fileName.c_str(), ios::out);
    if (! _outputStream->good() ) {
      ErrMsg(error)
      	<< "Job Report failed to open summary file " 
	<< fileName 
	<< endmsg;
      status = false;
    } else {
      setEnabled(true);
    }
  }
  else {
    ErrMsg(error)
      << "Job Report ignoring open command; file already opened"
      << endmsg;
    status = false;
  }
  return status;
}

bool
FjrJobReportFile::close() {
  if (_outputStream != 0) {
    _outputStream->close();
    delete _outputStream;
    _outputStream = 0;
  }
  return true;
}

bool
FjrJobReportFile::report(JobReportOwner who,
		      const std::string& hierarchy,
		      const std::string& valueMeaning,
		      const std::string& value) {  
  bool status = true;

  if (isEnabled()) { 

    if (validateHierarchy(hierarchy) && validateValue(value) 
	&& validateValueMeaning(valueMeaning) ) {
      *_outputStream << getJobReportOwnerStr(who) << "::" << hierarchy << "."
		     << valueMeaning << " " << value << endl;
    }
    else {
      ErrMsg(error)
	<< "Job Report failed for " 
	<< getJobReportOwnerStr(who) << "::" 
	<< hierarchy << "."
	<< valueMeaning << " " 
	<< value 
	<< endmsg;
      status = false;
    }
  }

  return status;
}



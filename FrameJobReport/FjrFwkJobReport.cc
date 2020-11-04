//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: FjrFwkJobReport.cc 506 2010-01-14 15:16:53Z stroili $
//
// Description:
//      Class FjrFwkJobReport
//      This is a singleton and a wrapper for the Job Report class 
//      in Framework.      
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

#include "FrameJobReport/FjrFwkJobReport.hh"
#include "FrameJobReport/FjrJobReportFile.hh"
#include "ErrLogger/ErrLog.hh"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

FjrFwkJobReport* FjrFwkJobReport::_theFwkJobReport = 0;

FjrFwkJobReport::FjrFwkJobReport() 
  : _theJobReport(new FjrJobReportFile())
  // starttime is cached so that when the file is opened, 
  // this will be written out first
  , _startTime(new timespec)
  , _theFileName(0)
{
  (void) clock_gettime( CLOCK_REALTIME, _startTime );
}

FjrFwkJobReport::~FjrFwkJobReport() {
  close();
  delete _theJobReport;
  delete _startTime;
  if (_theFileName != 0) delete _theFileName;
}

FjrFwkJobReport* 
FjrFwkJobReport::getFwkJobReport() {
  if (_theFwkJobReport == 0) {
    _theFwkJobReport = new FjrFwkJobReport();
  }
  return _theFwkJobReport;
}
  
bool 
FjrFwkJobReport::open() {
  bool status = false;
  if ( _theJobReport != 0) {

    const int maxlen(255);

    status = _theJobReport->open(*_theFileName);
    if (! status) {
      ErrMsg(fatal) << "Job Report file " << *_theFileName << "failed to open."
		    << endmsg;
    }
    // record start time    
    tm* gmtTime = gmtime(&_startTime->tv_sec); 
    char theTime[maxlen];
    int nStr = snprintf(theTime,sizeof(theTime), 
		"%04d%02d%02d %02d:%02d:%02d UTC",
		gmtTime->tm_year+1900, gmtTime->tm_mon+1, gmtTime->tm_mday,
		gmtTime->tm_hour, gmtTime->tm_min, gmtTime->tm_sec);
    assert(nStr > 0 && nStr < sizeof(theTime) );
    _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "startTime",
                          theTime);
    std::ostringstream gmtSecTime;
    gmtSecTime << _startTime->tv_sec;
    _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "startTimeGMTsec",
                          gmtSecTime.str());

    // record site
    char* theSite = getenv("BFSITE");
    if (theSite != 0) {
      _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "bfsite", 
			    theSite);
    }
    else {
      _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "bfsite", 
			    "unknown");
    }

    // record hostname
    char theHost[maxlen];
    gethostname(theHost, maxlen);
    _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "host", theHost);
    
    // record pid
    std::ostringstream thePid;
    thePid << getpid();
    _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "pid", 
			  thePid.str());


    status = true;
  }
  return status;
}

bool
FjrFwkJobReport::close() {
  bool status = false;
  if ( _theJobReport != 0) {
    // record stop time
    timespec stopTime;
    (void)  clock_gettime( CLOCK_REALTIME, &stopTime );
    std::ostringstream gmtSecTime;
    gmtSecTime << stopTime.tv_sec;
    _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "stopTimeGMTsec",
                          gmtSecTime.str() );
    tm* gmtTime = gmtime(&stopTime.tv_sec); 
    char theTime[255];
    int nStr = snprintf(theTime,sizeof(theTime),
	    "%04d%02d%02d %02d:%02d:%02d UTC",
	    gmtTime->tm_year+1900, gmtTime->tm_mon+1, gmtTime->tm_mday,
	    gmtTime->tm_hour, gmtTime->tm_min, gmtTime->tm_sec);
    assert(nStr > 0 && nStr < sizeof(theTime) );
    _theJobReport->report(FjrAbsJobReport::Bbr, "JobReport", "stopTime",
                          theTime);
    _theJobReport->close();
  }  
  return status;
}

bool
FjrFwkJobReport::setFileName(const std::string& fileName) {
  bool status = false;
  // check to see if file is already opened; if yes, don't change anything.
  if (! isEnabled() ) {
    if (_theFileName == 0) {
    _theFileName = new std::string(fileName);
    } 
    else {
      (*_theFileName) = fileName;
    }
    status = true;
  }
  return status;
}

bool 
FjrFwkJobReport::report(FjrAbsJobReport::JobReportOwner who,
		     const std::string& hierarchy,
		     const std::string& valueMeaning,
		     const std::string& value) {  
  // check if we are writing, if not, open the file if filename has been set.
  if ((! isEnabled()) && (_theFileName != 0))  {
    open();
  }  
  return (_theJobReport != 0) ?
    _theJobReport->report(who, hierarchy, valueMeaning, value) : false;
}

bool 
FjrFwkJobReport::isEnabled() {
  // check if we are writing, if not, open the file if filename has been set.
  if ((! _theJobReport->isEnabled()) && (_theFileName != 0)) {
    open();
  }
  return (_theJobReport != 0) ? _theJobReport->isEnabled() : false;
}







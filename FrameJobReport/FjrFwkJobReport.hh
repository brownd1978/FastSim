//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: FjrFwkJobReport.hh 506 2010-01-14 15:16:53Z stroili $
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

#ifndef FJRFWKJOBREPORT_HH
#define FJRFWKJOBREPORT_HH

#include "FrameJobReport/FjrAbsJobReport.hh"
#include <string>

class Time;
struct timespec;

class FjrFwkJobReport {

public:

  virtual ~FjrFwkJobReport();

  // get the job report object
  static FjrFwkJobReport* getFwkJobReport();
  
  // report the information to the opened file
  // This will be output as 
  //      Bbr::hierarchy.valueMeaning value 
  //       or
  //      Usr::hierarchy.valueMeaning value
  // where 
  //   hierarchy can have many levels delimited by '::',
  //     as for example, level1::level2::...::levelN;
  //     where levelX are words consisting of [a-zA-Z0-9_]
  //   valueMeaning is a word made from [a-zA-Z0-9_]
  //   value has no restrictions except for no \n
  //   who is either AbsJobReport::Bbr or AbsJobReport::Usr
  //
  bool report(FjrAbsJobReport::JobReportOwner who, 
	      const std::string& hierarchy, 
	      const std::string& valueMeaning, 
	      const std::string& value);

  // Check if the output file is available for writing
  bool isEnabled();

  // Check if a hierarchy exists
  bool existsHierarchy(const std::string& hierarchy) const {
    return (_theJobReport != 0) ? 
      _theJobReport->existsHierarchy(hierarchy) : 
      false;
  };

  // set file name for output
  bool setFileName(const std::string& fileName);
  const std::string* getFileName() {return _theFileName;}

protected:
  
  FjrFwkJobReport();

  // open and close files 
  bool open();
  bool close();

private:

  FjrAbsJobReport* _theJobReport;

  static FjrFwkJobReport* _theFwkJobReport;

  // Not implemented.
  FjrFwkJobReport( const FjrFwkJobReport& );
  FjrFwkJobReport& operator=( const FjrFwkJobReport& );

  // cache necessary information until such time as the file is opened
  // start date and time
  timespec* _startTime;

  std::string* _theFileName;

};

#endif


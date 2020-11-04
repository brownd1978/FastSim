//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: FjrJobReportFile.hh 506 2010-01-14 15:16:53Z stroili $
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

#ifndef FJRJOBREPORTFILE_HH
#define FJRJOBREPORTFILE_HH

#include "FrameJobReport/FjrAbsJobReport.hh"
#include <string>
#include <iosfwd>

class FjrJobReportFile : public FjrAbsJobReport {

public:
  FjrJobReportFile();
  virtual ~FjrJobReportFile();
  
  virtual bool open(const std::string& fileName);
  virtual bool close();

  virtual bool report(JobReportOwner who,
		      const std::string& hierarchy,
		      const std::string& valueMeaning,
		      const std::string& value);

private:

  // Not implemented.
  FjrJobReportFile( const FjrJobReportFile& );
  FjrJobReportFile& operator=( const FjrJobReportFile& );

  std::ofstream* _outputStream;

};

#endif

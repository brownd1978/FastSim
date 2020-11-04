//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: FjrAbsJobReport.hh 506 2010-01-14 15:16:53Z stroili $
//
// Description:
//      Class FjrAbsJobReport
//      Abstract class for the Framework job reporting classes.
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

#ifndef FJRABSJOBREPORT_HH
#define FJRABSJOBREPORT_HH

#include <string>
#include <set>

class FjrAbsJobReport {

public:

  enum JobReportOwner {Bbr, Usr};

  FjrAbsJobReport();
  virtual ~FjrAbsJobReport();

  // open and close files
  virtual bool open(const std::string& fileName) = 0;
  virtual bool close() = 0;

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
  //
  virtual bool report(JobReportOwner who, 
		      const std::string& hierarchy, 
		      const std::string& valueMeaning, 
		      const std::string& value) = 0;

  // Check if the output file is available for writing
  bool isEnabled() {return _enabled;}

  // Check if a hierarchy exists
  bool existsHierarchy(const std::string& hierarchy) const;
  
protected:

  // Validate the arguments that they do not contain forbidden characters etc.
  virtual bool validateHierarchy(const std::string& hierarchy);
  virtual bool validateValueMeaning(const std::string& valueMeaning);
  virtual bool validateValue(const std::string& value);

  std::set<std::string> _hierarchyNames;

  void setEnabled(bool flag) {_enabled = flag;}

  // convert the enumerated values to string
  const char* getJobReportOwnerStr(JobReportOwner who) const;

private:

  // enabled to wrtie to the output file
  bool _enabled;

  // Not implemented.
  FjrAbsJobReport( const FjrAbsJobReport& );
  FjrAbsJobReport& operator=( const FjrAbsJobReport& );

};

#endif

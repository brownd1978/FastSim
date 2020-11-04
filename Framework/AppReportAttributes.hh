//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppReportAttributes.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      AppReportAttributes
//        Specifications on how/what to list for modules/sequences/paths 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      A. De Silva
//       
//
// Copyright Information:
//
//------------------------------------------------------------------------

#ifndef APPREPORTATTRIBUTES_HH
#define APPREPORTATTRIBUTES_HH

class AppExecutable;
#include <vector>
#include <string>
#include <set>

class AppReportAttributes {

public:

  AppReportAttributes();
  virtual ~AppReportAttributes();

  // To decide whether descriptions should be appended 
  void setBrief(bool brief) {_brief = brief;}
  bool listBrief() const {return _brief;}

  // To decide if output should be to tcl
  void setTclOutput(const char* tclVarName) {_tclVarName = tclVarName;}
  bool listTclOutput() const {return _tclVarName.size() != 0;}
  const std::string& getTclVarName() const {return _tclVarName;}

  // How much indentation for listings
  virtual const std::string& getIndentation() const {return _indent;}

protected:

  std::string _indent;

private:

  bool _brief;
  std::string _tclVarName;

  // not implemented
  AppReportAttributes(const AppReportAttributes&);
  AppReportAttributes& operator=( const AppReportAttributes&);

};

#endif

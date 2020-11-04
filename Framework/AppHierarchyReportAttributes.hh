//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppHierarchyReportAttributes.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      AppHierarchyReportAttributes
//        Specifications on how/what to list for sequences/paths hiararchy
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

#ifndef APPHIERARCHYREPORTATTRIBUTES_HH
#define APPHIERARCHYREPORTATTRIBUTES_HH

#include "Framework/AppReportAttributes.hh"

class AppExecutable;
#include <vector>
#include <string>
#include <set>

class AppHierarchyReportAttributes : public AppReportAttributes {

public:

  AppHierarchyReportAttributes();
  virtual ~AppHierarchyReportAttributes();

  // To decide whether to print the next level down in the hierarchy
  bool listThisLevel() const {return _currentLevel <= _maxLevel;} 
  bool listNextLevel() const {return (_currentLevel+1) <= _maxLevel;}
  void increaseLevel(const AppExecutable* theExec);
  void decreaseLevel();
  void setLevel(int level) {_maxLevel = level;}
  const std::string& levelPath() const {return _levelPath;}

  // To decide if to compress or expand previously listed hierarchy
  void setExpand(bool expand) {_expand = expand;}
  bool listExpand(const AppExecutable* theExec) const;

private:

  bool _expand;
  int _maxLevel;
  int _currentLevel;

  std::set<const AppExecutable*> _listedSequences;
  std::vector<const AppExecutable*> _levelHistory;
  std::string _levelPath;

  void createLevelPath();
  void resetIndentation();

  // not implemented
  AppHierarchyReportAttributes(const AppHierarchyReportAttributes&);
  AppHierarchyReportAttributes& operator=( const AppHierarchyReportAttributes&);

};

#endif

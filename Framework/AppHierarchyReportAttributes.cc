//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppHierarchyReportAttributes.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      AppHierarchyReportAttributes
//        Specifications on how/what to list for sequences/paths hierarchy
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
#include "Experiment/Experiment.hh"

#include "Framework/AppHierarchyReportAttributes.hh"

#include "Framework/AppExecutable.hh"

AppHierarchyReportAttributes::AppHierarchyReportAttributes() 
  : _expand (false)
  , _maxLevel (9999999)
  , _currentLevel (0)
{
}

AppHierarchyReportAttributes::~AppHierarchyReportAttributes() {
}

void 
AppHierarchyReportAttributes::increaseLevel(const AppExecutable* theExec) {
  _listedSequences.insert(theExec);
  _levelHistory.push_back(theExec);
  createLevelPath();
  ++_currentLevel;
  resetIndentation();
}
  
void 
AppHierarchyReportAttributes::decreaseLevel() {
  _levelHistory.pop_back();
  createLevelPath();
  --_currentLevel;
  resetIndentation();
}

void
AppHierarchyReportAttributes::createLevelPath() {
  _levelPath = "";
  std::vector<const AppExecutable*>::const_iterator iter;
  for (iter = _levelHistory.begin(); iter != _levelHistory.end(); iter++) {
    _levelPath += "%";
    _levelPath += (*iter)->name();
  }
}

bool
AppHierarchyReportAttributes::listExpand(const AppExecutable* theExec) const {
  bool result = true;
  if ( ( ! _expand ) && 
       (_listedSequences.find(theExec) != _listedSequences.end() ) ) 
    result = false;
  return result;
}

void 
AppHierarchyReportAttributes::resetIndentation() {
  _indent = "    ";
  for ( int i = 0; i < _currentLevel; i++ ) {
    _indent += "    ";
  }
}

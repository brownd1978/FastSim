//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppReportAttributes.cc 509 2010-01-14 15:18:55Z stroili $
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
#include "Experiment/Experiment.hh"

#include "Framework/AppReportAttributes.hh"

#include "Framework/AppExecutable.hh"

AppReportAttributes::AppReportAttributes() 
  : _indent("    ")
  , _brief (false)
  , _tclVarName ("")
{
}

AppReportAttributes::~AppReportAttributes() {
}


//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppStopType.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppStopType Class Implementation
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List:
//	Marc Turcotte	                Original Author March '98
//      Marc Turcotte                   Removed const on getStopTypeKey 8/21/98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// Class Header
//

#include <sstream>
using std::ostringstream;
using std::string;

#include "Framework/AppStopType.hh"     

AppStopType::AppStopType(){ _stopType = never_assigned; }

AppStopType::AppStopType(StopType aStopType){ _stopType = aStopType; }

AppStopType::~AppStopType(){}

void 
AppStopType::setStopType(AppStopType::StopType aStopType) { 
  _stopType = aStopType; }

AppStopType::StopType 
AppStopType::getStopType(){ 
  return _stopType; }

string
AppStopType::getStopTypeKey() const {
  
  std::ostringstream myStream;
  myStream<<_stopType; // Get a character rendition of the Key
  return  myStream.str(); }

bool 
AppStopType::operator==(StopType x) const 
{ return _stopType==x; }

bool 
AppStopType::operator==(const AppStopType& r) const 
{ return r._stopType==_stopType; }

bool 
AppStopType::operator!=(StopType x) const 
{ return !operator==(x); }
  
bool AppStopType::operator!=(const AppStopType& r) const 
{ return !operator==(r); }

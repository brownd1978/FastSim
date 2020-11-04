//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppStop.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppStop Class Implementation
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List:
//	Marc Turcotte	                Original Author March '98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// Collaborating class headers
//
//#include "Framework/AppRecord.hh"
class AppRecord;
#include "Framework/AppStopType.hh"
#include "Framework/AppRecord.hh"
#include "Framework/AppStream.hh"

//
// Class Header
//

#include "Framework/AppStop.hh"   

AppStop::AppStop(AppRecord* aRecord, AppStopType* aStopType, AppStream* aStream){
  _theRecord   = aRecord;
  _theStopType = aStopType;
  _theStream   = aStream;
  
}

AppStop::~AppStop(){
  delete _theRecord;
  delete _theStopType;
  delete _theStream;  
}

AppRecord*
AppStop::record(){
  return _theRecord;
}

AppStopType*
AppStop::stopType(){
  return _theStopType;
}

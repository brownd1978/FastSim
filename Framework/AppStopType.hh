//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppStopType.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppStopType Class Definiton.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List: 
//	Marc Turcotte	                Original Author     March '98
//      Marc Turcotte                   Added support for
//                                      list of stopTypes   March '98
//      Marc Turcotte                   Removed const on getStopTypeKey
//                                      8/21/98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#ifndef APPSTOPTYPE_HH
#define APPSTOPTYPE_HH

#include <string>

//
// Class Interface
//
    
class AppStopType{
  
public:
    
  // 
  // Tentative BaBar stop types. The internal values are for
  // ease of debugging and are obviously arbitrary. The
  // syntax of the various enums cannot be changed unless 
  // the code is also changed. So don't mess with those
  // unless you know what you are doing. New members can be 
  // added at will though; that's perfectly ok.
  //
  // Marc Turcotte, 3/27/98
  //
  
  enum StopType {
    invalid            = 0,
    physics_event      = 100,
    begin_run          = 200,
    end_run            = 300,
    begin_job          = 400,
    end_job            = 500,
    begin_major        = 600,
    begin_minor        = 700,
    end_major          = 800,
    end_minor          = 900,
    level1_accept      = 1000,
    slow_calibration   = 1100,
    fast_calibration   = 1200,
    other_stop_type    = 8000,
    never_assigned     = 9000};
  
  
private:
  
  StopType _stopType;
  
public:
  
  AppStopType();
  AppStopType(StopType aStopType);

  AppStopType( const AppStopType& r) {
    _stopType = r._stopType;
  }
  AppStopType& operator=( const AppStopType& r) {
    _stopType = r._stopType;
    return *this;
  }

  virtual ~AppStopType();
  void setStopType(StopType aStopType);
  StopType getStopType();
  std::string getStopTypeKey() const; 
  bool operator==(StopType x) const;
  bool operator==(const AppStopType& r) const;
  bool operator!=(StopType x) const;
  bool operator!=(const AppStopType& r) const;
  
};

#endif

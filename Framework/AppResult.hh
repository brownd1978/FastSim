//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppResult.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//
//	AppStus Class Definiton.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory
//
//      This class is very lightweight and essentially consists of
//      a scoped enum returned on every user module standard member
//      functions. 
//
//      This software was developed in cooperation with CDF.
//
// Author List: 
//	Liz Sexton-Kennedy	        Original Author     June '98
//      Marc Turcotte                   Adopted in Framework June '98
//
// Copyright Information:
//      Copyright (C) 1998              FNAL
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPRESULT_HH
#define APPRESULT_HH

class AppResult {
  
public:
  
  enum Code{ OK, ERROR } ;
  
  AppResult():_value(OK) { }
  AppResult(Code x):_value(x) { }
  AppResult(const AppResult& r):_value(r._value) { }
  
  AppResult& operator=(Code x) { _value=x; return *this; }
  AppResult& operator=(const AppResult& r) { _value=r._value; return *this; }
  
  bool operator==(Code x) const { return _value==x; }
  bool operator==(const AppResult& r) const { return r._value==_value; }
  bool operator!=(Code x) const { return !operator==(x); }
  bool operator!=(const AppResult& r) const { return !operator==(r); }
  
  Code value() {return _value;}
  
private:
  
  Code _value;
  
};


#endif

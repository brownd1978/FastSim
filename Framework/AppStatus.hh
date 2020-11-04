//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppStatus.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//
//	AppStus Class Definiton.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This class is very lightweight and essentially consists of
//      a scoped enum returned on every user module standard member
//      functions. 
//
//      This software was developed in cooperation with CDF.
//
// Author List: 
//	Marc Turcotte	                Original Author     June '98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#ifndef APPSTATUS_HH
#define APPSTATUS_HH

class AppStatus {
public:  
  enum type {OK, ERROR};
};

#endif

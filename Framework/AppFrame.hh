//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFrame.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppFrame Class Definiton.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List: 
//	Marc Turcotte	                Original Author     March '98
//      Marc Turcotte                   Added dispatch method 4/8/98
//      Marc Turcotte                   Added stackStop method 4/24/98
//      Marc Turcotte                   Modified for AppStatus 6/19/98
//      Marc Turcotte                   Moved dispatch to AppModule 6/22/98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#ifndef APPFRAME_HH
#define APPFRAME_HH

//
// Collaborating class headers
//
class AppStop;
class AppModule;
template< class T > class APPList;
#include "Framework/AppStatus.hh"

//
// Class Definition and Interface
//
    
class AppFrame{
  
private:
  
  APPList<AppStop*>* _theStops;
  
public:
  
  AppFrame();
  virtual ~AppFrame();
  void appendStop(AppStop* aStop);
  void stackStop(AppStop* aStop, int stackSize = 1);
  APPList<AppStop*>* stops();

private:

  // Not implemented.
  AppFrame( const AppFrame& );
  AppFrame& operator=( const AppFrame& );
  
};

#endif

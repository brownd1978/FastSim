//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppStop.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppStop Class Definiton.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List: 
//	Marc Turcotte	                Original Author     March '98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#ifndef APPSTOP_HH
#define APPSTOP_HH

//
// Collaborating class headers
//
class AppRecord;
class AppStopType;
class AppStream;
class AppRecord;

//
// Class Interface
//
    
class AppStop{
    
  private:
  
    AppRecord*   _theRecord;
    AppStopType* _theStopType;
    AppStream*   _theStream;
    
  public:
     
    AppStop(AppRecord* aRecord, AppStopType* aStopType, AppStream* aStream);
    virtual ~AppStop();
    AppRecord* record();
    AppStopType* stopType();

private:
  // Not implemented.
  AppStop( const AppStop& );
  AppStop& operator=( const AppStop& );

          
};

#endif

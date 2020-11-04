//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppFileOutputModule.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppFileOutputModule declarations (header) file.
//
//      This is the standard file output module for the BaBar framework. 
//      It is designed as a replacement for the APPFileOutputModule class, 
//      inheriting from an abstract AppStreamsOutputModule class. 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	    David R. Quarrie		        Original Author
//
// Copyright Information:
//	    Copyright (C) 1999      	    Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

// Note case sensitive - otherwise clashes with the APPFileOutputModule
#ifndef AppFILEOUTPUTMODULE_HH
#define AppFILEOUTPUTMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppStreamsOutputModule.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppFileOutputModule : public AppStreamsOutputModule {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppFileOutputModule( );
  
  // Copy Constructor
  //    AppFileOutputModule( const AppFileOutputModule& );
  
  // Destructor
  virtual ~AppFileOutputModule( );

private:

  // Not implemented.
  AppFileOutputModule( const AppFileOutputModule& );
  AppFileOutputModule& operator=( const AppFileOutputModule& );
  
};

#endif

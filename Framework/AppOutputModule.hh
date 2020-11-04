//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppOutputModule.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppOutputModule. Abstract parent for output modules. Implementers
//	of such modules should inherit from this class rather than from the
//	AppModule class directly.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPOUTPUTMODULE_HH
#define APPOUTPUTMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

class AppReportAttributes;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppOutputModule : public AppModule {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppOutputModule( 
		  const char* const theName, 
		  const char* const theDescription );
  
  // Destructor
  virtual ~AppOutputModule( );
  
  virtual AppResult outputEvent( AbsEvent*  anEvent );
  virtual AppResult outputFrame( AppFrame* aFrame );    // Frame output  

  virtual void addToReport( AppReportAttributes* attrib, int mode) const;  

private:
  
  // Copy Constructor
  AppOutputModule( const AppOutputModule& );

  // Not implemented.
  AppOutputModule& operator=( const AppOutputModule& );

};

#endif

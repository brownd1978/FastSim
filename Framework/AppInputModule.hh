//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppInputModule.hh 509 2010-01-14 15:18:55Z stroili $
// Description:
//	Class AppInputModule. Abstract parent class for input modules.
//	Implementers of input modules should inherit from this class
//	rather than AppModule directly.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Removed dependency on DbiEvent 6/97
//      Marc Turcotte                   Introduced AppStopType   3/20/98
//      Marc Turcotte                   Added inputEvent for default
//                                      non-null empty event 9/22/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997              The University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPINPUTMODULE_HH
#define APPINPUTMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"
#include "Framework/AppStopType.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppInputModule : public AppModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppInputModule( const char* const theName, 
		  const char* const theDescription );
  
  // Destructor
  virtual ~AppInputModule( );

  virtual AppResult inputEvent( AbsEvent*& anEvent ) = 0;  
  virtual AppResult inputEvent( AbsEvent*& anEvent, 
				AppStopType& theDispatchStopType );
  virtual AppResult inputFrame( AppFrame*& aFrame,
				AppStopType& theDispatchStopType );
  
  virtual void setSilent( bool mode ) {};

  virtual void append ( const char* const theName,
                        const char* const theOther=NULL) {}
  virtual void remove ( const char* const theName) {}
  virtual void wipeout () {}

  virtual void addToReport( AppReportAttributes* attrib, int mode) const;

  // Framework needs this for deleting the last event.
  // This is because it is inputEvent() which deletes the previous
  // event before fetching a new one and Framework does not loop to call 
  // inputEvent() after a specified number of events are called.
  virtual void deleteEvent(AbsEvent*& anEvent);  
  
private:
  
  // Copy Constructor
  AppInputModule( const AppInputModule& );

  // Not implemented.
  AppInputModule& operator=( const AppInputModule& );

};
#endif


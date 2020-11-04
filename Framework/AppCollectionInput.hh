//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppCollectionInput.hh 509 2010-01-14 15:18:55Z stroili $
// Description:
//	Class AppCollectionInput. Abstract parent class for input modules.
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

#ifndef APPCOLLECTIONINPUT_HH
#define APPCOLLECTIONINPUT_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppInputModule.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppCollectionInput : public AppInputModule {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppCollectionInput( const char* const theName, 
		      const char* const theDescription );
  
  // Destructor
  virtual ~AppCollectionInput( );

  virtual AppResult inputEvent( AbsEvent*& anEvent );
  virtual AppResult inputEvent( AbsEvent*& anEvent,
                                AppStopType& theDispatchStopType );
 
  virtual APPList<char*>* collections() const { return NULL; }
  virtual bool isOpen () const { return false; }
  virtual const char* currentCollection() const { return NULL; }
  
private:
  
  // Copy Constructor
  AppCollectionInput( const AppCollectionInput& );

  // Not implemented.
  AppCollectionInput& operator=( const AppCollectionInput& );

};
#endif


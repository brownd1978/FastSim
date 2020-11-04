//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppPath.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppPath. This class describes paths within the BaBar
//	Application Framework.
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

#ifndef APPPATH_HH
#define APPPATH_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppSequence.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppPath : public AppSequence {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppPath( const char* const theName, const char* const theDescription );
  
  // Copy Constructor
  //    AppPath( const AppPath& );
  
  // Destructor
  virtual ~AppPath( );

private:

  // Not implemented.
  AppPath( const AppPath& );
  AppPath& operator=( const AppPath& );
  
};

#endif

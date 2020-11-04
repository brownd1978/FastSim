//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppExecutable.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppExecutable. This is the abstract parent class for the
//	executable elements (modules, sequences & paths etc.) in the
//	BaBar Application Framework.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Modifications to handle Actions '97
//      Marc Turcotte                   Modified for new style begin/end
//      Marc Turcotte                   Added execute member function
//                                      for dynamic dispatch support 3/4/98
//      Marc Turcotte                   Introduced AppStopType 3/20/98
//      Marc Turcotte                   Renamed execute to frame 3/26/98
//      Marc Turcotte                   Merged R.Kapur's TK interface 5/98
//      Marc Turcotte                   Modified for AppStatus 6/20/98
//      Marc Turcotte                   Added input/ouput Frame 6/22/98
//      Marc Turcotte                   Added theDispatchStopType to new
//                                      signatures 6/23/98
//      Marc Turcotte                   Changed AppStatus to AppResult 6/23/98
//      Marc Turcotte                   Modified for new AbsEvent sigs 6/24/98
//      Marc Turcotte                   Made name() return const char* 7/30/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997, 1998        University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPEXECUTABLE_HH
#define APPEXECUTABLE_HH

//-------------
// C Headers --
//-------------
#include <stddef.h>
#include <string>

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppFramework;
class AppJob;
#include "Framework/AppResult.hh"
//class AbsEvent;
class AppStopType;
class AppFrame;
class AppReportAttributes;

// Need to include this header here since it was included in AppRun.hh, 
// which is now removed. Otherwise there are too many compilation problems
// in the releaase.
// This should be fixed on another day ... ads, 31Mar04
#include "AbsEvent/AbsEvent.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppExecutable {
  
  //------------------
  // Static Members --
  //------------------

public:
  
  // Enum defining Module, Sequence and Path
  enum ExecType { APP_module,
		  APP_framework,
		  APP_input,
		  APP_output,
		  APP_filter,
		  APP_noclone,
		  APP_sequence,
		  APP_path};
  
  static AppExecutable* narrow( AppExecutable* anExec, ExecType aType );
  
  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppExecutable( );
  AppExecutable(
		const char* const theName,
		const char* const theDescription = NULL );
  
  // Copy Constructor
  //    AppExecutable( const AppExecutable& );
  
  // Destructor
  virtual ~AppExecutable( );
  
  // Selectors (const)
  const char*         description( )     const;	// The Description
  ExecType      execType   ( )     const;	// Type Executable type
  AppFramework* framework  ( )     const;	// The Framework
  virtual bool          isClonable ( )     const;	// Clonable?    
  bool          isEnabled  ( )     const;	// Enabled or Disabled?
  virtual bool          passed     ( )     const;	// Passed?
  const   char*         name       ( )     const;	// The Name    
  virtual void flushReport  ( ) const;
  
  virtual void fullReport   ( const char* const aMessage ) const;
  virtual void partialReport( const char* const aMessage ) const;
  void fullReport( const std::string& aMessage ) const {
    fullReport(aMessage.c_str());
  }
  void partialReport( const std::string& aMessage ) const{
    partialReport(aMessage.c_str());
  }

  virtual void report      (AppReportAttributes* attrib, int mode = 0) const;
  virtual void addToReport (AppReportAttributes* attrib, int mode = 0) const{};
  
  // Modifiers
  void setEnabled  ( bool mode );
  void setFramework( const AppFramework* const theFrame );
  
  // Operations
  
  virtual AppResult beginJob   ( AbsEvent* anEvent  ) = 0;
  virtual AppResult beginRun   ( AbsEvent* anEvent  ) = 0;
  
  virtual AppResult event      ( AbsEvent*  anEvent ) = 0;
  virtual AppResult other      ( AbsEvent*  anEvent ) = 0;
  virtual AppResult frame      ( AppFrame* aFrame,
				 const AppStopType& theDispatchStopType ) = 0; 
  // Frame dispatch
  
  virtual AppResult endRun ( AbsEvent* anEvent ) = 0;
  virtual AppResult endJob ( AbsEvent* anEvent ) = 0;
  
  virtual AppResult abortJob( AbsEvent* anEvent ) = 0;  
  
protected:

  // Not implemented.
  AppExecutable( const AppExecutable& );
  AppExecutable& operator=( const AppExecutable& );
  
  void setExecType ( ExecType aType );
  
  // Data members
  char*         _description;
  char*         _name;
  bool          _isEnabled;
  bool          _passed;
  ExecType      _execType;
  AppFramework* _framework;
  
};

// Inline implementations
#include "Framework/AppExecutable.icc"

#endif





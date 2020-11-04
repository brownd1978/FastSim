//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppStreamsOutputModule.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppStreamsOutputModule. This is the abstract streams output 
//	module for the BaBar framework. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//      Marc Turcotte                   Modified for new style begin/end
//      Marc Turcotte                   Modified for new style event.
//      Marc Turcotte                   Modified for FNAL sigs 6/24/98
//
// Copyright Information:
//      Copyright (C) 1994, 1995        Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPSTREAMSOUTPUTMODULE_HH
#define APPSTREAMSOUTPUTMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppOutputModule.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsEvent;
class AppStream;
class AppOutputCommand;
class AppFrame;
template<class T> class APPList;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppStreamsOutputModule : public AppOutputModule {

  //--------------------
  // Instance Members --
  //--------------------

public:
  
  // Constructors
  AppStreamsOutputModule( const char* const theName,
			  const char* const theDescription );
  
  // Copy Constructor
  //    AppStreamsOutputModule( const AppStreamsOutputModule& );
  
  // Destructor
  virtual ~AppStreamsOutputModule( );
  
  // Operations
  
  virtual AppResult beginJob ( AbsEvent* anEvent );
  virtual AppResult beginRun( AbsEvent* anEvent );
  
  virtual AppResult outputEvent ( AbsEvent* anEvent );
  virtual AppResult outputFrame ( AppFrame* aFrame  );
  
  virtual AppResult endRun( AbsEvent* anEvent );
  virtual AppResult endJob ( AbsEvent* anEvent );
  
  virtual AppResult abortJob( AbsEvent* );
  
  virtual void           help(int argc, char** argv);
  
  AppStream*     fetch( const char* const theName ) const;
  bool           has  ( const char* const theName ) const;
  
  // Selectors
  APPList< AppStream* >* streams( ) const;
  
  // Modifiers
  virtual void append ( const AppStream* const theStream );
  virtual void remove ( const AppStream* const theStream );
  virtual void remove ( const char* const theName );
  virtual void wipeout( );
  
protected:
  
  AbsEvent*              _theEvent;
  APPList< AppStream* >* _streams;
  AppOutputCommand*      _outputCmd;
  AppOutputCommand*      _outCmd;

private:

  // Not implemented.
  AppStreamsOutputModule( const AppStreamsOutputModule& );
  AppStreamsOutputModule& operator=( const AppStreamsOutputModule& );

};

#endif

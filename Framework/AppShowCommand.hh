//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppShowCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppShowCommand. Command handler for the "show" command. It
//	delegates the request to the show function member of its target.
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

#ifndef APPSHOWCOMMAND_HH
#define APPSHOWCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"
class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppShowCommand : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppShowCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppShowCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}

private:

  // Not implemented.
  AppShowCommand( const AppShowCommand& );
  AppShowCommand& operator=( const AppShowCommand& );

};

#endif

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppInputCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppInputCommand. Command handler for the "input" command for
//	the standard input module. Type "input help" at Framework prompt
//      for a list of available commands.
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

#ifndef APPINPUTCOMMAND_HH
#define APPINPUTCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"
class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppInputCommand : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppInputCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppInputCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* ) = 0;
  virtual void restoreState( char* [] );

  virtual std::string description( ) const;  

protected:
  
  virtual int addHandler    ( ) = 0;
  virtual int listHandler   ( ) = 0;
  virtual int removeHandler ( );
  virtual int wipeoutHandler( );
  virtual int helpHandler   ( ) = 0;

private:

  // Not implemented.
  AppInputCommand( const AppInputCommand& );
  AppInputCommand& operator=( const AppInputCommand& );

};

#endif

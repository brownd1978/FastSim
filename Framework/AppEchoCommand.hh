//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppEchoCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppEchoCommand. Command handler for the "echo" command. This
//	command just prints its arguments to standard output.
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

#ifndef APPECHOCOMMAND_HH
#define APPECHOCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppEchoCommand : public AppCommand {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppEchoCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppEchoCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}

private:
  // Not implemented.
  AppEchoCommand( const AppEchoCommand& );
  AppEchoCommand& operator=( const AppEchoCommand& );
  
};

#endif

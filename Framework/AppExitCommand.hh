//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppExitCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppExitCommand. Command handler for the "exit" command. If 
//	issued to the framework itself, this command will terminate the
//	current job. If issued to a module (i.e. during a "talkTo" session),
//	this command will return to the main framework.
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

#ifndef APPEXITCOMMAND_HH
#define APPEXITCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

class AppConfigRecorder;


//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppExitCommand : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:

  // Constructors
  AppExitCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppExitCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}

private:

  // Not implemented.
  AppExitCommand( const AppExitCommand& );
  AppExitCommand& operator=( const AppExitCommand& );

};

#endif



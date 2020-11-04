//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppEventsCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppEventsCommand. This command handler deals with the "events"
//	command which is used to control the looping over events. Subcommands
//	are:
//
//		begin    [-nev <n>]	Begin processing
//		continue [-nev <n>]	Continue processing
//
//	Where "begin" resets the current input module to start a new
//	event processing run, whereas "continue" continues the current
//	sequence (presumably because it was terminated prematurely by
//	the optional qualifier "-nev" which specifies a number of events).
//	The "continue" command acts identically to the "begin" command
//	if no prior "begin" command had been issued.
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

#ifndef APPEVENTSCOMMAND_HH
#define APPEVENTSCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppEventsCommand : public AppCommand {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppEventsCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppEventsCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  
protected:
  
  virtual int beginHandler   ( int argc, char* argv[] );
  virtual int continueHandler( int argc, char* argv[] );
  virtual int helpHandler( );
  virtual void show( ) const;
  virtual std::string description( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}

  virtual int dumpOnBeginHandler(int argc, char* argv[] );
  virtual int beginConfigDump(int argc, char* argv[] );

private:

  // Not implemented.
  AppEventsCommand( const AppEventsCommand& );
  AppEventsCommand& operator=( const AppEventsCommand& );

  std::string*  _dumpFile;

};

#endif

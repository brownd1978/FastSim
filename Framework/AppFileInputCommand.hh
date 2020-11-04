//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFileInputCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppFileInputCommand. Command handler for the "input" command for
//      the standard file input module. Type "input help" at Framework prompt
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

#ifndef APPFILEINPUTCOMMAND_HH
#define APPFILEINPUTCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppInputCommand.hh"
class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppFileInputCommand : public AppInputCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppFileInputCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppFileInputCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual void dumpState( AppConfigRecorder* );

protected:
  
  virtual int addHandler    ( );
  virtual int listHandler   ( );
  virtual int helpHandler   ( );

private:

  // Not implemented.
  AppFileInputCommand( const AppFileInputCommand& );
  AppFileInputCommand& operator=( const AppFileInputCommand& );

};

#endif

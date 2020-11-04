//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppHelpCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppHelpCommand. Command handler for the "help" command. It
//	delegates the request to the help function member of its target.
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

#ifndef APPHELPCOMMAND_HH
#define APPHELPCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppHelpCommand : public AppCommand {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppHelpCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppHelpCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}

private:

  // Not implemented.
  AppHelpCommand( const AppHelpCommand& );
  AppHelpCommand& operator=( const AppHelpCommand& );

};
#endif


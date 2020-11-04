//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppConfigCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppConfigCommand. This command handler deals with the Framework
//      configuration. Subcommands are:
//
//      config dump filename
//      config read filename
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Akbar Mokhtarani			Original Author
//
// Copyright Information:
//	Copyright (C) 1998	        LBNL
//
//------------------------------------------------------------------------

#ifndef APPCONFIGCOMMAND_HH
#define APPCONFIGCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppConfigCommand : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppConfigCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppConfigCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  
protected:
  
  virtual int dumpHandler    ( int argc, char* argv[] );
  virtual int restoreHandler    ( int argc, char* argv[] );
  virtual int helpHandler();
  
  virtual void show( ) const;
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}
  virtual bool isShowable( ) const;
  virtual std::string description( ) const;    

private:

  // Not implemented.
  AppConfigCommand( const AppConfigCommand& );
  AppConfigCommand& operator=( const AppConfigCommand& );

};

#endif








//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppActionCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppActionCommand. This command handler deals with the Framework
//      actions at the module level.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Marc Turcotte			Original Author
//
// Copyright Information:
//	Copyright (C) 1998	        University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPACTIONCOMMAND_HH
#define APPACTIONCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppActionCommand : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppActionCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
    virtual ~AppActionCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}
  
protected:
  
  virtual int listHandler    ( int argc, char* argv[] );
  virtual int enableHandler  ( int argc, char* argv[] );
  virtual int disableHandler ( int argc, char* argv[] );    
  virtual int helpHandler    ();      
  
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;

private:

  // Not implemented.
  AppActionCommand( const AppActionCommand& );
  AppActionCommand& operator=( const AppActionCommand& );
  
};

#endif

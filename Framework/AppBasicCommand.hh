//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppBasicCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppBasicCommand. This is an abstract class that acts as the
//	parent for the "module", "sequence" and "path" commands. In particular
//	it provides default handlers for the common sub-commands.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Introduced Actions '97 
//      Marc Turcotte                   Merged R. Kapur's TK interface 5/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997              University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPBASICCOMMAND_HH
#define APPBASICCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppNameNode;
class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppBasicCommand : public AppCommand {

public:
  enum {firstIndex = 2};
  
  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppBasicCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppBasicCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}
  
protected:
  
  virtual int updateHandler  ( ); // temporary!
  virtual int appendHandler  ( );
  virtual int cloneHandler   ( );
  virtual int createHandler  ( );
  virtual int deleteHandler  ( );
  virtual int disableHandler ( );
  virtual int enableHandler  ( );
  virtual int helpHandler    ( );
  virtual int insertHandler  ( );
  virtual int listHandler    ( );
  virtual int removeHandler  ( );
  virtual int talkToHandler  ( ); 
  virtual int inputHandler   ( ); 
  virtual int outputHandler  ( ); 

private:

  // Not implemented.
  AppBasicCommand( const AppBasicCommand& );
  AppBasicCommand& operator=( const AppBasicCommand& );
  
};

#endif

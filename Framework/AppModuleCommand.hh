//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppModuleCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppModuleCommand. Command handler for the "module" command for
//	the BaBar Application Framework. Valid subcommands are listed in 
//      helpHandler() implementation.
//
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Introduced Actions '97
//      Marc Turcotte                   Merged in R.Kapur's TK interface 5/98
//      Marc Turcotte                   Implemented clone command 8/6/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997, 1998        University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPMODULECOMMAND_HH
#define APPMODULECOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppBasicCommand.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppModuleCommand : public AppBasicCommand {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppModuleCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppModuleCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  
protected:
  
  virtual int disableHandler( );
  virtual int enableHandler ( );
  virtual int helpHandler   ( );
  virtual int inputHandler  ( );
  virtual int listHandler   ( );
  virtual int outputHandler ( );
  virtual int talkToHandler ( );
  virtual int cloneHandler  ( );

private:

  // Not implemented.
  AppModuleCommand( const AppModuleCommand& );
  AppModuleCommand& operator=( const AppModuleCommand& );
    
};

#endif



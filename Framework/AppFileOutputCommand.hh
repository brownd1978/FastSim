//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFileOutputCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppFileOutputCommand declarations file. 
//      Command handler for the "output" command for the file streams output 
//      module. Valid subcommands that are added to the standard output 
//      commands are:
//
//      output file <stream> [<file>]         Bind stream to file
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//
// Copyright Information:
//      Copyright (C) 1994, 1995        Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPFILEOUTPUTCOMMAND_HH
#define APPFILEOUTPUTCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppOutputCommand.hh"

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppFileOutputCommand : public AppOutputCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppFileOutputCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppFileOutputCommand( );
  
protected:
  
  /**
   ** Handler for the "help" command.
   **/
  virtual int helpHandler( );
  
  AppStream* createStream( const char* theName, const char* theDestination );
  const char* destinationString( ) const;
  int handleCommand( int argc, char* argv[] );

  private:

  // Not implemented.
  AppFileOutputCommand( const AppFileOutputCommand& );
  AppFileOutputCommand& operator=( const AppFileOutputCommand& );


};

#endif

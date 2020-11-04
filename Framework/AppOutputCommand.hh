//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppOutputCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//      Class AppOutputCommand. Command handler for the "output" command for
//      the standard file output module. Valid subcommands are:
//
//      output clear                            Empty the list of streams
//      output create <stream> [<dest>]         Create stream optionally 
//                                              bound to destination
//      output delete <stream1> [<str2> ..]     Remove stream(s) from list
//      output destination <stream> [<dest>]    Bind stream to destination
//      output disable <stream1> [<str2> ..]    Disable stream(s)
//      output enable  <stream1> [<str2> ..]    Enable stream(s)
//      output help                             Display help information
//      output list                             List current streams/files
//      output paths <stream> <path1>...        Bind <path1> etc. to stream
//      output reset                            Synonym for clear
//      output statistics                       List statistics for each stream
//      output stream <stream> [<dest>]         Synonym for create
//      output wipe-out                         Synonym for clear
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

// Note case sensitivity here - it avoids a clash with the APPOutputCommand
// class
#ifndef AppOUTPUTCOMMAND_HH
#define AppOUTPUTCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppFramework;
class AppStream;
class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppOutputCommand : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppOutputCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppOutputCommand( );
  
  
  /**
   ** High level command handler required by the AppCommand interface. 
   ** The function value denotes whether the command was a valid one or not.
   **/
  virtual int  handle( int argc, char* argv[] );
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* );
  virtual void restoreState( char* [] );

  virtual std::string description( ) const;
  
protected:
  
  /**
   ** Low level command handler. The function value denotes whether
   ** the command was a valid one or not.
   **/
  virtual int handleCommand( int argc, char* argv[] );
  
  /**
   ** Handler for the "create" command.
   **/
  virtual int createHandler( );
  
  /**
   ** Handler for the "delete" command.
   **/
  virtual int deleteHandler( );
  
  /**
   ** Handler for the "destination" command.
   **/
  virtual int destinationHandler( );
  
  /**
   ** Handler for the "disable" command.
   **/
  virtual int disableHandler( );
  
  /**
   ** Handler for the "enable" command.
   **/
  virtual int enableHandler( );
  
  /**
   ** Handler for the "help" command.
   **/
  virtual int helpHandler( );
  
  /**
   ** Handler for the "list" command.
   **/
  virtual int listHandler( );
  
  /**
   ** Handler for the "paths" command.
   **/
  virtual int pathsHandler( );
  
  /**
   ** Handler for the "statistics" command.
   **/
  virtual int statisticsHandler( );
  
  /**
   ** Handler for the "wipe-out" command.
   **/
  virtual int wipeoutHandler( );
  
  /**
   ** Create a concrete stream with the specified name and destination
   **/
  virtual AppStream*  createStream( const char* theName, 
				    const char* theDestination ) = 0;
  
  /**
   ** Return a string to be used in describing the destination
   **/
  virtual const char* destinationString( ) const = 0;
  
  /**
   ** Provide additional information to the "list" command. The
   ** default doesn't provide any addional information, but
   ** subclasses may choose to override this.
   **/
  virtual void listInformation( AppStream* theStream, AppFramework* theFrame );
  
  /**
   ** Provide additional information to the "statistics" command. 
   ** The default implementation doesn't provide any addional 
   ** information, but subclasses may choose to override this.
   **/
  virtual void statisticsInformation( AppStream* theStream, 
				      AppFramework* theFrame );

private:

  // Not implemented.
  AppOutputCommand( const AppOutputCommand& );
  AppOutputCommand& operator=( const AppOutputCommand& );
  
};

#endif

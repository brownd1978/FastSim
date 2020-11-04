//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppSourceFileCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppSourceFileCommand. 
//      Command handler for the "sourceFileFound" command. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Asoka De Silva		Original Author
//
// Copyright Information:
//
//
//------------------------------------------------------------------------

#ifndef APPSOURCEFILECOMMAND_HH
#define APPSOURCEFILECOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"
class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppSourceFileCommand : public AppCommand {
  
  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppSourceFileCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~AppSourceFileCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}
  virtual std::string description( ) const;

private:

  // Not implemented.
  AppSourceFileCommand( const AppSourceFileCommand& );
  AppSourceFileCommand& operator=( const AppSourceFileCommand& );

};

#endif



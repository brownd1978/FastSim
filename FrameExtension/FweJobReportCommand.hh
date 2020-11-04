//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: FweJobReportCommand.hh 505 2010-01-14 15:15:59Z stroili $
//
// Description:
//	Class FweJobReportCommand. 
//      This command interacts with Framework's job summary report.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A. De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#ifndef FWEJOBREPORTCOMMAND_HH
#define FWEJOBREPORTCOMMAND_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppCommand.hh"
#include <string>
class AppConfigRecorder;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class FweJobReportCommand : public AppCommand {

  //--------------------
  // Instance Members --
  //--------------------
  
public:

// Constructors
  FweJobReportCommand( const char* const theCommand, AppModule* theTarget );
  
  // Destructor
  virtual ~FweJobReportCommand( );
  
  // Handler
  virtual int handle( int argc, char* argv[] );
  virtual void dumpState( AppConfigRecorder* );
  virtual void restoreState( char* [] );
  
protected:
  
  virtual int fileHandler    ( int argc, char* argv[] );
  virtual int listHandler    ( int argc, char* argv[] );
  virtual int helpHandler    ();      
  
  virtual std::string description( ) const;
  virtual void show( ) const;
  virtual bool isShowable( ) const;

private:

  // Not implemented.
  FweJobReportCommand( const FweJobReportCommand& );
  FweJobReportCommand& operator=( const FweJobReportCommand& );
  
};

#endif

//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppConfigCommand.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppConfigCommand. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Marc Turcotte			Original Author, 4/15/98
//
// Copyright Information:
//	Copyright (C) 1998 		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// This Class's Header
//
#include "Framework/AppConfigCommand.hh"

//
// C Headers
//
#include <assert.h>
#include <string.h>
#include <time.h>
#include <fstream>

// the following routines locate the file

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE		// Make sure <sys/stat.h> defines S_ISREG()
#endif
#include <sys/stat.h>

//
// Collaborating Class Headers
//

#include "Framework/AppFramework.hh"
#include "Framework/AppMemento.hh"
#include "FrameUtil/AbsInterp.hh"
#include <string>
#include <fstream>
using std::cerr;
using std::endl;
using std::fstream;
using std::ios;


AppConfigCommand::AppConfigCommand( 
    const char* const theCommand, 
    AppModule* theTarget )
    : AppCommand( theCommand, theTarget ) {
}


AppConfigCommand::~AppConfigCommand( ) {
}

//
// Handler
//

int 
AppConfigCommand::handle( int argc, char* argv[] )
{
    AppFramework* theFrame = (AppFramework*)target( );
    int result = AbsInterp::ERROR;
    
    setArgs( argc, argv );
    if ( 3 <= argc ) {
      if        ( (0 == strcmp( argv[1], "dump" ))|| 
		  (0 == strcmp( argv[1], "dumptcl")) ||
		  (0 == strcmp( argv[1], "dumpraw"))) {
	result = dumpHandler( argc, argv );	
      }
      //get the file name and create it
      else if ( 0 == strcmp( argv[1], "restore" ) ) {
	result = restoreHandler( argc, argv );	
      }
      else {
	std::string txtmsg("invalid command name: ");
	for (int index=0; index < argc; index++ ) {
	  txtmsg += " ";
	  txtmsg += argv[index];
	}
	theFrame->setError( txtmsg.c_str() );
	theFrame->setError( "The syntax is: config dump filename or "
			    "config restore filename" );
      } 
    } else {
      if ( 0 == strcmp( argv[1], "help" ) ) {
	result = helpHandler();
      }
      else {
	theFrame->setError( "wrong # args" );
	result = helpHandler();	
      }
    }
    return result;
}

int
AppConfigCommand::helpHandler() {

  target()->fullReport( "The following commands are available:"); 

  target()->partialReport("  dump(tcl) <filename> [-cmd <commands>]" );
  target()->fullReport("Dump out the configuration as a tcl file");
  target()->partialReport("                                        " );
  target()->fullReport(" optional <commands> appended in tcl file.");

  target()->partialReport("  dumpraw <filename>                    " );
  target()->fullReport("Dump out the configuration in raw format");

  target()->partialReport("  help                                  " );
  target()->fullReport("Show available commands");

  target()->partialReport("  restore <filename>                    " );
  target()->fullReport("Restore the configuration");
  
  return AbsInterp::OK;
}

int 
AppConfigCommand::dumpHandler( int argc, char* argv[] ) {

  time_t mytime = time(NULL);
  AppFramework* theFrame = (AppFramework*)target( );
  int result              = AbsInterp::ERROR;
            
  setArgs( argc, argv );

  //check to see if the file exists
  struct stat fileStat;
  if (stat(argv[2], &fileStat) == 0 &&
      (S_ISREG(fileStat.st_mode) || S_ISDIR(fileStat.st_mode))) {
    target()->partialReport("Error: the file ");
    target()->partialReport(argv[2]);
    target()->fullReport(" exists and is not overwritten.");
    return result;
  }
    
  //create the file from argv[2]
  fstream dump;
  dump.open(argv[2], ios::out);

  dump << "# This file was created by " << getenv("USER") 
       << " on " << ctime(&mytime); // ctime returns a "\n" appended.
  dump << "# on host "
       << getenv("HOST") << " running "<< getenv("OSTYPE") << endl;

  // and call the the dumpTcl in appMemento
  AppMemento* frameMemento = new  AppMemento(theFrame);

  if( (0 == strcmp( argv[1], "dump" )) || (0 == strcmp(argv[1], "dumptcl" ))) {
    frameMemento->DumpConfigTcl( dump );
    char* theArg;
    int index = 3;    
    while ( 0 != (theArg = getArgument(index)) ) {
      if ( 0 == strcmp( theArg, "-cmd" ) ) {
	dump << endl;
      } else {
	dump << theArg << " ";
      }
      index++;
    }
    dump << endl;
  }
  else if( 0 == strcmp( argv[1], "dumpraw")) {
    frameMemento->DumpConfig( dump );
  }

  dump.close();
  
  delete frameMemento;

  result = AbsInterp::OK;
  return result;
}
int 
AppConfigCommand::restoreHandler( int argc, char* argv[] ) {

  AppFramework* theFrame = (AppFramework*)target( );
  int result              = AbsInterp::ERROR;
      
       
    setArgs( argc, argv );

    //check to see if the file exists
     struct stat fileStat;

     if (stat(argv[2], &fileStat) == 0 && 
	 (S_ISREG(fileStat.st_mode) || S_ISDIR(fileStat.st_mode)))
       {
	 //open the file from argv[2]
	 fstream restore;
	 restore.open(argv[2], ios::in);
	 
	 // and call the the restore in appMemento
	 AppMemento* frameMemento = new  AppMemento(theFrame);
	 
	 frameMemento->restoreConfig( restore );

	 restore.close();

	 delete frameMemento;
       }
     else
       {
	 cerr << "Could not find configuration file " << argv[2] <<
	   " in the current directory" << '\n';
       }
  result = AbsInterp::OK;
  return result;
}

void
AppConfigCommand::show( ) const {
}

bool
AppConfigCommand::isShowable( ) const {
   return false; 
}

std::string 
AppConfigCommand::description( ) const
{
   return "Dump and restore configuration.";
}


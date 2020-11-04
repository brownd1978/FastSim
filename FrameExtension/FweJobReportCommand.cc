//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: FweJobReportCommand.cc 668 2010-09-16 13:17:21Z stroili $
//
// Description:
//      Class FweJobReportCommand.
//      This command interacts with Framework's job summary report.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      A. De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "FrameExtension/FweJobReportCommand.hh"
#include "FrameJobReport/FjrFwkJobReport.hh"

#include "Framework/AppModule.hh"
#include "FrameUtil/AbsInterp.hh"
#include "Framework/AppFramework.hh"

#include <string.h>

FweJobReportCommand::FweJobReportCommand(const char* const theCommand, 
				   AppModule* theTarget )
  : AppCommand( theCommand, theTarget ) {
}


FweJobReportCommand::~FweJobReportCommand( ) {
}

int 
FweJobReportCommand::handle( int argc, char* argv[] )
{
    AppFramework* theFrame = (AppFramework*)target( );
    int result = AbsInterp::ERROR;
    
    setArgs( argc, argv );
    if ( 2 <= argc ) {
      if ( 0 == strcmp( argv[1], "filename" ) ) {
	result = fileHandler( argc, argv );
      } else if ( 0 == strcmp( argv[1], "help" ) ) {
	result = helpHandler( );
      } else if ( 0 == strcmp( argv[1], "list" ) ) {
	result = listHandler( argc, argv );
      } else {
	std::string txtmsg("invalid command name: ");
	for (int index=0; index < argc; index++ ) {
	  txtmsg += " ";
	  txtmsg += argv[index];
	}
	theFrame->setError( txtmsg.c_str() );
      } 
    } else {
      theFrame->setError( "wrong # args" );
    }
    return result;
}


int 
FweJobReportCommand::fileHandler( int argc, char* argv[] ) {

  int result = AbsInterp::OK;

  AppFramework* theFrame  = target()->framework();
            
  if ( 3 <= argc ) {
    std::string filename(argv[2]) ;
    FjrFwkJobReport* theJobReport = FjrFwkJobReport::getFwkJobReport();
    bool status = theJobReport->setFileName(filename);
    if (! status) {
      theFrame->setError( "filename could not be set in Job Report");      
      result = AbsInterp::ERROR;
    }
  }
  
  return result;
}

int 
FweJobReportCommand::listHandler( int argc, char* argv[] ) {

  int result = AbsInterp::OK;

  AppFramework* theFrame  = target()->framework();
            
  FjrFwkJobReport* theJobReport = FjrFwkJobReport::getFwkJobReport();
  const std::string* theFilename = theJobReport->getFileName();
  if (theFilename != 0) {
    theFrame->partialReport("     filename set to ");
    theFrame->fullReport(theFilename->c_str());
  }
  else {
    theFrame->fullReport("     filename is not set");
  }

  return result;
}

std::string 
FweJobReportCommand::description( ) const
{
   return "Command to control Framework's job summary report";
}

int 
FweJobReportCommand::helpHandler( )
{
  AppFramework* theFrame = (AppFramework*)target( );
 
  theFrame->fullReport( "The following commands are available:"); 

  theFrame->partialReport( "  filename <filename>                   " );
  theFrame->fullReport("Define the job summary file and enable reporting");
  theFrame->partialReport( "  list                                  " );
  theFrame->fullReport("List the configurations for Job Report");

  return AbsInterp::OK;
}

void
FweJobReportCommand::show( ) const {
}

bool
FweJobReportCommand::isShowable( ) const {
   return false; 
}

void
FweJobReportCommand::dumpState( AppConfigRecorder* r) {
  FjrFwkJobReport* theJobReport = FjrFwkJobReport::getFwkJobReport();
  const std::string* theFilename = theJobReport->getFileName();
  if (theFilename != 0) {
    const char* var[3];
    var[0] = "jobReport";
    var[1] = "filename";  
    var[2] = const_cast<const char*>(theFilename->c_str());
    r->store(3,var);
  }
}

void 
FweJobReportCommand::restoreState( char* argv[] ) {
  int i=0;
  while(argv[i]){
    i++;
  }
  int argc = i;
  handle(argc, argv);
}

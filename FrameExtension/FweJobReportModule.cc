//--------------------------------------------------------------------------
//
// File and Version Information:
//    $Id: FweJobReportModule.cc 505 2010-01-14 15:15:59Z stroili $
//
// Description:
//      class FweJobReportModule
//      This is a module which should be in AppUserBuild.cc
//      Its purpose is to let interface the job reporting class with Framework
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Authors:
//      A. De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "FrameExtension/FweJobReportModule.hh"
#include "FrameExtension/FweJobReportCommand.hh"

#include "FrameUtil/AbsInterp.hh"

FweJobReportModule::FweJobReportModule(const char* const theName, 
				 const char* const theDescription) 
  : AppModule( theName, theDescription )
  , _jrCommand(new FweJobReportCommand("jobReport", this) )
{

  //  commands()->append( _jrCommand);

  // make this a top level command in tcl
  AbsInterp* theInterp = AbsInterp::theInterpreter();
  if (theInterp != 0) {
    if ( ! theInterp->existsCommand("jobReport") ) {
      theInterp->createCommand("jobReport", _jrCommand);
    }
  }
  
}
  
FweJobReportModule::~FweJobReportModule( ) {
  delete _jrCommand;
}






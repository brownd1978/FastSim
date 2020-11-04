//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppMain.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppMain. Main program for BaBar Application Framework.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//      Marc Turcotte                   Added AppActionController setup
//                                      code here to make the placement
//                                      of where Actions are added in the
//                                      AppUserBuild c'tor irrelevant. 4/17/98
//      Marc Turcotte                   Merged R.Kapur's TK interface 5/18/98
//      Marc Turcotte                   Added creation of Abstract Interpreter 5/29/98
//      Marc Turcotte                   Modified extensively to allow use with TK 6/98
//      Marc Turcotte                   Migrated setupTheApp to AppFramework 7/28/98
//      Marc Turcotte                   Made APP_theFramework local 7/30/98
//      Marc Turcotte                   Made build and AppTheFramework auto 7/30/98
//      Marc Turcotte                   in tcl/tk 8.0 tcl_rcFilename used in tclUnixInit.c
//                                      which has the function Tcl_SourceRCFile invoked
//                                      from tclMain.c
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// This Class's Header
//
#include "Framework/AppMain.hh"

//
// Collaborating class headers
//
#include "Framework/AppFramework.hh"
#include "Framework/AppUserBuild.hh"
#include "FrameUtil/AbsInterp.hh"
#include "Framework/AppFwkExitCodes.hh"

#include "FrameJobReport/FjrFwkJobReport.hh"

#include <iostream>
#include <stdlib.h>

//
// Exported and Local Variable Declarations
// ----------------------------------------

int           global_argc;
char**        global_argv;

extern "C" {
#if (TCL_MAJOR_VERSION <7 || \
     TCL_MAJOR_VERSION==7 && TCL_MINOR_VERSION<=3) && !defined(__STDC__)
// Tim Adye, 30 Sep 97: tcl.h 7.3 doesn't typedef ClientData correctly
// unless __STDC__ is defined.
#define __STDC__
#include "tcl.h"
#undef  __STDC__
#else
#include "tcl.h"
using std::cout;
using std::endl;
#endif
}

int
TCL_ONLY_AppInit( Tcl_Interp* interp ) {
//----------------------------------------------------------------------
// 
//                     
//                 +------------------+ 
//                 |  AbsInterp       |
//                 |  ----            |
//                 |  _theInterpreter |
//                 +------------------+  
//                          ^
//                          |    
//                          |
//                 +------------------+ 
//                 |  FtclInterp      |
//                 |  ----            |
//                 |  _tclInterpreter |
//                 +------------------+  
//
// There is a problem with the order things are done in all this. In the
// Tcl interface version of Framework, we have delegated to Tcl_Main the
// job to create the tcl interpreter and to call this here Tcl_AppInit 
// function. However, in Framework, we have this abstract interpreter 
// layer. The way the interpreter should be created is through an initial 
// call to AbsInterp::theInterpreter(). But since we have the interpreter
// already, we use the alternate from for creation: 
// AbsInterp::theInterpreter(Tcl_Interp* interp). Note that AbsInterp
// is a singleton. There is only one interpreter in the system.
//
   AbsInterp::theInterpreter(interp);   
//
// M. Turcotte, UTD 6/98
//----------------------------------------------------------------------  
  
  int status;
  status = Tcl_Init(interp);
  if (status != TCL_OK) { return TCL_ERROR; }

  FjrFwkJobReport* theJobReport = FjrFwkJobReport::getFwkJobReport();
  AppFramework* AppTheFramework = 
  new AppFramework( global_argc, global_argv );
  AppUserBuild* build = 
  new AppUserBuild( AppTheFramework );
  AppTheFramework->setupTheApp(build);
  AppTheFramework->talkTo( );    
  // We have to do this here instead of in talkTo() for OEP and OPR
  AbsEvent* anEvent(0);
  AppTheFramework->endJob( anEvent);
  if (AppTheFramework->isStopRequested()) {
    status = AppFwkExitCodes::exitRequest; // clean but premature exit
  }
  else {
    status = AppFwkExitCodes::ok;
  }
  delete AppTheFramework;
  delete build;
  delete theJobReport;
 
  // ...should not have to do this here but framework's exit command
  // replaces tcl's and once you're out of the app with the framework's 
  // exit, there is no exit command left available to get out of tclsh!
  // You can only crtl-c out of it. This is not good. The real solution
  // to this mess is to not use exit in framework. But...  M.T.
  cout << "Framework is exiting now." << endl;
  ::exit(status);
  
  return TCL_OK; // you never do this...
    
}

int
main(int argc, char** argv){

  global_argc = 0;
  global_argv = new char*[argc];

  int i;
  int j = 0;
  for (i=0; i<argc ;i++) {
    if ( i==1 ) {         // just strip out any Framework command line
      if(*argv[1]!='-') { // -whatever option for first parameter
        global_argc++;
        global_argv[j]=new char[strlen(argv[i])+1];
        strcpy(global_argv[j], argv[i]); 
        j++;
      }
    } else {
      global_argc++;
      global_argv[j]=new char[strlen(argv[i])+1];
      strcpy(global_argv[j], argv[i]); 
      j++;
    }
  }      
  
  Tcl_Main(argc,argv,TCL_ONLY_AppInit); 
  
  return 0;  
}


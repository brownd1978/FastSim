//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppActionErrLogger.cc 507 2010-01-14 15:17:33Z stroili $
// 
// Description:
//	Class AppActionErrLogger
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen
//
// Copyright Information:
//	Copyright (C) 1998
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FrameLogger/AppActionErrLogger.hh"

//-----------------
// C/C++ Headers --
//-----------------
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "FrameLogger/AppErrLog.hh"


//
// Local Macros, Typedefs, Structures, Unions and Forward Declarations
//

//
// 	           Public Function Member Definitions
//

AppExecutable * AppActionErrLogger::_currentExec = 0;
AppModule * AppActionErrLogger::_currentModule = 0;

//
// Constructors
//

AppActionErrLogger::AppActionErrLogger()
	: 
	_logger(new AppErrLog)
{
  _actionName = "ErrLogger Action";                   // This Action's name
}

AppActionErrLogger::AppActionErrLogger(AppErrLog * theLogger)
	:
	_logger(theLogger)
{
  // sub-class must set _actionName
}

//
// Destructor
//

AppActionErrLogger::~AppActionErrLogger(){
  delete _logger;
}

//
// Member Functions
//

void AppActionErrLogger::record(AppExecutable* theExecutable) {

    // the following is a _poor_ replacement for dynamic_cast
  _currentModule = 0;
  if ( theExecutable->execType() == AppExecutable::APP_module
     ||theExecutable->execType() == AppExecutable::APP_input
     ||theExecutable->execType() == AppExecutable::APP_output
     ||theExecutable->execType() == AppExecutable::APP_filter )
  {
    _currentModule = (AppModule*) theExecutable; 
  }

  // Use all executable types.
  _currentExec = theExecutable;
}


void AppActionErrLogger::beforeBeginJob(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    record(theExecutable);
}

void AppActionErrLogger::beforeBeginRun(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    record(theExecutable);
}

void AppActionErrLogger::beforeEvent(AppExecutable* theExecutable, 
                                AbsEvent*      theEvent){
    record(theExecutable);
}

void AppActionErrLogger::beforeOther(AppExecutable* theExecutable, 
                                AbsEvent*      theEvent){
    record(theExecutable);
}

void AppActionErrLogger::beforeEndRun(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    record(theExecutable);
}

void AppActionErrLogger::beforeEndJob(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    record(theExecutable);
}


void AppActionErrLogger::afterBeginJob(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    _currentModule = 0;
    _currentExec = 0;
}

void AppActionErrLogger::afterBeginRun(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    _currentModule = 0;
    _currentExec = 0;
}

void AppActionErrLogger::afterEvent(AppExecutable* theExecutable, 
                                AbsEvent*      theEvent){
    _currentModule = 0;
    _currentExec = 0;
}

void AppActionErrLogger::afterOther(AppExecutable* theExecutable, 
                                AbsEvent*      theEvent){
    _currentModule = 0;
    _currentExec = 0;
}

void AppActionErrLogger::afterEndRun(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    _currentModule = 0;
    _currentExec = 0;
}

void AppActionErrLogger::afterEndJob(AppExecutable* theExecutable, 
                                AbsEvent*        theEvent){
    _currentModule = 0;
    _currentExec = 0;
}

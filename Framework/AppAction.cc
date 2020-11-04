//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppAction.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppAction
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Marc Turcotte		Original Author       May '97
//      Marc Turcotte           Modified for new style begin/end 2/98
//      Marc Turcotte           Added name() member function 4/10/98
//      Rainer Bartoldus        Added before/afterOther() 7/18/2001
//
// Copyright Information:
//	Copyright (C) '97,'98	University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// This Class's Header
//
#include "Framework/AppAction.hh"

//
// Collaborating Class Headers
//

//
// C Headers
//

#include <assert.h>

//
// 	           Public Function Member Definitions
//

//
// Constructors
//

AppAction::AppAction()
  : _actionName("")
  , _framework(0)
{
}

AppAction::AppAction(std::string theName)
  : _actionName( theName )
  , _framework(0)
{
}
//
// Destructor
//

AppAction::~AppAction(){
}

//
// Comparison Operator
//

int AppAction:: operator==(const AppAction& theAction) const {
  return (theAction._actionName == _actionName); 
}

//
// Interface
//

std::string AppAction::name() const { return _actionName; }
void   AppAction::report( bool ) const { }


void AppAction:: beforeBeginJob(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::  afterBeginJob(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction:: beforeBeginRun(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::  afterBeginRun(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::    beforeEvent(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::     afterEvent(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::    beforeOther(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::     afterOther(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::   beforeEndRun(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::    afterEndRun(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::   beforeEndJob(AppExecutable* anExec, AbsEvent* anEvent){
}

void AppAction::    afterEndJob(AppExecutable* anExec, AbsEvent* anEvent){
}






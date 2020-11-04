//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppActionController.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppActionController
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Marc Turcotte		Original Author  April 1998
//
// Copyright Information:
//	Copyright (C) '98	University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//
// This Class's Header
//

#include "Framework/AppActionController.hh"
#include "Framework/AppAction.hh"
//? #include "Framework/AppFramework.hh"

//
// Collaborating Class Headers
//

//
// C Headers
//

#include <assert.h>

//
// Constructor(s)
//

AppActionController::AppActionController(AppAction* anAction){
  _theAction = anAction;
  _doAction  = true;
}

//
// Destructor
//

AppActionController::~AppActionController(){
}

//
// Class Interface
//

void AppActionController::setDoAction(bool theValue){
  _doAction = theValue; }

bool AppActionController::doAction(){
  return _doAction; }
  
AppAction* AppActionController::action(){
  return _theAction; }

//  
// Replicate/wrap the AppAction interface
//

void AppActionController::beforeBeginJob(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->beforeBeginJob(anExec,anEvent);
  }
}


void AppActionController::afterBeginJob(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->afterBeginJob(anExec,anEvent); 
  }
}


void AppActionController::beforeBeginRun(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->beforeBeginRun(anExec,anEvent);
  }
}


void AppActionController::afterBeginRun(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->afterBeginRun(anExec,anEvent);
  }  
}


void AppActionController::beforeEvent(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->beforeEvent(anExec,anEvent);
  }
}


void AppActionController::afterEvent(AppExecutable* anExec, AbsEvent* anEvent){
  if(_doAction) {
    _theAction->afterEvent(anExec,anEvent);
  }
}
 

void AppActionController::beforeOther(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->beforeOther(anExec,anEvent);
  }
}


void AppActionController::afterOther(AppExecutable* anExec, AbsEvent* anEvent){
  if(_doAction) {
    _theAction->afterOther(anExec,anEvent);
  }
}
 

void AppActionController::beforeEndRun(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->beforeEndRun(anExec,anEvent);
  }
}


void AppActionController::afterEndRun(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->afterEndRun(anExec,anEvent);
  }
}


void AppActionController::beforeEndJob(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->beforeEndJob(anExec,anEvent);
  }
}


void AppActionController::afterEndJob(AppExecutable* anExec, AbsEvent* anEvent) {
  if (_doAction) {
    _theAction->afterEndJob(anExec,anEvent);
  }
}

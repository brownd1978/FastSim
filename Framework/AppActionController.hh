//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppActionController.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppActionController Class Description. This class is used only
//      from the AppFramework to control the execution of a Framework
//      action on a per module level. It replicates the AppAction
//      interface to facilitate useage.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//
//
// Author List:
//	Marc Turcotte	                Original Author   Apr '98
//      Rainer Bartoldus                Added before/afterOther()  Jul '01
//
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPACTIONCONTROLLER_HH
#define APPACTIONCONTROLLER_HH

//
// Collaborating Class Headers
//
#include "Framework/AppExecutable.hh"

//
// Forward declaration(s)
//

class AppAction;

class AppActionController {
private:
  bool       _doAction;
  AppAction* _theAction;
  
public:
  
  AppActionController(AppAction* anAction);
  virtual ~AppActionController();  
  
  void setDoAction(bool theValue);
  bool doAction();
  AppAction* action();
  
  // Replicate/wrap AppAction interface here
  
  virtual void beforeBeginJob(AppExecutable*, AbsEvent*);
  
  virtual void afterBeginJob (AppExecutable*, AbsEvent*);
  
  virtual void beforeBeginRun(AppExecutable*, AbsEvent*);
  
  virtual void afterBeginRun (AppExecutable*, AbsEvent*);
  
  virtual void beforeEvent   (AppExecutable*, AbsEvent*);

  virtual void afterEvent    (AppExecutable*, AbsEvent*);
  
  virtual void beforeOther   (AppExecutable*, AbsEvent*);

  virtual void afterOther    (AppExecutable*, AbsEvent*);
  
  virtual void beforeEndRun  (AppExecutable*, AbsEvent*);
  
  virtual void afterEndRun   (AppExecutable*, AbsEvent*);

  virtual void beforeEndJob  (AppExecutable*, AbsEvent*);
  
  virtual void afterEndJob   (AppExecutable*, AbsEvent*);
  
private:

  // Not implemented.
  AppActionController( const AppActionController& );
  AppActionController& operator=( const AppActionController& );

  
};

#endif








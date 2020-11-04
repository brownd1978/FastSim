//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppAction.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Action Class Description.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// See the AppModuleCommand header file for more details on commands
// related to this class such as "module actions enable all" etc...
//
// Author List:
//	Marc Turcotte	                Original Author          May '97
//      Marc Turcotte                   Removed dependence on RW 
//                                      to allow sharing.        June '97
//      Marc Turcotte                   Modified for new style begin/end 2/98
//      Marc Turcotte                   Added name() member function 4/10/98
//      Rainer Bartoldus                Added before/afterOther() 7/18/2001
//
// Copyright Information:
//	Copyright (C) 1997,1998		University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPACTION_HH
#define APPACTION_HH

//
// Collaborating Class Headers
//
#include <string>

class AbsEvent;
class AppExecutable;
class AppFramework;

class AppAction{

public:
  AppAction();
  AppAction(std::string);
  std::string name() const;
  virtual ~AppAction();
  int operator==(const AppAction& theAction) const;
  
  virtual void report( bool ) const;
  

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
  
  void setFramework(const AppFramework* theFrame) { 
    _framework = const_cast<AppFramework*>(theFrame);
  } 

protected:
  
  std::string _actionName;

  const AppFramework* framework( ) const {return _framework;}

private:
  // Not implemented.
  AppAction( const AppAction& );
  AppAction& operator=( const AppAction& );

  AppFramework* _framework;

};

#endif

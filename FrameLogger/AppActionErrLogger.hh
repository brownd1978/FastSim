#ifndef APPACTIONERRLOGGER_HH
#define APPACTIONERRLOGGER_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppActionErrLogger.hh 507 2010-01-14 15:17:33Z stroili $
// 
// Description:
//	AppActionErrLogger
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen
//
// Copyright Information:
//	Copyright (C) 1998       LBNL
//
//------------------------------------------------------------------------

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppAction.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppExecutable;
class AppModule;
class AbsEvent;
class AppErrLog;

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppActionErrLogger : public AppAction {
  public:
    AppActionErrLogger();
    virtual ~AppActionErrLogger();

    virtual void beforeBeginJob(AppExecutable*, AbsEvent*);
    virtual void beforeBeginRun(AppExecutable*, AbsEvent*);
    virtual void    beforeEvent(AppExecutable*, AbsEvent*);
    virtual void    beforeOther(AppExecutable*, AbsEvent*);
    virtual void   beforeEndRun(AppExecutable*, AbsEvent*);
    virtual void   beforeEndJob(AppExecutable*, AbsEvent*);

    virtual void  afterBeginJob(AppExecutable*, AbsEvent*);
    virtual void  afterBeginRun(AppExecutable*, AbsEvent*);
    virtual void     afterEvent(AppExecutable*, AbsEvent*);
    virtual void     afterOther(AppExecutable*, AbsEvent*);
    virtual void    afterEndRun(AppExecutable*, AbsEvent*);
    virtual void    afterEndJob(AppExecutable*, AbsEvent*);

    friend class AppErrLog;

  protected:
    virtual void record(AppExecutable*);

    static AppExecutable * currentExec() { return _currentExec;}
    static AppModule * currentModule() { return _currentModule; }
    // constructor for sub-classes
    AppActionErrLogger(AppErrLog * _logger);
      // transfer ownership of logger

  private:
    static AppExecutable * _currentExec;
    static AppModule * _currentModule;
    AppErrLog * _logger;
};

#endif // APPACTIONERRLOGGER_HH

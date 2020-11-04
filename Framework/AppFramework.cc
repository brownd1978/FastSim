//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFramework.cc 636 2010-04-29 14:05:25Z stroili $
//
// Description:
//      Class AppFramework. This is the overall framework for the BaBar
//      analysis framework. It controls the creation of sequences and
//      paths and the processing of events.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie	        Original Author
//      Marc Turcotte                   Introduced Actions '97
//                                      Removed dependency on DbiEvent 6/97
//                                      Enabled/modified other entry  12/97
//                                      Modified for new style begin/end 2/98
//      Marc Turcotte                   Added new style event 2/23
//      Marc Turcotte                   Added Dynamic Dispatch support 3/98
//                                      This work is done in collaboration
//                                      with CLEO.
//      Marc Turcotte                   Introduced AppStopType 3/20/98
//      Marc Turcotte                   Let zero frame thru frame()
//                                      so that null goes to module
//                                      which can have overriden the
//                                      frame() function and be a
//                                      provider instead of a consumer 4/8/98
//      Marc Turcotte                   Introduced ActionControllers 4/14/98
//      Marc Turcotte                   Introduced static _theFrame init 
//                                      4/30/98
//      Marc Turcotte                   Introduced static _theAbsEvent init
//                                      but /* */ it for migration 4/30/98
//      Marc Turcotte                   Introduced theEvent() & zeroTheEvent()
//                                      4/30/98
//      Marc Turcotte                   Merged in R.Kapur's TK interface 5/98
//      Marc Turcotte                   Added _TkInterface init 6/18/98
//      Marc Turcotte                   Modified for AppResult 6/19/98
//      Marc Turcotte                   Added theDispatchStopType to
//                                      signatures 6/23/98
//      Marc Turcotte                   Changed AppResult for AppResult 6/23/98
//      Marc Turcotte                   Changed for new AbsEvent sigs 6/24/98
//      Marc Turcotte			Migrated setupTheApp from AppMain 
//                                      7/28/98
//      Marc Turcotte                   Introduced non const heap temporaries
//                                      moduleName, pathName and execName 
//                                      copied into for safety reasons. 7/30/98
//      Marc Turcotte                   Added arg list to setupTheApp 8/6/98
//      Marc Turcotte                   Removed call to other(...) from
//                                      continueHandler(...) 9/23/98
//      A. De Silva                     Actions made to work on input & output
//                                      modules. 6/24/99
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997, 1998        University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppAction.hh"
#include "AbsEvent/AbsEvent.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppFramework.hh"
// note the above header now includes AbsCommandMemento.hh which in 
// turn includes iostream.h.  Including iostream.h is a very heavy thing
// to do and a consequence is that this AppFramework must be ordered after
// the timer stuff. -LSK
#include "Framework/AppExecNode.hh"
#include "Framework/AppPath.hh"
#include "Framework/AppSequence.hh"
#include "Framework/AppInputModule.hh"
#include "Framework/AppFileInput.hh"
#include "Framework/AppOutputModule.hh"
#include "Framework/AppFileOutputModule.hh"
#include "Framework/AppCommand.hh"
#include "Framework/AppEventsCommand.hh"
#include "Framework/AppModuleCommand.hh"
#include "Framework/AppPathCommand.hh"
#include "Framework/AppSequenceCommand.hh"
#include "Framework/AppActionCommand.hh"
#include "Framework/AppHash.hh"

#include "Framework/AppActionController.hh"

#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"

#include "FrameUtil/AbsInterp.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/resource.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <sstream>
using std::ostringstream;

#include "CommonUtils/ComPathNameSearch.hh"
#include "CommonUtils/ComTimeStamp.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//-----------------------
// Static Initializers --
//-----------------------

AppFrame* AppFramework::_theFrame = 0;
AppStopType AppFramework::_theDispatchStopType = AppStopType::never_assigned;

//----------------
// Constructors --
//----------------

namespace {
  // This function is for use only by Framework.
  // Here, Item is always a pointer and has a name() method.  Use it with care.
  template<class Item>
    void
    FwkkListDeleteAll(APPList<Item*> & alist, AppFramework* theFrame,
        const char* itemType);
}

AppFramework::AppFramework(int argc, char* argv[]) :
  AppNoCloneModule("AppFrame", "Application Framework"), _exitRequested(false),
      _theOutputModule(0), _isBegun(false), _maxNameLength(0), _argc(argc),
      _argv(argv), _stopRequested(false), _eventsCmd("events", this),
      _moduleCmd("module", this), _pathCmd("path", this), _sequenceCmd(
          "sequence", this), _configCmd("config", this), _actionCmd("action",
          this), _strictModuleDisable("strictModuleDisable", this, false)
{

  _execType = APP_framework;
  setFramework(this);

  _executables = new AppHash<AppExecutable> ;

  _execNodes = new APPList<AppExecNode*> ;
  _modules = new APPList<AppModule*> ;
  _paths = new APPList<AppPath*> ;
  _sequences = new APPList<AppSequence*> ;
  _cloneList = new APPList<AppCloneRecorder *> ;

  _inputModules = new APPList<AppInputModule*> ;
  _outputModules = new APPList<AppOutputModule*> ;

  _localInputModule = _theInputModule = new AppFileInput("DummyInput",
      "dummy input module");

  add(_theInputModule);

  _localOutputModule = _theOutputModule = new AppFileOutputModule;
  add(_theOutputModule);

  _defaultPath = new AppPath("AllPath", "Default (all modules) path");
  _defaultPath->setEnabled(false);
  add(_defaultPath);

  setPrompt("> ");
  commands()->append(&_eventsCmd);
  commands()->append(&_moduleCmd);
  commands()->append(&_pathCmd);
  commands()->append(&_sequenceCmd);
  commands()->append(&_configCmd);
  commands()->append(&_actionCmd);
  commands()->append(&_strictModuleDisable);
  _actions = new APPList<AppAction*> ; // Holds the list of Actions
  _theActionControllers = new APPList<AppActionController*> ;

  // Grab bag of AppMethods. This is not the Module hash table...
  _theAppMethodsGrabBag = new APPList<AppMethodBase*> ;

  // Create an Abstract Interpreter by calling it's class static
  // instance method.  The AbsInterp follows an extension of the
  // singleton pattern.  Note that class statics are similar to globals
  // in that one can gain access to the single instance from anywhere in
  // the program by calling AbsInterp::theInterpreter( ).
  AbsInterp* interp;
  interp = AbsInterp::theInterpreter();

}

//--------------
// Destructor --
//--------------

AppFramework::~AppFramework()
{
  //  print resource usage statistics
  printRusage();

  delete _executables;

  APPListDeleteAll(*_execNodes);
  delete _execNodes;

  ::FwkkListDeleteAll(*_modules, this, "module");
  delete _modules;

  ::FwkkListDeleteAll(*_paths, this, "path");
  delete _paths;

  ::FwkkListDeleteAll(*_sequences, this, "sequence");
  delete _sequences;

  ::FwkkListDeleteAll(*_inputModules, this, "input module");
  delete _inputModules;

  ::FwkkListDeleteAll(*_outputModules, this, "output module");
  delete _outputModules;

  ::FwkkListDeleteAll(*_actions, this, "action");
  delete _actions;

  APPListDeleteAll(*_theActionControllers);
  delete _theActionControllers;

  APPListDeleteAll(*_theAppMethodsGrabBag);
  delete _theAppMethodsGrabBag;

  APPListDeleteAll(*_cloneList);
  delete _cloneList;

  // Delete the singletons:
  delete AbsInterp::theInterpreter();

}

//--------------
// Operations --
//---------------

AppResult
AppFramework::beginJob(AbsEvent* anEvent)
{
  AppModule** mod;
  AbsEvent* nullEvent = 0;

  // Build a list with Input, Output and App Modules all stored
  // as AppModules (common base class)

  APPList<AppModule*> allAsModules;
  buildModuleList(allAsModules, *inputModules());
  buildModuleList(allAsModules, *outputModules());
  buildModuleList(allAsModules, *modules());

  // Now use that list to give beginJob to those modules

  APPListIterator<AppModule*> iter1(allAsModules);
  APPListIterator<AppActionController*> itr(*actionControllers());
  while ((mod = iter1())) {
    if ((*mod)->isEnabled() && !(*mod)->isInitialized()) {
      AppActionController** anActionController;
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->beforeBeginJob(*mod, anEvent);
      }
      (*mod)->beginJob(anEvent);
      (*mod)->setInitialized();
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->afterBeginJob(*mod, anEvent);
      }
    }
  }

  if (0 != theInputModule()) {
    theInputModule()->beginRun(nullEvent);
  }
  if (0 != theOutputModule() && theOutputModule()->isEnabled()) {
    theOutputModule()->beginRun(nullEvent);
  }

  return AppResult::OK;

}

AppResult
AppFramework::beginRun(AbsEvent* anEvent)
{
  AppModule** mod;

  // Build a list with Input, Output and App Modules all stored
  // as AppModules (common base class)
  APPList<AppModule*> allAsModules;
  buildModuleList(allAsModules, *inputModules());
  buildModuleList(allAsModules, *modules());
  buildModuleList(allAsModules, *outputModules());

  // Now use that list to give beginRun to those modules

  APPListIterator<AppModule*> iter1(allAsModules);
  APPListIterator<AppActionController*> itr(*actionControllers());
  while ((mod = iter1())) {
    if ((*mod)->isEnabled()) {
      AppActionController** anActionController;
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->beforeBeginRun(*mod, anEvent);
      }
      (*mod)->beginRun(anEvent);
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->afterBeginRun(*mod, anEvent);
      }
    }
  }

  return AppResult::OK;

}

AppResult
AppFramework::frame(AppFrame* aFrame, const AppStopType& theDispatchStopType)
{ // Dynamic dispatch

  AppPath** path;

  resetNodes(); // This will do all the nodes "again" but _no_ standard member functions
  // will be called; the user is expected to NOT bind those... Beware...
  resetAppMethods(false); // Reset the AppMethods for this event

  APPListIterator<AppPath*> theIterator(*paths());
  AppResult theResult = AppResult::OK;
  while ((path = theIterator())) {
    if ((*path)->isEnabled()) {
      theResult = (*path)->frame(aFrame, theDispatchStopType);
    }
  }

  return theResult.value();

}

AppResult
AppFramework::outputEvent(AbsEvent* theEvent)
{
  if (0 != theOutputModule() && theOutputModule()->isEnabled()) {
    APPListIterator<AppActionController*> itr1(*actionControllers());
    AppActionController** anActionController;
    while ((anActionController = itr1.next())) {
      (*anActionController)->beforeEvent(theOutputModule(), theEvent);
    }
    itr1.rewind();
    theOutputModule()->outputEvent(theEvent);
    while ((anActionController = itr1.next())) {
      (*anActionController)->afterEvent(theOutputModule(), theEvent);
    }
  }
  return AppResult::OK;
}

AppResult
AppFramework::event(AbsEvent* anEvent)
{
  AppPath** path;

  assert ( 0 != anEvent );

  resetNodes();

  APPListIterator<AppPath*> theIterator(*paths());
  while ((path = theIterator())) {
    if ((*path)->isEnabled()) {
      (*path)->event(anEvent);
    }
  }
  return AppResult::OK;
}

AppResult
AppFramework::other(AbsEvent* anOther)
{

  // As per request of Online, make "other run like path" meaning
  // that Framework::other(AbsEvent*) should act like
  // Framework::event(AbsEvent*) and not be called on disabled modules
  // and be called in "path order". M. Turcotte 12/2/97
  // This "other" deal will be phased out in favor of
  // frames. 6/25/98


  AppPath** path;
  assert ( 0 != anOther );

  resetNodes();
  APPListIterator<AppPath*> theIterator(*paths());
  while ((path = theIterator())) {
    if ((*path)->isEnabled()) {
      (*path)->other(anOther); // "other" processing...
    }
  }

  return AppResult::OK;

}

AppResult
AppFramework::endRun(AbsEvent* anEvent)
{
  AppModule** mod;

  //  Assertion removed because of the use case when the user
  //  ends the run without beginning.
  //  M. Turcotte, UTDallas, 2/13/98
  //
  //    assert ( 0 != anEvent );

  // Build a list with Input, Output and App Modules all stored
  // as AppModules (common base class)
  APPList<AppModule*> allAsModules;
  buildModuleList(allAsModules, *modules());
  buildModuleList(allAsModules, *inputModules());
  buildModuleList(allAsModules, *outputModules());

  // Now use that list to give endRun to those modules

  APPListIterator<AppModule*> theIterator(allAsModules);
  APPListIterator<AppActionController*> itr(*actionControllers());
  while ((mod = theIterator())) {
    if ((*mod)->isEnabled() && (*mod)->isInitialized()) {
      AppActionController** anActionController;
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->beforeEndRun(*mod, anEvent);
      }
      (*mod)->endRun(anEvent);
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->afterEndRun(*mod, anEvent);
      }
    }
  }
  return AppResult::OK;

}

AppResult
AppFramework::endJob(AbsEvent* anEvent)
{
  AppModule** mod;

  //   This assertion had to be removed because of the use
  //   case when the use just starts up the framework and
  //   then quits without begining...
  //   M. Turcotte, UTDallas, 2/13/98
  //
  //   assert ( 0 != anEvent );

  // Build a list with Input, Output and App Modules all stored
  // as AppModules (common base class)
  APPList<AppModule*> allAsModules;
  buildModuleList(allAsModules, *inputModules());
  buildModuleList(allAsModules, *outputModules());
  buildModuleList(allAsModules, *modules());

  // Now use that list to give beginJob to those modules

  APPListIterator<AppModule*> theIterator(allAsModules);
  APPListIterator<AppActionController*> itr(*actionControllers());
  //  while ( mod = theIterator( ) ) {
  // Go to the end and iterate backwords
  theIterator.skipAll();
  while ((mod = theIterator.prev())) {
    if ((*mod)->isEnabled() && (*mod)->isInitialized()) {
      AppActionController** anActionController;
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->beforeEndJob(*mod, anEvent);
      }
      (*mod)->endJob(anEvent);
      itr.rewind();
      while ((anActionController = itr.next())) {
        (*anActionController)->afterEndJob(*mod, anEvent);
      }
    }
  }

  return AppResult::OK;

}

AppResult
AppFramework::abortJob(AbsEvent* anEvent)
{
  AppModule** mod;

  //  Assertion removed because of a use case when the
  //  user aborts without beginning...
  //  M. Turcotte, UTDallas, 2/13/98
  //
  //    assert ( 0 != anEvent );

  APPListIterator<AppModule*> theIterator(*modules());
  while ((mod = theIterator())) {
    if ((*mod)->isEnabled() && (*mod)->isInitialized()) {
      (*mod)->abortJob(anEvent);
    }
  }
  if (0 != theOutputModule() && theOutputModule()->isEnabled()) {
    theOutputModule()->abortJob(anEvent);
  }

  return AppResult::OK;

}

void
AppFramework::talkTo()
{
  AbsInterp* interp = AbsInterp::theInterpreter();
  assert( interp->isInitialized( ) );

  interp->runCommandFile((const char*) ComPathNameSearch(
      "Framework/BaBar_FrameworkRc.tcl"));

  _exitRequested = false;
  enableCommands();

  if (2 <= _argc) {
    interp->runCommandFile(_argv[1]);
  }

  while (!_exitRequested) {
    interp->startInterpLoop();
  }

  // We cannot do this in BaBar since (from gpdf)
  // OEP and OPR jobs do all their event processing after talkTo()
  // endJob( anEvent );

  disableCommands();

}

void
AppFramework::help(int argc, char** argv)
{
  AppModule::help(argc, argv);
  if (argc == 1) {
    fullReport("");
    fullReport(
        "For more information on a particular command type '<command> help'.");
    fullReport("A minimal set to select an input file and run 5 events is:");
    fullReport("    module talk FileInput");
    fullReport("      input file <your_file>");
    fullReport("      exit");
    fullReport("    ev begin -nev 5");
    fullReport("    exit");
    fullReport("");
    fullReport(
        "A script can also be specified as the first argument when running the program.");
    fullReport("");
    fullReport("In addition the following command aliases can also be used.");
    fullReport("");
    fullReport("Alias: Command Equivalent:");
    AbsInterp* interp = AbsInterp::theInterpreter();
    interp->simulateInput("alias");
  }
}

void
AppFramework::show(int argc, char** argv) const
{
  AbsInterp* interp = AbsInterp::theInterpreter();
  std::string argStr;
  if (argc > 1) {
    argStr = argv[1]; // Just use first arg.
  } else {
    argStr = " "; // No args means show everything
  }
  if ((argc == 1) || (argStr == "module")) {
    fullReport("**** Listing of all available modules ****");
    fullReport(" ");
    interp->simulateInput("module list");
  }
  if ((argc == 1) || (argStr == "path")) {
    fullReport(" ");
    fullReport("**** Listing of all defined paths ****");
    fullReport(" ");
    interp->simulateInput("path list");
  }
  if ((argc == 1) || (argStr == "par")) {
    fullReport(" ");
    fullReport("**** Current value of items in the Framework ****");
    fullReport(" ");
    AppCommand** command;
    APPListIterator<AppCommand*> theIterator(*commands());
    while ((command = theIterator())) {
      if ((*command)->isShowable()) {
        partialReport("      ");
        (*command)->show();
      }
    }
  }
  if ((argc == 1) || (argStr == "action")) {
    fullReport(" ");
    fullReport("**** Listing of all actions ****");
    fullReport(" ");
    interp->simulateInput("action list");
  }
}

void
AppFramework::exit()
{
  _exitRequested = true;
  AbsInterp* interp = AbsInterp::theInterpreter();
  interp->setDoLoopInterp(false); // terminate interpreter loop
}

void
AppFramework::beginHandler(int nEvents)
{
  AbsEvent* anEvent(0);
  _theFrame = 0;
  _theDispatchStopType = AppStopType::never_assigned;

  //-----------------------------------------------------------------------
  //  This code will not work (for events) until we change the logic of input modules
  //  and putting it in would require moving the input call in the main event loop to
  //  _after_ the walk and output calls. But if this could be done, it would allow having
  //  an event or frame in hand to be handed over to beginJob.
  //
  //    if ( 0 != theInputModule() ) {
  //      if ( _enableFrames.value() ) {
  //        AppResult theResult = theInputModule()->inputFrame( _theFrame, _theDispatchStopType ); // Get a frame in...
  //      } else {
  //        AppResult theResult = theInputModule()->inputEvent( anEvent );                    // Get an event in...
  //      }
  //    }
  //-----------------------------------------------------------------------

  beginJob(anEvent);
  _isBegun = true;

  continueHandler(nEvents);
}

void
AppFramework::continueHandler(int nEvents)
{

  AbsEvent* anEvent(0);
  int number = 0;
  int numberOfFrames = 0;

  if (!_isBegun) {
    if (0 != anEvent) {
      delete anEvent;
      anEvent = 0;
    }
    _theFrame = 0;
    _theDispatchStopType = AppStopType::never_assigned;

    //**********************************************************************************************************************
    //  This code will not work until we change the logic of input modules in BaBar
    //  and putting it in would require moving the input call in the main event loop to
    //  _after_ the walk and output calls. But if this could be done, it would allow having
    //  an event or frame in hand to be handed over to beginJob.
    //
    //      anAbsEvent         = 0;
    //      _theFrame            = 0;
    //      _theDispatchStopType = AppStopType::never_assigned;
    //
    //      if ( 0 != theInputModule() ) {
    //
    //        if ( _enableFrames.value() ) {
    //          AppResult theResult = theInputModule()->inputFrame( _theFrame, _theDispatchStopType ); // Get Frame in...
    //          number++;
    //        } else {
    //          AppResult theResult = theInputModule()->inputEvent( anEvent );                    // Get an event in...
    //          number++;
    //        }
    //
    //      }
    //**********************************************************************************************************************

    beginJob(anEvent);
    _isBegun = true;

  }

  _stopRequested = false;

  //
  // Move frames...
  //
  if (_enableFrames.value()) {

    numberOfFrames = 0;

    do {

      AppResult theResult = theInputModule()->inputFrame(_theFrame,
          _theDispatchStopType); // Get a frame in...
      numberOfFrames++;

      if (_theFrame != 0) {
        theResult = frame(_theFrame, _theDispatchStopType); // Walk frame...

        if (theResult.value() != AppResult::OK) {
          fullReport(
              "AppFramework:: Bad result code on return from a frame consumer: != OK");
        }

        if (0 != theOutputModule() && theOutputModule()->isEnabled()) {
          theOutputModule()->outputFrame(_theFrame); // Push the frame out...
        }
      }

    } while (!_stopRequested && (nEvents <= 0 || numberOfFrames < nEvents));

  }

  //
  //  Move standard AbsEvent
  //

  if (0 != theInputModule() && !_enableFrames.value()) {

    number = 0;
    bool absEventZero = false;
    AppActionController** anActionController;
    APPListIterator<AppActionController*> itr1(*actionControllers());
    APPListIterator<AppActionController*> itr2(*actionControllers());

    do {

      while ((anActionController = itr1.next())) {
        (*anActionController)->beforeEvent(theInputModule(), anEvent);
      }
      itr1.rewind();
      theInputModule()->inputEvent(anEvent, _theDispatchStopType);
      while ((anActionController = itr1.next())) {
        (*anActionController)->afterEvent(theInputModule(), anEvent);
      }
      itr1.rewind();

      if (0 != anEvent) {

        //
        // Handle the traditional dispatch here
        //

        if (_theDispatchStopType == AppStopType::physics_event) {
          event(anEvent);
          number++;
        } else if (_theDispatchStopType == AppStopType::begin_run) {
          //  Begin Run
          beginRun(anEvent);

        } else if (_theDispatchStopType == AppStopType::end_run) {
          //  End Run
          endRun(anEvent);

        } else {
          //  Other record type
          other(anEvent);
        }
        outputEvent(anEvent);
      } else {
        absEventZero = true;
      }
    } while (!_stopRequested && !absEventZero && (nEvents <= 0 || number
        < nEvents));

    if (absEventZero) {
      _isBegun = false;
    }

    // Need this to fix memory leak
    if (anEvent != 0) theInputModule()->deleteEvent(anEvent);

  }
}

void
AppFramework::printName(const char* const theName, int mode)
{
  partialReport(theName);
  size_t len = strlen(theName);
  if (0 != mode) {
    ostringstream bufstream;
    bufstream << mode;
    partialReport(" (");
    partialReport(bufstream.str().c_str());
    partialReport(")");
    len = len + 4;
  }
  for (size_t i = len - 4; i < _maxNameLength; i++) {
    partialReport(" ");
  }
}

//-------------
// Selectors --
//-------------

AppExecutable*
AppFramework::fetch(const char* const aName) const
{

  assert ( 0 != aName );

  return _executables->fetch(aName);
}

AppModule*
AppFramework::fetchModule(const char* const aName) const
{
  AppExecutable* anExec;
  AppExecutable* theExec = 0;

  assert ( 0 != aName );

  if (0 != (anExec = _executables->fetch(aName))) {
    theExec = AppExecutable::narrow(anExec, AppExecutable::APP_module);
    if (0 == theExec) {
      theExec = AppExecutable::narrow(anExec, AppExecutable::APP_filter);
    }
    if (0 == theExec) {
      theExec = AppExecutable::narrow(anExec, AppExecutable::APP_input);
    }
    if (0 == theExec) {
      theExec = AppExecutable::narrow(anExec, AppExecutable::APP_output);
    }
  }
  return (AppModule*) theExec;
}

AppPath*
AppFramework::fetchPath(const char* const aName) const
{
  AppExecutable* anExec;

  assert ( 0 != aName );

  if (0 != (anExec = _executables->fetch(aName))) {
    anExec = AppExecutable::narrow(anExec, AppExecutable::APP_path);
  }
  return (AppPath*) anExec;
}

AppSequence*
AppFramework::fetchSequence(const char* const aName) const
{
  AppExecutable* anExec;

  assert ( 0 != aName );

  if (0 != (anExec = _executables->fetch(aName))) {
    anExec = AppExecutable::narrow(anExec, AppExecutable::APP_sequence);
  }
  return (AppSequence*) anExec;
}

bool
AppFramework::has(const char* const aName) const
{

  assert ( 0 != aName );

  return (_executables->has(aName));
}

bool
AppFramework::hasModule(const char* const aName) const
{
  AppExecutable* anExec;
  AppExecutable* theExec = 0;

  assert ( 0 != aName );

  if (0 != (anExec = _executables->fetch(aName))) {
    theExec = AppExecutable::narrow(anExec, AppExecutable::APP_module);
    if (0 == theExec) {
      theExec = AppExecutable::narrow(anExec, AppExecutable::APP_filter);
    }
  }
  return (0 != theExec);
}

bool
AppFramework::hasPath(const char* const aName) const
{
  AppExecutable* anExec;

  assert ( 0 != aName );

  if (0 != (anExec = _executables->fetch(aName))) {
    anExec = AppExecutable::narrow(anExec, AppExecutable::APP_path);
  }
  return (0 != anExec);
}

bool
AppFramework::hasSequence(const char* const aName) const
{
  AppExecutable* anExec;

  assert ( 0 != aName );

  if (0 != (anExec = _executables->fetch(aName))) {
    anExec = AppExecutable::narrow(anExec, AppExecutable::APP_sequence);
  }
  return (0 != anExec);
}

AppExecutable*
AppFramework::execFromName(const char* const theName) const
{
  AppExecutable* theExec;
  theExec = fetchModule(theName);
  if (0 == theExec) {
    theExec = fetchSequence(theName);
  }
  return theExec;
}

//-------------
// Modifiers --
//-------------

void
AppFramework::setBegun(bool state)
{
  _isBegun = state;
}

bool
AppFramework::setInputModule(const char* const aName)
{
  AppExecutable* anExec;
  AppInputModule* theInput;
  bool result = false;

  if (0 != (anExec = fetchModule(aName))) {
    if (0 != (theInput = (AppInputModule*) AppExecutable::narrow(anExec,
        AppExecutable::APP_input))) {
      if (0 != _theInputModule) {
        if (_theInputModule != theInput) {
          if (_theInputModule->isInitialized()) {
          }
          _theInputModule->setEnabled(false);
        }
      }
      _theInputModule = theInput;
      _theInputModule->setEnabled(true);
      result = true;
    }
  }
  return result;
}

void
AppFramework::setTheInputModule(AppInputModule* thatOne)
{
  APPListIterator<AppInputModule*> iter(*inputModules());
  AppInputModule** aMod;
  while ((aMod = iter())) {
    if (*aMod == thatOne) {
      _theInputModule = thatOne;
      break;
    }
  }
}

bool
AppFramework::setOutputModule(const char* const aName)
{
  AppExecutable* anExec;
  AppOutputModule* theOutput;
  bool result = false;

  if (0 != (anExec = fetchModule(aName))) {
    if (0 != (theOutput = (AppOutputModule*) AppExecutable::narrow(anExec,
        AppExecutable::APP_output))) {
      if (0 != _theOutputModule) {
        if (_theOutputModule != theOutput) {
          if (_theOutputModule->isInitialized()) {
          }
          _theOutputModule->setEnabled(false);
        }
      }
      _theOutputModule = theOutput;
      _theOutputModule->setEnabled(true);
      result = true;
    }
  }
  return result;
}

void
AppFramework::add(const AppExecNode* const aNode)
{

  assert ( 0 != aNode );

  _execNodes->append((AppExecNode*) aNode);
}

void
/* !!!
 F_createTree( _theFileNode );
 */
AppFramework::add(AppModule* const aModule)
{

  assert ( 0 != aModule );

  if (_executables->add(aModule->name(), aModule)) {
    _modules->append(aModule);
    aModule->setFramework(this);
    if (_defaultPath != 0) _defaultPath->append(aModule);
    if (strlen(aModule->name()) > _maxNameLength) {
      _maxNameLength = strlen(aModule->name());
    }
  } else {
    if (verbose()) {
      // This will never be printed out since verbose is set to f by default
      // and cannot be reset until the ">" prompt.  However, the ">" only 
      // happens after the modules have been added to Framework from 
      // AppuserBuild.
      partialReport("Module ");
      partialReport(aModule->name());
      fullReport(" already exists");
    }
    delete aModule;
  }
}

void
AppFramework::insert(AppModule* aModule, const AppModule* before)
{

  assert ( 0 != aModule );

  if (_executables->add(aModule->name(), aModule)) {
    _modules->insert(aModule, const_cast<AppModule*> (before));
    aModule->setFramework(this);
    if (_defaultPath != 0) _defaultPath->append(aModule);
    if (strlen(aModule->name()) > _maxNameLength) {
      _maxNameLength = strlen(aModule->name());
    }
  } else {
    if (verbose()) {
      // This will never be printed out since verbose is set to f by default
      // and cannot be reset until the ">" prompt.  However, the ">" only 
      // happens after the modules have been added to Framework from 
      // AppuserBuild.
      partialReport("Module ");
      partialReport(aModule->name());
      fullReport(" already exists");
    }
    delete aModule;
  }
}

void
AppFramework::add(AppInputModule* const aModule)
{

  assert ( 0 != aModule );

  if (_executables->add(aModule->name(), aModule)) {
    _inputModules->append(aModule);
    aModule->setFramework(this);
    if (strlen(aModule->name()) > _maxNameLength) {
      _maxNameLength = strlen(aModule->name());
    }
  } else {
    if (verbose()) {
      partialReport("Module ");
      partialReport(aModule->name());
      fullReport(" already exists");
    }
    delete aModule;
  }
}

void
AppFramework::add(AppOutputModule* const aModule)
{

  assert ( 0 != aModule );

  if (_executables->add(aModule->name(), aModule)) {
    _outputModules->append(aModule);
    aModule->setFramework(this);
    if (strlen(aModule->name()) > _maxNameLength) {
      _maxNameLength = strlen(aModule->name());
    }
  } else {
    if (verbose()) {
      partialReport("Module ");
      partialReport(aModule->name());
      fullReport(" already exists");
    }
    delete aModule;
  }
}

void
AppFramework::add(AppPath* const aPath)
{

  assert ( 0 != aPath );

  if (_executables->add(aPath->name(), aPath)) {
    _paths->append(aPath);
    aPath->setFramework(this);
    if (strlen(aPath->name()) > _maxNameLength) {
      _maxNameLength = strlen(aPath->name());
    }
  } else {
    partialReport("Path ");
    partialReport(aPath->name());
    fullReport(" already exists");
    delete aPath;
  }
}

void
AppFramework::add(AppSequence* const aSequence)
{

  assert ( 0 != aSequence );

  if (_executables->add(aSequence->name(), aSequence)) {
    _sequences->append(aSequence);
    aSequence->setFramework(this);
    if (strlen(aSequence->name()) > _maxNameLength) {
      _maxNameLength = strlen(aSequence->name());
    }
  } else {
    partialReport("Sequence ");
    partialReport(aSequence->name());
    fullReport(" already exists");
    delete aSequence;
  }
}
void
AppFramework::add(AppCloneRecorder* const aRecorder)
{

  assert ( 0 != aRecorder );
  _cloneList->append(aRecorder);
}

void
AppFramework::add(AppAction* const anAction)
{

  assert ( 0 != anAction );

  bool isNew(true);
  APPListIterator<AppAction*> itr(*actions());
  AppAction** hasAction;
  while ((hasAction = itr())) {
    if ((*hasAction)->name() == anAction->name()) {
      isNew = false;
      break;
    }
  }

  if (isNew) {
    anAction->setFramework(this);
    actions()->append(anAction);
  } else {
    if (verbose()) {
      partialReport("Action ");
      partialReport(anAction->name());
      fullReport(" already exists");
    }
    delete anAction;
  }
}

void
AppFramework::remove(const AppExecNode* const aNode)
{

  assert ( 0 != aNode );

  _execNodes->remove((AppExecNode*) aNode);
}

void
AppFramework::remove(const AppModule* const aModule)
{

  assert ( 0 != aModule );

  _modules->remove((AppModule*) aModule);
  _executables->remove(aModule->name());
}

void
AppFramework::remove(const AppInputModule* const aModule)
{

  assert ( 0 != aModule );

  _modules->remove((AppInputModule*) aModule);
  _executables->remove(aModule->name());
}

void
AppFramework::remove(const AppOutputModule* const aModule)
{

  assert ( 0 != aModule );

  _modules->remove((AppOutputModule*) aModule);
  _executables->remove(aModule->name());
}

void
AppFramework::remove(const AppPath* const aPath)
{

  assert ( 0 != aPath );

  if (aPath == _defaultPath) _defaultPath = 0;

  _paths->remove((AppPath*) aPath);
  _executables->remove(aPath->name());
}

void
AppFramework::remove(const AppSequence* const aSequence)
{

  assert ( 0 != aSequence );

  _sequences->remove((AppSequence*) aSequence);
  _executables->remove(aSequence->name());

  // deleted (nestled) sequences have to be removed
  APPListIterator<AppSequence*> iterSeq(*_sequences);
  AppSequence** aSeq;
  while ((aSeq = iterSeq())) {
    (*aSeq)->remove(aSequence);
  }
  APPListIterator<AppPath*> iterPath(*_paths);
  AppPath** aPath;
  while ((aPath = iterPath())) {
    (*aPath)->remove(aSequence);
  }

}

void
AppFramework::resetAppMethods(bool toThis)
{

  AppMethodBase** anAppMethod;

  APPListIterator<AppMethodBase*> itr(*appMethods()); // use the grab bag...
  while ((anAppMethod = itr.next())) {
    (*anAppMethod)->reset(false);
  } // reset 'em all
}

void
AppFramework::resetNodes()
{
  AppExecNode** node;

  APPListIterator<AppExecNode*> theIterator(*nodes());
  while ((node = theIterator())) {
    (*node)->setExecuted(false);
  }
}

//
// Return unowned pointer to user builder
//
AppUserBuild*
AppFramework::appBuilder()
{
  return _theAppUserBuild;
}

//
// Do any lingering App setup here that the User should
// not be bothered with.
//

void
AppFramework::setupTheApp(AppUserBuild* build)
{
  AbsInterp* absInterp = AbsInterp::theInterpreter();
  _theAppUserBuild = build;

  // Create the Actions Controllers
  APPListIterator<AppAction*> itr(*actions());
  AppAction** anAction;
  while ((anAction = itr())) {
    actionControllers()->append(new AppActionController(*anAction));
  }

  // Build a list with Input, Output and App Modules all stored
  // as AppModules (common base class)

  APPList<AppModule*> allAsModules;
  buildModuleList(allAsModules, *inputModules());
  buildModuleList(allAsModules, *outputModules());
  buildModuleList(allAsModules, *modules());

  // Now use that list to check if commands are redefined ...

  APPListIterator<AppModule*> modIter(allAsModules);
  AppModule** mod;
  disableCommands(); // Framework's
  while ((mod = modIter())) {
    AppCommand** aCommand;
    APPListIterator<AppCommand*> cmdIter(*((*mod)->commands()));
    while ((aCommand = cmdIter())) {
      if (absInterp->existsCommand((*aCommand)->command())) {
        partialReport("AppFramework::setupTheApp(): ");
        partialReport("Error : Redefining command ");
        partialReport((*aCommand)->command());
        partialReport(" in target ");
        partialReport((*mod)->name());
        fullReport("; aborting.");
        ::abort();
      } else {
        (*aCommand)->enable();
      }
    }
    cmdIter.rewind();
    while ((aCommand = cmdIter())) {
      (*aCommand)->disable();
    }
  }
  enableCommands(); // Framework's
}

// These methods iterate over a source list of specific kinds of modules and append them
// to the destination list as their base class, AppModule.  The Destination should not
// own the modules on it.  No protection from duplication is attempted.

void
AppFramework::buildModuleList(APPList<AppModule*>& destination, APPList<
    AppInputModule*>& source)
{

  APPListIterator<AppInputModule*> itr(source);
  AppModule** m;
  while ((m = (AppModule**) itr())) {
    destination.append(*m);
  }
  return;
}

void
AppFramework::buildModuleList(APPList<AppModule*>& destination, APPList<
    AppOutputModule*>& source)
{

  APPListIterator<AppOutputModule*> itr(source);
  AppModule** m;
  while ((m = (AppModule**) itr())) {
    destination.append(*m);
  }
  return;
}

void
AppFramework::buildModuleList(APPList<AppModule*>& destination, APPList<
    AppModule*>& source)
{

  destination.append(source);

  return;
}

bool
AppFramework::strictModuleDisable() const
{
  return _strictModuleDisable.value();
}

namespace {
  // This function is for use only by Framework.
  // alist is a container of pointers and Item has a name() method.
  template<class Item>
    void
    FwkkListDeleteAll(APPList<Item*> & alist, AppFramework* theFrame,
        const char* itemType)
    {
      // order of deletion is important, especially for modules which
      // have dtors calling other modules.  (order is first in, last deleted.)
      APPListIterator<Item*> iter(alist);
      iter.skipAll();
      while (Item** ptrItem = iter.prev()) {
        std::string identity = (*ptrItem)->name();
        if (theFrame->verbose()) {
          theFrame->partialReport(ComTimeStamp());
          theFrame->partialReport(" Before dtor of ");
          theFrame->partialReport(itemType);
          theFrame->partialReport(" ");
          theFrame->fullReport(identity);
        }
        // We are not setting the container's value to 0 now
        //  because we will erase all items shortly.
        delete (*ptrItem);
        if (theFrame->verbose()) {
          theFrame->partialReport(ComTimeStamp());
          theFrame->partialReport(" After  dtor of ");
          theFrame->partialReport(itemType);
          theFrame->partialReport(" ");
          theFrame->fullReport(identity);
        }
      }
      alist.removeAll();
    }
}

void
AppFramework::printRusage()
{
  rusage ru;
  int err = getrusage(RUSAGE_SELF, &ru);
  if (err == 0) {
    std::cout << "====================================================="
        << std::endl;
    std::cout << "|                                                   |"
        << std::endl;
    std::cout << "|           application usage informations          |"
        << std::endl;
    std::cout << "|                                                   |"
        << std::endl;
    std::cout << "====================================================="
        << std::endl;
    std::cout << "user time:   " << ru.ru_utime.tv_sec << " secs "
        << ru.ru_utime.tv_usec << " usecs" << std::endl;
    std::cout << "system time: " << ru.ru_stime.tv_sec << " secs "
        << ru.ru_stime.tv_usec << " usecs" << std::endl;
    std::cout << "max resident set size:        " << ru.ru_maxrss << std::endl;
    std::cout << "integral shared memory size:  " << ru.ru_ixrss << std::endl;
    std::cout << "integral unshared data size:  " << ru.ru_idrss << std::endl;
    std::cout << "integral unshared stack size: " << ru.ru_isrss << std::endl;
    std::cout << "page reclaims:                " << ru.ru_minflt << std::endl;
    std::cout << "page faults:                  " << ru.ru_majflt << std::endl;
    std::cout << "swaps:                        " << ru.ru_nswap << std::endl;
    std::cout << "block input operations:       " << ru.ru_inblock << std::endl;
    std::cout << "block output operations:      " << ru.ru_oublock << std::endl;
    std::cout << "messages sent:                " << ru.ru_msgsnd << std::endl;
    std::cout << "messages received:            " << ru.ru_msgrcv << std::endl;
    std::cout << "signals received:             " << ru.ru_nsignals
        << std::endl;
    std::cout << "voluntary context switches:   " << ru.ru_nvcsw << std::endl;
    std::cout << "involuntary context switches: " << ru.ru_nivcsw << std::endl;
    std::cout << "====================================================="
        << std::endl;
  } else {
    std::cout << "ERROR getting resource usage!" << std::endl;
  }
}

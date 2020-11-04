//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: RecCopyParms.cc 32 2010-01-15 11:45:31Z stroili $
//
// Description:
//      Class RecCopyParms -- utility class to add consistent TCL parameters
//	to modules for modifying tracks and moving them from an input to
//	an output list.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Michael Kelsey <kelsey@slac.stanford.edu
//
// Copyright Information:
//      Copyright (C) 2001  Trustees of Stanford University
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "RecoUtils/RecCopyParms.hh"

#include "AbsParm/AbsParmIfdStrKey.hh"
#include "AbsParm/AbsParmNamedValue.hh"
#include "Framework/AppModule.hh"
#include "ProxyDict/IfdKey.hh"


//---------------------------------
//-- Constructors and Destructor --
//---------------------------------
RecCopyParms::RecCopyParms(AppModule* theModule,
			   const char *inKeyString,
			   const char *outKeyString,
			   const RecCopyParms::Action& initialAction)
  : _inputKey(0), _outputKey(0), _outputAction(0) {
  createParms(theModule, inKeyString, outKeyString, initialAction);
}

// NOTE:  Default ctor will not create TCL parameters

RecCopyParms::RecCopyParms()
  : _inputKey(0), _outputKey(0), _outputAction(0) {}

RecCopyParms::~RecCopyParms() {
  delete _inputKey;
  delete _outputKey;
  delete _outputAction;
}


// Initial configuration

void RecCopyParms::createParms(AppModule* theModule,
			       const char *inKeyString,
			       const char *outKeyString,
			       const RecCopyParms::Action& initialAction) {
  // Sanity checks -- only called once, valid Framework module, valid Action
  assert(_inputKey == 0 && _outputKey == 0 && _outputAction == 0);
  assert(theModule != 0);
  assert(0 <= initialAction && initialAction < RecCopyParms::nActions);

  // Create and register TCL parameters
  _inputKey = new AbsParmIfdStrKey("InputList", theModule, inKeyString);
  theModule->commands()->append(_inputKey);

  _outputKey = new AbsParmIfdStrKey("OutputList", theModule, outKeyString);
  theModule->commands()->append(_outputKey);

  _outputAction =
    new AbsParmNamedValue<RecCopyParms::Action>("OutputAction", theModule);
  theModule->commands()->append(_outputAction);

  // First action in list is default TCL value
  _outputAction->addItem(actionName(initialAction), initialAction);

  // Add remaining actions in order
  for (RecCopyParms::Action a=RecCopyParms::inplace;
       a<RecCopyParms::nActions; 
       a=RecCopyParms::Action(a+1)) {
    if (a != initialAction) _outputAction->addItem(actionName(a), a);
  }
}


// Accessor functions

const IfdKey& RecCopyParms::inputKey() const {
  return _inputKey->value();
}

const IfdKey& RecCopyParms::outputKey() const {
  return _outputKey->value();
}

RecCopyParms::Action RecCopyParms::action() const {
  return _outputAction->value();
}

// Logic flags -- includes both action and list names
bool RecCopyParms::sameAsInput() const {
  return action()==RecCopyParms::inplace;
}

bool RecCopyParms::stealFromInput() const {
  return action()==RecCopyParms::steal;
}

bool RecCopyParms::copyFromInput() const {
  return action()==RecCopyParms::copy;
}

bool RecCopyParms::borrowFromInput() const {
  return action()==RecCopyParms::borrow;
}

bool RecCopyParms::noOutput() const {
  return action()==RecCopyParms::none;
}

bool RecCopyParms::sameLists() const {
  return _inputKey->valueString() == _outputKey->valueString();
}


// Return string corresponding to action flag

const char* RecCopyParms::actionName(const RecCopyParms::Action& anAction) {
  switch (anAction) {
  case RecCopyParms::inplace: return "SameAsInput";
  case RecCopyParms::steal:   return "StealFromInput";
  case RecCopyParms::copy:    return "CopyFromInput";
  case RecCopyParms::borrow:    return "BorrowFromInput";
  case RecCopyParms::none:    return "NoOutput";
  default: return 0;		//*** This may become an assert()
  }
}

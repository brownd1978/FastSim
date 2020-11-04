//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppModuleMemento.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppModuleMemento
//      Do not use this for templated class (foo<T>).  use TemplateTemplate.hh
//      instead.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Akbar Mokhtarani		originator
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1998	LBNL
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppModuleMemento.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "Framework/AppCommand.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppConfigRecorder.hh"
#include "Framework/AppFramework.hh"

#include <iostream>
#include <stdlib.h>
#include <fstream>
using std::endl;
using std::fstream;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AppModuleMemento::AppModuleMemento(AppExecutable* theExec) :
  AbsModuleMemento(theExec), _commandList(new APPList<Command*> )
{
  AppModule* module = (AppModule*) theExec;
  AppCommand** thecommand;
  APPListIterator<AppCommand*> theIterator(*module->commands());
  while ((thecommand = theIterator())) {
    if ((*thecommand)->isShowable()) {
      Command* thiscommand = new Command;
      thiscommand->itsCommand = (*thecommand)->command();
      thiscommand->itsRecorder = new AppConfigRecorder;
      (*thecommand)->dumpState(thiscommand->itsRecorder);
      _commandList->append(thiscommand);
    }
  }

}

//--------------
// Destructor --
//--------------
AppModuleMemento::~AppModuleMemento()
{

  //  APPListDeleteAll(*_commandList); // does not delete the struct members
  Command** aCmd;
  while ((aCmd = _commandList->last())) {
    delete (*aCmd)->itsRecorder;
    Command* tempCmd = *aCmd;
    _commandList->remove(aCmd);
    delete tempCmd;
  }
  delete _commandList;

}
//-------------
// Methods   --
//-------------

void
AppModuleMemento::Dump(ostream& o) const
{
  const char* fs = "#";

  o << endl;
  o << "M" << fs << name() << fs;
  o << (isEnabled() ? "Y" : "N") << fs;
  o << exectype() << fs << _commandList->length();
  o << endl;

  Command** com;
  APPListIterator<Command*> theIterator(*_commandList);
  while ((com = theIterator())) {
    (*com)->itsRecorder->Dump(o, (*com)->itsCommand);
  }
}

void
AppModuleMemento::Dump_tcl(ostream& o) const
{

  o << "module talk " << name() << endl;

  Command** com;
  APPListIterator<Command*> theIterator(*_commandList);
  while ((com = theIterator())) {
    (*com)->itsRecorder->Dump_tcl(o, (*com)->itsCommand);
  }

  o << "  exit " << endl;
  o << "module " << (isEnabled() ? "enable " : "disable ") << name() << endl;

}


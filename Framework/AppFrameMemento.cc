//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppFrameMemento.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppFrameMemento
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
#include "Framework/AppFrameMemento.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "Framework/AppCommand.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppSequence.hh"
#include "Framework/AppPath.hh"
#include "Framework/AppInputModule.hh"
#include "Framework/AppOutputModule.hh"
#include "Framework/AppConfigRecorder.hh"
#include "Framework/AppAction.hh"
#include "Framework/AppCloneRecorder.hh"
#include "Framework/AppFramework.hh"
#include "Framework/AppActionController.hh"
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
AppFrameMemento::AppFrameMemento(AppExecutable* theExec)
  : AppModuleMemento(theExec)
{
}

//--------------
// Destructor --
//--------------
AppFrameMemento::~AppFrameMemento()
{
}

//-------------
// Methods   --
//-------------

void AppFrameMemento::Dump(ostream& o) const
{
  const char* fs = "#";
  AppFramework* framework = exec()->framework();

  o << endl;
  o << "M" << fs << name() << fs;
  o << (isEnabled() ? "Y" : "N") <<  fs ;
  o << exectype() << fs << _commandList->length(); 
  o << endl;

  Command** com;  
  APPListIterator<Command*> theIterator( *_commandList );
  while ( ( com = theIterator( ) ) ) {
    (*com)->itsRecorder->Dump( o, (*com)->itsCommand );
  }
  
  APPListIterator<AppActionController*> iter( *framework->actionControllers());
  AppActionController** anActionController;
  while ( ( anActionController=iter() ) ) {          
    o << "FA" << fs  
      << ((*anActionController)->doAction() ? "Y" : "N") << fs 
      << (*anActionController)->action()->name()
      << endl;
  }

  APPListIterator<AppCloneRecorder*> iter2(*framework->cloneList());
  AppCloneRecorder** theClone;
  while ( ( theClone = iter2() ) ) {
    o << "FC" << fs << (*theClone)->clonename() << fs 
      << (*theClone)->targetname() << endl;
  }

  if (framework->theInputModule() != 0) 
    o << "FI" << fs << framework->theInputModule()->name() << endl;
  if (framework->theOutputModule() != 0) 
    o << "FO" << fs << framework->theOutputModule()->name() << endl;

  AppSequence** seq;
  APPListIterator<AppSequence*> iter3(*framework->sequences());
  while ( ( seq = iter3() ) ) {
    o << "FS" << fs << (*seq)->name() << endl;
  }

  AppPath** path;
  APPListIterator<AppPath*> iter4(*framework->paths());
  while ( ( path = iter4() ) ) {
    if (strcmp((*path)->name(), "AllPath") != 0) { 
      o << "FP" << fs << (*path)->name() << endl;
    }
  }

}

void AppFrameMemento::Dump_tcl(ostream& o)const
{
  AppFramework* framework = exec()->framework();

  Command** com;
  APPListIterator<Command*> theIterator( *_commandList );
  while ( ( com = theIterator( ) ) ) {
    (*com)->itsRecorder->Dump_tcl( o, (*com)->itsCommand );
  }    
  o << endl;
  
  APPListIterator<AppActionController*> iter( *framework->actionControllers());
  AppActionController** anActionController;
  while ( ( anActionController=iter() ) ) {       
    o << "action " 
      << ((*anActionController)->doAction() ? "enable " : "disable ") 
      << "\"" 
      << (*anActionController)->action()->name()
      <<"\""<< endl;
  }

  APPListIterator<AppCloneRecorder*> iter2(*framework->cloneList());
  AppCloneRecorder** theClone;
  while ( ( theClone = iter2() ) ) {
    o << "module clone "
      << (*theClone)->targetname() << " "
      << (*theClone)->clonename() << endl;
  }

  if (framework->theInputModule() != 0) 
    o << "module input " << framework->theInputModule()->name() << endl;
  if (framework->theOutputModule() != 0) 
    o << "module output " << framework->theOutputModule()->name() << endl;

  AppSequence** seq;
  APPListIterator<AppSequence*> iter3(*framework->sequences());
  while ( ( seq = iter3() ) ) {
    o << "sequence create " << (*seq)->name() << endl;
  }

  AppPath** path;
  APPListIterator<AppPath*> iter4(*framework->paths());
  while ( ( path = iter4() ) ) {
    if (strcmp((*path)->name(), "AllPath") != 0) { 
      o << "path create " << (*path)->name() << endl;
    }
  }

}




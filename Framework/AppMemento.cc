//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppMemento.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//	Class AppMemento
//      Do not use this for templated class (foo<T>).  use TemplateTemplate.hh
//      instead.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Akbar Mokhtarani	originator
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
#include "Framework/AppMemento.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "FrameUtil/APPListIterator.hh"
#include "FrameUtil/APPList.hh"
#include "Framework/AppPath.hh"
#include "Framework/AppSequence.hh"
#include "Framework/AppExecutable.hh"
#include "Framework/AppExecNode.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppInputModule.hh"
#include "Framework/AppOutputModule.hh"
#include "Framework/AbsModuleMemento.hh"
#include "Framework/AppPathMemento.hh"
#include "Framework/AppSequenceMemento.hh"
#include "Framework/AppModuleMemento.hh"
#include "Framework/AppFrameMemento.hh"
#include "Framework/AppFramework.hh"
#include "Framework/AppAction.hh"
#include "Framework/AppActionController.hh"
#include "Framework/AppUserBuild.hh"
using std::endl;
using std::fstream;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

AppMemento::AppMemento(AppFramework* framework) :
  _fs("#"), _framework(framework), _execList(new APPList<AbsModuleMemento*> ),
      _deleteAllPath(true)
{
}

AppMemento::~AppMemento()
{
  APPListDeleteAll(*_execList);
  delete _execList;
  APPListDeleteAll(_modulesNotFound);
  APPListDeleteAll(_modules);
}

void
AppMemento::load()
{

  AbsModuleMemento* theFwkMemento = new AppFrameMemento(_framework);
  _execList->append(theFwkMemento);

  AppSequence** seq;
  APPListIterator<AppSequence*> iter2(*_framework->sequences());
  while ((seq = iter2())) {
    AbsModuleMemento* thisSeq = new AppSequenceMemento(*seq);
    _execList->append(thisSeq);
  }

  AppPath** path;
  APPListIterator<AppPath*> iter3(*_framework->paths());
  while ((path = iter3())) {
    if (strcmp((*path)->name(), "AllPath") != 0) { // avoid default path
      AbsModuleMemento* thisPath = new AppPathMemento(*path);
      _execList->append(thisPath);
    } else {
      _deleteAllPath = false;
    }
  }

  APPList<AppModule*> allAsModules;
  _framework->buildModuleList(allAsModules, *_framework->inputModules());
  _framework->buildModuleList(allAsModules, *_framework->outputModules());
  _framework->buildModuleList(allAsModules, *_framework->modules());

  AppModule** mod;
  APPListIterator<AppModule*> iter(allAsModules);
  while ((mod = iter())) {
    AbsModuleMemento* thisMod = new AppModuleMemento(*mod);
    _execList->append(thisMod);
  }

}

void
AppMemento::DumpConfig(ostream& o)
{

  load();

  if (_deleteAllPath) {
    o << endl;
    o << "D" << _fs << "AllPath" << endl;
  }

  AbsModuleMemento** exec;
  APPListIterator<AbsModuleMemento*> iter(*_execList);
  while ((exec = iter())) {
    o << endl;
    (*exec)->Dump(o);
  }

  //dump the list of module names at the end

  APPList<AppModule*> allAsModules;
  _framework->buildModuleList(allAsModules, *_framework->inputModules());
  _framework->buildModuleList(allAsModules, *_framework->outputModules());
  _framework->buildModuleList(allAsModules, *_framework->modules());
  int i = 0;
  o << endl;
  bool newLine = true;
  AppModule** mod;
  APPListIterator<AppModule*> iter1(allAsModules);
  while ((mod = iter1())) {
    if (newLine) {
      o << "L";
      newLine = false;
    }
    o << _fs << (*mod)->name();
    i++;
    if ((i % 5) == 0) { //5 names per line
      o << endl;
      newLine = true;
    }
  }
  if (!newLine) o << endl;

}

void
AppMemento::DumpConfigTcl(ostream& o)
{

  load();

  if (_deleteAllPath) {
    o << endl;
    o << "path delete AllPath" << endl;
  }

  AbsModuleMemento** exec;
  APPListIterator<AbsModuleMemento*> iter(*_execList);
  while ((exec = iter())) {
    o << endl;
    (*exec)->Dump_tcl(o);
  }

}

void
AppMemento::restoreConfig(fstream& infile)
{

  int size = 1024;
  char buff[1024];
  char* var[15];
  bool moduleMode;

  int id;
  for (id = 0; id < 15; id++) {
    var[id] = 0;
  }

  while (!infile.eof()) {
    infile.getline(buff, size);
    if (*buff) { //skip empty line
      parseLine(buff, const_cast<char*> (_fs), var);

      if (strcmp(var[0], "M") == 0) {
        moduleMode = strcmp(var[2], "Y") == 0 ? true : false;
        AppExecutable::ExecType exectype = (AppExecutable::ExecType) (atoi(
            var[3]));
        if (exectype == AppExecutable::APP_framework) {
          restoreFramework(infile);
        } else {
          AppModule* thisMod = _framework->fetchModule(var[1]);
          if (thisMod != 0) {
            thisMod->setEnabled(moduleMode);
            restoreStates(infile, thisMod);
          } else {
            _modulesNotFound.append(new std::string(var[1]));
          }
        }
      }

      else if (strcmp(var[0], "S") == 0) {
        moduleMode = strcmp(var[2], "Y") == 0 ? true : false;
        AppSequence* thisSeq = _framework->fetchSequence(var[1]);
        thisSeq->setEnabled(moduleMode);
        read_seq_nodes(thisSeq, (int) atoi(var[4]), infile);
      }

      else if (strcmp(var[0], "P") == 0) {
        moduleMode = strcmp(var[2], "Y") == 0 ? true : false;
        AppPath* thisPath = _framework->fetchPath(var[1]);
        thisPath->setEnabled(moduleMode);
        read_seq_nodes(thisPath, (int) atoi(var[4]), infile);
      }

      else if (strcmp(var[0], "D") == 0) {
        AppPath* thePath = (AppPath*) _framework->fetchSequence(var[1]);
        if (thePath != 0) {
          _framework->remove(thePath);
          delete thePath;
        }
      }

      else if (strcmp(var[0], "L") == 0) {
        for (int i = 0; i < 10; i++) {
          if (var[i + 1]) {
            _modules.append(new std::string(var[i + 1]));
          }
        }
      }

      id = 0;
      while (0 != var[id]) {
        delete[] var[id];
        var[id] = 0;
        id++;
      }

    }
  }

  //check for any missing modules in both the stored list and the framework
  //module list

  missingModules();

}

void
AppMemento::missingModules()
{
  //list modules that were stored but no match in framework was found

  bool matched = false;

  if (!_modulesNotFound.isEmpty()) {

    _framework->fullReport("The following modules in stored configuration "
      "were not matched :");
    APPListIterator<std::string*> iterator(_modulesNotFound);
    std::string** thisModName;
    while ((thisModName = iterator())) {
      _framework->partialReport((*thisModName)->c_str());
      _framework->partialReport("  ");
    }
    _framework->fullReport("");
  }

  //list modules in framework that didn't have a match stored list
  //
  APPList<AppModule*> allAsModules;
  _framework->buildModuleList(allAsModules, *_framework->inputModules());
  _framework->buildModuleList(allAsModules, *_framework->outputModules());
  _framework->buildModuleList(allAsModules, *_framework->modules());
  AppModule** thisMod;
  APPListIterator<AppModule*> iter(allAsModules);
  while ((thisMod = iter())) {
    matched = false;
    APPListIterator<std::string*> iterator(_modules);
    std::string** thisModName;
    while ((thisModName = iterator())) {
      if (*(*thisModName) == (*thisMod)->name()) {
        matched = true;
        break;
      }
    }
    if (!matched) {
      _framework->partialReport("Module ");
      _framework->partialReport((*thisMod)->name());
      _framework->fullReport(" in Framework does not have a match in"
        " the stored list");
    }
  }

}

void
AppMemento::read_seq_nodes(AppSequence* seq, int nodes, fstream& infile)
{

  char buff[1024];
  int size = 1024;
  char* var[15];

  int id;
  for (id = 0; id < 15; id++) {
    var[id] = 0;
  }

  for (int i = 0; i < nodes; i++) {
    infile.getline(buff, size);
    if (*buff) { //skip empty line
      parseLine(buff, _fs, var);

      seq->append(var[1]);

      id = 0;
      while (0 != var[id]) {
        delete[] var[id];
        var[id] = 0;
        id++;
      }
    }

    else i--; //don't count the empty lines as nodes in this sequence

  }
}

void
AppMemento::makeClone(char* targetname, char* clonename)
{

  AppModule** thisMod;
  bool cloneFound = false;

  APPListIterator<AppModule*> iter(*_framework->modules());
  while ((thisMod = iter())) {
    if ((strcmp((*thisMod)->name(), targetname) == 0)) {
      AppModule* theClone = (*thisMod)->clone(clonename);
      if (theClone != 0) {
        cloneFound = true;
        _framework->appBuilder()->add(theClone); // Add clone to the Framework
        AppCloneRecorder* cloneRecorder = new AppCloneRecorder(targetname,
            clonename);
        _framework->add(cloneRecorder);
      }
      break;
    }
  }
  if (!cloneFound) {
    _framework->partialReport("Clonning failed: module name= ");
    _framework->partialReport(targetname);
    _framework->partialReport(" clone name= ");
    _framework->fullReport(clonename);
  }

}

void
AppMemento::restoreStates(fstream& infile, AppModule* theModule)
{

  int size = 1024;
  char buff[1024];
  char* var[15];

  int id;
  for (id = 0; id < 15; id++) {
    var[id] = 0;
  }

  APPListIterator<AppCommand*> theIterator(*theModule->commands());
  AppCommand** thecommand;

  while ('C' == (char) infile.peek()) {
    infile.getline(buff, size);
    parseLine(buff, _fs, var);

    theIterator.rewind();
    while ((thecommand = theIterator())) {
      if (((*thecommand)->isShowable() && strcmp((*thecommand)->command(),
          var[1]) == 0)) {
        (*thecommand)->restoreState(var + 1);
      }
    }

    id = 0;
    while (0 != var[id]) {
      delete[] var[id];
      var[id] = 0;
      id++;
    }

  }

}

void
AppMemento::restoreFramework(fstream& infile)
{

  int size = 1024;
  char buff[1024];
  char* var[15];

  int id;
  for (id = 0; id < 15; id++) {
    var[id] = 0;
  }

  APPListIterator<AppActionController*> actionIter(
      *_framework->actionControllers());
  AppActionController** anActionController;

  char nextCh = (char) infile.peek();
  while ('F' == nextCh || 'C' == nextCh) {
    infile.getline(buff, size);
    parseLine(buff, _fs, var);

    if (strcmp("C", var[0]) == 0) {
      restoreStates(infile, _framework);
    }

    else if (strcmp("FC", var[0]) == 0) {
      makeClone(var[2], var[1]);
    }

    else if (strcmp("FS", var[0]) == 0) {
      AppSequence* thisSeq = new AppSequence(var[1], 0);
      _framework->add(thisSeq);
    }

    else if (strcmp("FP", var[0]) == 0) {
      AppPath* thisPath = new AppPath(var[1], 0);
      _framework->add(thisPath);
    }

    else if (strcmp("FA", var[0]) == 0) {
      bool actionMode = (strcmp(var[1], "Y") == 0) ? true : false;
      actionIter.rewind();
      while ((anActionController = actionIter())) {
        if ((strcmp(var[2], (*anActionController)->action()->name().c_str())
            == 0)) {
          (*anActionController)->setDoAction(actionMode);
        }
      }
    }

    else if (strcmp("FI", var[0]) == 0) {
      _framework->setInputModule(var[0]);
    }

    else if (strcmp("FO", var[0]) == 0) {
      _framework->setOutputModule(var[1]);
    }

    id = 0;
    while (0 != var[id]) {
      delete[] var[id];
      var[id] = 0;
      id++;
    }

    nextCh = (char) infile.peek();
  }
}

void
parseLine(char* buff, const char* fs, char *field[])
{
  int i = 0, j = 0, k = 0;
  char temp[1024];

  while (buff[i] != 0) {
    for (k = 0; buff[i] != *fs && buff[i] != 0; k++) {
      temp[k] = buff[i];
      i++;
    }

    temp[k] = '\0';
    field[j] = new char[k + 1];
    strcpy(field[j], temp);
    j++;
    if (buff[i] != 0) i++;
    field[j] = 0;
  }
}

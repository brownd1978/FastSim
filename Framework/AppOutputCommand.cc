//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppOutputCommand.cc 631 2010-04-28 06:46:51Z stroili $
//
// Description:
//      Class AppOutputCommand. Command handler for the "output" command for
//      the standard file output module. Valid subcommands are:
//
//      output clear                            Empty the list of streams
//      output create <stream> [<dest>]         Create stream optionally bound to destination
//      output delete <stream1> [<str2> ..]     Remove stream(s) from list
//      output destination <stream> [<dest>]    Bind stream to destination
//      output disable <stream1> [<str2> ..]    Disable stream(s)
//      output enable  <stream1> [<str2> ..]    Enable stream(s)
//      output help                             Display help information
//      output list                             List current streams/files
//      output paths <stream> <path1>...        Bind <path1> etc. to stream
//      output reset                            Synonym for clear
//      output statistics                       List statistics for each stream
//      output stream <stream> [<dest>]         Synonym for create
//      output wipe-out                         Synonym for clear
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie                Original Author
//      Marc Turcotte                   Merged in R.Kapur's TK interface
//
// Copyright Information:
//      Copyright (C) 1994, 1995        Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppOutputCommand.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
#include <string.h>
#include <sstream>
using std::ostringstream;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppFramework.hh"
#include "Framework/AppStreamsOutputModule.hh"
#include "Framework/AppStream.hh"
#include "Framework/AppPath.hh"
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "FrameUtil/AbsInterp.hh"
using std::endl;

//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//----------------
// Constructors --
//----------------

AppOutputCommand::AppOutputCommand(const char* const theCommand,
    AppModule* theTarget) :
  AppCommand(theCommand, theTarget)
{
}

//--------------
// Destructor --
//--------------

AppOutputCommand::~AppOutputCommand()
{
}

//-----------
// Handler --
//-----------

int
AppOutputCommand::handle(int argc, char* argv[])
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  int result = AbsInterp::ERROR;

  setArgs(argc, argv);
  if (2 <= argc) {
    result = handleCommand(argc, argv);
    if (AbsInterp::OK != result) {
      std::string txtmsg("invalid command name: ");
      for (int index = 0; index < argc; index++) {
        txtmsg += " ";
        txtmsg += argv[index];
      }
      theOutput->setError(txtmsg.c_str());
    }
  } else {
    theOutput->setError("wrong # args");
  }
  return result;
}

std::string
AppOutputCommand::description() const
{
  std::string descr;
  descr = "(Command) : Type '";
  descr += command();
  descr += " help' for details.";
  return descr;
}

void
AppOutputCommand::show() const
{

  target()->partialReport(command());
  target()->partialReport(" : (command); type '");
  target()->partialReport(command());
  target()->fullReport(" list' for settings.");

}

bool
AppOutputCommand::isShowable() const
{
  return true;
}

//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------

int
AppOutputCommand::handleCommand(int argc, char* argv[])
{
  int result = AbsInterp::ERROR;

  if (0 == strcmp(argv[1], "clear") || 0 == strcmp(argv[1], "reset") || 0
      == strcmp(argv[1], "wipe") || 0 == strcmp(argv[1], "wipe-out")) {
    result = wipeoutHandler();
  } else if (0 == strcmp(argv[1], "create") || 0 == strcmp(argv[1], "cr") || 0
      == strcmp(argv[1], "stream") || 0 == strcmp(argv[1], "st")) {
    result = createHandler();
  } else if (0 == strcmp(argv[1], "delete")) {
    result = deleteHandler();
  } else if (0 == strcmp(argv[1], "destination") || 0 == strcmp(argv[1], "des")) {
    result = destinationHandler();
  } else if (0 == strcmp(argv[1], "disable") || 0 == strcmp(argv[1], "dis")) {
    result = disableHandler();
  } else if (0 == strcmp(argv[1], "enable") || 0 == strcmp(argv[1], "ena")) {
    result = enableHandler();
  } else if (0 == strcmp(argv[1], "help") || 0 == strcmp(argv[1], "he")) {
    result = helpHandler();
  } else if (0 == strcmp(argv[1], "list") || 0 == strcmp(argv[1], "li")) {
    result = listHandler();
  } else if (0 == strcmp(argv[1], "paths") || 0 == strcmp(argv[1], "path")) {
    result = pathsHandler();
  } else if (0 == strcmp(argv[1], "statistics") || 0 == strcmp(argv[1], "stat")) {
    result = statisticsHandler();
  }
  return result;
}

int
AppOutputCommand::createHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  AppStream* theStream;
  char* theName;
  char* theDestination;
  int index = 2;
  int result = AbsInterp::ERROR;

  if (NULL != (theName = getArgument(index))) {
    if (validateName(theName)) {
      if (!theOutput->has(theName)) {
        index++;
        theDestination = getArgument(index);
        theStream = createStream(theName, theDestination);
        theOutput->append(theStream);
        result = AbsInterp::OK;
      } else {
        ostringstream error;
        error << "***** Duplicate stream name: " << theName << " - ignored"
            << endl;
        theFrame->setError(error.str().c_str());
      }
    }
  }
  return result;
}

int
AppOutputCommand::deleteHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  char* theName;
  AppStream* theStream;
  ostringstream error;
  int index = 2;
  int result = AbsInterp::OK;

  while (NULL != (theName = getArgument(index))) {
    if (NULL != (theStream = theOutput->fetch(theName))) {
      theOutput->remove(theName);
    } else {
      if (AbsInterp::OK == result) {
        error << "***** Unknown stream name(s):";
      }
      error << " " << theName;
      result = AbsInterp::ERROR;
    }
    index++;
  }
  if (AbsInterp::OK != result) {
    error << " -ignored" << endl;
    theFrame->setError(error.str().c_str());
  }
  return result;
}

int
AppOutputCommand::destinationHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  AppStream* theStream;
  char* theName;
  int index = 2;
  int result = AbsInterp::ERROR;

  if (NULL != (theName = getArgument(index))) {
    if (validateName(theName)) {
      if (NULL != (theStream = theOutput->fetch(theName))) {
        index++;
        theName = getArgument(index);
        theStream->setDestination(theName);
        result = AbsInterp::OK;
      } else {
        ostringstream error;
        error << "***** Unknown stream name: " << theName << " - ignored"
            << endl;
        theFrame->setError(error.str().c_str());
        result = AbsInterp::ERROR;
      }
    }
  }
  return result;
}

int
AppOutputCommand::disableHandler()
{

  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  AppStream* theStream;
  char* theName;
  ostringstream error;
  int index = 2;
  int result = AbsInterp::OK;
  while (NULL != (theName = getArgument(index))) {
    if (NULL != (theStream = theOutput->fetch(theName))) {
      theStream->setEnabled(false);
    } else {
      if (AbsInterp::OK == result) {
        error << "***** Unknown stream name(s):";
      }
      error << " " << theName;
      result = AbsInterp::ERROR;
    }
    index++;
  }
  if (AbsInterp::OK != result) {
    error << " -ignored" << endl;
    theFrame->setError(error.str().c_str());
  }
  return result;
}

int
AppOutputCommand::enableHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  AppStream* theStream;
  char* theName;
  ostringstream error;
  int index = 2;
  int result = AbsInterp::OK;
  while (NULL != (theName = getArgument(index))) {
    if (NULL != (theStream = theOutput->fetch(theName))) {
      theStream->setEnabled(true);
    } else {
      if (AbsInterp::OK == result) {
        error << "***** Unknown stream name(s):";
      }
      error << " " << theName;
      result = AbsInterp::ERROR;
    }
    index++;
  }
  if (AbsInterp::OK != result) {
    error << " -ignored" << endl;
    theFrame->setError(error.str().c_str());
  }
  return result;
}

int
AppOutputCommand::helpHandler()
{
  int result = AbsInterp::OK;

  target()->partialReport("output clear                            ");
  target()->fullReport("Empty the list of streams");
  target()->partialReport("output create <stream> [<dest>]         ");
  target()->fullReport("Create stream");
  target()->partialReport("output delete <stream1> [<str2> ..]     ");
  target()->fullReport("Remove stream(s) from list");
  target()->partialReport("output destination <stream> [<dest>]    ");
  target()->fullReport("Bind stream to destination");
  target()->partialReport("output disable <stream1> [<str2> ..]    ");
  target()->fullReport("Disable stream(s)");
  target()->partialReport("output enable  <stream1> [<str2> ..]    ");
  target()->fullReport("Enable stream(s)");
  target()->partialReport("output help                             ");
  target()->fullReport("Display help information");
  target()->partialReport("output list                             ");
  target()->fullReport("List current streams/files");
  target()->partialReport("output paths <stream> <path1>...        ");
  target()->fullReport("Bind <path1> etc. to stream");
  target()->partialReport("output reset                            ");
  target()->fullReport("Synonym for clear");
  target()->partialReport("output statistics                       ");
  target()->fullReport("List statistics for each stream");
  target()->partialReport("output stream <stream> [<dest>]         ");
  target()->fullReport("Synonym for create");
  target()->partialReport("output wipe-out                         ");
  target()->fullReport("Synonym for clear");
  return result;
}

int
AppOutputCommand::listHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  APPList<AppStream*>* theList = theOutput->streams();
  APPList<AppPath*>* thePaths;
  AppStream** theStream;
  AppPath** thePath;
  const char* theDest;

  if (!theList->isEmpty()) {
    APPListIterator<AppStream*> iter1(*theList);
    while ((theStream = iter1())) {
      target()->fullReport("");
      target()->partialReport((*theStream)->name());
      target()->partialReport(":");
      if (!(*theStream)->isEnabled()) {
        target()->partialReport(" (disabled)");
      }
      target()->fullReport("");
      target()->partialReport("    ");
      target()->partialReport(destinationString());
      target()->partialReport(": ");
      if (NULL != (theDest = (*theStream)->destination())) {
        target()->partialReport(theDest);
      } else {
        target()->partialReport("(none)");
      }
      target()->fullReport("");
      listInformation(*theStream, theFrame);
      target()->partialReport("    Paths: ");
      thePaths = (*theStream)->paths();
      if (!thePaths->isEmpty()) {
        APPListIterator<AppPath*> iter2(*thePaths);
        while ((thePath = iter2())) {
          target()->partialReport((*thePath)->name());
          target()->partialReport(" ");
        }
      } else {
        target()->partialReport("(none)");
      }
      target()->fullReport("");
    }
  } else {
    target()->fullReport("No output streams are defined");
  }
  return AbsInterp::OK;

}

int
AppOutputCommand::pathsHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  AppStream* theStream;
  AppPath* thePath;
  char* theName;
  bool add = true;
  int index = 2;

  int result = AbsInterp::OK;

  if (NULL != (theName = getArgument(index))) {
    if (NULL != (theStream = theOutput->fetch(theName))) {
      index++;
      while (NULL != (theName = getArgument(index))) {
        if (0 == strcmp(theName, "-add") || 0 == strcmp(theName, "-a")) {
          add = true;
        } else if (0 == strcmp(theName, "-remove") || 0
            == strcmp(theName, "-r")) {
          add = false;
        } else {
          if (NULL != (thePath = theFrame->fetchPath(theName))) {
            if (add) {
              theStream->append(thePath);
            } else {
              theStream->remove(thePath);
            }
          } else {
            theFrame->setError("unknown path name");
            result = AbsInterp::ERROR;
          }
        }
        index++;
      }
    } else {
      ostringstream error;
      error << "***** Unknown stream name: " << theName << " - ignored" << endl;
      theFrame->setError(error.str().c_str());
      result = AbsInterp::ERROR;
    }
  } else {
    theFrame->setError("no arguments supplied");
    result = AbsInterp::ERROR;
  }
  return result;
}

int
AppOutputCommand::statisticsHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  AppFramework* theFrame = theOutput->framework();
  APPList<AppStream*>* theList = theOutput->streams();
  AppStream** theStream;

  if (!theList->isEmpty()) {
    APPListIterator<AppStream*> iter1(*theList);
    while ((theStream = iter1())) {
      target()->fullReport("");
      target()->partialReport((*theStream)->name());
      target()->partialReport(":   ");
      ostringstream stream;
      stream << (*theStream)->events();
      target()->partialReport(stream.str().c_str());
      target()->fullReport(" events");
      statisticsInformation(*theStream, theFrame);
    }
  } else {
    target()->fullReport("No output streams are defined");
  }
  return AbsInterp::OK;

}

int
AppOutputCommand::wipeoutHandler()
{
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  theOutput->wipeout();
  return AbsInterp::OK;
}

void
AppOutputCommand::listInformation(AppStream* theStream, AppFramework* theFrame)
{
}

void
AppOutputCommand::statisticsInformation(AppStream* theStream,
    AppFramework* theFrame)
{
}

void
AppOutputCommand::dumpState(AppConfigRecorder* r)
{

  const char* var[10];
  AppStreamsOutputModule* theOutput = (AppStreamsOutputModule*) target();
  if (theOutput->streams() != 0) {
    APPListIterator<AppStream*> theIterator(*theOutput->streams());
    AppStream** theStream;
    var[0] = "output";
    while ((theStream = theIterator())) {

      // create the stream
      var[1] = "create";
      var[2] = (*theStream)->name();
      //      var[2] = const_cast<char*>((*theStream)->name());
      if ((*theStream)->destination() != 0) {
        var[3] = const_cast<char*> ((*theStream)->destination());
        r->store(4, var);
      } else {
        r->store(3, var);
      }

      // enable or disable stream
      if ((*theStream)->isEnabled()) {
        var[1] = "enable";
      } else {
        var[1] = "disable";
      }
      var[2] = const_cast<char*> ((*theStream)->name());
      r->store(3, var);

      // paths bound to this stream
      AppPath** thePath;
      APPListIterator<AppPath*> itsPaths(*(*theStream)->paths());
      while ((thePath = itsPaths())) {
        var[1] = "paths";
        var[2] = const_cast<char*> ((*theStream)->name());
        var[3] = const_cast<char*> ((*thePath)->name());
        r->store(4, var);
      }
    }
  }
}

void
AppOutputCommand::restoreState(char* argv[])
{
  int i = 0;
  while (argv[i]) {
    i++;
  }
  int argc = i;
  handle(argc, argv);
}


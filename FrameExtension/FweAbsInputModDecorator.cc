//--------------------------------------------------------------------------
// File and Version Information:
// $Id: FweAbsInputModDecorator.cc 505 2010-01-14 15:15:59Z stroili $
//
// Description: class FweAbsInputModDecorator
//
// This is a decorator base class for AppInputModules.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Asoka De Silva
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "FrameExtension/FweAbsInputModDecorator.hh"
#include "Framework/AppFramework.hh"
#include "ErrLogger/ErrLog.hh"
#include "Framework/AppReportAttributes.hh"

FweAbsInputModDecorator::FweAbsInputModDecorator(const char* const theName,
						   const char* const theDescription)
  : AppInputModule(theName, theDescription) 
  , _theInputModule(0)
{
}

FweAbsInputModDecorator::~FweAbsInputModDecorator() {
}

bool
FweAbsInputModDecorator::setInputModule(const char* theModule) {
  bool status = true;
  AppModule* mod = framework()->fetchModule(theModule);
  if (mod == 0) {
    ErrMsg(error) << "Input Module " << theModule << " does not exist !"
		  << endmsg;
    status = false;
  }
  else if (mod->execType() != APP_input) {
    ErrMsg(error) << theModule << " is not an input module !"
                  << endmsg;
    status = false;
  }
  else {
    _theInputModule = dynamic_cast<AppInputModule*>(mod);
  }
  return status;
}

void
FweAbsInputModDecorator::setSilent(bool mode) {
  if (_theInputModule != 0) _theInputModule->setSilent(mode);
}

void 
FweAbsInputModDecorator::append ( const char* const theName,
                                  const char* const theOther) 
{
  if (_theInputModule != 0) _theInputModule->append(theName, theOther);
}
  
void
FweAbsInputModDecorator::remove ( const char* const theName) {
  if (_theInputModule != 0) _theInputModule->remove(theName);
}

void
FweAbsInputModDecorator::wipeout () {
  if (_theInputModule != 0) _theInputModule->wipeout();
}

void 
FweAbsInputModDecorator::addToReport( AppReportAttributes* attrib, int mode ) const {

  const AppInputModule* thisObject = this;
  if (framework()->theInputModule() == thisObject) {
    partialReport(" (Active");
    if (_theInputModule != 0) {
      partialReport(": ");
      partialReport(_theInputModule->name());
    }
    partialReport(") ");
  }
}

void
FweAbsInputModDecorator::deleteEvent(AbsEvent*& anEvent) {
  if (_theInputModule != 0) _theInputModule->deleteEvent(anEvent);
}




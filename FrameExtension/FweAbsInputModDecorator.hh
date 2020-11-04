//--------------------------------------------------------------------------
// File and Version Information:
// $Id: FweAbsInputModDecorator.hh 505 2010-01-14 15:15:59Z stroili $
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

#ifndef FWEABSINPUTMODDECORATOR_HH
#define FWEABSINPUTMODDECORATOR_HH

#include "Framework/AppInputModule.hh"

class AppReportAttributes;

class FweAbsInputModDecorator : public AppInputModule {

private:

  AppInputModule* _theInputModule;

public:

  FweAbsInputModDecorator(const char* const theName,
			   const char* const theDescription);
  
  virtual ~FweAbsInputModDecorator();

  virtual AppResult beginJob( AbsEvent* anEvent ) {
    return _theInputModule->beginJob(anEvent);
  }

  virtual AppResult beginRun( AbsEvent* anEvent ) {
    return _theInputModule->beginRun(anEvent);
  }

  virtual AppResult inputEvent( AbsEvent*& anEvent ) {
    return _theInputModule->inputEvent(anEvent);
  }  

  virtual AppResult inputEvent( AbsEvent*& anEvent, 
                                AppStopType& theDispatchStopType ) {
    return _theInputModule->inputEvent(anEvent, theDispatchStopType);
  }

  virtual AppResult inputFrame( AppFrame*& aFrame,
                                AppStopType& theDispatchStopType ) {
    return _theInputModule->inputFrame(aFrame, theDispatchStopType);
  }

  virtual AppResult endRun( AbsEvent* anEvent ) {
    return _theInputModule->endRun(anEvent);
  }

  virtual AppResult endJob( AbsEvent* anEvent ) {
    return _theInputModule->endJob(anEvent);
  }
  
  virtual void setSilent( bool mode );

  virtual void append ( const char* const theName,
                        const char* const theOther=NULL);

  virtual void remove ( const char* const theName);

  virtual void wipeout ();

  virtual void addToReport( AppReportAttributes* attrib, int mode ) const;

  virtual void deleteEvent(AbsEvent*& anEvent);

  bool setInputModule(const char* theModule);
  const AppInputModule* getInputModule() const {return _theInputModule;}

private:
  
  // disable
  FweAbsInputModDecorator(const FweAbsInputModDecorator&);
  FweAbsInputModDecorator& operator = (const FweAbsInputModDecorator&);

};

#endif



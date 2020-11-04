//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppActionApplicator.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	See .hh file for general comments.  Comments here only concern
//      implementation.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//
// Author List:
//	Ed Frank                        University of Pennsylania
//      Gregory Dubois-Felsmann         California Institute of Technology
//
// Copyright Information:
//	Copyright (C) 1999
//
// BUGS:
//    o Alot of replicated code.
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

#include "Framework/AppActionApplicator.hh"
#include "Framework/AppModule.hh"
#include "Framework/AppActionController.hh"
#include "Framework/AppFramework.hh"
#include "FrameUtil/APPListIterator.hh"
#include <assert.h>

//****************************************************************************
AppActionApplicator::AppActionApplicator( ) {
//****************************************************************************
};

//****************************************************************************
AppActionApplicator::~AppActionApplicator( ){
//****************************************************************************
}

//****************************************************************************
AppResult
AppActionApplicator::beginJob(    AppModule& mod, AbsEvent* anEvent  ){
//****************************************************************************

  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;

  while ( ( anActionController=itr.next() ) ) {                    
    (*anActionController)->beforeBeginJob(&mod,anEvent);
  }

  AppResult r = mod.beginJob( anEvent );

  itr.rewind();
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->afterBeginJob(&mod,anEvent);
  }

  return r;
}

//****************************************************************************
AppResult
AppActionApplicator::beginRun(    AppModule& mod, AbsEvent* anEvent  ){
//****************************************************************************

  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;
      
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->beforeBeginRun(&mod,anEvent);
  }

  AppResult r = mod.beginRun( anEvent );

  itr.rewind();
  while ( ( anActionController=itr.next() )  ) {
    (*anActionController)->afterBeginRun(&mod,anEvent);
  }

  return r;
}

//****************************************************************************
AppResult
AppActionApplicator::event(       AppModule& mod, AbsEvent*  anEvent ){
//****************************************************************************

  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;      

  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->beforeEvent( &mod, anEvent);
  }

  AppResult r = mod.event( anEvent );
                      
  itr.rewind();
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->afterEvent( &mod, anEvent);
  }

  return r;
}

//****************************************************************************
AppResult
AppActionApplicator::other(       AppModule& mod, AbsEvent*  anEvent ){
//****************************************************************************

  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;      

  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->beforeOther( &mod, anEvent);
  }

  AppResult r = mod.other( anEvent );
                      
  itr.rewind();
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->afterOther( &mod, anEvent);
  }

  return r;
}

//****************************************************************************
AppResult
AppActionApplicator::inputEvent(  AppModule& mod, AbsEvent*& anEvent ){
//****************************************************************************
  //  NOTE:  This routine calls inputEvent() on the module, BUT
  //  calls beforeEvent and afterEvent on the action.  This is because
  //  current actions do not have a before/after inputEvent() method.
  //  That might be too much detail anyway...no loss yet.
 
  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;      
  
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->beforeEvent(&mod, anEvent);
  }
  itr.rewind();
  
  // This will no longer work in the new Framework 
  mod.framework()->fullReport( "Code needs to be fixed in "
			       "AppActionApplicator::inputEvent.");
  assert(0);
  AppResult result;
  //result = mod.inputEvent( anEvent );
  
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->afterEvent(&mod, anEvent);
  }
  itr.rewind();

  return result;
}

//****************************************************************************
AppResult
AppActionApplicator::outputEvent( AppModule& mod, AbsEvent*  anEvent ){
//****************************************************************************

  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;      

  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->beforeEvent(&mod, anEvent);
  }
  itr.rewind();

  // This will no longer work in the new Framework
  mod.framework()->fullReport("Code needs to be fixed in "
			      "AppActionApplicator::outputEvent.");
  assert(0);  
  AppResult r;
  // r = mod.outputEvent( anEvent );
  
  while ( ( anActionController=itr.next() )  ) {
    (*anActionController)->afterEvent( &mod, anEvent );
  }
  itr.rewind();

  return r;
}


//****************************************************************************
AppResult
AppActionApplicator::endRun(      AppModule& mod, AbsEvent* anEvent ){
//****************************************************************************

  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;      

  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->beforeEndRun(&mod,anEvent);
  }

  AppResult r= mod.endRun( anEvent );      

  itr.rewind();
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->afterEndRun(&mod,anEvent);
  }

  return r;
}

//****************************************************************************
AppResult
AppActionApplicator::endJob(      AppModule& mod, AbsEvent* anEvent ){
//****************************************************************************

  APPListIterator<AppActionController*> 
    itr(*(mod.framework()->actionControllers()));
  AppActionController** anActionController;      

  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->beforeEndJob(&mod,anEvent);
  }

  AppResult r = mod.endJob( anEvent );

  itr.rewind();
  while ( ( anActionController=itr.next() ) ) {
    (*anActionController)->afterEndJob(&mod,anEvent);
  }

  return r;
}







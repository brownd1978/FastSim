//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppActionApplicator.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppActionApplicator.  This class contains the rules for what
//	it means to apply an AppAction before/after a call to a callback
//      like event() or beginJob() in a module.  We attempt here to centralize
//      that recipe.
//
//      Ideally, we would use a formal decorator pattern here.  In that case, 
//      this class would inherit from AppModule and contain a pointer to
//      an AppModule.  Thus it would function as a call through or handle
//      that modifies the action of the callback in the pointed to module
//      by doing somethings before/after the call.  The stuff before/after
//      is in the action itself.  The module has a list of these actions.
//
//      So, the modules have the list of actions, the actions have the
//      decoration, and AppActionApplicator has the rules like "you only
//      call the action if the module is action enabled, and the module
//      is enabled and thht  actions have before/after components that
//      sandwhich a call.   In short, the actions are the "what" and the
//      AppActionApplicator is the "how."  As an example of a future
//      modification, the actions could be moved from living in the modules
//      to just living in a central location, common to all modules.  Only
//      this  AppActionApplicator class would need to change.
//
//      Ideally, we would create a AppActionApplicator based on a Module
//      whenever that Module is first added to the framework.  Then, when
//      we later called the module, the decoration whould take effect.  This
//      is difficult to do right now, and we do not attempt it.  
//
//      Questions:  should this be a module decorator or an AppExecutable
//                  decorator?
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
//------------------------------------------------------------------------

#ifndef APPACTIONAPPLICATOR_HH
#define APPACTIONAPPLICATOR_HH

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "Framework/AppResult.hh"


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AbsEvent;
class AppModule; 

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppActionApplicator {


//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppActionApplicator( );
  
  // Destructor
  virtual ~AppActionApplicator( );
  
  // Operations
  
  AppResult beginJob(    AppModule& m, AbsEvent* anEvent  );
  AppResult beginRun(    AppModule& m, AbsEvent* anEvent  );
  
  AppResult event(       AppModule& m, AbsEvent*  anEvent );
  AppResult other(       AppModule& m, AbsEvent*  anEvent );
  AppResult inputEvent(  AppModule& m, AbsEvent*& anEvent );
  AppResult outputEvent( AppModule& m, AbsEvent*  anEvent );
  
  AppResult endRun(      AppModule& m, AbsEvent* anEvent );
  AppResult endJob(      AppModule& m, AbsEvent* anEvent );
  
  
private:
  
  // Copy Constructor
  AppActionApplicator( const AppActionApplicator& );
  AppActionApplicator& operator= ( const AppActionApplicator& );
  
  // Data members
  
};

#endif

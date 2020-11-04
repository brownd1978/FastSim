//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFramework.hh 636 2010-04-29 14:05:25Z stroili $
//
// Description:
//	Class AppFramework. This is the overall framework for the BaBar
//	analysis framework. It controls the creation of sequences and
//	paths and the processing of events.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie       	Original Author
//      Marc Turcotte           Introduced Actions '97      May '97
//      Marc Turcott            Removed explicit dependence
//                              on RW to allow sharing.     June '97
//      Marc Turcotte           Modified for new style begin/end 2/11/98
//      Marc Turcotte           Added execute member function
//                              to deal with dynamic dispatch    3/4/98 
//      Marc Turcotte           Introduced AppStopType           3/20/98
//      Marc Turcotte           Renamed execute to frame         3/26/98
//      Marc Turcotte           Introduced static _theFrame      4/30/98
//      Marc Turcotte           Introduced static _theAbsEvent
//                              but /* */ it for migration       4/30/98
//      Marc Turcotte           Introduced theEvent() &zeroTheEvent() 4/30/98
//      Marc Turcotte           Merged R.Kapur's TK interface         5/98
//      Marc Turcotte           Added setTkInterfaceON/Off and
//                              getTkInterface             6/18/98
//      Marc Turcotte           Modified for AppStatus           6/20/98
//      Marc Turcotte           Added theDispatchStopType to
//                              signatures 6/23/98
//      Marc Turcotte           Changed AppStatus for AppResult 6/23/98
//      Marc Turcotte           Modified for new AbsEvent sigs 6/24/98
//      Marc Turcotte	      	Migrated setupTheApp from AppMain 7/28/98
//      Marc Turcotte           Added arg list to setupTheApp 8/6/98
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1997, 1998        University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPFRAMEWORK_HH
#define APPFRAMEWORK_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppNoCloneModule.hh"

//-------------
// C Headers --
//-------------
#include <stddef.h>

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppExecNode;
class AppPath;
class AppSequence;
class AppInputModule;
class AppOutputModule;
class APPFileOutputModule;
class APPFileInputModule;
class AbsEvent;
class AppFrame;
class AppUserBuild;
class AppActionController;

#include "Framework/AppCloneRecorder.hh"
#include "Framework/AppPathCommand.hh"
#include "Framework/AppSequenceCommand.hh"
#include "Framework/AppModuleCommand.hh"
#include "Framework/AppEventsCommand.hh"
#include "Framework/AppConfigCommand.hh"
#include "Framework/AppActionCommand.hh"
#include "Framework/AppAction.hh"
#include "Framework/AppStopType.hh"
#include "Framework/AppMethodBase.hh"
#include "Framework/AbsParmBool.hh"

template<class Item> class AppHash;
template<class Item> class APPList;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppFramework : public AppNoCloneModule {

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppFramework( int argc, char* argv[]);

  // Destructor
  virtual ~AppFramework( );
  
  // Operations
  
  virtual AppResult      beginJob( AbsEvent* anEvent );    
  virtual AppResult      beginRun( AbsEvent* anEvent );
  
  virtual AppResult      event( AbsEvent* anEvent );
  virtual AppResult      outputEvent(AbsEvent* anEvent);
  virtual AppResult      other( AbsEvent* anEvent );
  
  virtual AppResult      frame( AppFrame* aFrame,
				const AppStopType& theDispatchStopType ); 
  // Dynamic Dispatch
  
  virtual AppResult      endRun( AbsEvent* anEvent );
  virtual AppResult      endJob( AbsEvent* anEvent );
  
  virtual AppResult      abortJob( AbsEvent* anEvent );  
  
  virtual void           talkTo( );
  virtual void           help( int argc, char** argv );
  virtual void           exit( );
  
  // Overload show since Framework has lots to show but some of those
  // are not associated with commands
  virtual void           show( int argc, char** argv ) const;
  
  virtual void beginHandler   ( int nEvents );
  virtual void continueHandler( int nEvents );
  
  void printName     ( const char* const theName, int mode = 0 );
  
  // Selectors (const)
  
  void setTheInputModule (AppInputModule* thatOne);

  AppInputModule*  theInputModule  ( ) const;    
  AppOutputModule* theOutputModule ( ) const; 
  
  AppExecutable* fetch        ( const char* const aName ) const;    
  AppModule*     fetchModule  ( const char* const aName ) const;    
  AppPath*       fetchPath    ( const char* const aName ) const;    
  AppSequence*   fetchSequence( const char* const aName ) const;    
  bool           has          ( const char* const aName ) const;    
  bool           hasModule    ( const char* const aName ) const;  
  bool           hasPath      ( const char* const aName ) const;    
  bool           hasSequence  ( const char* const aName ) const; 
  
  APPList< AppExecNode* >*     nodes    ( ) const;
  APPList< AppModule* >*       modules  ( ) const;
  APPList< AppPath* >*         paths    ( ) const;
  APPList< AppSequence* >*     sequences( ) const;
  
  APPList< AppInputModule* >*  inputModules ( ) const;
  APPList< AppOutputModule* >* outputModules( ) const;
  
  APPList< AppCloneRecorder* >*  cloneList( ) const;

  AppExecutable* execFromName( const char* const theName ) const;
  int maxNameLength( ) const;
  
  // Modifiers
  
  void setBegun        ( bool state );
  
  bool setInputModule  ( const char* const aName );    
  bool setOutputModule ( const char* const aName );    
  
  // the argument objects in all the following add members
  // have their ownership transferred to the Framework object
  virtual void add   ( AppModule* const aModule );
  virtual void insert   ( AppModule* aModule, const AppModule* before );
  virtual void add   ( const AppExecNode* const aNode );
  virtual void add   ( AppInputModule* const aModule );
  virtual void add   ( AppOutputModule* const aModule );
  virtual void add   ( AppSequence* const aSequence );
   
  virtual void add   ( AppPath* const aPath );  // takes ownership of aPath
  
  virtual void add (AppCloneRecorder* const aRecorder);
 
  virtual void add (AppAction* const anAction);
 
  // the argument objects in all the following remove members
  // have their ownership transferred from the Framework object
  // to the caller
  virtual void remove( const AppExecNode* const aNode );
  virtual void remove( const AppModule* const aModule );
  virtual void remove( const AppInputModule* const aModule );
  virtual void remove( const AppOutputModule* const aModule );
  virtual void remove( const AppSequence* const aSequence );
  virtual void remove( const AppPath* const aPath );

  // how a running module can request that this event is the last one
  void requestStop() {_stopRequested = true;}
  bool isStopRequested() const { return _stopRequested; }
  
  // Actions
  
  APPList<AppAction*>* actions() const { return _actions;}
  APPList<AppActionController*>* actionControllers() const { 
    return _theActionControllers;
  }
  
  // Grab bag of AppMethods
  APPList<AppMethodBase*>* appMethods() const {return _theAppMethodsGrabBag;}  
    
  int         argc           () const;
  char**      argv           () const;
  
  // These methods iterate over a source list of specific kinds of modules 
  // and append them to the destination list as their base class, AppModule.  
  // The Destination should not own the modules on it.  No protection from 
  // duplication is attempted.
  
  void buildModuleList( APPList<AppModule*>& destination , 
			APPList<AppInputModule*>& source );
  void buildModuleList( APPList<AppModule*>& destination , 
			APPList<AppOutputModule*>& source );
  void buildModuleList( APPList<AppModule*>& destination , 
			APPList<AppModule*>& source );
  
  void setupTheApp(AppUserBuild* build);      
  
  AppUserBuild* appBuilder();

  bool strictModuleDisable() const;
  
  //  print resource usage statistics
  void printRusage(void);

private:
  
  // Copy Constructor
  AppFramework( const AppFramework& );

  // Not implemented.
  AppFramework& operator=( const AppFramework& );
  
  void resetNodes( );
  void resetAppMethods( bool toThis );
  
  // Data members
  
  bool                         _exitRequested;
  
  AppHash< AppExecutable >*    _executables;
  
  APPList< AppExecNode* >*     _execNodes;
  APPList< AppModule* >*       _modules;
  APPList< AppPath* >*         _paths;              // owned list
  APPList< AppSequence* >*     _sequences;
  
  APPList< AppInputModule* >*  _inputModules;
  AppInputModule*              _theInputModule;
  APPList< AppOutputModule* >* _outputModules;
  AppOutputModule*             _theOutputModule;
  
  AppPath*                     _defaultPath;
  
  bool                         _isBegun;
  size_t                       _maxNameLength;
  
  int    		       _argc;
  char** 	               _argv;
  
  bool                         _stopRequested;
  
  AppEventsCommand   _eventsCmd;
  AppModuleCommand   _moduleCmd;
  AppPathCommand     _pathCmd;
  AppSequenceCommand _sequenceCmd;
  AppConfigCommand   _configCmd;
  AppActionCommand   _actionCmd;
  
  // owned pointers to default modules
  AppInputModule* _localInputModule;  
  AppOutputModule* _localOutputModule;
  
  // Actions
  
  APPList<AppAction*>* _actions;
  APPList<AppActionController*>* _theActionControllers;
  
  APPList<AppCloneRecorder*>* _cloneList;
  
  // AppMethods 
  // This is a toss 'em grab bag and is NOT a hash table. 
  // Look inside modules for that.
  //
  
  APPList<AppMethodBase*>* _theAppMethodsGrabBag;
  
  // Home for the Framework's Frame. There can only be
  // one framework so it's ok to make the frame
  // static. In fact, the Framework could migrate to
  // a singleton class, someday...
  
  static AppFrame*   _theFrame;
  static AppStopType _theDispatchStopType;
  
  AppUserBuild* _theAppUserBuild; // main() owns this.

  // flag to determine whether disabling non-existing modules
  // should be an error or a warning
  AbsParmBool _strictModuleDisable;

};

// Inline implementations
#include "Framework/AppFramework.icc"

#endif



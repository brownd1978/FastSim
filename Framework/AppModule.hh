//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AppModule.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppModule. This class describes modules within the BaBar
//	Application Framework.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//
// Author List:
//	David R. Quarrie       	Original Author
//      Bob Jacobsen            Sep 12, 1996 Fixed up memory allocation
//      Marc Turcotte           Signature of member functions changed
//      Marc Turcotte           Installed dynamic dispatch, Feb/March '98 
//                              This work done in collaboration with CLEO.
//      Marc Turcotte           Introduced AppStopType 3/20/98
//      Marc Turcotte           Renamed execute to frame 3/26/98
//      Marc Turcotte           Added AppActionController 4/13/98
//      Marc Turcotte           Removed static _theFrame 4/30/98
//      Marc Turcotte           Merged in R.Kapur's TK interface 5/15
//      Marc Turcotte           Added AppStatus.hh include 6/18/98
//      Marc Turcotte           Modified for AppStatus 6/20/98
//      Marc Turcotte           Added input/output Frame 6/22/98
//      Marc Turcotte           Added theDispatchStopType to
//                              signatures 6/23/98
//      Marc Turcotte           Changed AppStatus to AppResult 6/23/98
//      Marc Turcotte           Modified for new AbsEvent sigs 6/24/98
//      Marc Turcotte           Added macros to allow backwards commits
//                              of packages using either old or new
//                              signatures on different package branch
//                              as per suggestion of BobJ.
//      Marc Turcotte           Added clone command. 8/6/98
//      Marc Turcotte           Modified for new macros 8/10/98
//      Marc Turcott            Added flush out code for old
//                              interface 9/25/98
//      Marc Turcotte           Removed flush out code as 
//                              per Ed Frank's request. 10/28/98 
//      A. De Silva             Moved APPMODULE* macros to 
//                              BaBar.hh. 1/29/99.
//                                     
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//      Copyright (C) 1998              University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPMODULE_HH
#define APPMODULE_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppExecutable.hh"
class AppJob;
class AppRun;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppEchoCommand.hh"
#include "Framework/AppExitCommand.hh"
#include "Framework/AppHelpCommand.hh"
#include "Framework/AppShowCommand.hh"
#include "Framework/AppSourceFileCommand.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AppMenu.hh"
#include "Framework/AppResult.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppCommand;
class AbsEvent;
class AppMethodBase;
class AppStopType;
class AppFrame;
template<class Item> class APPList;
template<class Item> class AppHash;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppModule : public AppExecutable {

  //------------------
  // Static Members --
  //------------------
  
  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppModule( const char* const theName, const char* const theDescription );
  
  // Destructor
  virtual ~AppModule( );
  
  // Operations
  
  virtual AppResult beginJob   ( AbsEvent* anEvent  );
  virtual AppResult beginRun   ( AbsEvent* anEvent  );
  
  virtual AppResult event      ( AbsEvent*  anEvent );
  virtual AppResult other      ( AbsEvent*  anEvent );
  
  virtual AppResult frame      ( AppFrame* aFrame,
				 const AppStopType& theDispatchStopType ); 
  // Frame dispatch
  
  virtual AppResult endRun  ( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );
  
  virtual AppResult abortJob( AbsEvent* anEvent);
  
  void      talkTo( ); // I'm taking out virtual because we really don't
                       // want user written versions of this fucntion
  
  virtual void           help( int argc, char** argv );

  // overload this for the Framework migration
  void help( );
  void show();

  virtual void           show( int argc, char** argv ) const;
  virtual bool           isShowable( ) const;
  virtual void           exit( );
  virtual AppModule* clone(const char* cloneName);
  
  virtual void menuHandler(char* menu, char* command, int argc, char** argv);
  
  void disableCommands( );
  void enableCommands ( );
  
  // Selectors (const)
  
  bool                    isInitialized( ) const;
  char*                   prompt       ( ) const;
  APPList< AppCommand* >* commands     ( ) const;
  virtual bool                    passed       ( ) const;	// Passed?
  
  char*                   menuFileName () const;
  
  void setError       ( const char* const theError );
  void setInitialized ( );
  void setPrompt      ( const char* const thePrompt );
   
  void setMenuFileName( const char* const filename);
  
  // access to standard parameters
  
  bool verbose() const;
  bool production() const;
  
protected:
  
  static AppMenu moduleMenu; 
  
  //
  // This is for holding the dynamic dispatch map.
  // This is a hash table of AppMethodBase pointers; each AppMethod 
  // (derived from AppMethodBase) is a wrapper type
  // which holds a module pointer and its method. The hash table
  // is indexed by a character key. M. Turcotte 12/20/98
  //
  
  AppHash < AppMethodBase >* _theDynamicDispatchMap;
        
private:

  // Copy Constructor
  AppModule( const AppModule& );
  AppModule& operator= ( const AppModule& );
  
  // Data members
  
  APPList< AppCommand* >* _commands;          // unowned list
  bool                    _isInitialized;
  char*                   _prompt;
  char*                   _error;
  int                     _errorLen;
  
  char*                   _menuFileName;      // name of menu/help file
  
public:
  
  // 
  // Provide a means to bind a derived module's method to a 
  // particular frame state
  // 
  void bindMethodToState(const AppStopType& aStopType,
                         const AppMethodBase* const aMethod);
  
  //
  // Provide access to the dynamic dispatch map of this module
  // 
  AppHash < AppMethodBase >*  dynamicDispatchMap();

protected:

  // commands
  AppEchoCommand   _echo;
  AppExitCommand   _exit;
  AppHelpCommand   _help;
  AppShowCommand   _show;
  AppSourceFileCommand _sourceFileCmd;

  // parameters  
  AbsParmBool _verbose;        // standard parameter to control 
                               //  diagnostic output
  AbsParmBool _production;     // standard parameter to control routine output
  
  AbsParmBool _enableFrames;   // enable full frame producer/consumer mode   
        
  friend class AppMenu;
  
};

#endif





//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppMenu.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppMenu. This class inherits from AppCommand because
//      menues are invoked via a command.  In addition menues contain
//      a list of commands which become enabled/disabled when the
//      menu is active/inactive. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Liz Sexton-Kennedy		Original Author
//
// Copyright Information:
//	Copyright (C) 1997	        FNAL
//
//------------------------------------------------------------------------

#ifndef APPMENU_HH
#define APPMENU_HH

//-----------
// Headers --
//-----------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppModule;
class AppConfigRecorder;
#include "Framework/AppCommand.hh"
#include <string>
template<class Item> class APPList;

//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppMenu : public AppCommand {

public:
  
  // Constructors
  
  // A menu created this way will be in an invalid state because it
  // will be initialized with the default AppCommand constructor.
  // It becomes valid when the menu intialize member is called on it.
  AppMenu( ) ;
  
  // Destructor
  virtual ~AppMenu( );
  
  // Selectors (const)
  APPList< AppCommand* >* commands( ) const;
  char* title( ) const;
  
  std::string description ( ) const;
  
  // Modifiers
  void disableCommands( );
  void enableCommands ( );
  
  void initialize( const char* const theCommand, AppModule* theTarget,
		   AppMenu* parentMenu = 0 );
  void initTitle( const char* theTitle );
  
  // Handler
  virtual int  handle( int argc, char* argv[] );
  
  virtual void help( );
  virtual void exit( );
  virtual void show( ) const;
  virtual bool isShowable( ) const;
  
  virtual void dumpState( AppConfigRecorder* ) {}
  virtual void restoreState( char* [] ) {}
  // Also needed are special help and exit command classes for menus,
  // since these classes are only used by the menu class I'll make
  // them a nested class:
  class APPExitMenuCommand : public AppCommand {
  public:
    // Constructors
    APPExitMenuCommand( const char* const theCommand,
			AppModule* theTarget, const AppMenu* theMenu );
    
    // Handler
    virtual int handle( int argc, char* argv[] );
    virtual void show( ) const;
    virtual bool isShowable( ) const;
    virtual void dumpState( AppConfigRecorder* ) {}
    virtual void restoreState( char* [] ) {}
    
  private:
    // Not implemented.
    APPExitMenuCommand( const APPExitMenuCommand& );
    APPExitMenuCommand& operator=( const APPExitMenuCommand& );

    // Keep a pointer to the menu this command belongs to
    AppMenu* _menu;
  };
  class APPHelpMenuCommand : public AppCommand {
  public:
    // Constructors
    APPHelpMenuCommand( const char* const theCommand, 
			AppModule* theTarget, const AppMenu* theMenu );
    
    // Handler
    virtual int   handle( int argc, char* argv[] );
    virtual void  show( ) const;
    virtual bool  isShowable( ) const;
    virtual void dumpState( AppConfigRecorder* ) {}
    virtual void restoreState( char* [] ) {}
    
  private:
    // Not implemented.
    APPHelpMenuCommand( const APPHelpMenuCommand& );
    APPHelpMenuCommand& operator=( const APPHelpMenuCommand& );

    // Keep a pointer to the menu this command belongs to
    AppMenu* _menu;
  };
  class APPShowMenuCommand : public AppCommand {
  public:
    // Constructors
    APPShowMenuCommand( const char* const theCommand, 
			AppModule* theTarget, const AppMenu* theMenu );
    
    // Handler
    virtual int handle( int argc, char* argv[] );
    virtual void show( ) const;
    virtual bool isShowable( ) const;
    virtual void dumpState( AppConfigRecorder* ) {}
    virtual void restoreState( char* [] ) {}
    
  private:
    // Not implemented.
    APPShowMenuCommand( const APPShowMenuCommand& );
    APPShowMenuCommand& operator=( const APPShowMenuCommand& );

    // Keep a pointer to the menu this command belongs to
    AppMenu* _menu;
  };
  
protected:
  
  // commands
  APPExitMenuCommand* _exit;
  APPHelpMenuCommand* _help;
  APPShowMenuCommand* _show;
  
private:
  // Make the copy constructor/assignment private since the compiler 
  // generated ones are dangerous and we don't need it.
  AppMenu( const AppMenu& );
  AppMenu& operator= ( const AppMenu& );
  APPList< AppCommand* >*  _commands;
  char*                    _title;
  // Keep a pointer to the menu this menu belongs to so we know whose
  // commands we have to enable/disable.
  AppMenu*                 _parent;
};

#endif








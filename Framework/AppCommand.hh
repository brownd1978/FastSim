//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppCommand.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppCommand. The abstract parent of all command classes. Each
//	descendant class handles a single command, either for the framework
//	itself, or for one of its modules. A command has a target upon which
//	it acts, and can be enabled or disabled.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David R. Quarrie		Original Author
//
// Copyright Information:
//	Copyright (C) 1994, 1995	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------

#ifndef APPCOMMAND_HH
#define APPCOMMAND_HH

//-----------
// Headers --
//-----------
#include "FrameUtil/AbsCommand.hh"
#include <string>

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class AppModule;
class AppNameNode;
class AppConfigRecorder;
//		---------------------
// 		-- Class Interface --
//		---------------------
 
class AppCommand : public AbsCommand {

//------------------
// Static Members --
//------------------

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AppCommand( );
  AppCommand( const char* const theCommand, AppModule* theTarget );
  // Destructor
  virtual ~AppCommand( );
  
  
  // Selectors (const)
  const char*      command  ( ) const;
  virtual void       show        ( ) const { }
  virtual bool       isShowable  ( ) const { return true; }      
  virtual std::string  description ( ) const { return ""; }
  AppModule* target   ( ) const;
  bool       isEnabled( ) const;
  
  // Modifiers
  void initialize( const char* const theCommand, AppModule* theTarget );
  void disable( );
  void enable ( );
  
  // Handler
  virtual int handle( int argc, char* argv[]);        
  
  virtual void dumpState( AppConfigRecorder* ) = 0; 
  virtual void restoreState( char* [] ) = 0; 
  virtual void initForRestore (){};
  //this function is used to set lists to zero when restoring the module
  //from a dump file. The problem only arises when the module is cloned.
  //Restoring the clone and setting the variables results in item bbeing 
  //duplicated. this will be made pure in near future and we will have to
  //migrate other packages to implement this.
  
protected:

  void setEnabled( bool mode );
  
  char*        getArgument ( int  index ) const;
  AppNameNode* getNameNode ( int* index ) const;
  bool         validateName( const char* const theName ) const;
  
  void   setArgs( int argc, char* argv[] );
  bool   isValid( );
  
  int    _argc;
  char** _argv;

private:

  // Copy and assignement not implemented.
  AppCommand( const AppCommand& );
  AppCommand& operator=( const AppCommand& );

  AppModule* _target;
  char*      _command;
  bool       _enabled;
  bool       _isValid;
};

#endif








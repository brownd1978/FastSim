//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppMenu.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppMenu. This class inherits from AppCommand because
//      menues are invoked via a command.  In addition menues contain
//      a list of commands which become enabled/disabled when the
//      menu is active/inactive.
//
// Environment:
//	Software developed for CDF
//
// Author List:
//	Liz Sexton-Kennedy		Original Author
//
// Copyright Information:
//	Copyright (C) 1997      	FNAL
//
//------------------------------------------------------------------------ 
#include "Experiment/Experiment.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppMenu.hh"

//-------------
// C Headers --
//-------------
//#include <string.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "Framework/AppCommand.hh"
#include "FrameUtil/AbsInterp.hh"
#include "FrameUtil/APPListIterator.hh"

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
AppMenu::AppMenu( ) 
   : AppCommand( ), 
     _commands( NULL ), 
     _title( NULL )
{
}

void
AppMenu::initialize( const char* const theCommand, AppModule* theTarget,
		     AppMenu* parentMenu )
{
   AppCommand::initialize( theCommand, theTarget );
   _commands = new APPList< AppCommand* >;
   _exit = new APPExitMenuCommand( "exit", theTarget, this );
   _help = new APPHelpMenuCommand( "help", theTarget, this );
   _show = new APPShowMenuCommand( "show", theTarget, this );
   _parent = parentMenu;
   _commands->append( _exit );
   _commands->append( _help );
   _commands->append( _show );
}

void
AppMenu::initTitle( const char* theTitle )
{
   if ( _title != NULL )
   {
      delete [] _title;
   }
   _title = new char[strlen( theTitle )+1];
   strcpy( _title, theTitle );
}

//--------------
// Destructor --
//--------------

AppMenu::~AppMenu( )
{
   if ( isValid( ) )
   {
      if ( _title != NULL )
      {
	 delete [] _title;
      }
      delete _show;
      delete _help;
      delete _exit;
      // Don't do APPListDeleteAll( *_commands ) because this list 
      // doesn't own the commands on it.
      delete _commands;
   }
}

//-------------
// Selectors --
//-------------

APPList< AppCommand* >*
AppMenu::commands( ) const
{
    return _commands;
}
char*
AppMenu::title( ) const
{
   return _title;
}
//-------------
// Modifiers --
//-------------

void
AppMenu::exit( )
{
   disableCommands( );
   if ( _parent == &AppModule::moduleMenu )
   {
      target( )->enableCommands( );
   }
   else
   {
      _parent->enableCommands( );
   }
}

void
AppMenu::disableCommands( )
{
   AppCommand** command;
   APPListIterator<AppCommand *> theIterator( *commands( ) );
   while ( ( command = theIterator( ) ) ) 
   {
      (*command)->disable( );
   }
}

void
AppMenu::enableCommands( )
{
   AppCommand** command_tmp;
   APPListIterator<AppCommand *> theIterator( *commands( ) );
   while ( ( command_tmp = theIterator( ) ) ) 
   {
      (*command_tmp)->enable( );
   }
   AbsInterp* interp = AbsInterp::theInterpreter( );
   // Construct prompt from the name of the module this menu belongs to
   // and the name of the command that invokes this menu
   char prompt_buf[80];
   strcpy( prompt_buf, target( )->name( ) );
   strcat( prompt_buf, "/" );
   strcat( prompt_buf, command( ) );
   strcat( prompt_buf, "> " );
   interp->setCommandPrompt( prompt_buf );
}
void
AppMenu::help( )
{
   AbsInterp* interp = AbsInterp::theInterpreter( );
   // default behavior, unless overridden, is to list commands
   // available on this menu
   interp->sendPartialMessage( "Commands belonging to the \"" );
   interp->sendPartialMessage( title( ) );
   interp->sendFullMessage("\" Menu:");
  
   AppCommand** command;
   APPListIterator<AppCommand*> theIterator( *commands( ) );
   while ( ( command = theIterator( ) ) ) {
      interp->sendPartialMessage( "      " );
      interp->sendPartialMessage( (*command)->command() ) ;
      if ( strlen( (*command)->command() ) < 10 )
      {
	 interp->sendPartialMessage( "\t\t" );
      }
      else
      {
	 interp->sendPartialMessage( "\t" );	
      }
      interp->sendFullMessage((*command)->description( ).c_str( ));
   }
}
void
AppMenu::show( ) const
{
   AbsInterp* interp = AbsInterp::theInterpreter( );
   // default behavior, unless overridden, is to list commands
   // available on this menu
   interp->sendPartialMessage( "Current value of items on the \"");
   interp->sendPartialMessage( title( ) );
   interp->sendFullMessage( "\" Menu:" );

   AppCommand** command;
   APPListIterator<AppCommand *> theIterator( *commands( ) );
   while ( ( command = theIterator( ) ) ) 
   {
      if ( (*command)->isShowable( ) )
      {
	 interp->sendPartialMessage( "            " );
	 (*command)->show( );
      }
   }
}
bool
AppMenu::isShowable( ) const 
{
   // always true for now.  If people don't like showing submenu's
   // automatically then this can be switchable...maybe only make it 
   // showable if the module is in verbose mode.
   return true; 
}
//char*      
std::string
AppMenu::description( ) const 
{ 
   std::string ret_str( "envokes the " );
   ret_str += this->command( );
   return ( ret_str + " submenu" ); 

}
//-----------
// Handler --
//-----------

int 
AppMenu::handle( int argc, char* argv[] )
{
   if ( _parent == &AppModule::moduleMenu )
   {
      target( )->disableCommands( );
   }
   else
   {
      _parent->disableCommands( );
   }
   enableCommands( );
   return AbsInterp::OK;
}
// --------------------------------------
// Member functions for embedded class --
// --------------------------------------

AppMenu::APPExitMenuCommand::APPExitMenuCommand( const char* const theCommand, 
						 AppModule* theTarget,
						 const AppMenu* theMenu )
   : AppCommand( theCommand, theTarget ), _menu( (AppMenu*)theMenu )
{
}
int 
AppMenu::APPExitMenuCommand::handle( int argc, char* argv[] )
{
   _menu->exit( );
   return AbsInterp::OK;
}
void 
AppMenu::APPExitMenuCommand::show( ) const 
{
}
bool
AppMenu::APPExitMenuCommand::isShowable( ) const 
{
   return false; 
}
AppMenu::APPHelpMenuCommand::APPHelpMenuCommand( const char* const theCommand, 
						 AppModule* theTarget,
						 const AppMenu* theMenu )
   : AppCommand( theCommand, theTarget ), _menu( (AppMenu*)theMenu )
{
}
int 
AppMenu::APPHelpMenuCommand::handle( int argc, char* argv[] )
{
   _menu->help( );
   return AbsInterp::OK;
}
void 
AppMenu::APPHelpMenuCommand::show( ) const 
{
}
bool
AppMenu::APPHelpMenuCommand::isShowable( ) const 
{
   return false; 
}
AppMenu::APPShowMenuCommand::APPShowMenuCommand( const char* const theCommand, 
						 AppModule* theTarget,
						 const AppMenu* theMenu )
   : AppCommand( theCommand, theTarget ), _menu( (AppMenu*)theMenu )
{
}
int 
AppMenu::APPShowMenuCommand::handle( int argc, char* argv[] )
{
   _menu->show( );
   return AbsInterp::OK;
}
void 
AppMenu::APPShowMenuCommand::show( ) const 
{
}
bool
AppMenu::APPShowMenuCommand::isShowable( ) const 
{
   return false; 
}

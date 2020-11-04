//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TclInterp.cc 508 2010-01-14 15:18:18Z stroili $
//
// Description:
//	Class TclInterp
//
//      See header for more info
//
// Author List:
//	Liz Sexton-Kennedy		        Original Author
//
// Copyright Information:
//	Copyright (C) 1997		        FNAL
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FrameUtil/TclInterp.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include <iostream>

#include <string.h>
//-------------
// C Headers --
//-------------
#include <assert.h>
extern "C" {
#include "tcl.h"
using std::cin;
}
//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

TclInterp::TclInterp(Tcl_Interp* interp) {
  _tclInterpreter = interp;
}

TclInterp::TclInterp( ) {
  _tclInterpreter = Tcl_CreateInterp( );
}

//--------------
// Destructor --
//--------------

TclInterp::~TclInterp( ) {
   Tcl_DeleteInterp( _tclInterpreter );
}


// members

AbsInterp::StatusCode
TclInterp::runCommandFile( const char* filename )
{
   int result = Tcl_VarEval( _tclInterpreter,
			     "source ", filename , (char*) NULL );

   if (TCL_OK != result) {
     sendFullMessage( _tclInterpreter->result );
     return AbsInterp::ERROR;
   }
   return AbsInterp::OK;
}

void
TclInterp::startInterpLoop( )
{
   char line[256];
   Tcl_DString cmd;

   Tcl_DStringInit( &cmd );
   bool complete = false;
   while ( ! complete ) 
   {
      sendPartialMessage( commandPrompt( ) );
      flushMessage( );
      complete = true;
      cin.getline( line, sizeof( line ) );
      if ( 0 < strlen( line ) ) 
      {
	 Tcl_DStringAppend( &cmd, line, -1 );
	 if ( ! Tcl_CommandComplete(Tcl_DStringValue( &cmd ) )
	    ) 
	 {
	    complete = false;
	 }
      }
   }
   // Note: return value in Tcl_RecordAndEval is being iognored
   //       You may want to use this for error checking later
   (void)  Tcl_RecordAndEval( _tclInterpreter, 
			       Tcl_DStringValue( &cmd ), 0 );
   sendFullMessage( _tclInterpreter->result );

   Tcl_DStringFree( &cmd );
}

void
TclInterp::deleteCommand( const char* command )
{
   Tcl_DeleteCommand( _tclInterpreter, (char *)command );
}

void
TclInterp::createCommand( const char* commandName, const AbsCommand* command )
{
   Tcl_CreateCommand( _tclInterpreter, (char *)commandName, 
		      (Tcl_CmdProc*) commandHandler, (ClientData)command, 
		      (Tcl_CmdDeleteProc*) NULL );  
}

bool
TclInterp::existsCommand( const char* commandName ) const
{

  bool result = false;

  Tcl_CmdInfo* cmdInfo = new Tcl_CmdInfo;
  int tclResult = Tcl_GetCommandInfo( _tclInterpreter, 
				      const_cast<char*>(commandName),
				      cmdInfo);
  if (tclResult != 0) result = true;
  delete cmdInfo;

  return result; 

}

int
TclInterp::ascii2Int( const char* intString, int* intPtr ) const
{
   int status = Tcl_GetInt( _tclInterpreter, (char *)intString, intPtr ); 
   return status;
}

bool
TclInterp::fileOperation( 
    const char* const fileName,
    const char* const operation ) const
{
    bool result = false;
    
    assert( NULL != operation );
    if ( NULL != fileName ) 
    {
       // Note: return value from Tcl_VarEval is being ignored
       //       You may want to use this for error checking later
       (void) Tcl_VarEval( _tclInterpreter, 
				 "file ", operation, " ", fileName, NULL );
       if ( 0 == strcmp( "1", _tclInterpreter->result ) ) 
       {
	  result = true;
       }
    }
    return result;
}

void
TclInterp::setResult( const char* result ) const
{
  Tcl_SetResult(_tclInterpreter, (char*) result, TCL_VOLATILE);
  //   _tclInterpreter->result = (char *)result;
}

void
TclInterp::appendResult( const char* result ) const
{
  Tcl_AppendResult(_tclInterpreter, (char*) result, (char*) NULL);
}
//#ifndef NeXT
//int 
//Tcl_AppInit( Tcl_Interp* interp )
//{
//    int status;
//    if ( status = ( TCL_OK == Tcl_Init( interp ) ) ) {
//// As of tcl v7_6, tcl_RcFileName has been changed to a tcl variable,
//// tcl_rcFileName -LSK
////	tcl_RcFileName = "~/.bfapprc";
//       char *VarPtr = Tcl_SetVar( interp, "tcl_rcFileName", "~/.fapprc", 0 );
//    } 
//    return status;
//}
//#endif

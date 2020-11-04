//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: FtclInterp.cc 508 2010-01-14 15:18:18Z stroili $
//
// Description:
//	Class FtclInterp
//
//      See header for more info
//
// Author List:
//	Liz Sexton-Kennedy		        Original Author
//	Glenn Cooper				some ftcl modifications
//
// Copyright Information:
//	Copyright (C) 1997		        FNAL
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FrameUtil/FtclInterp.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include <iostream>

#include <string.h>
//-------------
// C Headers --
//-------------
#include <assert.h>
#include <stdio.h>
extern "C" {
#include "FrameUtil/ftcl.h"
#include "FrameUtil/ftclCmdLine.h"
void ftclAliasInit (Tcl_Interp *interp);
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

FtclInterp::FtclInterp( )
{
   char cmdBuf[1024];
   _tclInterpreter = Tcl_CreateInterp( );

   // Intialize help and alias
   ftclHelpInit(_tclInterpreter);
   ftclAliasInit(_tclInterpreter);
   ftclParseInit(_tclInterpreter);

   // redefine tcl's exit since Framework clobbers it otherwise
   strcpy(cmdBuf, "rename exit exitTcl\n" );
   Tcl_Eval(_tclInterpreter, cmdBuf);

   // Define convenient aliases
   strcpy(cmdBuf, "alias quit exit\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias logout exit\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias talk \"module talk\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias clone \"module clone\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias up exit\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias begin \"events begin\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias cont \"events continue\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias ev \"events\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias mod \"module\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias seq \"sequence\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
}

FtclInterp::FtclInterp(Tcl_Interp* interp) {

   _tclInterpreter = interp;

   char cmdBuf[1024];

   // Intialize help and alias
   ftclHelpInit(_tclInterpreter);
   ftclAliasInit(_tclInterpreter);
   ftclParseInit(_tclInterpreter);

   // redefine tcl's exit since Framework clobbers it otherwise
   strcpy(cmdBuf, "rename exit exitTcl\n" );
   Tcl_Eval(_tclInterpreter, cmdBuf);

   // Define convenient aliases
   strcpy(cmdBuf, "alias quit exit\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias logout exit\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias talk \"module talk\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias clone \"module clone\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias up exit\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias begin \"events begin\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias cont \"events continue\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias ev \"events\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias mod \"module\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);
   strcpy(cmdBuf, "alias seq \"sequence\"\n");
   Tcl_Eval(_tclInterpreter, cmdBuf);

}

//--------------
// Destructor --
//--------------

FtclInterp::~FtclInterp( )
{
   Tcl_DeleteInterp( _tclInterpreter );
}


// members
void 
FtclInterp::simulateInput( std::string strInput )
{
   Tcl_Eval( _tclInterpreter, (char *)strInput.c_str( ) );
}

AbsInterp::StatusCode
FtclInterp::runCommandFile( const char* filename )
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
FtclInterp::startInterpLoop( )
{
    // The following code is borrowed and slightly modified from 
    // function ftcl_CommandLoop, part of the ftcl package.

    CMD_EDITHNDL cmdHandle;
    CMD l_line;
    CMD tmp_line;
    int lineEntered;
    CMD_EDITHIST history;

    Tcl_DString cmdBuf;

    char inputBuf[1024];
    char partbuf[1024];		// used when partial command is entered

    bool topLevel = true;

// ---------------------------------------------------------------------------
	Tcl_DStringInit(&cmdBuf);

	inputBuf[0] = 0;
	partbuf[0]  = 0;	

// Initialize interrupt handler
	ftclCmd_INTDec(2);

// Initialize history
	ftclCmd_HistInit(&history,_tclInterpreter);
	
	setDoLoopInterp(true);

	while( getDoLoopInterp() )		
	{

// If a signal came in, reset the signals and drop any pending command.
	   if (ftclCmd_INTChk() )
	   {
		Tcl_DStringFree(&cmdBuf);
            	topLevel = true;
	   }

           clearerr (stdin);
           clearerr (stdout);

// Get ready for line entry : if the command is not complete and is split 
// over multiple lines, send a continuation line prompt
	   if (topLevel == false) 
	   {
	      (ftclCmd_LineStart(&cmdHandle, "=>", &history, 0) );
	   }
	   if (topLevel == true) 
	   {
	      (ftclCmd_LineStart(&cmdHandle, commandPrompt(), &history, 0) );
	   }
	   do			// Loop until line entry is complete
	   {

// Get single character input from stdin
	         ftclCmd_GetChar(&cmdHandle);

// Now process the entered character
	         lineEntered = ftclCmd_ProcChar(&cmdHandle, l_line);
	         	         
           } while (!lineEntered);		// Complete line

	   if (lineEntered == -1) // bad stuff happened...
           {
           
           //----------
           // M. Turcotte, UTDallas 2/9/98
           // Cath EOF condition and fake exit
                strcpy(l_line,"exit");
                strcpy(inputBuf,"exit\n");
                strcat(partbuf,"exit\n");
           //----------    
           
           } else { // everything normal...

	   strcpy (inputBuf,l_line);
	   strcat (inputBuf,"\n");		// Adding newline character

	   strcat (partbuf,l_line);
	   strcat (partbuf,"\n");
	   
	   }

	Tcl_DStringAppend(&cmdBuf, inputBuf, -1);
           if (!Tcl_CommandComplete(Tcl_DStringValue(&cmdBuf)))
	   {
	            topLevel = false;

   	   	    lineEntered = 0;		// Resetting line

	            continue;  			// Next line
           }

	   topLevel = true; 		// Reset Flag

	   // Note: return value from Tcl_RecordAndEval below is being ignored
	   //       You may want to use this for error checking in future
	   (void) Tcl_RecordAndEval(_tclInterpreter, partbuf, 0 );

	   // strip blanks before and after command
	   size_t cmdLen = strlen(l_line);
	   size_t indx = 0;
           size_t firstCh = cmdLen;
           size_t lastCh = 0;
	   while ( indx < cmdLen ) {
	     if (l_line[indx] != ' ') {
	       lastCh = indx;
	       if (firstCh == cmdLen) {firstCh = indx;}
	     }
	     indx++;
	   }
	   tmp_line[0] = '\0';
	   strncat(tmp_line, &l_line[firstCh], lastCh-firstCh+1);

           if ( (!strcmp(tmp_line, "exit")) |
                (!strcmp(tmp_line, "quit")) |
                (!strcmp(tmp_line, "logout")) )
           {
                Tcl_DStringFree(&cmdBuf);
 
                break;
           }

	   strcpy (partbuf,"");			   // Reinitialize buffer

           sendFullMessage( _tclInterpreter->result );

   	   lineEntered = 0;

   } // while(1)...
}

void
FtclInterp::deleteCommand( const char* commandName )
{
   Tcl_DeleteCommand( _tclInterpreter, (char *)commandName );
}
void
FtclInterp::createCommand( const char* commandName, const AbsCommand* command )
{
   Tcl_CreateCommand( _tclInterpreter, (char*) commandName, 
		      (Tcl_CmdProc*) commandHandler, (ClientData) command, 
		      (Tcl_CmdDeleteProc*) NULL );  
}

bool
FtclInterp::existsCommand( const char* commandName ) const
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

AbsInterp::StatusCode
FtclInterp::parseArgs( const std::string formalCmd, int argc, char *argv[] ) const
{
//    if ( ftclFullParseArg( (const_cast<std::string>(formalCmd)).c_str(), 
//		argc, argv ) == 0 )
    if ( ftclFullParseArg( (char *)formalCmd.c_str(), argc, argv ) == 0 )
    {
       return AbsInterp::ERROR;
    }
    else
    {
       return AbsInterp::OK;
    }
}

bool
FtclInterp::present( const std::string Key ) const
{
   if ( ftclPresent( (char *)Key.c_str() ) == 0 )
   {
      return false;
   }
   else
   {
      return true;
   }
}

int
FtclInterp::ascii2Int( const char* intString, int* intPtr ) const
{
   int status = Tcl_GetInt( _tclInterpreter, (char *)intString, intPtr ); 
   return status;
}
AbsInterp::StatusCode
FtclInterp::getInt( const std::string intKey, int &intPtr ) const
{
   if ( present( intKey.c_str() ) )   
   {
      intPtr = ftclGetInt( (char *)intKey.c_str() );
      return AbsInterp::OK;
   }
   else
   {
      return AbsInterp::ERROR;
   }
}

AbsInterp::StatusCode
FtclInterp::getUnsigned( const std::string uintKey, unsigned int &uintPtr ) const
{
   if ( present( uintKey.c_str() ) )
   {
      uintPtr = ftclGetUnsigned( (char *)uintKey.c_str() );
      return AbsInterp::OK;
   }
   else
   {
      return AbsInterp::ERROR;
   }   
}

AbsInterp::StatusCode
FtclInterp::getDouble( const std::string doubleKey, double &doublePtr ) const
{
   if ( present( doubleKey.c_str() ) )
   {
      doublePtr = ftclGetDouble( (char *)doubleKey.c_str() );
      return AbsInterp::OK;
   }
   else
   {
      return AbsInterp::ERROR;
   }  
}

AbsInterp::StatusCode
FtclInterp::getString( const std::string strKey, std::string* strPtr ) const
{
   if ( present( strKey.c_str() ) )
   {
      *strPtr = ftclGetStr( (char *)strKey.c_str() );
      return AbsInterp::OK;
   }
   else
   {
      return AbsInterp::ERROR;
   }
}

bool
FtclInterp::fileOperation( 
    const char* const fileName,
    const char* const operation ) const
{
    bool result = false;
    
    assert( NULL != operation );
    if ( NULL != fileName ) 
    {
       // Note: return value from Tcl_VarEval is ignored below
       //       You may want to use this for error checking in future
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
FtclInterp::setResult( const char* result ) const
{
  Tcl_SetResult(_tclInterpreter, (char*) result, TCL_VOLATILE);
  //   _tclInterpreter->result = (char *)result;
}

void
FtclInterp::appendResult( const char* result ) const
{
  Tcl_AppendResult(_tclInterpreter, (char*) result, (char*) NULL ); 
}


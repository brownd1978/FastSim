#ifndef FTCLLINE_HEADER
#define FTCLLINE_HEADER
/*****************************************************************************
******************************************************************************
**
** FILE:
**	ftclCmdLine.h
**
** ABSTRACT:    This file contains all necessary defintions, prototypes, and
**		macros for the routines used to support command line 
**              entry, editing, and command history recall.
**
** REQUIRED INCLUDE FILES:
**		"tcl.h"		-   Tcl_Interp definition
**
** ENVIRONMENT: ANSI C.
**
******************************************************************************
******************************************************************************
*/

#ifdef _ULTRIX
#include <sys/types.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif  /* ifdef cpluplus */



#define LINELEN		1024	/* Number of characters in command line */
#define HISTSIZE	50	/* Number of lines saved in command history */

/*
** COMMAND LINE STRING
*/
typedef char CMD[LINELEN];

/*
** COMMAND EDIT HISTORY STRUCTURE
*/
typedef struct cmd_edithist
   {
   CMD buf[HISTSIZE];	/* History buffer of line buffers */
   int nhndx;		/* Index to next available line buffer */
   int buffull;		/* Flag indicating history buffer is full (=1) */
#ifndef NO_TCL_HISTORY
   Tcl_Interp *interp;	/* TCL interp structure pointer */
#endif
   } CMD_EDITHIST;

/*
** COMMAND EDIT STRUCTURE (HANDLE)
*/
typedef struct cmd_edithndl
   {
   int nextchar;	/* Integer value of entered character */
   int lndx;		/* Index to last byte in current line */
   int cndx;		/* Index to cursor in current line */
   int hndx;		/* Index to history list */
   CMD line;		/* Current command line string */
   CMD saveline;	/* Saves current line when line history is used */
   CMD savehdr;		/* Saves line header (prompt) */
   int firstcol;	/* First user enterable column of line (after prompt) */
   int linemodified;    /* Flag indicating when line has been modified (=1) */
#ifdef VXWORKS
   int ed_savearg;	/* Original terminal characteristics - VxWorks only */
#endif
   CMD_EDITHIST *histptr;    /* History structure pointer */
   int escmode;		/* Escape sequence mode flag */
   } CMD_EDITHNDL;

#define L_UP 'A'
#define L_DOWN 'B'
#define L_RIGHT 'C'
#define L_LEFT 'D'
#define L_ESC '\033'
#define L_EOF '\004'
#define L_EOL '\005'
#define L_CR '\r'
#define L_LF '\n'
#define L_BS '\b'
#define L_DEL '\177'
#define L_BELL '\007'
#define L_RUB '\025'
#define L_REDRAW '\022'
#define L_FWD '\006'
#define L_BWD '\002'
#define L_SOH '\001'


/*---------------------------------------------------------------------
**
** FUNCTION PROTOTYPES
*/
void ftclCmd_LineStart(CMD_EDITHNDL *, const char *, CMD_EDITHIST *, int);
int  ftclCmd_GetChar(CMD_EDITHNDL *);
int  ftclCmd_ProcChar(CMD_EDITHNDL *, char *);
#ifdef NO_TCL_HISTORY
void ftclCmd_HistInit(CMD_EDITHIST *);
#else
void ftclCmd_HistInit(CMD_EDITHIST *, Tcl_Interp *);
#endif

#ifndef VXWORKS  /* These routines are not for vxWorks */
void ftclCmd_Reset(void);
void ftclCmd_INTDec(int);
int  ftclCmd_INTChk(void);
#endif /* !VXWORKS */

#ifdef __cplusplus
}
#endif  /* ifdef cpluplus */

#endif /* !FTCLLINE_HEADER */

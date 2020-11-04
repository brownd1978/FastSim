/*****************************************************************************
******************************************************************************
**
** FILE:
**	ftclCmdLine.c
**
** ABSTRACT:
**	This file contains routines used to support command line entry,
**      editing, and command history recall.  The command line routine
**	interface was written to work within a multiplexed I/O environment
**	provided by the Unix "select" function.  As such, this file also
**	contains "select" function utility routines.
**
**	A test program (originally for debug purposes) remains at the end of
**	this file and can be refered to as an example of how some of the 
**	routines in this file may be used.  The test program can be run
**	by compiling with the conditional directives listed following the
**	entry points.
**
**	AUTHOR'S DISCLAIMER: Hey, look out !!! This is admittedly a hastily
**	written, non-generic line editor.  It was written to suit our immediate
**	needs at the time (for another project) and no effort was made to
**	support the variety of terminals available.  It is hoped in the future
**	we can do something to remedy the situation and remove this disclaimer.
**	Until then, beware of this software.
**
**   ENTRY POINT	SCOPE	DESCRIPTION
**   -------------------------------------------------------------------------
**   ftclCmd_LineStart	public	Initialize, setup to read command line and 
**				print prompt.
**   ftclCmd_Reset	public	Restore original terminal characteristics.
**   ftclCmd_GetChar	public	Get next character from command line.
**   ftclCmd_ProcChar	public	Process single character input from command
**				line.
**
**   ftclCmd_HistInit	public	Initialize a command history list.	
**   ftclCmd_HistPrint  local	Print the history list to stdout.
**   ftclCmd_HistTCLRec	local	Recall command from TCL history.
**   ftclCmd_Internal   local	Check for and handle special command line
**				entries.
**
**   ftclCmd_INT	local	Signal handler called for Ctrl C interrupts.
**   ftclCmd_TSTP	local	Signal handler called for Ctrl Z interrupts. 
**   ftclCmd_CONT	local	Signal handler called for continuation.
**   ftclCmd_INTDec	public	Declare interrupt handler. 
**   ftclCmd_INTChk	public	Check if interrupt occurred.
**
**   ftclSelect_Init	public	Initialize select structure (handle).
**   ftclSelect_Add	public	Add FD to list of FD's to listen to.
**   ftclSelect_Remove	public	Remove FD from list of FD's to listen to.
**   ftclSelect_Check	public	Check FD to see if data is available to be read.
**   ftclSelect_RecvWait public	Wait until data is available on one or more FDs.
**
** ENVIRONMENT: ANSI C.
**
**   Conditional compilation is controlled by the following macros:
**
**   	VXWORKS		Build for VxWorks.
**	NO_TCL_HISTORY	Don't use TCL history recall.
**      EDITORTEST	Build line editing test program (main program).
**      SELECTTEST	Use select function in line editing test pgm.
**
**
**   For Unix Only (as of now) a 'locking mechanism' has been added to allow
**   two (or more) ftcl to run in the foreground at the same time!
**   They will actually alternate for the control of the terminal
**   
**   A better solution in the LONG run would probably to have job control
**   mechanism similar to the shells ones !!!
**   The current solution has the advantage to be quick to set-up and not
**   too disturbing .... A new solution should probably come at a time when
**   somebody decide the do something to remove the disclamer above
**
**   Philippe Canal, October 1995.
**
**   Apr 8th, 96: M.Vittone: Add check on interactive application; if not
**			     do not do file lock mechanism.
******************************************************************************
******************************************************************************
*/

#ifdef LINUX
#include <linux/posix_types.h>  /* for __FD_SETSIZE */
#include <linux/time.h>         /* for FD_SETSIZE */
#endif

#ifdef VXWORKS
#include <vxWorks.h>
#include <ioLib.h>
#include <stdioLib.h>
#include <ctype.h>
#elif VMS
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <iodef.h>
#include <ssdef.h>
#include <descrip.h>
#define MAXREC 4096	/* SYSGEN parameter MAXREC */

typedef struct{
	unsigned long	input_channel;
	int		input_socket;
	short int	iosb[4];
	char		string[MAXREC];
	} FTCL_QIO;

FTCL_QIO	ftclQioInfo;

#else
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>    /* 'Modern' interface to the terminal settings 
			   replace termio.h */
#include <sys/types.h>  /* Needed for select function */
#include <signal.h>     /* Needed for signal function */
#ifdef sun
/*#define GETPGRP_ARG 0 This is not correct for Solaris*/
#define GETPGRP_ARG
#else
#define GETPGRP_ARG
#endif
#endif

#ifndef NO_TCL_HISTORY
#include "tcl.h"
#endif

#include "FrameUtil/ftclCmdLine.h"
#include "FrameUtil/ftclCmdSelect.h"

/* included for test on fork */
#include <unistd.h>
#include <sys/types.h>
#include <string.h>


/*============================================================================
**============================================================================
**
** LOCAL PROTOTYPES, DEFINITIONS, MACROS, ETC.
** (NOTE: NONE OF THESE WILL BE VISIBLE OUTSIDE THIS MODULE)
**
**============================================================================
*/

/*---------------------------------------------------------------------
**
** LOCAL FUNCTION PROTOTYPES
*/
static void ftclCmd_HistPrint(CMD_EDITHIST *);
static int  ftclCmd_Internal(char *, int, char *, CMD_EDITHIST *, int *, int *);
#ifndef VMS
#ifndef VXWORKS
static void ftclCmd_INT();
static void ftclCmd_TSTP();
#endif
#ifndef NO_TCL_HISTORY
static int ftclCmd_HistTCLRec(Tcl_Interp *, int);
#endif
#endif
#ifdef VMS
static void *ftclASTEntered();
#endif

/*---------------------------------------------------------------------
**
** LOCAL DEFINITIONS, MACROS
*/

#define FTCL_TCLHIST	"history"	 /* TCL history command */
#define FTCL_TCLHISTREC	"history event"	 /* TCL history recall command */
#define FTCL_TCLHISTNXT	"history nextid" /* TCL history next event ID command */

#ifdef VMS
static	int		g_ed_savearg;
#else
#ifndef VXWORKS			 /* For UNIX only */
/*
** LOCALLY GLOBAL LINE EDITOR "ORIGINAL" TERMINAL CHARACTERISTICS
*/
static	struct termios	g_ed_savearg;
static  int             g_ed_was_on;     /* For ftclCmd_CONT and ftclCmd_TSTP
					    internal use only */
static  int             g_ed_on = 0;     /* 0 indicated the line editor itself 
					     in not currently running.
					     Set by ED_ON and ED_OFF */
/*
** LOCALLY GLOBAL INTERRUPT SIGNAL FLAGS
*/
static	pid_t		g_exit_pid;
static  int		g_exit_flag=0;
static	int		g_sigint_flag=0;
static	int		g_sigint_flag_max=0;
static  int		g_ed_timeout=0;
struct sigaction	g_sigint_old;
struct sigaction	g_sigtstp_old;
struct sigaction	g_sigcont_old;
struct sigaction	g_sigterm_old;
#endif
#endif

/*---------------------------------------------------------------------
**
** GETIT, PRINTLINE, CLRTOEOL : FTCLCMD_ LOCAL MACRO DEFINITONS
*/
#define GETIT getchar()
#define PRINTLINE fputs(a_hl->line, stdout)
#define CLRTOEOL \
   { \
   int i; \
   fputc('\r',stdout); \
   for(i=0; i < a_hl->lndx; i++) {fputc(' ', stdout);} \
   }

#define MOVECRSR \
   { \
   int i; \
   if (a_hl->cndx < a_hl->lndx) \
      { \
      for (i = 0; i < (a_hl->lndx - a_hl->cndx); i++) {fputc(L_BS, stdout);} \
      } \
   }


/*---------------------------------------------------------------------
**
** ED_ON : FTCLCMD_ MACRO TO SET TERMINAL CHARACTERISTICS
*/
/*
 * We added g_ed_on and g_ed_was_on variable.
 * g_ed_on is set to 1 when a ED_ON is executed
 *         and set to 0 after a ED_OFF (and at start up time)
 * ED_ON is actually executed only if g_ed_on is 0 (i.e. a ED_ON executed
 *         immediatly after another ED_ON won't have any effect)!
 * ED_OFF is actually executed only if g_ed_on is 1  (i.e. a ED_OFF executed
 *         immediatly after another ED_OFF won't have any effect)!
 * Also ftclCmd_CONT (Scheduled by ftclCmd_TSTP) will execute a ED_ON only
 *         g_ed_was_on is set to 1! (i.e. only if we went to sleep in the
 *         middle of a command line).
 *
 * CLEAN_LOCK is to be used for a 'end' of process life clean up of 
 *         the lock system. Particularly usefull when receiving SIGINT or
 *         SIGTERM.
 */
/* We implemented a locking mechanism to prevent two 'ftcl applications'
   to access the same tty input at the same time.

   Each time we enter the command line editing mode (ED_ON) we try to 
   lock the terminal (ftclLockTerminal()).  This routine will return
   only when we will have successfully locked the terminal.  Eventually
   we will have waited for another process to unlock it.
   When we exit ther command line editing mode (ED_OFF) we unlock the
   terminal (ftclUnlockTerminal())


   The locking mechanism is implement through ftclLockTerminal and
   ftclUnlockTerminal.  It base on the Patterson scheme
   for securing critical section.
   This busy waiting solution have been chosed for two main reasons:
        - the occasion were two ftcl wants to access the same tty
	  should be SELDOM, so bad performance should not be too much
	  of a problem.
	- a proper solution, using signals, will be much more complex.

   ftclLockTerminal():
      This function main purpose is to check for the existence of
      the file ftcl.#gpid# (#gpid# replaced by the value a the
      group process ID).
      
      The 'reservationFile' is used to prevent concurrent look up
      at the lockFile.
      
      Only the process which pid is on top of the reservationFile
      can 'trust' its checking of the existance of the lockFile.
      All other process have to wait and check later (even if they
      thought the terminal was not locked). 
      
      This is to prevent two processes that would execute 
      ftclLockTerminal at about the same time, to both believe
      they can lock the terminal for themself.

      if the lockFile does not exist yet and we are the reserving 
         process, we create the lockFile, delete the reservationFile
	 and proceed.
      if it does exist or if we are not the reserving process 
         we add our pid to the lockFile, wait some time and 
	 restart the checking process again.
 
   ftclUnlockTerminal()
      This function main purpose is to unlock the terminal so that
      other processes can access it again.

      We first check if any other process are waiting, if this is
      the case the lockFile contain at leat one pid.
      We delete the lockFile and if another process was waiting for
      the terminal we sleep for a while to give it a better chance to
      lock the terminal for itself.

   DEADLOCK ALERT:
      If the process that got the 'reservation' is put to interrupted
      before it removes the reservation file, we are in a deadlock
      until this process is continued or killed (cf ftclCmd_TERM).
      IN any case an SIGINT in the middle of ftclLockTerminal is
      a potential problem!
 */

char * lockName = "/tmp/ftcl";
char * reservationName = "/tmp/ftcl.res";

#ifdef VMS
#define ED_ON
#define ED_OFF
#define CLEAN_LOCK
#else
#ifdef VXWORKS
#define ED_ON(a_savearg, a_timeout) \
   { \
   int ed_arg; \
   (a_savearg) = ioctl(1, FIOGETOPTIONS, NULL); \
   ed_arg = (a_savearg); \
   ed_arg &= ~(OPT_LINE | OPT_ECHO); \
   ed_arg |= OPT_CRMOD; \
   ioctl(1, FIOOPTIONS, (void *)ed_arg); \
   }
#define CLEAN_LOCK
#else
#define CLEAN_LOCK \
  { \
   char lockFile[32],reservationFile[32]; \
   if (isatty (STDIN_FILENO) ) \
   { \
    sprintf(lockFile,"%s.%d",lockName,getpgrp(GETPGRP_ARG)); \
    sprintf(reservationFile,"%s.%d",reservationName,getpgrp(GETPGRP_ARG)); \
    remove(reservationFile); \
    remove(lockFile); \
   } \
  }

static void ftclLockTerminal ()
{
   char lockFile[32],reservationFile[32]; 
   FILE *lock, *reserv; 
   pid_t pid = 0; 
   sprintf(lockFile,"%s.%d",lockName,getpgrp(GETPGRP_ARG)); 
   sprintf(reservationFile,"%s.%d",reservationName,getpgrp(GETPGRP_ARG));

   /* 
    * Locking Mechanism 
    */ 
   if (!isatty (STDIN_FILENO) ) return;
   if ( (reserv = fopen(reservationFile,"a+"))==NULL ) 
     printf("Locking mechanism might fail\nReservation file not created\n"); 
   else 
     { 
       fprintf(reserv,"%d\n",getpid()); 
       rewind(reserv); 
       /*
	* Loop until both the lockFile does not exit and
	* we are the first process to reserve the access to
	* the lockFile
	*/
       while ( ((lock=fopen(lockFile,"r")) != NULL) || 
	       ( fscanf(reserv,"%d",&pid),pid!=getpid())
	     ) 
	 { 
	   if (lock!=NULL) 
	     { 
	       fclose(lock); 
	       lock = fopen(lockFile,"a+"); 
	       if (fscanf(lock,"%d",&pid)!=1) 
		 fprintf(lock,"%d\n",getpid()); 
	       fclose(lock); 
	     }; 
	   fclose(reserv); 
	   sleep(1); 
	   reserv = fopen(reservationFile,"a+"); 
	   fprintf(reserv,"%d\n",getpid()); 
	   rewind(reserv); 
	   };
       /*
	* Actually lock the terminal and 
	* remove old reservation
	*/
       if ( (lock=fopen(lockFile,"w")) == NULL ) 
	 printf("Locking mechanism might fail.\nLocking file not created\n"); 
       else 
	 { 
	   fclose(lock); 
	   remove(reservationFile); 
	   fclose(reserv); 
	 };  
     }; 
}
#define ED_ON(a_savearg,a_timeout) \
   { \
   struct termios ed_arg; \
       /* \
	* Lock terminal ONLY if interactive \
	*/ \
  if ( isatty(STDIN_FILENO) ) \
  {\
   if (g_ed_on==0) \
     { \
         ftclLockTerminal(); \
       /* \
	* Saving & setting terminal characteristic \
	*/ \
       tcgetattr( 1, a_savearg); \
       ed_arg = *(a_savearg); \
       ed_arg.c_lflag &= ~(ICANON | ECHO); \
       ed_arg.c_iflag &= ~(INLCR | ICRNL); \
       ed_arg.c_cc[VMIN] = ((a_timeout) ? 0 : 1); \
       ed_arg.c_cc[VTIME] = (a_timeout); \
       tcsetattr( 1, TCSANOW, &ed_arg); \
       g_ed_on = 1; /* Potential problem if a CTRL Z happens EXACTLY between the \
		       last two instructions! Sol:Disable CTRL Z around them. \
		       Not done since there is too faint of a chance! */ \
     } \
   }\
 }
#endif


/*---------------------------------------------------------------------
**
** ED_OFF : FTCLCMD_ MACRO TO RESET TERMINAL CHARACTERISTICS
*/
#ifdef VXWORKS
#define ED_OFF(a_savearg) \
   { \
   ioctl(1, FIOOPTIONS, (void *)(a_savearg)); \
   }
#else
static void ftclUnlockTerminal()
{
   char lockFile[32]; 
   FILE* lock; 
   pid_t next_pid; 
   /* 
    * 'Unlock' the terminal 
    */ 
   if (!isatty (STDIN_FILENO) ) return;

   sprintf(lockFile,"%s.%d",lockName,getpgrp(GETPGRP_ARG)); 
   if ( NULL != (lock=fopen(lockFile,"r")) ) 
     { 
       if ( fscanf(lock,"%d",&next_pid) == 1) 
         { 
           remove(lockFile); 
	   fclose(lock); 
	   sleep(1); 
         } 
       else 
         { 
	   remove(lockFile); 
           fclose(lock); 
         }; 
     } 
   else 
     { 
       printf("Error in the locking mechanism of the command Line\n *** Lock file not found!\n"); 
     }; 
}
#define ED_OFF(a_savearg) \
   { \
       /* \
	* 'Unlock' the terminal ONLY if interactive\
	*/ \
  if ( isatty(STDIN_FILENO) ) \
  {\
   if (g_ed_on==1) \
     { \
       tcsetattr( 1, TCSANOW, a_savearg); \
       g_ed_on = 0; /* Potential problem if a CTRL Z happens EXACTLY between the \
		       last two instructions! Sol:Disable CTRL Z around them. \
		       Not done since there is to faint of a chance! */ \
       ftclUnlockTerminal(); \
     } \
  }\
 }
#endif


#ifndef VXWORKS
static void ftclCmd_ExitHandler();	/* forward declaration */
#endif

#endif

#ifdef VMS
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclGetInputChannel
*/
int ftclGetInputChannel()
{
/* VMS initialization, to get the input channel for sys$input */
/* returns -1 or ftclQioInfo.input_channel */

  int status;
  struct sockaddr_in sin;
  struct hostent *hp;

  struct servent *sp;
$DESCRIPTOR(sys_input,"SYS$INPUT");

/*
 *  Get the IP address of the host named "LOCALHOST". This is
 *  the loopback address for ourselves
 */

  hp = gethostbyname("localhost");
  if (hp == NULL) {
        printf("ftclGetInputChannel: localhost unknown\n");
        return(-1);
        }
/*
 *  Create an IP-family socket on which to make the connection
 */

  ftclQioInfo.input_socket = socket(hp->h_addrtype, SOCK_STREAM, 0);
  if (ftclQioInfo.input_socket < 0) {
        printf("ftclGetInputChannel: call to socket failed\n");
         return(-1);
        }

/* 
 *  Get the TCP port number of the "echo" server.
 *
 */

  sp = getservbyname("echo", "tcp");
  if (sp == NULL) {
        printf("ftclGetInputChannel: echo/ttcp: unknown service\n");
	return(-1);
        }

  sin.sin_family = hp->h_addrtype;
  bcopy(hp->h_addr, &sin.sin_addr, hp->h_length);
  sin.sin_port = sp->s_port;

/*
 *  Connect to that address...
 */

  if (connect(ftclQioInfo.input_socket, &sin, sizeof (sin), 0) < 0) {
        printf("ftclGetInputChannel: call to connect failed\n");
        return(-1);
  }

  /* assign the terminal channel */
  status=sys$assign(&sys_input,&ftclQioInfo.input_channel,0,0);
  if(status != SS$_NORMAL){
                printf("ftclGetInputChannel: unable to assign sys$input status=%d\n",status);
                return(-1);
                }

  return(ftclQioInfo.input_socket);
}

/*============================================================================  
**============================================================================
**
** ROUTINE: ftclASTEntered
*/
static void *ftclASTEntered()
{
/* AST routines for SYS$QIO completion */

/* zero end of string - sys$qio leaves a 13 there */
  ftclQioInfo.string[ftclQioInfo.iosb[1]] = '\0';

/* write data and null to the socket*/
  socket_write(	ftclQioInfo.input_socket,
		ftclQioInfo.string,
		ftclQioInfo.iosb[1]+1);
}
#endif
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_LineStart
*/
void ftclCmd_LineStart
   (
   CMD_EDITHNDL *a_hl,	    /* MOD: ftclCmd handle (internal structure ptr) */
   const char   *a_prompt,  /* IN : Prompt to be displayed on input line */
   CMD_EDITHIST *a_histptr, /* MOD: Command history list */
   int		a_timeout   /* IN : Timeout (in tenths of seconds) for 
			            character input (0 = no timeout */
   )   
/*
** DESCRIPTION:
**	Initialize the structure (handle) used by ftclCmd_ routines,
**      setup to read characters from standard input, and print the line
**	prompt to standard output.  This routine is to be used in conjunction
**	with ftclCmd_GetChar and ftclCmd_ProcChar, and must be called
**	prior to line entry, for each line that is entered.
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	User's application
**
** GLOBALS REFERENCED
**	g_ed_savearg
**	g_ed_timeout
**	g_sigint_flag 
**
**============================================================================
*/
{
int status;
#ifndef VMS
#ifndef VXWORKS
/*
** SETUP EXIT HANDLER
*/
if (g_exit_flag == 0)
   {
   g_exit_flag++;
   g_exit_pid = getpid();
   atexit(ftclCmd_ExitHandler);
   }
#endif
#endif
/*
** SAVE THE HISTORY LIST POINTER
*/
a_hl->histptr = a_histptr;

/*
** SETUP THE BEGINNING OF THE LINE
*/
a_hl->line[0] = '\r';
strcpy(&a_hl->line[1], a_prompt);
strcpy(a_hl->saveline, a_hl->line);
strcpy(a_hl->savehdr, a_hl->line);
a_hl->firstcol = strlen(a_prompt) + 1;

/*
** SET TERMINAL CHARACTERISTICS
*/
/* For UNIX, use the global copy of the original characteristics - in case of 
   interrupt (CTRL C). */
#ifndef VMS
#ifndef VXWORKS
if (a_timeout > 0)
   {g_ed_timeout = a_timeout;}
else
   {g_ed_timeout = 0;}
ED_ON(&g_ed_savearg, g_ed_timeout); /* UNIX */
#else
ED_ON(a_hl->ed_savearg, 0);	    /* VxWorks */
#endif

#ifndef VXWORKS
/*
** RESET THE GLOBAL INTERRUPT FLAG
*/
g_sigint_flag  = 0; /* This flag is incremented whenever an interrupt occurs */
#endif
#endif
/*
** SETUP TO READ CHARACTERS UNTIL A CARRIAGE RETURN IS HIT
*/
a_hl->escmode = 0;  /* This flag is set when an escape character is entered */
a_hl->linemodified = 0;  /* This flag is set when a line has been modified */
a_hl->hndx = a_hl->histptr->nhndx;
a_hl->lndx = a_hl->firstcol;
a_hl->cndx = a_hl->firstcol;
a_hl->line[a_hl->firstcol] = (char)0;
a_hl->nextchar = 0;

/*
** PRINT PROMPT TO STANDARD OUTPUT
*/
#ifndef VMS
PRINTLINE;
#else
  status = sys$qio(     0,
                        ftclQioInfo.input_channel,
                        IO$_READPROMPT,
                        ftclQioInfo.iosb,
                        ftclASTEntered,
                        (unsigned long) 0,
                        ftclQioInfo.string,
                        MAXREC,
                        0,
                        0,
                        a_prompt,
                        strlen(a_prompt));

#endif
/* The fflush statement was inserted because of the inexplicable buffering that
   occurs when ftclCmd routines are used with the select function */
fflush(stdout); 
}



#ifndef VXWORKS  /* Global variable is not available under VxWorks */
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_Reset
*/
void ftclCmd_Reset
   (
   )   
/*
** DESCRIPTION:
**	Restore the original terminal characteristics.
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	User's application
**
** GLOBALS REFERENCED
**	g_ed_savearg
**
**============================================================================
*/
{
/* Reset terminal characteristics */
ED_OFF(&g_ed_savearg);
CLEAN_LOCK;
}
#endif

/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_GetChar
*/
int ftclCmd_GetChar
   (
   CMD_EDITHNDL  *a_hl	    /* MOD: ftclCmd handle (internal structure ptr) */
   )   
/*
** DESCRIPTION:
**	Get next character from command line (stdin).  The returned value
**	is for information purposes only - the caller may ignore it.
**	Not used for VMS
**
** RETURN VALUES:
**	An integer value representing the character just obtained is returned.
**	Note that this value is for information purposes only - the caller may
**	ignore it.  If an error occurred, then the subsequent call to 
**	ftclCmd_ProcChar will reflect the error, so the user need only check
**	the return status from ftclCmd_ProcChar.
**
** CALLED BY:
**	User's application
**
**============================================================================
*/
{
#ifndef VMS
#ifndef VXWORKS
errno = 0;
#endif
a_hl->nextchar = GETIT;

#ifndef VXWORKS
if (a_hl->nextchar == -1)
   {
   /* If there was an interrupted system call, then try one more time.  We
      do this since when we resume from a Ctrl Z (stopping) interrupt, an
      interrupted system call error is expected. */
   if (errno == EINTR)
      {a_hl->nextchar = GETIT;}

   /* Clear the EOF indicator.  This is required because under SunOS, getchar
      will always return an EOF on subsequent calls once an actual EOF 
      occurs (until it is cleared). */
   clearerr(stdin);
   }
#endif
#endif
return(a_hl->nextchar);
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_ProcChar
*/
int ftclCmd_ProcChar
   (
   CMD_EDITHNDL *a_hl,	    /* MOD: ftclCmd handle (internal structure ptr) */
   char	        *a_line	    /* OUT: Command line entered by user. NOTE: this
                                    parameter remains unchanged until command
                                    line entry is complete (return status = 1)*/
   )   
/*
** DESCRIPTION
**      Processes a single character input from the command line (stdin).
**	This routine must be preceded by a call to ftclCmd_LineStart (to
**	initialize) and ftclCmd_GetChar (to get the next character
**	from the command line).  ftclCmd_GetChar and this routine
**	should be called repeatedly until command line entry is complete
**	(return value = 1), at which time the output parameter a_line is
**	set.  Once line entry is complete, ftclCmd_LineStart must be invoked
**	again, if additional lines are expected.
**
**	VMS version, returns line back to user.
**
** RETURN VALUES:
**	 0 - Character entry processed successfully.
**	 1 - Command line entry is complete.
**	-1 - End-of-file detected (all processing is complete).
**
** CALLED BY:
**	User's application
**
** CALLS TO:
**	ftclCmd_Internal
**
**============================================================================
*/
{
CMD tmpline;
int tmp, rstatus, i, foundSpace, foundChar;
int entry, entry2, entry3;
int last_nhndx;
char savech;
int linecomplete;   /* Flag indicating when line entry is complete */
#ifndef NO_TCL_HISTORY
int	    tclstat;
#endif

#ifdef _ULTRIX
char escseq[5];
escseq[0] = L_ESC;
escseq[1] = '[';
escseq[2] = 'C';
escseq[3] = '\0';
#else
char escseq[] = {L_ESC,'[','C','\0'};
#endif

#ifdef VMS
int 	n;
char	buf[MAXREC];
#endif


/*---------------------------------------------------------------------
**
** PROCESS CHARACTER ENTERED FROM TERMINAL
*/
rstatus = 0;		/* Default return status */
linecomplete = 0;	/* Line entry is not complete yet */
entry = a_hl->nextchar;	/* Get local copy of entered character */

#ifndef VMS /* let sys$qio deal with editing */
/*------------------------------------------------------------------------
**
** IF ESCAPE SEQUENCE STARTED - CATCH THE WHOLE THING
*/
#ifndef VXWORKS		/* Under UNIX only */
if (a_hl->escmode)
   {
   /* 
   ** STILL IN ESCAPE MODE, MAKE SURE ENTIRE SEQUENCE IS COLLECTED
   */
   a_hl->escmode++;

   /* Second pass - make sure we got a '[' */
   if ((a_hl->escmode == 2) && (entry != (int)'['))
      {
      fputc(L_BELL, stdout);	/* Invalid sequence - yell at the user */
      a_hl->escmode = 0;	/* No longer in escape mode */
      fflush(stdout);
      return(rstatus);		/* Go fish */
      }
   /* Third pass - sequence complete, so continue */
   else if (a_hl->escmode == 3)
      {
      entry3 = entry;		/* Save last character of sequence */
      entry  = L_ESC;		/* Setup to process sequence */
      a_hl->escmode = 0;	/* No longer in escape mode - complete */
      }
   else
      {
      return(rstatus);		/* Vamoose */
      }
   }
else if (entry == L_ESC)
   {
   /* 
   ** ESCAPE SEQUENCE DETECTED, PUT IN ESCAPE MODE, THEN RETURN
   */
   a_hl->escmode = 1;
   return(rstatus);		/* Scadoodle */
   }
#endif

switch(entry)
   {

   /*------------------------------------------------------------------
   **
   ** IF AN ESCAPE SEQUENCE IS DETECTED - DEAL WITH IT
   */
   case L_ESC:
#ifdef VXWORKS
      entry2 = GETIT;
      entry3 = GETIT;
#endif
      switch(entry3)
         {

#ifdef NO_TCL_HISTORY
         /*
         ** UP ARROW WAS PRESSED - RECALL PREVIOUS LINE FROM HISTORY LIST
         */
         case L_UP:
            tmp = a_hl->hndx;	/* Temporary storage */
            a_hl->hndx--;

            /* Adjust index if we are at beginning of history list */
            if (a_hl->hndx < 0)
               {
               if (a_hl->histptr->buffull)
                  {a_hl->hndx = HISTSIZE-1;}
               else
                  {a_hl->hndx = 0; fputc(L_BELL, stdout);}
               }

            /* As long as we haven't recalled all the lines from the 
               history list yet, recall the previous line */
            if (a_hl->hndx != a_hl->histptr->nhndx)
               {
               CLRTOEOL;
               if (a_hl->linemodified)
                  {strcpy(a_hl->saveline, a_hl->line); a_hl->linemodified = 0;}
               strcpy(a_hl->line, a_hl->histptr->buf[a_hl->hndx]);
               a_hl->lndx = strlen(a_hl->line);
               a_hl->cndx = a_hl->lndx;
               PRINTLINE;
               }

            /* No no, we have recalled all the lines, so don't let them 
               recall anymore */
            else
               {
               a_hl->hndx = tmp; /* Undo the previous decrement */
               fputc(L_BELL, stdout); /* Yell at the user */
               }
            break;

         /*
         ** DOWN ARROW WAS PRESSED - GET NEXT LINE FROM HISTORY LIST
         */
         case L_DOWN:

            /* We can get the next line only if the UP key was
               previously pressed */
            if (a_hl->hndx != a_hl->histptr->nhndx)
               {
               a_hl->hndx++;

               /* Adjust index if we are at end of history list */
               if (a_hl->hndx == HISTSIZE)
                  {a_hl->hndx = 0;}

               /* As long as we haven't stepped forward to the latest line
                  in the history list, go ahead and get the next line */
               if (a_hl->hndx != a_hl->histptr->nhndx)
                  {
                  CLRTOEOL;
                  if (a_hl->linemodified)
                     {strcpy(a_hl->saveline, a_hl->line); a_hl->linemodified=0;}
                  strcpy(a_hl->line, a_hl->histptr->buf[a_hl->hndx]);
                  a_hl->lndx = strlen(a_hl->line);
                  a_hl->cndx = a_hl->lndx;
                  PRINTLINE;
                  }

               /* Hey, we are already at the latest line, there's nothing
                  left to recall, so get the original line */
               else
                  {
                  CLRTOEOL;
                  strcpy(a_hl->line, a_hl->saveline);
                  a_hl->lndx = strlen(a_hl->line);
                  a_hl->cndx = a_hl->lndx;
                  PRINTLINE;
                  }
               }

            /* The UP key was not previously pressed */
            else
               {
               fputc(L_BELL, stdout); /* Yell at the user */
               }
            break;

#else
         /*
         ** UP OR DOWN ARROW WAS PRESSED - RECALL LINE FROM TCL HISTORY LIST
         */
         case L_UP:
         case L_DOWN:
            tmp = a_hl->hndx;	/* Temporary storage */
            if (entry3 == L_UP)
               {a_hl->hndx--;}
            else
               {
               a_hl->hndx++;
               if (a_hl->hndx == 0)
                  {
                  /* Hey, we are already at the latest line, there's nothing
                     left to recall, so get the original line */
                  CLRTOEOL;
                  strcpy(a_hl->line, a_hl->saveline);
                  a_hl->lndx = strlen(a_hl->line);
                  a_hl->cndx = a_hl->lndx;
                  PRINTLINE;
                  break;
                  }
               }

            /* Use TCL history to get previous command */
            tclstat = ftclCmd_HistTCLRec(a_hl->histptr->interp, a_hl->hndx);
            if (tclstat != TCL_OK)
               {
               a_hl->hndx = tmp;
               fputc(L_BELL, stdout);
               break;
               }

            /* If we made it to here, then a command has been successfully
               located in the history - replace current line with it */
            CLRTOEOL;
            /* Save current line first, if it was modified */
            if (a_hl->linemodified)
               {strcpy(a_hl->saveline, a_hl->line); a_hl->linemodified = 0;}
            strcpy(a_hl->line, a_hl->savehdr);
            strcat(a_hl->line, a_hl->histptr->interp->result);
            a_hl->lndx = strlen(a_hl->line);
            a_hl->cndx = a_hl->lndx;
            PRINTLINE;
            break;
#endif  /* #else (from #ifdef NO_TCL_HISTORY) */

         /*
         ** RIGHT ARROW WAS PRESSED - MOVE CURSOR TO THE RIGHT
         */
         case L_RIGHT:
            if (a_hl->cndx < a_hl->lndx)
               {
               a_hl->cndx++;
               fputs(escseq, stdout);
               }
            break;

         /*
         ** LEFT ARROW WAS PRESSED - MOVE CURSOR TO THE LEFT
         */
         case L_LEFT:
            if (a_hl->cndx > a_hl->firstcol)
               {
               a_hl->cndx--;
               fputc(L_BS, stdout);
               }
            break;

         /*
         ** DEFAULT - UNKNOWN ESCAPE SEQUENCE
         */
         default:
            fputc(L_BELL, stdout);	/* Bitch at the user */
            break;
         }  /* switch */

      break;	    /* Done with handling of escape sequence */

   /*------------------------------------------------------------------
   **
   ** IF A CARRIAGE RETURN WAS HIT, THE LINE IS COMPLETE
   */
   case L_CR:
   case L_LF:

      /*
      ** CHECK FOR AND HANDLE SPECIAL COMMAND LINE ENTRIES
      */
      if (ftclCmd_Internal(a_hl->line, a_hl->firstcol, a_hl->savehdr, 
                          a_hl->histptr, &a_hl->cndx, &a_hl->lndx))
         {
         a_hl->linemodified = 0;
         CLRTOEOL;
         PRINTLINE;
         }

      else
         {
         /*
         ** IF WE GET TO HERE, THEN NO SPECIAL COMMANDS WERE ENTERED, SO
         ** ADD THE LINE TO THE LINE HISTORY ONLY IF IT IS DIFFERENT FROM
         ** THE PREVIOUS LINE
         */
         linecomplete = 1;  /* Set flag indicating line entry is done */

#ifdef NO_TCL_HISTORY
         last_nhndx = a_hl->histptr->nhndx - 1;
         if (last_nhndx < 0)
            {
            if (a_hl->histptr->buffull) {last_nhndx = HISTSIZE-1;}
            else {last_nhndx = 0;}
            }
         if ((strcmp(a_hl->histptr->buf[last_nhndx], a_hl->line) != 0) &&
             (strlen(a_hl->line) > a_hl->firstcol))
            {
            strcpy(a_hl->histptr->buf[a_hl->histptr->nhndx], a_hl->line);
            a_hl->histptr->nhndx++;
            if (a_hl->histptr->nhndx == HISTSIZE)
               {
               a_hl->histptr->nhndx = 0;
               a_hl->histptr->buffull = 1;
               }
            }
#endif

         PRINTLINE;
         fputc('\n', stdout);
         }  /* end else from "if (!(ftclCmd_Internal(a_hl->line, ..." */

      break;

   /*------------------------------------------------------------------
   **
   ** IF A '^A' WAS ENTERED, GO TO THE START OF THE LINE
   */
   case L_SOH:
      a_hl->cndx = a_hl->firstcol;
      PRINTLINE;
      MOVECRSR;
      break;

   /*------------------------------------------------------------------
   **
   ** IF A RUBOUT ('^U') WAS ENTERED, CLEAR ALL TO LEFT OF CURSOR
   */
   case L_RUB:
      CLRTOEOL;
      if (a_hl->cndx < a_hl->lndx)
         {
         strcpy(tmpline, &a_hl->line[a_hl->cndx]);
         strcpy(&a_hl->line[a_hl->firstcol], tmpline);
         a_hl->lndx = strlen(tmpline) + a_hl->firstcol;
         }
      else
         {
         a_hl->line[a_hl->firstcol] = (char)0;
         a_hl->lndx = a_hl->firstcol;
         }
      a_hl->cndx = a_hl->firstcol;
      PRINTLINE;
      MOVECRSR;
      break;

   /*------------------------------------------------------------------
   **
   ** IF A '^F' WAS ENTERED, MOVE CURSOR TO NEXT WORD
   */
   case L_FWD:
      foundSpace = 0;
      for (i=a_hl->cndx; i<=a_hl->lndx; i++)
         {
         if (a_hl->line[i] == (char)' ')
            {
            foundSpace = 1;
            }
         else if (foundSpace)
            {
            a_hl->cndx = i;
            PRINTLINE;
            MOVECRSR;
            break;
            }
         }
      break;

   /*------------------------------------------------------------------
   **
   ** IF A '^B' WAS ENTERED, MOVE CURSOR TO PREVIOUS WORD
   */
   case L_BWD:
      foundChar = 0;
      for (i=a_hl->cndx-1; i>=a_hl->firstcol; i--)
         {
         if (a_hl->line[i] == (char)' ')
            {
            if (foundChar == 1)
               {
               a_hl->cndx = i+1;
               PRINTLINE;
               MOVECRSR;
               break;
               }
            }
         else if (i == a_hl->firstcol)
            {
            a_hl->cndx = i;
            PRINTLINE;
            MOVECRSR;
            break;
            }
         else
            {
            foundChar = 1;
            }
         }
      break;

   /*------------------------------------------------------------------
   **
   ** IF '^E' WAS ENTERED, GO TO THE END OF THE LINE
   */
   case L_EOL:
      a_hl->cndx = a_hl->lndx;
      PRINTLINE;
      break;

   /*------------------------------------------------------------------
   **
   ** IF '^R' WAS ENTERED, REDRAW THE LINE
   */
   case L_REDRAW:
      CLRTOEOL;
      PRINTLINE;
      break;

   /*------------------------------------------------------------------
   **
   ** IF THE DELETE KEY WAS HIT, REMOVE LAST CHARACTER FROM LINE
   */
   case L_DEL:
   case L_BS:
      if (a_hl->cndx > a_hl->firstcol)
         {
         if (a_hl->cndx < a_hl->lndx)
            {
            strcpy(tmpline, &a_hl->line[a_hl->cndx]);
            strcpy(&a_hl->line[a_hl->cndx - 1], tmpline);
            }
         else
            {
            a_hl->line[a_hl->cndx - 1] = (char)0;
            }
         a_hl->cndx--;
         a_hl->lndx--;
         a_hl->linemodified = 1;
         CLRTOEOL;
         PRINTLINE;
         MOVECRSR;
         }
      break;

   /*------------------------------------------------------------------
   **
   ** IF END OF FILE THEN GET OUT
   */
   
   /* M. Turcotte, UTDallas 2-9-98 
      EOF was not detected and this leads to an infinite loop
      There seems to be a misunderstanding to what an EOF is;
      in iostream.h it is -1 and not \004 as is set in L_EOF.
      But put in the check anyway; it doesn't hurt...
   */
   
   case L_EOF:
     printf("EOF encountered while trying to read stdin\n");
     linecomplete = -1;
     break;

   
   /* PCG: In case of -1 it could also be an error! 
   **      errno == EINTR  means that the read operation was interrupted 
   **               by a signal which was caught and no data was transferred.
   **      So we should not do anything except continue! Especially we should
   **               tell ftcl that we reach an End of File!
   
   EOF condition is -1. The other two conditions are not detected when
   this happens. M. Turcotte, UTDallas 2-9-98
   
   **
   **      errno == EIO (at least on Unix) means;
   **               The call is attempting to read from the process's 
   **               controlling terminal and either the process is 
   **		    ignoring or blocking the SIGTTIN signal or the 
   **		    process group is orphaned.
   **      So if we see it here it means that this process is orphaned
   **               (since there is (as of today 10-19-95) no ignoring or
   **               blocking of SIGTTIN)
   */
   case -1:
/*   
     Check for EOF condition as per iostream.h (.hxx)
     M. Turcotte, UTDallas 2-9-98
*/
     printf("Detected EOF condition encountered while trying to read stdin\n");
     switch(errno)
	{
	case EIO:
	   printf("Detected EIO while trying to read stdin\n");
	   linecomplete = -1;
	   break;
	case EINTR: 
	   printf("Detected EINTR while reading stdin\n");
           /* Do nothing */
	   break;                 
	default:
	   linecomplete = -1;
	   break;
	 };	 	 
      break; 
   case  0:
      linecomplete = -1;
      break;

   /*------------------------------------------------------------------
   **
   ** ADD PRINTABLE CHARACTERS TO THE LINE
   */
   default:

      /* Only add printable characters, ignore anything else */
      if (isprint((char)entry))
         {

         /* Make sure the line is not full */
         if ((a_hl->lndx + 1) < LINELEN)
            {
            if (a_hl->cndx < a_hl->lndx)
               {
               /* We're not at the end of the line, so we need to copy */
               strcpy(tmpline, &a_hl->line[a_hl->cndx]);
               a_hl->line[a_hl->cndx] = (char)entry;
               a_hl->cndx++;
               strcpy(&a_hl->line[a_hl->cndx], tmpline);
               a_hl->lndx++;
               a_hl->line[a_hl->lndx]   = (char)0;	/* Null terminate */
               a_hl->linemodified = 1;
               PRINTLINE;
               MOVECRSR;
               }
            else
               {
               /* We're at the end of the line, so just add it */
               a_hl->line[a_hl->cndx] = (char)entry;
               a_hl->cndx++;
               a_hl->lndx++;
               a_hl->line[a_hl->lndx]   = (char)0;	/* Null terminate */
               a_hl->linemodified = 1;
               fputc((char)entry, stdout);
               }
            }

         /* Uh oh, the line is full - scream at the user */
         else
            {
            fputc(L_BELL, stdout);
            }
         }
      break;

   } /* switch(entry)... */

/*---------------------------------------------------------------------
**
** END OF CHARACTER PROCESSING - CHECK IF CARRIAGE RETURN WAS HIT.
*/

/*
** IF LINE IS COMPLETE, THEN COPY THE LINE TO THE USER'S ARGUMENT
*/
if (linecomplete == 1)
   {
#ifndef VXWORKS
   ED_OFF(&g_ed_savearg);	/* UNIX */
#else
   ED_OFF(a_hl->ed_savearg);	/* VxWorks */
#endif
   strcpy(a_line, &a_hl->line[a_hl->firstcol]);
   rstatus = 1;  /* Set return status so user knows line entry is complete */
   }
else if (linecomplete == -1)
   {
#ifndef VXWORKS
   ED_OFF(&g_ed_savearg);	/* UNIX */
#else
   ED_OFF(a_hl->ed_savearg);	/* VxWorks */
#endif
   rstatus = -1;  /* Set return status so user knows end-of-file was detected */
   }

#endif	/* VMS does a lot of this - will need a fix for history */

#ifdef VMS
  /* read the socket */
  n = socket_read(ftclQioInfo.input_socket, buf, MAXREC);
  if(n < 1){
	*a_line = '\0';
	printf("ftclCmd_ProcChar: Error from socket_read, n=%d\n",n);
	return(2);
	}
  rstatus=1;
  strncpy(a_line,buf,n);
#endif

/* The fflush statement was inserted because of the inexplicable buffering that
   occurs when ftclCmd routines are used with the select function */
fflush(stdout); 

return(rstatus);

} /* ftclCmd_ProcChar */



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_HistInit
*/
void ftclCmd_HistInit
   (
#ifdef NO_TCL_HISTORY
   CMD_EDITHIST *a_histptr   /* OUT: Command history struct to be initialized */
#else
   CMD_EDITHIST *a_histptr,  /* OUT: Command history struct to be initialized */
   Tcl_Interp	*a_interp    /* IN : TCL interp handle */
#endif
   )   
/*
** DESCRIPTION:
**      Initialize a command history structure.
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	User's application
**
**============================================================================
*/
{
#ifndef NO_TCL_HISTORY
a_histptr->interp  = a_interp;
#endif
a_histptr->buffull = 0;
a_histptr->nhndx   = 0;
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_HistPrint
*/
static void ftclCmd_HistPrint
   (
   CMD_EDITHIST *a_histptr   /* IN: Command history struct list to be printed */
   )   
/*
** DESCRIPTION:
**      Print the history list to stdout.
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	ftclCmd_Internal
**
**============================================================================
*/
{
#ifdef NO_TCL_HISTORY
int  pndx, i;
char *lstr;

fputc('\n', stdout);
if (a_histptr->buffull)
   {
   pndx = a_histptr->nhndx + 1;
   if (pndx >= HISTSIZE)
      {pndx = 0;}
   }
else
   {pndx = 0;}

i = 0;
while (pndx != a_histptr->nhndx)
   {
   lstr = a_histptr->buf[pndx]; 
   fprintf(stdout, "  %d ", i);
   fputs(&lstr[1], stdout);
   fputc('\n', stdout);
   pndx++;
   if (pndx == HISTSIZE)
      {pndx = 0;}
   i++;
   if (i > HISTSIZE) {break;}  /* Sanity check - no infinite loops here */
   }

#else
int  tclstat;

tclstat = Tcl_Eval(a_histptr->interp, FTCL_TCLHIST);
if (tclstat == TCL_OK)
   {
   if (*a_histptr->interp->result != 0)
      {printf("\n%s\n", a_histptr->interp->result);}
   }
#endif
}



#ifndef NO_TCL_HISTORY
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_HistTCLRec
*/
static int ftclCmd_HistTCLRec
   (
   Tcl_Interp	*a_interp,  /* MOD: TCL Interp handle */
   int		a_hndx	    /* IN : History recall offset */
   )   
/*
** DESCRIPTION:
**	Recall command from TCL history.
**
** RETURN VALUES:
**	TCL_OK
**	TCL_ERROR
**
** CALLED BY:
**	ftclCmd_ProcChar
**	ftclCmd_Internal
**
** CALLS TO:
**	Tcl_Eval
**
**============================================================================
*/
{
int	tclstat, l_tclndx;
char	l_histcmd[40];

/* Had to use this funky (rather indirect) method to retrieve commands
   since TCL won't let me retrieve the most recent command entered.  It
   assumes the history command is the most recent, but in this case it's
   not since I'm using Tcl_Eval instead of Tcl_RecordAndEval.
*/
tclstat = Tcl_Eval(a_interp, FTCL_TCLHISTNXT);
if (tclstat != TCL_OK)
   {goto err_exit;}

#ifdef VXWORKS
sscanf(a_interp->result, "%d", &l_tclndx);
l_tclndx += a_hndx;
#else
l_tclndx = atoi(a_interp->result) + a_hndx;
#endif
if (l_tclndx <= 0)
   {tclstat = TCL_ERROR; goto err_exit;}

sprintf(l_histcmd, "%s %d", FTCL_TCLHISTREC, l_tclndx);
tclstat = Tcl_Eval(a_interp, l_histcmd);
if (tclstat != TCL_OK)
   {goto err_exit;}

return(TCL_OK);

err_exit:
return(tclstat);
}
#endif


#ifndef VMS
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_Internal
*/
static int ftclCmd_Internal
   (
   char *a_line,	    /* MOD: Entered line */
   int  a_firstcol,         /* IN : First user-enterable column in line */
   char *a_savehdr,	    /* IN : Command line header (prompt) */
   CMD_EDITHIST *a_histptr, /* IN : Command history struct */
   int  *a_cndx,            /* MOD: Index to cursor in line */
   int  *a_lndx             /* MOD: Index to last byte in line */
   )   
/*
** DESCRIPTION:
**      Check for and handle special command line entries 
**      (e.g., history recall).
**
** RETURN VALUES:
**	0 - No special commands were entered.    
**	1 - Special command was entered and processed.
**
** CALLED BY:
**	ftclCmd_ProcChar
**
** CALLS TO:
**	ftclCmd_HistPrint
**	Tcl_Eval
**
**============================================================================
*/
{
int  pndx, histndx, i, l_num;
char *l_line;
char *lstr;
char l_histcmd[40];

l_line = &a_line[a_firstcol];

/*
** IF THERE'S NOTHING ON THE LINE, GET OUT
*/
if (strlen(l_line) <= 0)
   {return(0);}

/*
** IF "h" WAS ENTERED, DISPLAY THE HISTORY LIST 
*/
if (!strcmp(l_line, "h"))
   {
   ftclCmd_HistPrint(a_histptr);
   *a_cndx = a_firstcol;
   *a_lndx = a_firstcol;
   a_line[a_firstcol] = (char)0;
   return(1);
   }

/*
** IF "!" WAS ENTERED, RECALL THE DESIRED LINE
*/
if (l_line[0] == '!')
   {

   /*
   ** IF A '!!' WAS ENTERED, RECALL THE LAST LINE FROM THE HISTORY LIST
   */
   if (l_line[1] == '!')
      {
#ifdef NO_TCL_HISTORY
      /* Determine the index to the last line */
      pndx = a_histptr->nhndx - 1;
      if (pndx < 0)
         {
         if (a_histptr->buffull)
            {pndx = HISTSIZE-1;}
         else
            {
            fputc(L_BELL, stdout);
            return(1);
            }
         }

      /* Get the last line from the history */
      strcpy(a_line, a_histptr->buf[pndx]);
      *a_lndx = strlen(a_line);
      *a_cndx = *a_lndx;

#else
      if (ftclCmd_HistTCLRec(a_histptr->interp, -1) == TCL_OK)
         {
         strcpy(a_line, a_savehdr);
         strcat(a_line, a_histptr->interp->result);
         *a_lndx = strlen(a_line);
         *a_cndx = *a_lndx;
         }
      else
         {
         fputc(L_BELL, stdout);
         return(1);
         }
#endif
      return(1);
      }
   
   /*
   ** IF A NUMBER WAS ENTERED, RECALL THE ASSOCIATED LINE FROM THE HISTORY
   ** LIST
   */
   if (isdigit(l_line[1]))
      {
      l_num = HISTSIZE; /* Set this just in case the sscanf fails */

      /* Determine the number entered */
      sscanf(&l_line[1], "%3d", &l_num);

#ifdef NO_TCL_HISTORY
      /* If user entered some huge number, yell at him */
      if (l_num >= (HISTSIZE-1))  
         {
         fputc(L_BELL, stdout);
         return(1);
         }

      /* Set 'pndx' to point to the oldest line in the history */
      if (a_histptr->buffull)
         {
         pndx = a_histptr->nhndx + 1;
         if (pndx >= HISTSIZE)
            {pndx = 0;}
         }
      else
         {
         pndx = 0;
         if (l_num >= a_histptr->nhndx)
            {
            fputc(L_BELL, stdout);
            return(1);
            }
         }

      /* Map the entered number to the appropriate line in the history */
      histndx = l_num + pndx;
      if (histndx >= HISTSIZE)
         {
         histndx -= HISTSIZE;
         }

      /* Get the line from the history */
      strcpy(a_line, a_histptr->buf[histndx]);
      *a_lndx = strlen(a_line);
      *a_cndx = *a_lndx;

#else
      sprintf(l_histcmd, "%s %d", FTCL_TCLHISTREC, l_num);
      if (Tcl_Eval(a_histptr->interp, l_histcmd) == TCL_OK)
         {
         strcpy(a_line, a_savehdr);
         strcat(a_line, a_histptr->interp->result);
         *a_lndx = strlen(a_line);
         *a_cndx = *a_lndx;
         }
      else
         {
         fputc(L_BELL, stdout);
         }
#endif
      return(1);
      }
   
   /*
   ** OTHERWISE, SEARCH THE HISTORY LIST FOR THE ASSOCIATED TEXT STRING
   */
   else
      {
#ifdef NO_TCL_HISTORY
      i=0; /* Sanity check counter */
      pndx = a_histptr->nhndx;
      while(1)
         {
         /* Determine the index to the last line */
         pndx--;
         if (pndx < 0)
            {
            if (a_histptr->buffull)
               {pndx = HISTSIZE-1;}
            else
               {break;} /* Get out of while loop */
            }
         if (pndx == a_histptr->nhndx) 
            {break;} /* Get out of while loop */

         lstr = a_histptr->buf[pndx]; 
         if (!(strncmp(&l_line[1], &lstr[a_firstcol], strlen(&l_line[1]))))
            {
            /* We found a match, so get the line from the history */
            strcpy(a_line, lstr);
            *a_lndx = strlen(a_line);
            *a_cndx = *a_lndx;
            return(1);
            }
         i++;
         if (i > HISTSIZE) {break;}  /* Sanity check - avoid infinite loops */
         } /* end: "while(1)..." */

      /* If we make it to here, then no string match was found */
      fputc(L_BELL, stdout);
      return(1);

#else
      /* Since TCL won't check the most recent command for us, we have
         to do it */
      if (ftclCmd_HistTCLRec(a_histptr->interp, -1) == TCL_OK)
         {
         if (!(strncmp(&l_line[1], a_histptr->interp->result,
               strlen(&l_line[1]))))
            {
            /* We found a match, so get the line from the history */
            strcpy(a_line, a_savehdr);
            strcat(a_line, a_histptr->interp->result);
            *a_lndx = strlen(a_line);
            *a_cndx = *a_lndx;
            return(1);
            }
         }

      /* Well, looks like the string wasn't in the most recent command, so
         check the rest */
      sprintf(l_histcmd, "%s %s", FTCL_TCLHISTREC, &l_line[1]);
      if (Tcl_Eval(a_histptr->interp, l_histcmd) == TCL_OK)
         {
         strcpy(a_line, a_savehdr);
         strcat(a_line, a_histptr->interp->result);
         *a_lndx = strlen(a_line);
         *a_cndx = *a_lndx;
         return(1);
         }
      else
         {
         fputc(L_BELL, stdout);
         return(1);
         }
#endif
      } /* end else: "if (isdigit(l_line[1]))..." */
   }

/*
** IF WE MADE IT TO HERE, THEN NO SPECIAL COMMANDS WERE ENTERED 
*/
return(0);
}
#endif /* not for VMS */



#ifndef VXWORKS  /* Interrupt routines are not for VxWorks */
#ifndef VMS	/* or VMS */
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_INT
*/
static void ftclCmd_INT
   (
   )   
/*
** DESCRIPTION:
**	Signal handler routine that is called to cleanup when an interrupt
**	signal (e.g., Ctrl C) occurs.  This routine restores the original
**	terminal characteristics and then sets a global flag (g_sigint_flag) 
**	indicating the interrupt occurred.  This flag can be checked
**	(via ftclCmd_INTChk) by the user to determine when an interrupt
**	occurred.
**
** RETURN VALUES:
**	None.
**
** DEFINED AS SIGNAL HANDLER BY:
**	ftclCmd_INTDec
**
** GLOBALS REFERENCED
**	g_ed_savearg
**	g_sigint_flag 
**	g_sigint_flag_max
**	g_sigint_old
**
**============================================================================
*/
{
struct sigaction    act;

/*
** INCREMENT THE GLOBAL INTERRUPT FLAG
*/
g_sigint_flag++;

/*
** TELL THE USER AN INTERRUPT WAS DETECTED
*/
fputc(L_BELL, stdout);
fputs("\n-- INTERRUPT --\n", stdout);
if ((g_sigint_flag + 1) == g_sigint_flag_max)
   {fputs("-- Hit ^C again to exit program --\n", stdout);}

/*
** IF WE'VE EXCEEDED THE MAX NUMBER OF INTERRUPTS COUNT, THEN EXIT THE PROGRAM
*/
if (g_sigint_flag >= g_sigint_flag_max)
   {
   /*
   ** FIRST RESTORE THE ORIGINAL TERMINAL CHARACTERISTICS
   */
   ED_OFF(&g_ed_savearg);
   CLEAN_LOCK;
   /*
   ** CALL THE OLD HANDLER, THEN RESTORE DEFAULT HANDLER AND RE-SEND INTERRUPT
   ** SO DEFAULT ACTION TAKES PLACE
   */
   if ((g_sigint_old.sa_handler != SIG_IGN) && 
       (g_sigint_old.sa_handler != SIG_DFL) &&
       (g_sigint_flag == g_sigint_flag_max))
      {
      g_sigint_old.sa_handler(SIGINT);
      }

   /* If we're still here, then restore the default handler so we exit */
   sigemptyset(&act.sa_mask);
   act.sa_flags = 0;
   act.sa_handler = (void (*)())SIG_DFL;
   sigaction(SIGINT, &act, NULL);
   raise(SIGINT);
   }
}



static void ftclCmd_CONT();	/* forward declaration */
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_TSTP
*/
static void ftclCmd_TSTP
   (
   )   
/*
** DESCRIPTION:
**	Signal handler routine that is called to cleanup when a Ctrl Z
**	interrupt signal (SIGTSTP) occurs.  This routine restores the original
**	terminal characteristics then re-sends the SIGTSTP signal so the 
**	default action will take place (i.e., the process will be stopped).
**	When the process is resumed, this routine will once again be declared
**	as the interupt handler.
**
**	This only reason this handler is required, is to restore the original
**	terminal characteristics.
**
** RETURN VALUES:
**	None.
**
** DEFINED AS SIGNAL HANDLER BY:
**	ftclCmd_INTDec
**	ftclCmd_CONT
**
** GLOBALS REFERENCED
**	g_ed_savearg
**	g_sigtstp_old
**	g_sigcont_old
**
**============================================================================
*/
{
struct sigaction    act;

if (g_ed_on)      /* If the line editor characteric are set */
  {
    /*
     ** RESTORE THE ORIGINAL TERMINAL CHARACTERISTICS
     */
    ED_OFF(&g_ed_savearg);
    g_ed_was_on = 1;
  }
else
  g_ed_was_on = 0;
 /*
 ** DECLARE OUR SIGCONT SIGNAL HANDLER
 */
sigemptyset(&act.sa_mask);
act.sa_flags = 0;
act.sa_handler = (void (*)())ftclCmd_CONT;
sigaction(SIGCONT, &act, &g_sigcont_old);  /* Continue from CTRL Z interrupt */
/*
** RESTORE OLD HANDLER, THEN RE-SEND INTERRUPT SO DEFAULT ACTION TAKES PLACE
*/
sigaction(SIGTSTP, &g_sigtstp_old, NULL);
raise(SIGTSTP);
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_CONT
*/
static void ftclCmd_CONT
   (
   )   
/*
** DESCRIPTION:
**	Signal handler routine that is called once the process is resumed from
**	a Ctrl Z interrupt signal (SIGTSTP).  This routine simply redeclares
**	ftclCmd_TSTP as the Ctrl Z interrupt handler, sets the terminal
**	characteristics back to command line input, then re-issues the SIGCONT
**	signal with the previous handler.
**
** RETURN VALUES:
**	None.
**
** DEFINED AS SIGNAL HANDLER BY:
**	ftclCmd_TSTP
**
** GLOBALS REFERENCED
**	g_ed_savearg
**	g_sigcont_old
**	g_sigtstp_old
**
**============================================================================
*/
{
struct termios      l_ed_savearg;

struct sigaction    act;

/*
** REDECLARE THE SIGTSTP SIGNAL HANDLER
*/
sigemptyset(&act.sa_mask);
act.sa_flags = 0;
act.sa_handler = (void (*)())ftclCmd_TSTP;
sigaction(SIGTSTP, &act, &g_sigtstp_old);    /* CTRL Z interrupt */
/*
** SET TERMINAL CHARACTERISTICS FOR COMMAND LINE EDITING
*/
if (g_ed_was_on)
  {
    l_ed_savearg = g_ed_savearg;
    ED_ON(&l_ed_savearg, g_ed_timeout);
  };
/*
** RESTORE OLD HANDLER, THEN RE-SEND INTERRUPT SO DEFAULT ACTION TAKES PLACE
*/
sigaction(SIGCONT, &g_sigcont_old, NULL);
raise(SIGCONT);
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_TERM
*/
static void ftclCmd_TERM
   (
   )   
/*
** DESCRIPTION:
**	Signal handler routine that is called when a kill command is issued.
**	This routine simply resets the terminal characteristics, then re-issues
**	the SIGTERM signal with the previous handler.
**
** RETURN VALUES:
**	None.
**
** DEFINED AS SIGNAL HANDLER BY:
**	ftclCmd_INTDec
**
** GLOBALS REFERENCED
**	g_ed_savearg
**	g_sigterm_old
**
**============================================================================
*/
{
/*
** RESTORE THE ORIGINAL TERMINAL CHARACTERISTICS
** Remove reservationFile that could be left over
*/
ED_OFF(&g_ed_savearg);
CLEAN_LOCK;
/*
** RESTORE OLD HANDLER, THEN RE-SEND INTERRUPT SO DEFAULT ACTION TAKES PLACE
*/
sigaction(SIGTERM, &g_sigterm_old, NULL);
raise(SIGTERM);
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_INTDec
*/
void ftclCmd_INTDec
   (
   int	    a_cnt	/* IN: Number of successive interrupts (CTRL C's)
                               allowed before exit() is called */
   )   
/*
** DESCRIPTION:
**	Declare the ftclCmd_INT routine as a signal handler and set the
**	number of successive interrupts allowed before we exit the program.
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	User's application
**
** GLOBALS REFERENCED
**	g_ed_savearg
**	g_sigint_flag
**	g_sigint_flag_max
**	g_sigint_old
**	g_sigtstp_old
**	g_sigcont_old
**	g_sigterm_old
**
**============================================================================
*/
{
struct sigaction act, curHndlr;

/* Initialize global variables just in case a CTRL C is hit before any of the
   ftclCmd_ routines are called.
   Also insure that the lock mechanism is cleaned. */
bzero(&g_ed_savearg, sizeof(g_ed_savearg));
g_sigint_flag = 0;
tcgetattr( 1, &g_ed_savearg);
CLEAN_LOCK;

if (a_cnt <= 0) {a_cnt = 1;}
g_sigint_flag_max = a_cnt;

/* Set SIGINT handler (if we're not already the handler) */
sigaction(SIGINT, NULL, &curHndlr);		/* Get current handler */
if (curHndlr.sa_handler != (void (*)())ftclCmd_INT)
   {
   sigemptyset(&act.sa_mask);
   act.sa_flags = 0;
   act.sa_handler = (void (*)())ftclCmd_INT;
   sigaction(SIGINT, &act, &g_sigint_old);	/* CTRL C interrupt */
   }

/* Set SIGTSTP handler (if we're not already the handler) */
sigaction(SIGTSTP, NULL, &curHndlr);		/* Get current handler */
if (curHndlr.sa_handler != (void (*)())ftclCmd_TSTP)
   {
   sigemptyset(&act.sa_mask);
   act.sa_flags = 0;
   act.sa_handler = (void (*)())ftclCmd_TSTP;
   sigaction(SIGTSTP, &act, &g_sigtstp_old);   /* CTRL Z interrupt */
   }

/* Set SIGTERM handler (if we're not already the handler) */
sigaction(SIGTERM, NULL, &curHndlr);		/* Get current handler */
if (curHndlr.sa_handler != (void (*)())ftclCmd_TERM)
   {
   sigemptyset(&act.sa_mask);
   act.sa_flags = 0;
   act.sa_handler = (void (*)())ftclCmd_TERM;
   sigaction(SIGTERM, &act, &g_sigterm_old);   /* KILL */
   }
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_INTChk
*/
int ftclCmd_INTChk
   (
   )   
/*
** DESCRIPTION:
**	Check if a interrupt signal (i.e., Ctrl C) occurred since the last
**	call to ftclCmd_LineStart.
**
** RETURN VALUES:
**	0 - No interrupt occurred.
**	1 - Yes, an interrupt occurred.
**
** CALLED BY:
**	User's application
**
** GLOBALS REFERENCED
**	g_sigint_flag 
**
**============================================================================
*/
{
/* If the global SIGINT flag is non-zero, then an interrupt occurred */
if (g_sigint_flag)
   {return(1);}
else
   {return(0);}
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclCmd_ExitHandler
*/
static void ftclCmd_ExitHandler
   (
   )   
/*
** DESCRIPTION:
**	This routine is used as the exit handler for the command line
**	editing functions of FTCL.  It is needed to restore the original
**	terminal characteristics.
**
**	If we are the process that declared the exit handler, then 
**	go ahead and restore the terminal characteristics.  We have to
**	be careful since the user may have forked and inherited this 
**	exit handler (we don't want to be restoring terminal characteristics
**	for all child processes).
**
** RETURN VALUES:
**	N/A
**
** DEFINED AS EXIT HANDLER BY:
**	ftclCmd_LineStart
**
** GLOBALS REFERENCED
**	g_exit_pid
**
**============================================================================
*/
{
if (g_exit_pid == getpid())
   {ftclCmd_Reset();}
}
#endif  /* Interrupt routines are not for VxWorks */
#endif  /* or VMS */


#ifndef VXWORKS  /* ftclSelect routines are not for VxWorks */
/*============================================================================  
**============================================================================
**
** ROUTINE: ftclSelect_Init   
*/
void ftclSelect_Init
   (
   SELECT_HANDLE  *a_shl    /* MOD: ftclSelect handle (ptr to select structure)*/
   )   
/*
** DESCRIPTION:
**	Initialize the structure (handle) used by ftclSelect routines.
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	User's application
**
**============================================================================
*/
{
FD_ZERO(&a_shl->fd_list);
FD_ZERO(&a_shl->check_fd_list);
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclSelect_Add    
*/
void ftclSelect_Add
   (
   SELECT_HANDLE  *a_shl,   /* MOD: ftclSelect handle (ptr to select structure)*/
   int		   a_fd	    /* IN : FD to add to FD list */
   )   
/*
** DESCRIPTION
**	Add the FD to the list of FD's to listen to. 
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	User's application
**
**============================================================================
*/
{
FD_SET(a_fd, &a_shl->fd_list);
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclSelect_Remove 
*/
void ftclSelect_Remove
   (
   SELECT_HANDLE  *a_shl,   /* MOD: ftclSelect handle (ptr to select structure)*/
   int		   a_fd	    /* IN : FD to remove from FD list */
   )   
/*
** DESCRIPTION:
**	Remove the FD from the list of FD's to listen to. 
**
** RETURN VALUES:
**	None.
**
** CALLED BY:
**	Unused at this time.
**
**============================================================================
*/
{
FD_CLR(a_fd, &a_shl->fd_list);
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclSelect_Check  
*/
int ftclSelect_Check
   (
   SELECT_HANDLE  *a_shl,   /* MOD: ftclSelect handle (ptr to select structure)*/
   int		   a_fd	    /* IN : FD to check */
   )   
/*
** DESCRIPTION:
**	Check indicated FD to see if there's data is available to be read.
**
**	NOTE: The ftclSelect functions DO NOT read the data, they only check 
**            if data is available.  It it up to the caller to read the data.
**
** RETURN VALUES: 
**	0        - Nothing available.
**	Non-zero - Data available.
**
** CALLED BY:
**	User's application
**
**============================================================================
*/
{
return((int) FD_ISSET(a_fd, &a_shl->check_fd_list));
}



/*============================================================================  
**============================================================================
**
** ROUTINE: ftclSelect_RecvWait
*/
unsigned long ftclSelect_RecvWait
   (
   SELECT_HANDLE  *a_shl    /* MOD: ftclSelect handle (ptr to select structure)*/
   )   
/*
** DESCRIPTION:
**	Wait indefinitely, until there is data available on one or more of
**	the FD's in the FD list.
**
**	NOTE: The ftclSelect functions DO NOT read the data, they only check 
**            if data is available.  It it up to the caller to read the data.
**
** RETURN VALUES:
**	0 - Function failed miserably.
**	1 - Function completed successfully.
**
** CALLED BY:
**	User's application
**
**============================================================================
*/
{
int l_status, l_cnt;
char *l_csl, *l_sl;
int retry=2;

/*
** LOOP SO IF SELECT FUNCTION FAILS, WE CAN TRY A SECOND TIME
*/
while(retry > 0)
   {
   retry--;

   /*
   ** COPY THE "LIST OF FD'S" TO THE "LIST OF FD'S TO BE CHECKED"
   */
   /* Because of the boney ass way the select function works, we incur this
      extra overhead. */
   l_csl = (char *)&a_shl->check_fd_list;
   l_sl  = (char *)&a_shl->fd_list;
   for (l_cnt=0; l_cnt < sizeof(fd_set); l_cnt++) 
      {
      ((char *)l_csl)[l_cnt] = ((char *)l_sl)[l_cnt];
      }

   /*
   ** WAIT FOR SOMETHING TO HAPPEN ON ANY OF THE SPECIFIED FD'S
   */
   l_status = select(FD_SETSIZE, &a_shl->check_fd_list,
                     (fd_set *)0,(fd_set *)0,NULL);

   if (l_status > 0)
      {break;}		/* If successful, then we don't need to retry */
   }


if (l_status <= 0)
   {
   /* Zero the list in case caller ignores status */
   FD_ZERO(&a_shl->check_fd_list); 
   return(0);
   }
else
   {return(1);}
}
#endif /* Not VXWORKS (ftclSelect routines are not for VxWorks) */


/*============================================================================  
**============================================================================
**
** EDITORTEST : MAIN TEST PROGRAM
**
**============================================================================
*/
#ifdef SELECTTEST
#define EDITORTEST
#endif

#ifdef VXWORKS
#undef SELECTTEST /* Select function not needed under VxWorks */
#endif

#ifdef EDITORTEST

#ifdef VXWORKS
editortest()
#else
/* main() */
static editortest()
#endif
{
CMD_EDITHNDL cmdHandle;
CMD l_line;
int lineEntered;
int l_char;
CMD_EDITHIST history;
unsigned long sstatus;
#ifdef SELECTTEST
SELECT_HANDLE selHandle;
#endif
#ifndef NO_TCL_HISTORY
Tcl_Interp *interp;
int  tclstat;
#endif

#ifdef VXWORKS
int option;
printf("\nEnter option (0 - exit, 1 - continue) : ");
scanf("%1d", &option);
if (option == 0) {exit(0);}
#else
ftclCmd_INTDec(1);	/* Setup exit handler to exit on 1st Ctrl 'C' */
#endif

#ifdef SELECTTEST
ftclSelect_Init(&selHandle);
ftclSelect_Add(&selHandle, 0);	/* Add stdin file descriptor (=0) */
#endif

printf("\n");
#ifdef NO_TCL_HISTORY
ftclCmd_HistInit(&history);		/* Setup history structure */
#else
interp = Tcl_CreateInterp();
ftclCmd_HistInit(&history, interp);	/* Setup to use TCL history structure */
#endif

lineEntered = 0;
l_line[0]   = (char)0;

while(1)		/* Loop forever */
   {
   /* Get ready for line entry */
#ifdef SELECTTEST
   ftclCmd_LineStart(&cmdHandle, "> ", &history, 10);
#else
   ftclCmd_LineStart(&cmdHandle, "> ", &history, 0);
#endif

   do			/* Loop until line entry is complete */
      {
#ifdef SELECTTEST
      /* Wait for command line input*/
      if (!ftclSelect_RecvWait(&selHandle))
         {printf("\nHey ! There was a select error.\n"); exit(0);}
      if (ftclSelect_Check(&selHandle, 0))
         {
#endif
         /* Get single character input from stdin */
         ftclCmd_GetChar(&cmdHandle);
         /* Now process the entered character */
         lineEntered = ftclCmd_ProcChar(&cmdHandle, l_line);
#ifndef VXWORKS
         /* Check for anything else */
         while (!lineEntered)
            {
            l_char = ftclCmd_GetChar(&cmdHandle);
            if (l_char <= 0)
               {break;}
            lineEntered = ftclCmd_ProcChar(&cmdHandle, l_line);
            }
#endif
#ifdef SELECTTEST
         }
      else
         {
         printf("\nHey ! Select returned but don't know why.\n");
         exit(0);
         }
#endif
      } while (!lineEntered);

   printf("Line complete\n");
   if (lineEntered == -1) {break;}
   if (!strcmp(l_line, "exit")) {break;}
   if (!strcmp(l_line, "quit")) {break;}
   if (!strcmp(l_line, "logout")) {break;}
#ifndef NO_TCL_HISTORY
   tclstat = Tcl_RecordAndEval(interp, l_line, 0 );
   if (tclstat != TCL_OK)
      {
      printf("TCL Error");
      if (*interp->result != 0)
         {printf(": %s\n", interp->result);}
      else
         {printf("\n");}
      }
   else
      {
      if (*interp->result != 0)
         {printf("%s\n", interp->result);}
      }
#endif
   lineEntered = 0;

   } /* while(1)... */
}
#endif /* EDITORTEST */

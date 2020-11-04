/*
 *
 * ftclHelp.c: C jacket for the ftcl TCL Help procedures.
 * P. Constanta-Fanourakis
 *
 * ftclHelp provides a simple help mechanism for user defined TCL commands.
 *
 * All procedures are implemented in TCL.  
 * These routines provide a C binding to them.
 */

#include <stdlib.h>
#include <errno.h>
#ifndef VXWORKS
#include <stdio.h>
#endif
#include <string.h>
#include "FrameUtil/ftcl.h"

/*
 *	The following character arrays contain the TCL procedures
 *	that are executed at initialization time
 *
 *      Note: the ftclHelpArray element name format $hlpfac\\t$hlpcmd
 *      used in proc ftclHelpDefine must match the element name format
 *      used in ftclHelpIsDefined() to check if help is defined already
 *      for a given facility and command name.
 */
   char *ftclHelpTcl[] = {
"proc ftclHelpDefine {hlpfac hlpcmd hlpstr} {\n\
   global ftclHelpArray\n\
   set ftclHelpArray($hlpfac\\t$hlpcmd) $hlpstr\n\
   }",

"proc ftclHelpIsDefined {hlpfac hlpcmd} {\n\
   global ftclHelpArray\n\
   info exists ftclHelpArray($hlpfac\\t$hlpcmd)\n\
   }",

"proc ftclHelpDisplay { args } {\n\
   global ftclHelpArray\n\
   ftclFullParse {hlpCmd=^ -facil=* -search -full -sort} $args\n\
   set cmdstr [string tolower [ftclGetStr hlpCmd]]\n\
   set allinfo 0\n\
   if {\"$cmdstr\" == \"*\"} {set allinfo 2}\n\
   if {\"$cmdstr\" == \"^\"} { set cmdstr * } else { set allinfo 2}\n\
   if {\"$cmdstr\" == \"*\"} { set cmdstr \"\\[a-z\\]|\\[0-9\\]\" }\n\
   if [ftclPresent facil] {\n\
      set allinfo 1\n\
      set facilstr [ftclGetList facil]\n\
      if {\"$facilstr\" == \"*\"} {set facilstr \"\\[a-z\\]|\\[A-Z\\]|\\[0-9\\]\"}\n\
      }\n\
   if [ftclPresent search]   {\n\
      set allinfo 0\n\
      set searchstr [string tolower [ftclGetStr search]]\n\
      if {\"$searchstr\" == \"*\"} {set searchstr \"\\[a-z\\]|\\[0-9\\]\"}\n\
      }\n\
   if [ftclPresent full] {set allinfo 3}\n\
   set cmdsListed 0\n\
   set cmpfac #\n\
   if {$allinfo == 0} {ftclHelpMsg}\n\
   foreach cmd [lsort [array names ftclHelpArray]] {\n\
     set fstr [format {%s} [lindex [split $cmd \\t] 0]]\n\
     set cstr [format {%s} [lindex [split $cmd \\t] 1]]\n\
     set fullstr [format \"%-25s %s\" $cmd $ftclHelpArray($cmd)]\n\
     set sfstr   [format \"%-20s %s\" $cstr $ftclHelpArray($cmd)]\n\
     set echoflag 1\n\
     if [regexp -nocase $cmdstr $cstr] {\n\
        if [ftclPresent facil] { \n\
           set echoflag 0\n\
           foreach fel $facilstr {\n\
              if {[regexp $fel $fstr]} {set echoflag 1} \n\
              }\n\
           }\n\
        if [ftclPresent search] {if {![regexp -nocase $searchstr $fullstr]} {set echoflag 0} }\n\
        if {$allinfo == 0} {if {[regexp $cmpfac $fstr]} {set echoflag 0} }\n\
        if {$echoflag == 1} {\n\
           if {$allinfo == 3} {lappend hlplist \"$fullstr\" }\n\
           if {$allinfo == 2} {lappend hlplist \"$sfstr\"}\n\
           if {$allinfo == 1} {lappend hlplist \"$cstr\"}\n\
           if {$allinfo == 0} {lappend hlplist \"$fstr\"; set cmpfac $fstr} \n\
           incr cmdsListed\n\
           }\n\
        }\n\
     }\n\
     if {$cmdsListed == 0} then {return \"\"} else {\n\
        if [ftclPresent sort] {set hlplist [lsort $hlplist]}\n\
        return [ftclHelpFormat $cmdsListed $allinfo $hlplist] }\n\
 }",

"proc ftclHelpFormat {cmdsListed allinfo hlplist} {\n\
  set rstr \"\" \n\
  if {$allinfo == 0 || $allinfo == 1} { \n\
    set pl 0\n\
    foreach el $hlplist {\n\
       append rstr [format \"%-25s \" $el]\n\
       incr pl \n\
       if {$pl == 3} {\n\
          append rstr \" \\n\"\n\
          set pl 0\n\
          }\n\
       }\n\
    } else {\n\
       foreach el $hlplist {\n\
          foreach tmp [split $el \\n] {\n\
	    if [string length $rstr]>0 {append rstr \"\\n\"} \n\
            append rstr \"$tmp\"\n\
            }\n\
         }\n\
    }\n\
  return $rstr\n\
}",

"proc ftclHelpMsg {} {\n\
   echo \"\n\
 Typing help with NO arguments, a list of known facilities is displayed.\n\
 Available options: \n\
\\t help * \n\
\\t\\t List ALL commands and full messages associated . \n\
\\t help cmd \n\
\\t\\t Show the message text associated to cmd \n\
\\t help cmd -full \n\
\\t\\t Show the message text associated to cmd and the facility\n\
\\t help -facil \n\
\\t\\t List all available commands. \n\
\\t help -facil=facil1,facil2 \n\
\\t\\t List commands with facility = facil1 and facil2\n\
\\t help -search=pattern \n\
\\t\\t List all commands whose messages contain pattern \n\
\\t help -search=pattern -full\n\
\\t\\t List all commands whose messages contain pattern; \n\
\\t\\t the full text message will be displayed.\n\
\\t help -sort \n\
\\t\\t List all commands sorted by command name.\\n\n\
Existing facilities:\"\n\
}",

 ""};


/* 	Defines help text for the given facility and command.
 *	Returns TRUE (1) if the help for the command was added, 
 *	false otherwise. 
 *	Error messages will be printed to stderr. 
 *	This routine is available under TCL
 */

int ftclHelpDefine (
  Tcl_Interp *interp,   /* IN: Interpreter   */
  const char *facility, 	/* IN: facility name */
  const char *hlpcmd,		/* IN: command name  */
  const char *hlp		/* IN: help text     */
  )
  {
  char *cmd;
                        
  int rvalue = 1;		/* Assume success. */
  
  cmd = (char *)malloc ((strlen(facility) + strlen(hlpcmd) + strlen(hlp) + 25));
  if (cmd == (char *)0)
     {
     printf("ftclHelpDefine malloc failure\n");
     rvalue = 0;
     }
  else
     {
     sprintf(cmd, "ftclHelpDefine %s %s {%s}", facility, hlpcmd, hlp);
     if (Tcl_Eval(interp, cmd) == TCL_ERROR)
        {
        if (strlen(interp->result) > 0)
          {
          printf("ftclHelpDefine error: %s\n", interp->result);
          rvalue = 0;
          }
        }
     free(cmd);
     }
  return rvalue;
 }


/* 	checks if help is defined for the given facility and command.
 *	Returns TRUE (1) if help is already defined, 
 *	false otherwise. 
 *	Error messages will be printed to stderr. 
 *	This routine is available under TCL
 */

int ftclHelpIsDefined (
  Tcl_Interp *interp,   /* IN: Interpreter   */
  char *facility, 	/* IN: facility name */
  char *hlpcmd		/* IN: command name  */
  )
  {
  char *cmd;
                        
  int rvalue = 1;		/* Assume success. */
  
  cmd = (char *)malloc ((strlen(facility) + strlen(hlpcmd) + 25));
  if (cmd == (char *)0)
     {
     printf("ftclHelpIsDefined malloc failure\n");
     rvalue = 0;
     }
  else
     {
     sprintf(cmd, "ftclHelpIsDefined %s %s", facility, hlpcmd);
     if (Tcl_Eval(interp, cmd) == TCL_ERROR)
        {
        if (strlen(interp->result) > 0)
          {
          printf("ftclHelpIsDefined Tcl_Eval error: %s\n", interp->result);
          rvalue = 0;
          }
        }
     else   /* Tcl_Eval succeeded */ 
        {
        if (0 == strcmp(interp->result,"1"))
            rvalue = 1;
        else if (0 == strcmp(interp->result,"0"))
            rvalue = 0;
        else
            {
            rvalue = 0;
            if (strlen(interp->result) > 0)
                printf("ftclHelpIsDefined unexpected result: %s\n", interp->result);
            else
                printf("ftclHelpIsDefined unexpected empty result\n");
	    }  /* end if result not 1 or 0 */
        }  /* end if Tcl_Eval succeeded */

     free(cmd);
     }

  return rvalue;
 }


/*
 *	Displays help information as follows:
 *	hlpcmd = ""  --> lists all commands
 *	hlpcmd = cmd --> displays help text for specified command
 *	hlpcmd = *   --> lists all commands and their help text
 *	hlpcmd = -search=str --> lists commands that their command or help 
 *	                         contain string.
 *	hlpcmd = -search=str -full --> displays commands and help text that 
 *	                               their command or help contain string.
 *	       -facility =  --> qualifier that applies to all commands:
 *	                        list/display commands/help for specified facility
 *	This routine is available under TCL
 */

int ftclHelpDisplay (
  Tcl_Interp *interp,   /* IN: Interpreter   */
  char *hlpcmd		/* IN: help display command */
  )
  {
  char *cmd;
                        
  int rvalue = 1;		/* Assume success. */

  cmd = (char *)malloc ((strlen(hlpcmd) + 25));
  if (cmd == (char *)0)
     {
     printf("ftclHelpDisplay malloc failure\n");
     rvalue = 0;
     }
  else
     {
     sprintf(cmd, "ftclHelpDisplay %s", hlpcmd);

     if (Tcl_Eval(interp, cmd) == TCL_ERROR)
        {
        if (strlen(interp->result) > 0)
          {
          printf("ftclHelpDisplay error: %s\n", interp->result);
          rvalue = 0;
          }
        }
     free(cmd);
     }
  return rvalue;
 }



/*
 *	This routine initializes the Help commands. 
 *	It needs to be called by C application programs
 */
void ftclHelpInit(Tcl_Interp *interp)
  {
  int n=0;

  ftclParseInitCmds (interp);

  while (*ftclHelpTcl[n])
     {
     if (Tcl_Eval(interp, ftclHelpTcl[n]) == TCL_ERROR)
       {
       if (strlen(interp->result) > 0)
          {
          printf("ftclHelpInit error: %s\n", interp->result);
          }
       else
          {
          printf("ftclHelpInit error" );
          }
       }
     n++;
     }
  }

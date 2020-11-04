/***************************************************************************
 *
 * ftclParse.c: C jacket for the ftcl TCL command line parsing.
 * B. MacKinnon
 *
 * ftclParse provides a parsing capability for TCL procs and C functions.
 * See the description in ftclParse.tcl for a full description.
 *
 * The work is actually done in TCL.  This file merely provides a 
 * C binding to it.
 *
 * Modifications: 12-may-95 mvw  Changed to call to strtol to strtoul.
 */

#include <stdlib.h>
#include <errno.h>
#ifndef VXWORKS
#include <stdio.h>
#endif
#include <string.h>
#include "tcl.h"

/* ftclParse specific global variables. */

Tcl_Interp *ftclParseInterp;
/* Tcl_CmdBuf ftclParseBuffer; */


/*
 *	The following character arrays contain the actual TCL procedures
 */
  char *ftclParseTcl[] = {
"proc ftclParse {options params delim cmd} { \n\
  upvar 1 $params lclParams \n\
  upvar 1 $options lclOptions \n\
  if [info exists lclOptions] {unset lclOptions} \n\
  if [info exists lclParams] {unset lclParams} \n\
  set lclOptions(size) 0 \n\
  set lclParams(size) 0 \n\
  set lclParams(defsize) 0 \n\
  set pndx 0 \n\
  foreach arg $cmd { \n\
    if {[string compare [string range $arg 0 0] $delim]==0} { \n\
      set argName [lindex [split [string range $arg 1 end] =] 0] \n\
      set argVal [lindex [split $arg =] 1] \n\
      set lclOptions($argName) $argVal \n\
      set lclParams($argName) $argVal \n\
      incr lclOptions(size) \n\
      } else { \n\
      set argl [split $arg \"'\"] \n\
      if [llength $argl]>1 {set arg [lindex $argl 1]} \n\
      set pname [lindex [split $arg =] 0] \n\
      set pval [lindex [split $arg =] 1] \n\
      set lclParams(#$pndx) $pval \n\
      if {\"$pval\"==\"\"} {set lclParams(#$pndx) $arg} else {incr lclParams(defsize)} \n\
      set lclParams($pname) #$pndx \n\
      incr lclParams(size) \n\
      incr pndx \n\
      } \n\
    } \n\
  }",

"proc ftclVerify {formalOptsP formalParamsP optsP paramsP} { \n\
  upvar $formalOptsP fopts $formalParamsP fparams  \n\
  upvar $optsP opts $paramsP params \n\
  if $params(size)>$fparams(size) {error \"Too many parameters entered\" } \n\
  if ($params(size)<$fparams(size))&&($params(size)+$fparams(defsize)<$fparams(size)) {error \"Too few parameters entered\" } \n\
  for {set andx $params(size)} {$andx<$fparams(size)} {incr andx} { \n\
    if {\"$fparams(#$andx)\"!=\"\"} {set params(#$andx) $fparams(#$andx)} else { \n\
         error [format \"Missing parameter %s\" $andx]} \n\
    } \n\
  foreach opt [array names opts] { \n\
    if [info exists fopts($opt)] { \n\
      if [expr (\"$opts($opt)\"==\"\")] \"set opts($opt) $fopts($opt)\" \n\
      } else { error [format \"Unknown option '%s'\" $opt] } \n\
    } \n\
  return \"\" \n\
  }",

"proc ftclFullParse {fcmd cmd} { \n\
  upvar ftclOptions opts ftclParameters params \n\
  upvar ftclFormalOptions fopts ftclFormalParameters fparams \n\
  if {(\"$cmd\"==\"?\")||(\"$cmd\"==\"help\")}  {error [format \"options available: %s \" $fcmd]} \n\
  ftclParse fopts fparams - $fcmd \n\
  ftclParse opts params - $cmd \n\
  ftclVerify fopts fparams opts params \n\
  }",

"proc ftclPresent opt { \n\
  upvar ftclOptions opts \n\
  info exists opts($opt) \n\
  }",

"proc ftclGetStr {key} { \n\
  upvar ftclParameters params \n\
  upvar ftclOptions opts \n\
  upvar ftclFormalParameters fparams \n\
  upvar ftclFormalOptions fopts \n\
  if [info exists opts($key)] { \n\
    return $opts($key)  \n\
    }  else { \n\
    if [info exists fopts($key)] { \n\
      return $fopts($key) \n\
      }  else { \n\
      if [info exists fparams($key)] {  \n\
        if [info exists params($fparams($key))] { \n\
          return $params($fparams($key))  \n\
          }  else { \n\
          if [info exists params($key)] { \n\
  	    return $params($key) \n\
 	    }  \n\
	  } \n\
	} else {error [format \"No such parameter or option: %s\" $key]} \n\
      } \n\
    } \n\
  }",

"proc ftclGetList key { \n\
  upvar ftclParameters ftclParameters \n\
  upvar ftclOptions ftclOptions \n\
  upvar ftclFormalParameters ftclFormalParameters \n\
  return \"[split [ftclGetStr $key] ,]\" \n\
  }",

"proc ftclDump {} { \n\
  upvar ftclParameters params \n\
  upvar ftclOptions opts \n\
  upvar ftclFormalParameters fparams \n\
  upvar ftclFormalOptions fopts \n\
  foreach andx [array names params] {echonn [format \"%s \" $params($andx)]} \n\
  foreach andx [array names opts] {echonn [format \"-%s=%s \" $andx $opts($andx)]} \n\
  echonn \"\\n\" \n\
  }",

"proc ftclParseSave {ctx} { \n\
  upvar ftclParameters params 			ftclOptions opts \n\
  upvar ftclFormalParameters fparams 		ftclFormalOptions fopts \n\
  upvar ftclParameters$ctx paramsSv 		ftclOptions$ctx optsSv \n\
  upvar ftclFormalParameters$ctx fparamsSv 	ftclFormalOptions$ctx foptsSv \n\
  if {! [info exists fparams] } {set fparams(x) {}; set params(x) {}; set fopts(x) {}; set opts(x) {}} \n\
  ftclCopyArray paramsSv params \n\
  ftclCopyArray optsSv opts \n\
  ftclCopyArray fparamsSv fparams \n\
  ftclCopyArray foptsSv fopts  \n\
}",

"proc ftclParseRestore {ctx} { \n\
  upvar ftclParameters params 			ftclOptions opts \n\
  upvar ftclFormalParameters fparams 		ftclFormalOptions fopts \n\
  upvar ftclParameters$ctx paramsSv 		ftclOptions$ctx optsSv \n\
  upvar ftclFormalParameters$ctx fparamsSv 	ftclFormalOptions$ctx foptsSv \n\
  if {! [info exists fparamsSv]} {error \"No such ftclParse context: '$ctx'\"} \n\
  ftclCopyArray params paramsSv \n\
  ftclCopyArray opts optsSv \n\
  ftclCopyArray fparams fparamsSv \n\
  ftclCopyArray fopts foptsSv \n\
  }",

"proc ftclCopyArray {outArrayName inArrayName} { \n\
  upvar $inArrayName in $outArrayName out \n\
  foreach ndx [array names in] { \n\
    set out($ndx) $in($ndx) \n\
  } \n\
 }",

""
 };



/* Parses a command line against a prototype.  Returns TRUE (1)
   if the user command is syntatically correct, false otherwise. 
   Syntax error messages will be printed to normal stdout. */

int ftclFullParse
  (
  const char *formalCmd, 	/* IN: Formal prototype command to parse against. */
  const char *userCmd		/* IN: User entered command to parse. */
  )
  {
  int result;
  char *cmd;
                        
  int rvalue = 1;		/* Assume success. */

  if ((cmd = malloc(strlen(formalCmd) + strlen(userCmd) + 20)) == NULL)
    {
    printf("ftclFullParse error: malloc failure\n");
    rvalue = 0;
    goto fncend;
    }

  sprintf(cmd, "ftclFullParse \"%s\" \"%s\"", formalCmd, userCmd);
  result = Tcl_Eval(ftclParseInterp, cmd);
  if (strlen(ftclParseInterp->result) > 0)
    {
    printf("ftclFullParse error: %s\n", ftclParseInterp->result);

    /* Check if TCL is complaining about quotes.  If so, give the user a clue about
       how to quote. */

    if (strspn(ftclParseInterp->result, "quote") != 0) 
      {
      printf("  Options with embedded blanks must be entirely quoted (e.g., \"-opt1=dog cat\")\n");
      }
    rvalue = 0;
    }
  free(cmd);
fncend:
  return rvalue;
  }

/* argv/argc interface to ftclFullParse.  */

int ftclFullParseArg
  (
  const char *formalCmd,    /* IN: Formal prototype command to parse against. */
  int  argc,          /* IN: Number of parameters in the argc array, argv[0] is the command. */
  const char *argv[]        /* IN: Parameters and options to parse. */
  )

  {
#define LOCALCMDSIZE 255
  char *cmd;			/* Concatanated command. */
  char cmdLocal[LOCALCMDSIZE];
  int  cndx, cmdsize, rvalue;


  /* Go through each argv parameter to get total size, 
     then malloc if necessary */

  cmdsize = 1;  /* Space for NULL terminator */
  for (cndx = 1; cndx < argc; cndx ++)
    {
    cmdsize += strlen(argv[cndx]) + 4;
    }

  if (cmdsize >= LOCALCMDSIZE)
     {
     if ((cmd = malloc(cmdsize)) == NULL)
       {
       printf("ftclFullParseArg error: malloc failure\n");
       rvalue = 0;
       goto fncend;
       }
     }
  else
     {
     cmd = cmdLocal;
     cmdsize = LOCALCMDSIZE;
     }
  cmd[0] = (char)0;

  /* We must go through each argv parameter and quote it if it's not a option or
     not already quoted. But make sure we do not overflow the string. */

  for (cndx = 1; (cndx < argc) && (strlen(cmd) + 3 < cmdsize); cndx ++)
    {
    if (strspn(argv[cndx], "\"{") == 0)   /* Check for option (-) or quote ({"). */
      {  /* Must be quoted. */
      strcat(cmd, "{");
      strcat(cmd, argv[cndx]);
      strcat(cmd, "} ");
      }
    else 
      { /* Do not quote it. */
      strcat(cmd, argv[cndx]);
      strcat(cmd, " ");
      }  /* if, else */
    }  /* for */
  rvalue = ftclFullParse(formalCmd, cmd);
  if (cmd != cmdLocal)
    {free(cmd);}
fncend:
  return(rvalue);
  }  /* ftclFullParseArg */
 

/* Following routines assume that ftclFullParse has been called prior
   to calling them. */

/* Returns true (1) if the specified option or parameter is present,
   false otherwise. */

int ftclPresent(char *keyword)
  {
  int result;
  char cmd[300];

  sprintf(cmd, "ftclPresent %s", keyword);
  result = Tcl_Eval(ftclParseInterp, cmd);
  return strcmp(ftclParseInterp->result, "1") == 0;
  }

/* Returns the value of a parameter or option as a string that has been determined to
   be present. */

char *ftclGetStr(char *keyword)
  {
  int result;
  char cmd[300];

  sprintf(cmd, "ftclGetStr %s", keyword);
  result = Tcl_Eval(ftclParseInterp, cmd);
  return ftclParseInterp->result;
  }

/* Returns the value of a parameter or option as an integer that has been determined to
    be present. */

long int ftclGetInt(char *keyword)
  {
  return strtoul(ftclGetStr(keyword), (void *) NULL, 0);
  }

/* Returns the value of a parameter or option as an unsigned integer that has been determined to
    be present. */

unsigned long int ftclGetUnsigned(char *keyword)
  {
  return strtoul(ftclGetStr(keyword), (void *) NULL, 0);
  }

/* Returns the value of a parameter or option as double precision real that has been determined to
    be present. */

double ftclGetDouble(char *keyword)
  {
  return strtod(ftclGetStr(keyword),  (void *) NULL);
  }

/* Returns the value of a parameter or option as a space delimited list that has been determined to
    be present. */

char *ftclGetList(char *keyword)
  {
  int result;
  char cmd[300];

  sprintf(cmd, "ftclGetList %s", keyword);
  result = Tcl_Eval(ftclParseInterp, cmd);
  return ftclParseInterp->result;
  }

/* Saves the current ftclFullParse context so that nested C routines can call ftclParse
   without affecting the caller.  The context is a unique flag used to save
   the context. */

int ftclParseSave(char *ctx  /* IN: context variable to save with. */)
{
return Tcl_VarEval(ftclParseInterp, "ftclParseSave ", ctx, (char *) NULL);
}

/* Restores the specified ftclFullParse context. This procedure preserves
   the entry tcl result area. */

int ftclParseRestore(char *ctx)
{
int rstatus;
char *resultSave = (char * ) NULL;
if (strlen(ftclParseInterp->result) > 0) 
  {
  resultSave = malloc(strlen(ftclParseInterp->result) + 1);
  strcpy(resultSave, ftclParseInterp->result);
  }
rstatus = Tcl_VarEval(ftclParseInterp, "ftclParseRestore ", ctx, (char *) NULL);
if (resultSave) 
  {
  Tcl_SetResult(ftclParseInterp, resultSave, TCL_VOLATILE);
  free(resultSave);
  }
return rstatus;
}
 




/* Test and example program. */

int ftclParseTest(ClientData clientData, Tcl_Interp *interp, int argc, const char *argv[])
  {
  char *formalCmd = "p1 p2=cat -opt1 -opt2=dog -unsigned -double -int=3 -list";
  int ndx;

  for (ndx = 0; ndx < argc; ndx ++)
    {
    printf("argv[%d]=(%s)\n", ndx, argv[ndx]);
    }

    if (ftclFullParseArg(formalCmd, argc,argv))
      {
      printf("p1=(%s)\n", ftclGetStr("p1"));
      printf("p2=(%s)\n", ftclGetStr("p2"));
      if (ftclPresent("opt1")) printf("opt1=%s\n",ftclGetStr("opt1"));
      if (ftclPresent("opt2")) printf("opt2=%s\n",ftclGetStr("opt2"));
      if (ftclPresent("double")) printf("double=%g\n",ftclGetDouble("double"));
      if (ftclPresent("int")) printf("int=%d\n",ftclGetInt("int"));
      if (ftclPresent("unsigned")) printf("unsigned=%x\n",ftclGetUnsigned("unsigned"));
      if (ftclPresent("list")) printf("list=%s\n",ftclGetList("list"));
      }
    else printf("ftclParseTest: Parse failed\n");

  return TCL_OK;

} /* ftclParseTest */

/* Must be called to initialize the ftclParse commands. */

void ftclParseInitCmds(Tcl_Interp *interp)
  {
/*
 *	The following are the TCL procedures for the ftclParse
 *	It is included so that this code will be independent of 
 *	external files.
 */

  int n = 0;

  while (*ftclParseTcl[n])
     {
     if (Tcl_Eval(interp, ftclParseTcl[n]) == TCL_ERROR)
       {
       if (strlen(interp->result) > 0)
          {
          printf("ftclParseInitCmds error: %s\n", interp->result);
          }
        else
          {
          printf("ftclParseInitCmds error" );
          }
       }
     n++;
     }
  Tcl_CreateCommand(interp,"ftclParseTest",ftclParseTest, (ClientData) 0,
		    (Tcl_CmdDeleteProc *) NULL);

  }


/* Must be called first and only once per interpreter to initialize the parser. */

void ftclParseInit(Tcl_Interp *interp)
  {
/*
 *	The following are the TCL procedures for the ftclParse
 *	It is included so that this code will be independent of 
 *	external files.
 */

  int n = 0;
/*
  *	Check if we need to create an interpreter
 */
  if (interp == 0)
     {
     ftclParseInterp = Tcl_CreateInterp();
/*     ftclParseBuffer = Tcl_CreateCmdBuf(); */
     }
  else
     {
     ftclParseInterp = interp;
     }

  ftclParseInitCmds(ftclParseInterp);

  }



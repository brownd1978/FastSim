#include "FrameUtil/ftcl.h"
/* #include "tclHash.h" */
#include "tcl.h"
#include <string.h>
#include <stdlib.h>

Tcl_HashTable ftclAliasTable;


/* This routine will substitute one string for another inside of a string. */

int ftclAliasStrSub
  (
  const char *str, 		/* IN: String to search. */
  const char *searchStr, 	/* IN: String to search for. */
  const char *replaceStr, 	/* IN: String to replace with. */
  char *resultStr	/* OUT: Resulting string, it must be long enough. */
  )

  {
  int rvalue = 0;
  char *sptr;			/* Where we are in the main string. */
  char *newstr = (char * ) malloc(strlen(str) * 4); /* Some reasonable max. */

  newstr[0] = 0;

   do
      {
      sptr = (void *) strstr(str, searchStr);
      if (sptr != NULL)
        {
	rvalue ++;
        strncat(newstr, str, (long) sptr - (long) str);	/* Get part leading up to sub. */
        strcat(newstr, replaceStr);	/* Sub in string. */
        str = &sptr[strlen(searchStr)];  /* Advance. */
        }
      else strcat(newstr, str); /* Append remainging string. */
      }
  while (sptr);
  strcpy(resultStr, newstr);
  free(newstr);
  return rvalue;
  }


/*******************************************************************************/
/* @+Public+@
 * ROUTINE: ftclAliasExecute: Added by MACKINNON on 12-AUG-1992 */

int ftclAliasExecute
  (
  ClientData cdata,	    /* Alias index. */
  Tcl_Interp *interp,
  int argc,
  const char *argv[]
  )
/*	
 * DESCRIPTION
 *	Executes a given alias
 *                  
 * RETURN VALUES:	TCL_ERROR or TCL_OK
 *
 * SIDE EFFECTS:	None
 *
 * SYSTEM CONCERNS:	None.
 * @-Public-@
 ******************************************************************************/


  { /* ftclAliasExecute */
  char *cmd;
  char *alias =  (char *) cdata;
  int  ndx;
  int  pos;
  int rstatus;
  char param[15];
  int  cmdSize;

  cmdSize = strlen(alias) * 2;  /* Assumed max size of a command. */
  if (cmdSize < 255) cmdSize = 255;

  /* Construct the command by appending any specified parameters. */

  cmd = (char *) malloc(cmdSize);

  strcpy(cmd, alias);

  for (ndx = 1; ndx < argc; ndx ++)
    {
    sprintf(param, "^%d", ndx);		/* Make ^n */
    if (ftclAliasStrSub(cmd, param, argv[ndx], cmd) == 0)
      {
      strncat(cmd, " {", cmdSize - strlen(cmd));
      strncat(cmd, argv[ndx], cmdSize - strlen(cmd));
      strncat(cmd, "}", cmdSize - strlen(cmd));
      }
   }
  rstatus = Tcl_Eval(interp, cmd);
  free(cmd);
  return rstatus;
  } /* ftclAliasExecute */


/******************************************************************************/

/*******************************************************************************/
/* @+Public+@
 * ROUTINE: ftclAlias: Added by MACKINNON on 9-JUN-1992 */

int ftclAlias
  (                 
  ClientData cdata,	    /* Command index. */
  Tcl_Interp *interp,
  int argc,
  const char *argv[]             /* [1] = alias, [2]=equivalence */
  )

/*	
 * DESCRIPTION
 *	Aliass a CLI Verb
 *                  
 * RETURN VALUES:	dsc_success, dsc_cliBadParam, dsc_cliTooMany
 *
 * SIDE EFFECTS:	None
 *
 * SYSTEM CONCERNS:	None.
 * @-Public-@
 ******************************************************************************/


  { /* ftclAlias */
            
  int             rstatus = TCL_OK;		/* Assume ok. */
  Tcl_HashEntry   *eptr;
  int             new;
  char            *cmdPtr;
  Tcl_HashSearch  hashSearch;

  if (argc > 3)
    {
    printf("Usage: %s alias \"equivalence\"\n", argv[0]);
    rstatus = TCL_ERROR;
    goto end;
    }


  if (argc == 1)
    {
    /* User wants to view all of the aliases. */

    eptr = Tcl_FirstHashEntry(&ftclAliasTable, &hashSearch);
    while (eptr)
      {
      Tcl_AppendResult(interp, (char *) Tcl_GetHashKey(&ftclAliasTable, eptr), "\t", (char *) Tcl_GetHashValue(eptr), "\n", (char *) NULL);
      eptr = Tcl_NextHashEntry(&hashSearch);
      }
    goto end;
    }

    /* See if the alias exists. */

    eptr = (void *) Tcl_CreateHashEntry(&ftclAliasTable, argv[1], &new);
    if (! new)
      {  /* It exists. */

      /* If a new value was not specified, return the existing definition. */

      if (! argv[2]) 
	{
	Tcl_AppendResult(interp, "Alias for: ", (char *) Tcl_GetHashValue(eptr), (char *) NULL);
	goto end;
	}

      /* A new value was specified, supercede the existing one. */

      free(Tcl_GetHashValue(eptr));    /* Free the existing entry. */
      }
    
    /* Malloc space and define the new entry. */

    if ( ! argv[2] )
      {

      /* Error: user did not specify equivalence or alias does not
	 exist for query. */

      Tcl_AppendResult(interp, "No such alias: ", argv[1], (char *) NULL);
      rstatus = TCL_ERROR;
      goto end;
      }

    /* The user wants to define a new command. */

    cmdPtr = (char *) malloc(strlen(argv[2]) + 1);
    if (! cmdPtr)
      {
      printf("ftclAlias: cannot malloc space for command.\n");
      rstatus = TCL_ERROR;
      goto end;
      }

    strcpy(cmdPtr, argv[2]);  /* Make our own copy to be safe. */
    Tcl_SetHashValue(eptr, cmdPtr);
    Tcl_CreateCommand(interp, argv[1], ftclAliasExecute, (ClientData) cmdPtr, (Tcl_CmdDeleteProc *) NULL);
    ftclHelpDefine(interp, "alias", argv[1], cmdPtr);
end:
    return (rstatus); 
  } /* ftclAlias */


/*******************************************************************************/
/* @+Public+@
 * ROUTINE: ftclUnalias: Added by MACKINNON on 9-JUN-1993 */

int ftclUnalias
  (                 
  ClientData cdata,	    /* Command index. */
  Tcl_Interp *interp,
  int argc,
  const char *argv[]             /* [1] = alias. */
  )

/*	
 * DESCRIPTION
 *	Unaliases a CLI Verb
 *                  
 * RETURN VALUES:	TCL_OK
 *
 * SIDE EFFECTS:	None
 *
 * SYSTEM CONCERNS:	None.
 * @-Public-@
 ******************************************************************************/


  { /* ftclUnalias */
            
  int             rstatus = TCL_OK;		/* Assume ok. */
  Tcl_HashEntry   *eptr;
  int             new;
  char            *cmdPtr;

  if ( (argc != 2) )
    {
    printf("Usage: %s alias\n", argv[0]);
    rstatus = TCL_ERROR;
    goto end;
    }


    /* See if the alias exists. */

    eptr = (void *) Tcl_CreateHashEntry(&ftclAliasTable, argv[1], &new);
    if (! new)
      {  /* It exists. */
      Tcl_DeleteCommand(interp, (char *)  Tcl_GetHashKey(&ftclAliasTable, eptr));
      free(Tcl_GetHashValue(eptr));    /* Free the existing entry. */
      Tcl_DeleteHashEntry(eptr);       /* Delete hash entry. */
      }
    
end:
    return (rstatus); 
  } /* ftclUnalias */


/******************************************************************************/


/*****************************************************************************/
/* @+Public+@                                                                 
 * ROUTINE: ftclAliasInit:                                                 */

void        ftclAliasInit
   (
   Tcl_Interp *interp  /* IN: Interpreter to define alias command in. */
   )

/*
 * DESCRIPTION
 *       Initializes the ftcl command alias support
 *
 * RETURN VALUES:       None.
 *
 * SIDE EFFECTS:        None
 *
 * SYSTEM CONCERNS:     None.
 * @-Public-@
 ******************************************************************************/  

{  /* ftclAliasInit */

Tcl_InitHashTable(&ftclAliasTable, TCL_STRING_KEYS);
Tcl_CreateCommand(interp, "alias", ftclAlias, (ClientData) 0, (Tcl_CmdDeleteProc *) NULL);
Tcl_CreateCommand(interp, "unalias", ftclUnalias, (ClientData) 0, (Tcl_CmdDeleteProc *) NULL);

}  /* ftclAliasInit */


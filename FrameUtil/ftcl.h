/* ftcl.h */

#include "tcl.h"

#ifndef FTCL_H
#define FTCL_H

#ifdef __cplusplus
extern "C"
{
#endif  /* ifdef cplusplus */

/* defines prototypes in fermitcl area, and includes tcl.h */

int Tcl_GetFloat(Tcl_Interp *interp, char *string, float *value);

char *envscan(char *path);

void ftclVxSystemCmdDeclare(Tcl_Interp *interp, char *cmd);

void envfree(char *result);

int ftclEnvscan(ClientData clientdata, Tcl_Interp *interp, int argc, char **argv);

int ftclFullParse(char *formalCmd, char *userCmd);

int ftclFullParseArg(char *formalCmd, int argc, char *argv[]);

int ftclPresent(char *keyword);

char *ftclGetStr(char *keyword);

long int ftclGetInt(char *keyword);

unsigned long int ftclGetUnsigned(char *keyword);

double ftclGetDouble(char *keyword);

char *ftclGetList(char *keyword);

void ftclParseInit(Tcl_Interp *interp);

void ftclParseInitCmds(Tcl_Interp *interp);

int ftclCreateHelp (Tcl_Interp *interp);

int ftclHelpDefine (Tcl_Interp *interp, const char *facility, const char *hlpcmd, const char *hlp);

int ftclHelpIsDefined (Tcl_Interp *interp, char *facility, char *hlpcmd);

int ftclHelpDisplay (Tcl_Interp *interp, char *hlpcmd);

void ftclHelpInit(Tcl_Interp *interp);

int ftclParseSave(char *context);

int ftclParseRestore(char *context);

int ftclParseArg  ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclPassed    ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclGetArgInfo( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclGetHelp   ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclGetUsage  ( ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);

float *ftclList2Floats (Tcl_Interp *interp, char *list, int *nfloat, int maxfloat);

int *ftclList2Ints (Tcl_Interp *interp, char *list, int *nint, int maxint);

float *ftclLol2FtnFloats (Tcl_Interp *interp, char *list, int *ncount1, int *ncount2);

int *ftclLol2FtnInts (Tcl_Interp *interp, char *list, int *ncount1, int *ncount2);

int ftclFloats2List (Tcl_Interp *interp, float *floats, int nfloats, int asint);

int ftclCreateCommands(Tcl_Interp *interp);

int ftclMsgDefine(Tcl_Interp *interp);

void ftcl_CommandLoop (Tcl_Interp *interp, FILE *inFile, FILE *outFile, int(*evalProc)(),unsigned options);

int ftcl_Interactive  (Tcl_Interp *interp, int argc , char **argv);

int ftclVxLoop (Tcl_Interp *interp);

int ftcl_EchoCmd (ClientData clientData, Tcl_Interp *interp , int argc, char **argv);

int ftcl_EchonnCmd(ClientData clientData, Tcl_Interp *interp , int argc, char **argv);

/* Parameter handling							*/

int ftclInitParam(ClientData clientdata, Tcl_Interp *interp, int argc, char **argv);
int ftclGetParamF(ClientData clientdata, Tcl_Interp *interp, int argc, char **argv);
int ftclGetParamI(ClientData clientdata, Tcl_Interp *interp, int argc, char **argv);
int ftclGetParamS(ClientData clientdata, Tcl_Interp *interp, int argc, char **argv);

int ftclIniPar (char *default_files, char *home_dir);
int ftclGetParF (char *name, float *value);
int ftclGetParI (char *name, int *value);
int ftclGetParS (char *name, char *value, int len);

/* Fortran interfaces							*/

int ftclinipar_ (char *default_files, char *home_dir);
int ftclgetparf_ (char *name, float *value);
int ftclgetpari_ (char *name, int *value);
int ftclgetpars_ (char *name, char *value, int *len);

#ifdef __cplusplus
}
#endif  /* ifdef cplusplus */

#include "ftclParseArgv.h"

#ifdef VXWORKS
#include "ftclKeyList.h"		/* To have keyed list functions declarations */
#endif

#endif

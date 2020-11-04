/*
 * ftcl_ParseArgv.h - Generic ftcl command-line parser. 
 *                    Heavily inspired by the Tk command line parser.
 *
 * Vijay K. Gurbani
 * Fermi National Accelerator Laboratory
 * Batavia, Illinois
 *
 * $Author: turcotte $   $Date: 1998/02/10 16:08:11 $   $Revision: 1.1 $
 */

#ifndef _FTCL_PARSEARGV_H
#define _FTCL_PARSEARGV_H

/*
 * Structure to specify how to handle command line options
 */
typedef struct {
    char *key;          /* The key string that flags the option in the
                         * argv array. */
    int type;           /* Indicates option type;  see below. */
    void *src;          /* Value to be used in setting dst;  usage
                         * depends on type. */
    void *dst;          /* Address of value to be modified;  usage
                         * depends on type. */
    char *help;         /* Documentation message describing this option. */
} ftclArgvInfo;

/*
 * Public functions
 */
#ifdef __cplusplus
extern "C"  {
#endif
int ftcl_ParseArgv(Tcl_Interp *, int *, char **, ftclArgvInfo *, int);

int ftcl_ArgIsPresent(int, char **, char *, ftclArgvInfo *);

char *ftcl_GetUsage(Tcl_Interp *interp, ftclArgvInfo *argTable, int flags, 
                    char *cmd_name, int indent, 
                    char *usage_prefix, char *usage_suffix);

char *ftcl_GetArgInfo(Tcl_Interp *interp, ftclArgvInfo *argTable, int flags, 
                      char *cmd_name, int indent);

char *ftcl_GetHelp(Tcl_Interp *interp, ftclArgvInfo *argTable, int flags, 
                   char *cmd_name, int indent, 
                   char *usage_prefix, char *usage_suffix, char *help_suffix);

int ftclParseArg(
    ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclPassed(
    ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclGetUsage(
    ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclGetArgInfo(
    ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
int ftclGetHelp(
    ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);
#ifdef __cplusplus
}
#endif

/* defines for return value of ftcl_ParseArgv */

#define FTCL_ARGV_SUCCESS     10        /* Don't confuse with TCL_OK */
#define FTCL_ARGV_GIVEHELP    -1
#define FTCL_ARGV_BADSYNTAX   -2

/*
 * Legal values for the type field of a ftclArgvInfo: see the user
 * documentation for details.
 */
 
#define FTCL_ARGV_CONSTANT                0x64
#define FTCL_ARGV_INT                     0x65
#define FTCL_ARGV_STRING                  0x66
#define FTCL_ARGV_DOUBLE                  0x68
#define FTCL_ARGV_FUNC                    0x69
#define FTCL_ARGV_HELP                    0x6A
#define FTCL_ARGV_END                     0x6B

/*
 * Flag bits for passing to ftcl_ParseArgv()
 */
 
#define FTCL_ARGV_NO_DEFAULTS             0x1
#define FTCL_ARGV_NO_LEFTOVERS            0x2
#define FTCL_ARGV_NO_ABBREV               0x4
#define FTCL_ARGV_DONT_SKIP_FIRST_ARG     0x8
#define FTCL_ARGV_IGNORE_FLAGS	          0x10
#define FTCL_PARSEARG                     0x20

/* help string and usage string max sizes */

#define FTCL_ARGV_USAGE_BUFSIZE    1025
#define FTCL_ARGV_ARGINFO_BUFSIZE  8193

/* storage class define to allow destination variables to be
 * used in argTable initialization without being static in VxWorks
 * (This works ok as non-static in vxworks when using gcc)
 */
#ifdef VXWORKS
#define FTCL_ARGV_STORAGE
#else
#define FTCL_ARGV_STORAGE static
#endif

#endif



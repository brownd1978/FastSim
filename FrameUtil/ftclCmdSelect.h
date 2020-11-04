#ifndef FTCLSELECT_HEADER
#define FTCLSELECT_HEADER
/*****************************************************************************
******************************************************************************
**
** FILE:
**	ftclCmdSelect.h
**
** ABSTRACT:    This file contains all necessary defintions, prototypes, and
**		macros for the routines used to support the command line 
**              select capabilities under UNIX.
**
** REQUIRED INCLUDE FILES:
**		Was: <sys/types.h> - fd_set definition
**		Now: <sys/time.h>  - fd_set definition under Unix95+ (20011127)
**
** ENVIRONMENT: ANSI C. 
**		UNIX only. No support under VxWorks
**
** AUTHOR:      Creation date: Aug. 17, 1992
**              Gary Sergey
**
******************************************************************************
******************************************************************************
*/
#ifndef VXWORKS  /* ftclSelect routines are not for vxWorks */
#include <sys/time.h>

/* 2001.11.17 This should no longer be needed on standardized platforms:
#ifdef AIX
#include <sys/select.h> 
#endif
*/
 
#ifdef __cplusplus
extern "C"
{
#endif  /* ifdef cplusplus */

/*
** SELECT STRUCTURE (HANDLE)
*/

typedef struct select_handle
   {
   fd_set fd_list;
   fd_set check_fd_list;
   } SELECT_HANDLE;
   
/*---------------------------------------------------------------------
**
** FUNCTION PROTOTYPES
*/
void ftclSelect_Init(SELECT_HANDLE *);
void ftclSelect_Add(SELECT_HANDLE *, int);
void ftclSelect_Remove(SELECT_HANDLE *, int);
int  ftclSelect_Check(SELECT_HANDLE *, int);
unsigned long ftclSelect_RecvWait(SELECT_HANDLE *);

#ifdef __cplusplus
}
#endif  /* ifdef cpluplus */

#endif /* !VXWORKS */
#endif /* !FTCLSELECT_HEADER */





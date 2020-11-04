//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: ComPathNameSearch.cc 443 2010-01-14 12:24:42Z stroili $
// 
// Description:
//	Global function ComPathNameSearch
//
//      Finds a pathname for a file, using a standard 
//      search order and the BFSEARCHPATH environment variable
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//
// Copyright Information:
//	Copyright (C) 1998	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "Experiment/Experiment.hh"

#include "CommonUtils/ComPathNameSearch.hh"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <iostream>

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE		// Make sure <sys/stat.h> defines S_ISREG()
#endif
#include <sys/stat.h>

#include "ErrLogger/ErrLog.hh"

#ifndef FILENAME_MAX
#  define FILENAME_MAX 256 	// should be FILENAME_MAX, but gcc stdio.h 
#endif				// for a Sun does not have it.

#ifndef DEFAULTBFSEARCHPATH
#  define DEFAULTBFSEARCHPATH ".:RELEASE:ONLINEPARENT:PARENT"
#endif

// Define a datafile search path

static HepString defaultPath()
{

     HepString defPath(DEFAULTBFSEARCHPATH);
     
     char *penv = getenv("BFSEARCHPATH");
     HepString path;
     if (penv != NULL && strlen(penv) > 0) {
       path = penv;
     }	  
     else {
       path = defPath;
            
       char* pathBfRoot = getenv("SBROOT");
       if (pathBfRoot != 0 && strlen(pathBfRoot) > 0)
	 {
	   path += ":";
	   path += pathBfRoot;
	 }  

     }
     
     return path;
}

static HepString _dataPath = defaultPath();

static const char *getDataPath() 
{ 
     return (const char *)_dataPath;
}

// Find a datafile in the search path

HepString ComPathNameSearch(const char *fileName, bool printWarning)
{
  // check for absolute pathname
  struct stat fileStat;
  
  if ( fileName[0] == '/' )
    {
      if (stat(fileName, &fileStat) == 0 && 
	  (S_ISREG(fileStat.st_mode) || S_ISDIR(fileStat.st_mode)))
	return HepString(fileName);
      else
	{
          if ( printWarning ) 
            {
	      ErrMsg(error) << "Could not find datafile " << fileName << endmsg;
            }
	  return HepString("");
	}
    }

  
  // finally, see if we can find it in the search path
  
  HepString path(getDataPath());
  char directory[FILENAME_MAX];
  int i = 0;
  while ( path(i) != 0 )
    {
      int j=0;
      while ((directory[j] = path(i++)) != ':' && directory[j++] != 0)
	{}
      directory[j] = 0;
      
      HepString fullFileName(directory);
      fullFileName += '/';
      fullFileName += fileName;
      
      if (stat((const char*)fullFileName, &fileStat) == 0
	  && (S_ISREG(fileStat.st_mode) || S_ISDIR(fileStat.st_mode)))
	return HepString(fullFileName);
    }
  
  if ( printWarning ) 
    {
      ErrMsg(error) << "Could not find datafile \"" << fileName << "\"\n" 
                    << "   Path was \"" << _dataPath << "\"" << endmsg ;
    }
  
  return HepString("");	// avoid error message
}

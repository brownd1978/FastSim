//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      Global function ComTimeStamp
//
//      Returns the time
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Asoka De Silva          Original Author
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "CommonUtils/ComTimeStamp.hh"

#include <string>
using std::string;

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

string ComTimeStamp() {

  struct timeval tv;
  gettimeofday(&tv, 0);
  
  // hour::minute::sec.millisec
  char buf[16];
  char stamp[16]; 
  strftime(buf, 16, "%T", localtime(&tv.tv_sec) ); 
  sprintf(stamp, "%s.%03d", buf, (int)tv.tv_usec/1000);

  return string(stamp);
}



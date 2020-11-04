//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: terminationDecoder.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Debugging tool: prints a message to stdout describing the 
//      significance of of a process termination status code returned
//      to a shell.  Does not write a newline to stdout.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Gregory Dubois-Felsmann  (originator)
//
// Copyright Information:
//	Copyright (C) 2000   California Institute of Technology
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <iostream>
#include <sstream>

#include "Framework/AppFwkExitCodes.hh"
using std::cerr;
using std::cout;
using std::endl;
using std::flush;
using std::istringstream;

void usage( const char* name ) {
  cerr << "Usage: " << name << " {code}\n"
       << "       describe meaning of process termination status\n"
       << "       (does not emit a newline)" << endl;
}

int
main( int argc, char** argv )
{
  if ( argc != 2 ) {
    usage( argv[0] );
    return EINVAL;
  }

  istringstream snum( argv[1] );
  int n = -1;
  snum >> n;

  if ( !snum ) {
    // Conversion error.
    usage( argv[0] );
    return EINVAL;
  }

  if ( n < 0 ) {
    cout << "exited with unexpected negative code " << n;
  }
  else if ( n == 0 ) {
    cout << "terminated successfully";
  }
  else if ( n <= 127 ) {
    cout << "exited with code " << n;
    if ( n == AppFwkExitCodes::exitRequest ) {
      cout << " (in Framework: stop requested, e.g., by CpuCheck)";
    }
  }
  else if ( n == 128 ) {
    cout << "exited with unusual code 128 (undefined signal 0?)";
  }
//  else if ( n <= (128+SIGRTMAX) ) {
//    cout << "terminated by signal " << n-128 << " (" << strsignal(n-128) << ")";
//  }
  else if ( n < 255 ) {
    cout << "exited with code " << n << " (undefined signal " << n-128 << "?)";
  }
  else if ( n == 255 ) {
    cout << "exited with code 255/-1 (undefined signal 127?)";
  }
  else {  // n > 255
    cout << "exited with unexpectedly > 255 code " << n;
  }

  cout.flush();

  return 0;
}

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: terminator.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Debugging tool: simply terminates with either a specified exit
//      code or with a specified signal.  If the specified signal does not
//      produce a termination in ten seconds, terminates due to SIGALRM
//      at that point.
//
//      It also writes a message to each of stdout and stderr describing
//      what it's doing.
//
//      This executable can be used to test the response of a shell script
//      to unexpected terminations of the executables it normally runs.
//      The messages can be used to check the script's redirection and
//      logging of output.
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
using std::cerr;
using std::cout;
using std::endl;
using std::istringstream;

void usage( const char* name ) {
  cerr << "Usage: " << name << " [-e] {code}    -- exit with specified return code" << endl
       << "       " << name << " -s {sig}       -- attempt self-termination with signal sig" << endl
       << "                 (based on default disposition; if default does not terminate" << endl
       << "                  process, will terminate with SIGALRM in 10 seconds)" << endl;
}

int
main( int argc, char** argv )
{
  if ( argc < 2 || argc > 3 ) {
    usage( argv[0] );
    return 101;
  }

  int signal = 0;
  if ( argc == 3 ) {
    if ( strcmp( argv[1], "-s" ) == 0 )  signal = 1;
    else if ( strcmp( argv[1], "-e" ) != 0 ) {
      usage( argv[0] );
      return 102;
    }
  }

  istringstream snum( argv[argc-1] );
  int number = -1;
  snum >> number;

  if ( !snum ) {
    // Conversion error.
    usage( argv[0] );
    return 103;
  }

  if ( signal ) {
//    if ( number < 1 || number > SIGRTMAX ) {
//      cerr << argv[0] << ": signal number must be in [1.." << SIGRTMAX 
//	   << "]" << endl;
//      return 104;
//    }
  }
  else {
    if ( number < 0 || number > 255 ) {
      cerr << argv[0] << ": return code must be in [0..255]" << endl;
      return 105;
    }
  }

  cout << argv[0] << "[to stdout]: "
       << ( signal ? "self-terminating with signal " : "exiting with code " )
       << number << endl;

  cerr << argv[0] << "[to stderr]: "
       << ( signal ? "self-terminating with signal " : "exiting with code " )
       << number << endl;

  if ( signal ) {
    // Set an alarm clock signal to go off in ten seconds.  In case the
    // specified signal doesn't actually cause a process termination, the
    // alarm clock will.
    (void) alarm(10);

    // Send the specified signal back to this process.
    int raisecode = raise( number );
    if ( raisecode < 0 ) {
      cerr << argv[0] << ": error " << raisecode << " from raise("
	   << number << ") - " << strerror(errno) << " (" << errno << ")"
	   << endl;
      return 106;
    }

    // Wait for the signal.
    (void) pause();

    // We could get here if the signal is caught.  Shouldn't happen, since
    // we install no handlers.
    cerr << argv[0] << ": survived attempt to self-terminate with signal "
	 << number << ";" << endl
	 << "           (errno " << errno << " - " << strerror(errno) << endl;
    return 107;
  }

  // Terminate with the specified code.
  return number;
}

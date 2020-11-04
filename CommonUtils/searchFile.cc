//--------------------------------------------------------------------------
// File and Version Information:
// 
// Description:
//      searchFile <file1> [ <file2> ... ]
//
//      Searches for the file(s) in the argument in the standard path
//      as defined in ComPathNameSearch.cc.  The full name of the resolved 
//      file is written to stdout.  If one of more files are not found, 
//      a return value of ENOENT (2 on Solaris) is returned. 
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Asoka De Silva            Original Author
//
// Modifications:
//      Gregory Dubois-Felsmann   2000.02.17
//        Added error code for invalid arguments, changed error code for
//        file-not-found.
//
// Copyright Information:
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "Experiment/Experiment.hh"

#include "CommonUtils/ComPathNameSearch.hh"
#include "CLHEP/String/Strings.h"

#include <iostream>

#include <errno.h>
using std::cerr;
using std::cout;
using std::endl;

int main( int argc, char* argv[] ) {

  if (argc < 2) {
    cerr << "Usage: searchFile <file1> [ <file2> ...]" << endl;
    return EINVAL;
  } 

  HepString fileName;
  int successCode = 0; 

  for (int i=1; i < argc; i++) {
    fileName = ComPathNameSearch(argv[i]);
    if (fileName.length() != 0) {
      cout << fileName << endl;
    }
    else {
      successCode = ENOENT;
    }
  }
  
  return successCode;
}


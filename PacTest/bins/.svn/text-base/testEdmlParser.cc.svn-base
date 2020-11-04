/*
This is the test bed program for testing an API for parsing
EDML (Experiment Description Markup Language) documents and producing
a transient representation of the experiment descriptions.
*/
#include "BaBar/BaBar.hh"

#include "PacEnv/EdmlParser.hh"
#include "PacEnv/EdmlDetector.hh"

#include <vector>
#include <string>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <string.h>

namespace {
    void usage( )
    {
        cerr << "Usage: [<file1> [<file2> [...]]] [-verbose]" << endl;
    }
}

int
main( int argc, char* argv[] )
{
    if( argc < 2 ) {
        ::usage( );
        return 1;
    }

    bool verbose = false;
    std::vector<std::string > filenames;
    for( int i = 1; i < argc; ++i ) {
        if( !strcmp( "-verbose", argv[i] )) {
            verbose = true;
        } else {
            filenames.push_back( argv[i] );
        }
    }

    // Invoke the parser
    //
    EdmlParser parser( verbose );
    for( size_t i = 0; i < filenames.size(); ++i )
        if( !parser.parse( filenames[i].c_str( ))) return 1;

    if( verbose ) parser.dump( );

    return 0;
}

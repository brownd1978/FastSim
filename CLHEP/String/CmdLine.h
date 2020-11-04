// -*- C++ -*- 
// CLASSDOC OFF
// $Id: CmdLine.h 600 2010-06-21 07:46:30Z stroili $
// CLASSDOC ON
//
// CmdLine.h - C++ base class for command line parsing. Uses HepCmdArg and
//  subclasses.
//
// This code was inspired by the cmdline package of Brad Appleton
//    <brad@ssd.csd.harris.com>  
//
// The user interfaces of this class and for HepCmdArg...  were inspired by
// the CmdLine package written by Brad Appleton <brad@ssd.csd.harris.com>.
// However, the coding is entirely new. It was felt that the cmdline package
// was too large and complex when all we wanted was straightforward command
// line argument parsing.
//
// General Usage :
//    Create a HepCmdLine instance at the start of your program, passing it
//    argc and argv as they come from the call to main. Then create all the
//    needed HepCmdArg instances. Use the append() method to add them to the
//    list of handled argument. The final argument of append MUST be a '0'
//    (NULL) to signal the end of the list. Note that the order does not
//    matter for non-positional arguments, but does for positional ones.
//    (If two optional arguments share a flag, the last one appended wins.)
//    A call to the parse() method gets all the parameters set. After that,
//    you get the values out of the HepCmdArg classes by using them just
//    like a variable of the type of its argument.
//
//    All programs automatically get handling of usage options, ie. -help,
//    -usage, --help, --usage.
//
// Example Usage :
//    main(int argc, char **argv) {
//
//       HepCmdLine cmd(argc,argv);
//
//         // an optional argument
//       HepCmdArgInt nev('n', "nevents","count","Number of events",10);
//
//        // a positional argument
//       HepCmdArgString infileName("inputFile","Input filename","data.in");
//
//       cmd.append(&nev, &infileName, 0);
//       cmd.parse();
//
//       FILE *infile = fopen(infileName,"r");
//       for (int i=0; i<nev; i++) {
//             :
//             :     
//       
// See also :
//    CmdArg.h

#ifndef _CMDLINE_H_
#define _CMDLINE_H_

#ifdef GNUPRAGMA
#pragma interface
#endif

#include <iostream>
#include "CLHEP/Alist/AList.h"
#include "CLHEP/String/CmdArg.h"

#ifdef HEP_NO_INLINE_IN_DECLARATION
#define inline
#endif

class HepCmdLine {

public:

  HepCmdLine(int argc, char **argv);
  ~HepCmdLine();
     
  void append(HepCmdArg & arg);
  // append an argument.

  void append(HepCmdArg *arg1, ...);
  // append arguments
  // last parameter MUST be 0
  // order matters for positional arguments
  // if 2 optional arguments share a letter, last one appended wins.
     
  void parse();
  // The parse command. Pass argc, argv directly from main(), don't remove
  // the first arg.

  void printUsage(std::ostream &);
     
  void syntaxError(const char *message);
  void printHelp();
  // print to cerr and exit when done

protected:

  HepCmdArg * shortArgSearch(char);
  HepCmdArg * longArgSearch(const HepString &);
  void handleOptions();
  void handlePositionals();
   
  int _argc;
  char **_argv;
  HepString _progName;
   
  HepAList<HepCmdArg> _posList;
  HepAList<HepCmdArg> _dashList;

};

#ifdef HEP_NO_INLINE_IN_DECLARATION
#undef inline
#endif

#ifdef HEP_SHORT_NAMES
typedef HepCmdLine CmdLine;
#endif

#endif

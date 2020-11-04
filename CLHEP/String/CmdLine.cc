// $Id: CmdLine.cc 600 2010-06-21 07:46:30Z stroili $
//
// CmdLine.cc - class to parse command line arguments
//
// See CmdLine.h for usage notes

#ifdef GNUPRAGMA
#pragma implementation
#endif

#include <iomanip>
#include "CLHEP/String/CmdLine.h"
#include "CLHEP/Alist/AIterator.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#undef index

HepCmdLine::HepCmdLine(int argc, char ** argv) {
  _argc = argc - 1;
  char *s = strrchr(argv[0],'/');
  if (s) {
    while (*s == '/') s++;
    _progName = s;
  }
  else {
    _progName = argv[0];
  }
  _argv = argv + 1;
}

HepCmdLine::~HepCmdLine() {}

void HepCmdLine::append(HepCmdArg & arg) {
  if ( arg.isPositional() ) _posList.append(arg);
  else _dashList.append(arg);
}

void HepCmdLine::parse() {
  handleOptions();
  handlePositionals();
}

void HepCmdLine::printHelp() {
  printUsage(std::cerr);
  exit(0);
}

void HepCmdLine::append(HepCmdArg *arg1, ... ) {
  if (! arg1) return;

  if (arg1->isPositional())
    _posList.append(arg1);
  else
    _dashList.append(arg1);

  va_list ap;
  va_start(ap, arg1);
  HepCmdArg *cmd;
  while ( cmd = va_arg(ap, HepCmdArg *) )
    {
      if (cmd->isPositional())
	_posList.append(cmd);
      else
	_dashList.append(cmd);
    }
  va_end(ap);
}

void HepCmdLine::handleOptions() {
  while (_argc) {
    // first, determine if this is still an option

    if (_argv[0][0] != '-') break;
    _argv[0]++;

    // check if it is long arg
    int longArg = 0;
    if (_argv[0][0] == '-') {
      longArg = 1;
      _argv[0]++;
    }

    // check if it is for usage
    if (strcmp(_argv[0],"help") == 0 || strcmp(_argv[0],"usage") == 0)
      printHelp();

    // find the appropriate HepCmdArg
    HepCmdArg *cmd;
    if (longArg)	  
      cmd = longArgSearch(_argv[0]);
    else {
      cmd = shortArgSearch(_argv[0][0]);
         //syntaxError("Invalid option format");
    }
    if (!cmd) syntaxError("Unrecognized command");
    if (!longArg && strlen(_argv[0]) != 1) 
      _argv[0]++;
    else {
      _argv++;                // move to next arg
      _argc--;
    }
    
    // set the value and bump pointers by the number of arguments used
    int rc = cmd->setValue(_argc,_argv);
    if (rc < 0) {
      char err[80];
      sprintf(err,"Invalid parameter for option %s",*(_argv-1));
      syntaxError(err);
    }
    _argc -= rc;
    _argv += rc;
  }
}

void HepCmdLine::handlePositionals()
{
  int i = 0;
  int l = _posList.length();
  while (_argc && i<l) {
    int rc = _posList[i++]->setValue(_argc,_argv);
    if (rc < 0) syntaxError("Invalid parameter for positional argument");
    _argc -= rc;
    _argv += rc;
  }

  while (i < l) {
    if (_posList[i++]->isRequired()) 
      syntaxError("Missing required positional parameter");
  }
}

void HepCmdLine::syntaxError(const char *message)
{
     std::cerr << "Syntax error: " << message << std::endl;
     printUsage(std::cerr);
     exit(1);
}

void HepCmdLine::printUsage(std::ostream &sout) {
  // first, print usage line
  int ldash = _dashList.length();
  sout << "Usage : " << _progName;
  if (ldash > 0) sout << " <options>";
  int lpos = _posList.length();
  int j;
  for (j=0; j < lpos; j++) {
    if (_posList[j]->isRequired()) 
      sout  << " " << _posList[j]->valueName();
    else
      sout  << " [" << _posList[j]->valueName() << ']';
  }
  sout << std::endl;

  // next, description of positional arguments
  for (j=0; j < lpos; j++) {
    sout << "     " << std::setw(8);
    sout.setf(std::ios::left,std::ios::adjustfield);
    sout << _posList[j]->valueName() << "   "
	 << _posList[j]->description() << std::endl;
//    sout.form("     %-8s   %s\n", (const char *) (_posList[j]->valueName()),
//		    (const char *) (_posList[j]->description()) );
  }
  // finally, a list of options
  if (ldash > 0) sout << "<options> are:\n";
  for (int i=0; i<ldash; i++ ) {
    sout << "     ";
    char c = _dashList[i]->charName();
    HepString s = _dashList[i]->keywordName();
    if (c && s.length()) {
      sout << "[-" << c << " | --" << std::setw(8);
      sout.setf(std::ios::left,std::ios::adjustfield);
      sout << s << "]";
//      sout.form("[-%c | --%-8s]",c,(const char *)s);
    }
    else {
      if (c) sout << " -" << c << "              ";
//	sout.form(" -%c              ",c);
      if (s.length()) {
 	sout << "      --" << std::setw(8);
 	sout.setf(std::ios::left,std::ios::adjustfield);
 	sout << s << " ";
//	sout.form("      --%-8s ",(const char *)s);
      }
    }
    HepString v = _dashList[i]->valueName();
    if (v.length()) {
      sout << " " << std::setw(8);
      sout.setf(std::ios::left,std::ios::adjustfield);
      sout << v;
//      sout.form(" %-8s",(const char *)v);
    }
    else sout << "         ";
//      sout.form("         ");

    HepString d = _dashList[i]->description();
    if (d.length()) sout << "  " << d;

    sout << std::endl;
  }
}


     
HepCmdArg *HepCmdLine::shortArgSearch(char c) {
  HepAListIterator<HepCmdArg> it(_dashList);
  HepCmdArg * arg;
  while ( (arg = it()) &&  arg->charName() != c );
  return arg;
}

HepCmdArg *HepCmdLine::longArgSearch(const HepString & s) {
  HepAListIterator<HepCmdArg> it(_dashList);
  HepCmdArg * arg;
  while ( (arg = it()) &&  arg->keywordName() != s );
  return arg;
}

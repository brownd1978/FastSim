//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdTypeKeyIFace.cc 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//    Implementation of IfdTypeKeyIFace.  See .hh file.
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      02 Dec 97         Creation of first version
//
//
// Bugs:
//
//------------------------------------------------------------------------



#include "BaBar/BaBar.hh"
#include "ProxyDict/IfdTypeKeyIFace.hh"
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
using std::endl;
using std::ostream;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION


IfdTypeKeyIFace::IfdTypeKeyIFace() 
   : IfdKey( typeKey )
{}


IfdTypeKeyIFace::~IfdTypeKeyIFace() {
}

void
IfdTypeKeyIFace::print( ostream &o ) const {
  o << "IfdTypeKeyIFace::print:  error:  use of base class print." << endl;
}

void
IfdTypeKeyIFace::dumpName( ostream& o ) const {
  // just print() with a newline to force a flush.  Intended for
  // use in a debugger.

  print( o );
  o<<endl;
}

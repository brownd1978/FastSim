// File and Version Information:
//      $Id: AbsGHit.cc 487 2010-01-13 16:40:34Z stroili $
//
// Description:
//      implementation for AbsGHit class
//
//
// Author List:
//      Bob Jacobsen
//      Ed Frank                     University of Pennsylvania
//
// History:
//      00 XXX 00   Bob Jacobsen     Create
//      23 Jan 96   Ed Frank         Add default implementation too.  This
//                                   may be a bad idea, or it may mean that
//                                   "Abs" is a bad name.
//      12 Feb 96   Bob J            printing update
// 
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include <assert.h>
#include "AbsEvent/AbsGHit.hh"
using std::ostream;


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------


//--------------
// Destructor --
//--------------
AbsGHit::~AbsGHit() {
}


// We provide a pair of default prints that do nothing.  If a GHit does
// not want to print, we just burn some cycles.  Note the operator <<

void 
AbsGHit::print(ostream &) const {
  return;
}
void 
AbsGHit::printAll(ostream &o) const {
  print(o);
}


ostream&  operator << (ostream& o, const AbsGHit& a) {a.print(o); return o;}



// common method for time-shifting, used to mix GHits
// shift the hit time by shift.

void
AbsGHit::shiftTime( float shift ) {
  setTime( time() + shift );
}

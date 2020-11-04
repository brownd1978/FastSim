// File and Version Information:
//
// Description:
//      implementation for AbsDigi class
//
//
// Author List:
//      Bob Jacobsen
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include <assert.h>
#include "AbsEvent/AbsDigi.hh"
using std::ostream;


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------


//--------------
// Destructor --
//--------------
AbsDigi::~AbsDigi() {
}


// We provide a pair of default prints that do nothing.  If a Digi does
// not want to print, we just burn some cycles.  Note the operator <<

void 
AbsDigi::print(ostream &) const {
  return;
}
void 
AbsDigi::printAll(ostream &o) const {
  print(o);
}


ostream&  operator << (ostream& o, const AbsDigi& a) {a.print(o); return o;}


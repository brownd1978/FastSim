// File and Version Information:
//      $Id: AbsEvtObj.cc 487 2010-01-13 16:40:34Z stroili $
//
// Description:
//      implementation for AbsEvtObj class
//
//
// Author List:
//      Bob Jacobsen
//
// 
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include <assert.h>
#include "AbsEvent/AbsEvtObj.hh"
#include <iostream>
using std::cout;
using std::endl;
using std::ostream;

//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//--------------
// Destructor --
//--------------
AbsEvtObj::~AbsEvtObj() {}

void AbsEvtObj::print(ostream& ) const {
  // single line print; this is a default behavior
  cout << "(AbsEvtObj print() called with no override; this is an error!)";
}

void AbsEvtObj::printAll(ostream& ) const {
  // single line print; this is a default behavior
  cout << "AbsEvtObj printAll() called with no override; this is an error!"<<endl;
}


ostream&  operator << (ostream& o, const AbsEvtObj& a) {a.print(o); return o;}



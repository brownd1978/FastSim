// File and Version Information:
//      $Id: AbsEnvObj.cc 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//      implementation for AbsEnvObj class
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
#include "AbsEnv/AbsEnvObj.hh"
#include <iostream>
using std::endl;
using std::ostream;


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//---------------
// Constructor --
//---------------
AbsEnvObj::AbsEnvObj()
{
}

//--------------
// Destructor --
//--------------
AbsEnvObj::~AbsEnvObj()
{
}

void
AbsEnvObj::print(ostream& o) const
{
  // single line print; this is a default behavior
  o << "(AbsEnvObj print() called with no override; this is an error!)";
}

void
AbsEnvObj::printAll(ostream& o) const
{
  // single line print; this is a default behavior
  o << "AbsEnvObj printAll() called with no override; this is an error!"<<endl;
}

ostream&
operator<<( ostream& o, const AbsEnvObj& a )
{
  a.print(o);
  return o;
}



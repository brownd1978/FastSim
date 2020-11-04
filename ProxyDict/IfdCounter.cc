//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdCounter.cc 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      17 Nov 96         -Creation of first version
//    Ed Frank      07 Jan 98         -Remove static data from class by
//                                     moving it into count(void).
//                                    -Move everything from inline (oops) to
//                                     out of line.
//
// Bugs:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "ProxyDict/IfdCounter.hh"


long&
IfdCounter::count( void ) const {
  // Acts as class data but without having
  // the static live in the global namespace.

  static long _counter = 0;
  return _counter;
}

IfdCounter::IfdCounter() {
  _myCount = count();
  count() = count() + 1;
}

IfdCounter::~IfdCounter()
{}

long
IfdCounter::value() const {
  return _myCount;
}


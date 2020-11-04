//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      This class measures the elapsed CPU time.
//
//      Returns the time
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Asoka De Silva          Original Author
//
// Copyright Information:
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include "CommonUtils/ComTimer.hh"

#include <time.h>

ComTimer::ComTimer() 
  : _elapsedTime(0.0)
  , _isStopped(true)
{
}

ComTimer::~ComTimer() {
}

void
ComTimer::start() {
  _isStopped = false;
  _timeStart = clock();
}

void 
ComTimer::stop() {
  if (! _isStopped) {
    _elapsedTime += (double (clock() - _timeStart))  / double (CLOCKS_PER_SEC);
    _isStopped = true;
  }
}

double
ComTimer::elapsedTime() const {
  if (! _isStopped) {
    double deltaTime = (double (clock() - _timeStart)) / 
      double (CLOCKS_PER_SEC);
    return _elapsedTime + deltaTime;
  }
  return _elapsedTime;
}

void
ComTimer::reset() {
  if (_isStopped) {
    _elapsedTime = 0.0;
    _isStopped = true;
  }
}


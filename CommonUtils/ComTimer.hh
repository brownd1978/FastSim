//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//      This class measures the elapsed CPU time.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Asoka De Silva		Original Author
//
// Copyright Information:
//
//------------------------------------------------------------------------
#ifndef COMTIMER
#define COMTIMER

#include <time.h>

class ComTimer {

public:

  ComTimer();
  ~ComTimer();

  double elapsedTime() const;
  void reset();
  void start();
  void stop();

private:
  
  double   _elapsedTime;
  clock_t  _timeStart;
  bool     _isStopped;

};

#endif

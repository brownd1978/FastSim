//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BbrEventId.hh 475 2009-12-04 13:34:21Z stroili $
//
// Description:
//      Class BbrEventId
//
//      Encapsulates the event identification for a BaBar event.
//      Now dummied up with run and event numbers, may eventually
//      have unique ID, timestamps, etc.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen           November 1995
//
// Copyright Information:
//      Copyright (C) 1994     LBNL
//
//------------------------------------------------------------------------

#ifndef BBREVENTID_HH
#define BBREVENTID_HH

class BbrEventId {
public:
  void setRun(const int r) { _run = r; };
  int run() const { return _run;};

  void setEvent(const int e) {_event = e;};
  int  event() const { return _event;};

protected:
  int _run,_event;
};

#endif

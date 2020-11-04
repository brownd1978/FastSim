//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkIdManDefault.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Standard manager of track ids; just a counter, really.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#ifndef TRKIDMANDEFAULT_HH
#define TRKIDMANDEFAULT_HH

#include "TrkBase/TrkIdManager.hh"

// Class interface //
class TrkIdManDefault : public TrkIdManager {

public:
  TrkIdManDefault(long firstId);
  virtual ~TrkIdManDefault();
  
  virtual long nextId();
  virtual long lastId() const;
// allow updating the maximum value
  virtual void setMax(long maxid);

private:	
  long _nextId;

  // Preempt 
  TrkIdManDefault&   operator= (const TrkIdManDefault&);
  TrkIdManDefault(const TrkIdManDefault &);
};

#endif








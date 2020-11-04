//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkDeadMaker.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Used to add a DeadRep to an existing track
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKDEADMAKER_HH
#define TRKDEADMAKER_HH
#include "TrkBase/TrkFitMaker.hh"
#include "PDT/PdtPid.hh"

class TrkRep;

// Class interface //
class TrkDeadMaker : public TrkFitMaker {
public:
  TrkDeadMaker();
  virtual ~TrkDeadMaker();
  
  // Adds a DeadRep to an existing track

  void        addHypo(TrkRecoTrk* theTrack, PdtPid::PidType hypo);

private:	
  // Preempt 
  TrkDeadMaker&   operator= (const TrkDeadMaker&);
  TrkDeadMaker(const TrkDeadMaker &);
};

#endif

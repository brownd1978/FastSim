//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkHelixMaker.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//   Creates tracks with HelixReps inside them (makeTrack()), or swaps 
//   in a HelixRep for the existing Rep (changeFit()).  In neither case 
//   is a fit performed.  HelixReps have an option of allowing hit-dropping 
//   and ambiguity-flipping while fitting; these can be turned on  
//   by invoking setFlipAndDrop() for that track.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKHELIXMAKER_HH
#define TRKHELIXMAKER_HH
#include "TrkFitter/TrkSimpleMaker.hh"
#include "TrkFitter/TrkHelixRep.hh"


class TrkHelixMaker : public TrkSimpleMaker<TrkHelixRep> {
public:
  TrkHelixMaker();
  virtual ~TrkHelixMaker();

  void addZValues(TrkRecoTrk& theTrack, double z0,
                  double tanDip, double chi2);

protected:
  TrkRep* makeRep(TrkRecoTrk& theTrack) const;

private:

  // Preempt 
  TrkHelixMaker&   operator= (const TrkHelixMaker&);
  TrkHelixMaker(const TrkHelixMaker &);
};

#endif

//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkSimpleMaker.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//   Creates tracks with reps of type T inside them (makeTrack()), or swaps 
//   in a T rep for the existing Rep (changeFit()).  In neither case 
//   is a fit performed.  T reps have an option of allowing hit-dropping 
//   and ambiguity-flipping while fitting; these can be turned on 
//   by invoking setFlipAndDrop() for that track.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Gerhard Raven
//
//------------------------------------------------------------------------

#ifndef TRKSIMPLEMAKER_HH
#define TRKSIMPLEMAKER_HH
#include "TrkBase/TrkFitMaker.hh"

class TrkRep;

template <class T>
class TrkSimpleMaker : public TrkFitMaker {
public:
  TrkSimpleMaker();
  virtual ~TrkSimpleMaker();

  virtual void changeFit(TrkRecoTrk& theTrack) const;
  TrkRecoTrk* makeTrack(const TrkExchangePar& helix, const
                        double chi2, const TrkContext&, double trackT0) const;

  // Change control parameters in track -- only valid for input SimpleReps.
  bool setFlipAndDrop(TrkRecoTrk&, bool allowFlips, bool allowDrops) const;

  void setValidRange(TrkRecoTrk& track, double newLo, double newHi) const;
  void setValues(TrkRecoTrk& theTrack,
                 const TrkExchangePar& newPars, double chi2) const;

private:
  // Preempt
  TrkSimpleMaker&   operator= (const TrkSimpleMaker&);
  TrkSimpleMaker(const TrkSimpleMaker &);
};
#include "TrkFitter/TrkSimpleMaker.icc"

#endif

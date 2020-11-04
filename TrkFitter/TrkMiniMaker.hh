//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkMiniMaker.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//   Creates tracks with HelixReps inside them (makeTrack()), but without 
// any hits.  Used to create minimal transient tracks (from persistent store) 
// or MC tracks.  Requires full input info; assumes that input fit is valid, 
// and done taking multiple-scattering into account.
//
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKMINIMAKER_HH
#define TRKMINIMAKER_HH
#include "TrkBase/TrkFitMaker.hh"
#include "PDT/PdtPid.hh"

class TrkRep;

// Class interface //
class TrkMiniMaker : public TrkFitMaker {
public:
  TrkMiniMaker();
  virtual ~TrkMiniMaker();
  
  TrkRecoTrk* makeTrack(const TrkExchangePar& helix, 
			int nact, int nsv, int ndc, double chi2, 
			double startFoundRng, double endFoundRng, 
			PdtPid::PidType def, const TrkContext&, 
			double trackT0);

  // Add a fit to an existing track.  <hypo> cannot already have its own fit.
  //   Useful if you're filling tracks from an external source (like database)
  void        addHypo(TrkRecoTrk& theTrack, const TrkExchangePar& helix, 
		      int nact, int nsv, int ndc, double chi2, 
	       double startFoundRng, double endFoundRng, PdtPid::PidType hypo);

private:	
  // Preempt 
  TrkMiniMaker&   operator= (const TrkMiniMaker&);
  TrkMiniMaker(const TrkMiniMaker &);
};

#endif

//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkMergeMap.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Map to navigate from a merged track to the two tracks that made it up.
// Simplest possible implementation (using STL map).
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#ifndef TRKMERGEMAP_HH
#define TRKMERGEMAP_HH

#include <map>

class TrkRecoTrk;
class TrkId;

// Class interface //
class TrkMergeMap {

public:
  TrkMergeMap();
  virtual ~TrkMergeMap();
  void        insert(TrkRecoTrk* mergedTrk,
                     TrkRecoTrk* parent1, TrkRecoTrk* parent2);
  void        remove(const TrkRecoTrk* mergedTrk);
  TrkRecoTrk* track1(const TrkRecoTrk* mergedTrk) const;
  TrkRecoTrk* track2(const TrkRecoTrk* mergedTrk) const;

private:
  typedef std::map< TrkId, std::pair<TrkRecoTrk*,TrkRecoTrk*> > map_t;
  map_t  _map;
  mutable map_t::const_iterator _lastRequest; // try to speed things up

  // Preempt
  TrkMergeMap&   operator= (const TrkMergeMap&);
  TrkMergeMap(const TrkMergeMap &);
};

#endif

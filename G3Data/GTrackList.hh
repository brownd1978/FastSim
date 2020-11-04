//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: GTrackList.hh 430 2010-01-14 15:20:24Z stroili $
//
// Description:
//      Class GTrackList
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bill Lockman
//
// History:
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#ifndef GTrackList_HH
#define GTrackList_HH

#include <iostream>
#include <vector>
using std::vector;

#include "CLHEP/Alist/AList.h"

class GTrack;

class GTrackList {

public:
  GTrackList(const HepAList< GTrack >* gList);
  ~GTrackList(){;}
  void PrintTree(std::ostream& o);
  int numStableOrphans() { return _stableOrphanList.size(); };
  const GTrack* StableOrphan(int k);

private:
  void printGTracks(const GTrack* g, int level, std::ostream& o);
  void constructDaughterLists();
  void constructStableOrphanList();
  vector <const GTrack*> _stableOrphanList;
  const HepAList< GTrack >* _gList;
};

#endif

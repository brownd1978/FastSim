//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: GTrackList.cc 703 2010-09-13 11:47:05Z stroili $
//
// Description:
//      Class GTrackList
//      Class to help the GTracks construct their daughters (GTrackList
//      knows about the HepAList <GTrack>* list, whereas GTracks are ignorant
//      of this)
//      Also counts the number of outgoing stable orphans (numStableOrphans())
//      and prints the GTrack genealogy (PrintTree(ostream& o))
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
#include <iomanip>

#include "G3Data/GTrackList.hh"
#include "G3Data/GVertex.hh"
#include "G3Data/GTrack.hh"
#include "PDT/Pdt.hh"
using std::endl;
using std::ostream;

GTrackList::GTrackList(const HepAList< GTrack >* gList)
  : _stableOrphanList(0)
  , _gList(gList)
{
  constructDaughterLists();
  constructStableOrphanList();
}

const GTrack*
GTrackList::StableOrphan(int k) {
  if (k >= numStableOrphans() || k < 0) return 0;
  else return _stableOrphanList[k];
}  

void
GTrackList::constructStableOrphanList() {
  if (_gList == 0) return;
  int _nGTracks = _gList->length();
  for (int i=0; i<_nGTracks; i++) {
    GTrack* gTrack = (*_gList)[i];
    assert (gTrack != 0);
    bool incoming = (gTrack->terminalVertex() != 0 && 
		     gTrack->terminalVertex()->cause() == GVertex::generator &&
		     gTrack->parentTrack() == 0);
    if (gTrack->parentTrack()==0 && gTrack->NumDaughters()==0 && !incoming){
      _stableOrphanList.push_back(gTrack);
    }
  }
}

void
GTrackList::PrintTree(ostream& ostr) {
  if ( _gList == 0 ) return;
  int _nGTracks = _gList->length();
  for( int i=0; i<_nGTracks; i++) {
    GTrack* gTrack = (*_gList)[i];
    assert (gTrack != 0);
    if (gTrack->parentTrack() == 0) {
      int level=0;
      printGTracks(gTrack, level, ostr);
    }
  }
}

//
// private methods
//
void
GTrackList::constructDaughterLists() {
  if ( _gList == 0 ) return;
  int _nGTracks = _gList->length();
  for (int k=0; k < _nGTracks; k++) {
    GTrack* _gTrack = (*_gList)[k];
    _gTrack->RemoveAllDaughters(); 
    for (int i=k+1; i < _nGTracks; i++) {
      GTrack* dau = (*_gList)[i];
      GTrack* mom = dau->parentTrack();
      if (mom == _gTrack) {
	// tell the GTrack to add a daughter to its list
	_gTrack->AddDaughter(dau);
      }
    }
  }
}

void
GTrackList::printGTracks(const GTrack* gTrack, int level, ostream& o) {
  for (int j=0; j<level; j++) o << "---";
  o << " GTrack " << gTrack->index() << " hepid " << gTrack->hepid() << " " 
    << gTrack->pdt()->name();
  int num = gTrack->NumDaughters();
  if (num > 0) {
    o << " " << num << " daughters:";
    int k;
    for (k=0; k<num; k++) o << " " << gTrack->daughter(k)->pdt()->name();
    o << endl;
    level++;
    for (k=0; k<num; k++) printGTracks(gTrack->daughter(k), level, o);
  } else {
    o << endl;
  }
}

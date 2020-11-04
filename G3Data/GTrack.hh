//
// $Id: GTrack.hh 430 2010-01-14 15:20:24Z stroili $
//
// GTrack
// ------
// Stores information on the true characteristics of a 
// Monte Carlo simulated track.
//
// This object belongs to the framework, and is usually stored
// in a HepAlist.
//
// In general, only some of the simulated tracks are saved in
// GTrack, in order to save space. See GTrimMarker for more info.
//
// Units are cm, GeV, second
//
// --------------------------------------------------
// Copyright (c) 2000      BaBar Collaboration
//

#ifndef GTrack_HH
#define GTrack_HH

#include <iostream>
#include <vector>
using std::vector;

#include "AbsEvent/AbsEvtObj.hh"

#include "CLHEP/Vector/LorentzVector.h"
using CLHEP::HepLorentzVector;

class PdtEntry;
class GVertex;
class GTrimMarker;

class GTrack : public AbsEvtObj {
public:

  //
  // Default constructor.
  //
  GTrack();

  ~GTrack();

  //
  // Constants
  //
  enum { unassigned = -1 };
  
  //
  // Equivalence operator and < operator , for compatibility with containers
  //
  bool operator==( const GTrack &other ) const { 
    if ( _index == unassigned ) return false;
    return _index == other._index; 
  };

  bool operator<( const GTrack &other ) const { 
    if ( _index == unassigned || other._index == unassigned ) assert(0);
    return _index < other._index; 
  };

  //
  // returns the number of daughters
  //
  int NumDaughters () const { return _daughterList.size(); }
  
  //
  // returns kth daughter (no bounds checking is done)
  //
  const GTrack* daughter(int k) const { return _daughterList[k]; }

  //
  // return daughter vectpr
  //
  const std::vector<const GTrack*>& daughterList() const { return _daughterList; }

  //
  // add a daughter
  //
  void AddDaughter( const GTrack* daug ) { _daughterList.push_back(daug); }

  // 
  // remove all daughters
  //
  void RemoveAllDaughters() { _daughterList.clear(); }

  //
  // =========== Print methods
  //
  virtual void printAll (std::ostream &o = std::cout) const {o << *this; }   
  virtual void print (std::ostream &o = std::cout) const {o << *this; }
  
  friend std::ostream &operator<<(std::ostream &, const GTrack&);
  friend std::ostream &operator<<(std::ostream &, const GTrack*);
  
  //
  // A nice string for printing a header for the << operators
  // For example:
  //        cout << GTrack::ostreamHeader << endl;
  //        cout << track << endl;
  //
  static const char *ostreamHeader;
  
  //
  // =========== Standard accessors
  //
  // Return four momentum at origin.
  // This is GEANT3 or GEANT4's values that may not be
  // precisely consistent with the mass from BaBar's PDT. Beware.
  //
  HepLorentzVector p4() const { return _fourMomentum; }
  void setP4( const HepLorentzVector &p4 ) { _fourMomentum = p4; }

  //
  // Return particle type, BaBar style
  //
  PdtEntry *pdt() const { return _pdt; }
  void setPDT( PdtEntry *pdt ) { _pdt = pdt; }
  
  //
  // Access vertex associated with the beginning
  // of the track. For Bogus, this is guaranteed to be
  // the actual vertex. For BBSIM, under some circumstances,
  // it may be the vertex of a distant progenitor.
  //
  GVertex *vertex() const { return _vertex; }
  void setVertex( GVertex *v ) { _vertex = v; };
  
  //
  // For Bogus: Access vertex associated with the death of the track,
  // or return zero if the track exited the detector.
  // For BBSIM: Always returns zero (sorry, there is no easy
  // fix for this)
  //
  GVertex *terminalVertex() const { return _terminalVertex; }
  void setTerminalVertex( GVertex *v ) { _terminalVertex = v; };
  
  //
  // Returns the index of this track. For Bogus, this is
  // simply the index of the track in its HepAList. For BBSIM,
  // I would expect the same thing, but this may not be
  // guaranteed. 
  //
  signed long index() const { return _index; }
  void setIndex( signed long newIndex ) { _index = newIndex; }
  
  //
  // Return the youngest parent saved to GTrack. This may or
  // may not be the immediate parent of the track. To find
  // out, check the trim marker.
  //
  GTrack *parentTrack() const;

  //
  // Return the trim marker associated with this track,
  // or zero if the immediate parent was saved in GTrack.
  //
  // NOTE: this method of checking for parent immediacy
  // is now deprecated, as GTrimMarkers are not persisted
  // in the CM2 Event Store. Use parentIsImmediate 
  // instead.
  GTrimMarker *parentTrimMarker() const;
  bool parentIsImmediate() const;

  //
  // Returns the most immediate progenitor of this particle
  // created by the event generator (e.g. JETSET)
  // 
  const GTrack *ultimateParent() const;
  
  //
  // ===========  Depreciated methods
  // These are retained for backward
  // compatibility with the old dbio generated ones.
  //
  // Applications should migrate to the newer accessors above.
  //
  // Please note that n() counts from 1 (FORTRAN style),
  // whereas index, above, counts from 0.
  //
  GTrack(const HepLorentzVector&);
  
  int n() const { return _index+1; }
  void setN(const int i) { _index = i-1;}
  
  int id() const;
  void setId(const int);
  
  float px() const { return _fourMomentum.px(); }
  void setPx(const float px) { _fourMomentum.setPx(px); }
  
  float py() const { return _fourMomentum.py(); }
  void setPy(const float py) { _fourMomentum.setPy(py); }
  
  float pz() const { return _fourMomentum.pz(); }
  void setPz(const float pz) { _fourMomentum.setPz(pz); }
  
  float e() const { return _fourMomentum.e(); }
  void setE(const float e) { _fourMomentum.setE(e); }
  
  int ivtx() const;
  void setIvtx(const int) {;} 	// Do nothing!
  
  int hepid() const { return _hepid; }
  void setHepid(const int i) { _hepid = i; }
  
protected:
  
  //
  // _fourMomentum: the four-momentum of the particle in the
  // lab frame at the point of origin (as stored in GVertex)
  //
  HepLorentzVector _fourMomentum;
  
  //
  // _vertex: the GVertex object representing the origin
  // of this particle. Not owned by GTrack.
  //
  GVertex *_vertex;
  
  //
  // _terminalVertex: the GVertex object representing the
  // end of this particle, or zero if the particle exits
  // the detector. Not owned by GTrack.
  //
  GVertex *_terminalVertex;
  
  //
  // _pdt: pointer to the particle in the Pdt table.
  // Not owned by GTrack.
  //
  PdtEntry *_pdt;
  
  //
  // _index: the index of this object in the owning
  // HepAList. Counting from zero. Set to GTrack::unassigned
  // if this object is not in a list.
  //
  // In BBSIM, we expect:
  //      HepAList<GTrack> gtracks;
  //      gtracks[n]->n() == n+1;
  // However, it is not clear if this is guaranteed!
  // See, for example, SimUtils/G3accessor
  //
  signed long _index;
  
  //
  // Some GTracks are associated with stdhep tracks
  // (The input to BBSIM or Bogus). For those,
  // This index value is set appropriately, counting
  // from 1.
  //
  short _hepid;
  
private:

  vector<const GTrack *> _daughterList;

};

#endif



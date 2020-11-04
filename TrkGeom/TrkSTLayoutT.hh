//---------------------------------------------------------------------
//
// TrkSTGLayout
//
// Description: non-persistent class for describing inert beampipe material 
//
// Author List: Johan Blouw, Colorado State University
//
// Modifications: Created  April 25, 2001.
//                Adopted from TrkSimGLayout for the Reco Detector Model
//                by Johan Blouw, April 25, 2001
//
//---------------------------------------------------------------------
#ifndef TRKSTLAYOUTT_HH
#define TRKSTLAYOUTT_HH

#include "TrkGeom/TrkSTLayout.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "assert.h"

#include <iosfwd>

class TrkSTLayoutT : public TrkSTLayout {
public:
  // dummy constructor
  TrkSTLayoutT();
  TrkSTLayoutT(const char*);                 // Construct from ASCII file
  TrkSTLayoutT(const TrkSTLayoutT &other); //copy constructor
  TrkSTLayoutT(const TrkSTLayout &other); //construct from bass class
  const TrkSTLayoutT &operator=( const TrkSTLayoutT &other ); //equivalence operator
  const TrkSTLayoutT &operator=( const TrkSTLayout &other ); //equivalence operator
  // destructor
  ~TrkSTLayoutT();
  
  // TrkSTLayout interface
  virtual d_Double radius() const { return _radius; }

  // number of segments
  virtual d_ULong nSegments() const { return _nsegs; }

  // segment-dependent values
  virtual d_Double thickness(unsigned iseg) const { return _thickness[iseg]; }

  // material is referenced by name
  virtual const char* getSTMaterial(unsigned iseg) const { return _materials[iseg].c_str(); }

  // high z end of one segment is the low z end of the next.
  virtual d_Double zSTEnds(unsigned iseg,TrkSTLayout::zends end) const {
    assert(iseg < _nsegs);
    return (end == TrkSTLayout::lowZEnd) ? _stends[iseg] : _stends[iseg+1]; }

private:

  d_ULong _nsegs; // number of z segments in this beampipe representation
  d_Double _radius; // radius; fixed for all segments
  std::vector<d_Double> _stends;
  std::vector<d_Double> _thickness; // z std::ends and thickness depend on segment
  std::vector<std::string> _materials; // material can depend on segment

  const char* name() {return "TrkSTLayoutT";}
  void findTag(const char*,std::ifstream&);
  void readLayout(const char*);
protected:
  void copyFromBase( const TrkSTLayout * const other );           // Copy from base class

  friend class TrkSTLayoutCdbR;
};
#endif // TRKSTLAYOUTT_HH





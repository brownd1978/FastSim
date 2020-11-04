//---------------------------------------------------------------------
//
// TrkBPGLayout
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
#ifndef TRKBPLAYOUTT_HH
#define TRKBPLAYOUTT_HH

#include "TrkGeom/TrkBPLayout.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "assert.h"
#include <vector>
#include <string>

#include <iosfwd>

class TrkBPLayoutT : public TrkBPLayout {
public:
  // dummy constructor
  TrkBPLayoutT();
  TrkBPLayoutT(const char*);                 // Construct from ASCII file
  TrkBPLayoutT(const TrkBPLayoutT &other); //copy constructor
  TrkBPLayoutT(const TrkBPLayout &other); //construct from bass class
  const TrkBPLayoutT &operator=( const TrkBPLayoutT &other ); //equivalence operator
  const TrkBPLayoutT &operator=( const TrkBPLayout &other ); //equivalence operator
  // destructor
  ~TrkBPLayoutT();
  
  // TrkBPLayout interface
  virtual d_Double radius() const { return _radius; }

  // number of segments
  virtual d_ULong nSegments() const { return _nsegs; }

  // segment-dependent values
  virtual d_Double thickness(unsigned iseg) const { return _thickness[iseg]; }

  // material is referenced by name
  virtual const char* getBPMaterial(unsigned iseg) const { return _materials[iseg].c_str(); }

  // high z end of one segment is the low z end of the next.
  virtual d_Double zBPEnds(unsigned iseg,TrkBPLayout::zends end) const {
    assert(iseg < _nsegs);
    return (end == TrkBPLayout::lowZEnd) ? _bpends[iseg] : _bpends[iseg+1]; }

private:

  d_ULong _nsegs; // number of z segments in this beampipe representation
  d_Double _radius; // radius; fixed for all segments
  std::vector<d_Double> _bpends;
  std::vector<d_Double> _thickness; // z std::ends and thickness depend on segment
  std::vector<std::string> _materials; // material can depend on segment

  const char* name() {return "TrkBPLayoutT";}
  void findTag(const char*,std::ifstream&);
  void readLayout(const char*);

protected:
  void copyFromBase( const TrkBPLayout * const other );           // Copy from base class

  friend class TrkBPLayoutCdbR;
};

#endif // TRKBPLAYOUTT_HH

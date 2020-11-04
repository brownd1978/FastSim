//---------------------------------------------------------------------
//
// TrkSvtSGLayout
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
#ifndef TRKSVTSLAYOUTT_HH
#define TRKSVTSLAYOUTT_HH

#include "TrkGeom/TrkSvtSLayout.hh"
#include "DetectorModel/DetSurfaceElem.hh"
#include "assert.h"
#include <string>

#include <iosfwd>

class TrkSvtSLayoutT : public TrkSvtSLayout {
public:
  // dummy constructor
  TrkSvtSLayoutT();
  TrkSvtSLayoutT(const char*);                 // Construct from ASCII file
  TrkSvtSLayoutT(const TrkSvtSLayoutT &other); //copy constructor
  TrkSvtSLayoutT(const TrkSvtSLayout &other); //construct from bass class
  const TrkSvtSLayoutT &operator=( const TrkSvtSLayoutT &other ); //equivalence operator
  const TrkSvtSLayoutT &operator=( const TrkSvtSLayout &other ); //equivalence operator
  // destructor
  ~TrkSvtSLayoutT();
  
  // TrkSvtSLayout interface

  // number of elements
  virtual d_ULong nElements() const { return _nelems; }

  // number of segments
  virtual d_ULong nSegments() const { return _nsegs; }

  // radii
  virtual d_Double radii(unsigned ielem) const { return _radii[ielem]; }

  // segment-dependent values
  virtual d_Double thickness(unsigned ielem,unsigned iseg) const { return _thickness[ielem][iseg]; }

  // material is referenced by name
  virtual const char* getSvtSMaterial(unsigned ielem,unsigned iseg) const { return _materials[ielem][iseg].c_str(); }

  // high z end of one segment is the low z end of the next.
  virtual d_Double zSvtSZEnds(unsigned ielem,unsigned iseg,TrkSvtSLayout::zends end) const {
    assert(iseg < _nsegs);
    assert(ielem < _nelems);
    return (end == TrkSvtSLayout::lowZEnd) ? _zends[ielem][iseg] : _zends[ielem][iseg+1]; }

  private:
  d_ULong _nelems; // number of elements (i.e. number of shields...)
  d_ULong _nsegs; // number of z segments in each of the shields
  d_Double *_radii; // radii; fixed for all segments
  d_Double **_zends, **_thickness; // z std::ends and thickness depend on segment
  std::string **_materials; // material can depend on segment
//
  const char* name() {return "TrkSvtSLayoutT";}
  void findTag(const char*,std::ifstream&);
  void readLayout(const char*);
protected:
  void copyFromBase( const TrkSvtSLayout * const other );           // Copy from base class

  friend class TrkSvtSLayoutCdbR;
};

#endif // TRKSVTSLAYOUTT_HH

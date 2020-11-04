//-------------------------------------------------------------------
//
// TrkSvtSLayoutT
//
// Description: non-persistent class for describing inert beampipe material 
//
// Author List: Johan Blouw
//
// Modifications: Created August 2000
//                Adopted from TrkSimGLayout for the Reco Detector Model
//                by Johan Blouw, April 25, 2001.
//
//-------------------------------------------------------------------
#include "BaBar/BaBar.hh" 
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "TrkGeom/TrkSvtSLayoutT.hh"
#include <stdio.h>
#include <fstream>
#include "ErrLogger/ErrLog.hh"
using std::endl;
using std::fstream;
using std::ifstream;
using std::ostream;
//
//  Dummy constructor; does nothing
//
TrkSvtSLayoutT::TrkSvtSLayoutT() {;}

// Construct from an ascii file
TrkSvtSLayoutT::TrkSvtSLayoutT(const char* fname) :
  _nsegs(0),_radii(0),_zends(0),
  _thickness(0),_materials(0)
{
  readLayout(fname);            // Get numbers of components

  ErrMsg(routine) << name() 
                  << ": finished constructing transient Svt shields Trk objects" 
		  << endmsg;
}

//
// Copy constructor, from base class
//
TrkSvtSLayoutT::TrkSvtSLayoutT( const TrkSvtSLayout &other ) :
  _nsegs(0),_radii(0),_zends(0),
  _thickness(0),_materials(0)
{
        copyFromBase( &other );
}
// Copy constructor, from same class
//
TrkSvtSLayoutT::TrkSvtSLayoutT( const TrkSvtSLayoutT &other ) :
  _nsegs(0),_radii(0),_zends(0),
  _thickness(0),_materials(0)
{
        copyFromBase( &other );
}



//
// Assignment operator, from base class
//
const TrkSvtSLayoutT &TrkSvtSLayoutT::operator=( const TrkSvtSLayout &other )
{
  if (this == &other) return *this;
  copyFromBase( &other );
  return *this;
}


//
// Assignment operator, from same class
//
const TrkSvtSLayoutT &TrkSvtSLayoutT::operator=( const TrkSvtSLayoutT &other )
{
  if (this == &other) return *this;
  copyFromBase( &other );
  return *this;
}


void 
TrkSvtSLayoutT::findTag(const char* tagname, ifstream& datafile)
{
  //
  // Read datafile until tag is found
  //
  bool tag = false;
  std::string fline;
  while(!tag && !datafile.eof()){
    getline(datafile,fline);
    tag = ( fline.find(tagname)!= std::string::npos);
  }
  if (!tag) ErrMsg(fatal) << name() << ": failed to find tag \""
                          << tagname << "\"" << endmsg;
}


void TrkSvtSLayoutT::readLayout(const char* filename){
  ifstream datafile(filename);
  if (!datafile.good()) ErrMsg(fatal) << name() << ": unable to open file "
                                      << filename << endmsg;
  findTag("TrkSvtSLayoutT",datafile);
  ErrMsg(routine) << name() << " reading file " << filename << endmsg;
  // for n elements of m segments...
  // Read the number of elements
  datafile >> _nelems;
  // read the number of segments per element
  datafile >> _nsegs;
// delete and remake arrays
  delete [] _radii;
  delete [] _thickness;
  delete [] _materials;
  delete [] _zends;
// dynamically allocate memory for the arrays
  _radii = new d_Double[_nelems];
  _thickness = new d_Double *[_nelems];
  _materials = new std::string *[_nelems];
  _zends = new d_Double *[_nelems];
  for(unsigned i=0; i < _nelems; i++){
     _thickness[i] = new d_Double [_nsegs];
     _materials[i] = new std::string[_nsegs];
     _zends[i] = new d_Double[(_nsegs+1)];
  }
// 
  unsigned ielem;
  unsigned iseg;
  // read in the properties.
  // The order is elements, segments.
// first radii of the elements
  for(ielem=0;ielem<_nelems;ielem++){
    datafile >> _radii[ielem];
    // then the properties of the segments
    for( iseg=0; iseg<_nsegs;iseg++){
        // thickness
        datafile >> _thickness[ielem][iseg];
        // then, materials
        datafile >> _materials[ielem][iseg];
    }
    // note that a tube has two ends...
    for (iseg=0; iseg<_nsegs+1;iseg++){
        // then the z ends
        datafile >> _zends[ielem][iseg];
    }
  }
  ostream& os = ErrMsg(routine);
  os << name() << endl;
  print(os);
  os << endmsg;
}

// Destructor

TrkSvtSLayoutT::~TrkSvtSLayoutT()
{
  delete [] _radii;
  for(unsigned i = 0; i<_nelems; i++){
    delete [] _thickness[i];
    delete [] _materials[i];
    delete [] _zends[i];
  }
  delete [] _thickness;
  delete [] _materials;
  delete [] _zends;
}


//
// CopyFromBase (protected)
//
// Allocate and copy all objects from a base class
//
void TrkSvtSLayoutT::copyFromBase( const TrkSvtSLayout * const other )
{
  _nsegs = other->nSegments();
  _nelems = other->nElements();
// delete and remake arrays
  delete [] _radii;
  delete [] _thickness;
  delete [] _materials;
  delete [] _zends;
// dynamically allocate memory for the arrays
  _radii = new d_Double[_nelems];
  for(unsigned i=0; i < _nelems; i++){
     _thickness[i] = new d_Double [_nsegs];
     _materials[i] = new std::string[_nsegs];
     _zends[i] = new d_Double[(_nsegs+1)];
  }
// copy the information
  for(unsigned ielem=0;ielem<_nelems;ielem++){
    _radii[ielem] = other->radii(ielem);
    for(unsigned iseg=0;iseg<_nsegs;iseg++){
      _thickness[ielem][iseg] = other->thickness(ielem,iseg);
      _materials[ielem][iseg] = other->getSvtSMaterial(ielem,iseg);
      _zends[ielem][iseg] = other->zSvtSZEnds(ielem,iseg,TrkSvtSLayout::lowZEnd);
    }
    _zends[_nelems][_nsegs] = other->zSvtSZEnds(_nelems,_nsegs+1,TrkSvtSLayout::highZEnd);
  }
}

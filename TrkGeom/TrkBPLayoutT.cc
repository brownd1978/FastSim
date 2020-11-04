//-------------------------------------------------------------------
//
// TrkBPLayoutT
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
#include "TrkGeom/TrkBPLayoutT.hh"
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
TrkBPLayoutT::TrkBPLayoutT() {;}

// Construct from an ascii file
TrkBPLayoutT::TrkBPLayoutT(const char* fname) :
  _nsegs(0),_radius(0.0)
{
  readLayout(fname);            // Get numbers of components

  ErrMsg(routine) << name() 
                  << ": finished constructing transient BeamPipe Trk objects" 
                  << endmsg;
}

//
// Copy constructor, from base class
//
TrkBPLayoutT::TrkBPLayoutT( const TrkBPLayout &other ) :
  _nsegs(0),_radius(0.0)
{
        copyFromBase( &other );
}
// Copy constructor, from same class
//
TrkBPLayoutT::TrkBPLayoutT( const TrkBPLayoutT &other ) :
  _nsegs(0),_radius(0.0)
{
        copyFromBase( &other );
}



//
// Assignment operator, from base class
//
const TrkBPLayoutT &TrkBPLayoutT::operator=( const TrkBPLayout &other )
{
  if (this == &other) return *this;
  copyFromBase( &other );
  return *this;
}


//
// Assignment operator, from same class
//
const TrkBPLayoutT &TrkBPLayoutT::operator=( const TrkBPLayoutT &other )
{
  if (this == &other) return *this;
  copyFromBase( &other );
  return *this;
}


void 
TrkBPLayoutT::findTag(const char* tagname, ifstream& datafile)
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


void TrkBPLayoutT::readLayout(const char* filename){
  ifstream datafile(filename);
  if (!datafile.good()) ErrMsg(fatal) << name() << ": unable to open file "
				      << filename << endmsg;
  findTag("TrkBPLayoutT",datafile);
  ErrMsg(routine) << name() << " reading file " << filename << endmsg;
  datafile >> _radius;
  datafile >> _nsegs;
// delete and remake arrays
  _thickness.clear(); _thickness.resize(_nsegs);
  _materials.clear(); _materials.resize(_nsegs);
  _bpends.clear(); _bpends.resize(_nsegs+1);
// 
  unsigned iseg;
// first, thickness
  for(iseg=0;iseg<_nsegs;iseg++)
    datafile >> _thickness[iseg];
// then, materials
  for(iseg=0;iseg<_nsegs;iseg++)
    datafile >> _materials[iseg];
// then, ends.  Note there is one additional end
  for(iseg=0;iseg<_nsegs+1;iseg++)
    datafile >> _bpends[iseg];
  ostream& os = ErrMsg(routine);
  os << name() << endl;
  print(os);
  os << endl;
}

TrkBPLayoutT::~TrkBPLayoutT()
{
}


//
// CopyFromBase (protected)
//
// Allocate and copy all objects from a base class
//
void TrkBPLayoutT::copyFromBase( const TrkBPLayout * const other )
{
  _radius = other->radius();
  _nsegs = other->nSegments();
  _thickness.clear(); _thickness.resize(_nsegs);
  _materials.clear(); _materials.resize(_nsegs);
  _bpends.clear(); _bpends.resize(_nsegs+1);
  for(unsigned iseg=0;iseg<_nsegs;iseg++){
    _thickness[iseg] = other->thickness(iseg);
    _materials[iseg] = other->getBPMaterial(iseg);
    _bpends[iseg] = other->zBPEnds(iseg,TrkBPLayout::lowZEnd);
  }
  _bpends[_nsegs] = other->zBPEnds(_nsegs-1,TrkBPLayout::highZEnd);
  
}

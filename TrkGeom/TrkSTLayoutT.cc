//-------------------------------------------------------------------
//
// TrkSTLayoutT
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
#include "TrkGeom/TrkSTLayoutT.hh"
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
TrkSTLayoutT::TrkSTLayoutT() {;}

// Construct from an ascii file
TrkSTLayoutT::TrkSTLayoutT(const char* fname) :
  _nsegs(0),_radius(0.0),
  _stends(0),_thickness(0),_materials(0)
{
  readLayout(fname);            // Get numbers of components

  ErrMsg(routine) << name() 
                  << ": finished constructing transient Support Tube Trk objects" 
                  << endmsg;
}

//
// Copy constructor, from base class
//
TrkSTLayoutT::TrkSTLayoutT( const TrkSTLayout &other ) :
  _nsegs(0),_radius(0.0),
  _stends(0),_thickness(0),_materials(0)
{
        copyFromBase( &other );
}
// Copy constructor, from same class
//
TrkSTLayoutT::TrkSTLayoutT( const TrkSTLayoutT &other ) :
  _nsegs(0),_radius(0.0),
  _stends(0),_thickness(0),_materials(0)
{
        copyFromBase( &other );
}



//
// Assignment operator, from base class
//
const TrkSTLayoutT &TrkSTLayoutT::operator=( const TrkSTLayout &other )
{
  if (this == &other) return *this;
  copyFromBase( &other );
  return *this;
}


//
// Assignment operator, from same class
//
const TrkSTLayoutT &TrkSTLayoutT::operator=( const TrkSTLayoutT &other )
{
  if (this == &other) return *this;
  copyFromBase( &other );
  return *this;
}


void 
TrkSTLayoutT::findTag(const char* tagname, ifstream& datafile)
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


void TrkSTLayoutT::readLayout(const char* filename){
  ifstream datafile(filename);
  if (!datafile.good()) ErrMsg(fatal) << name() << ": unable to open file "
				      << filename << endmsg;
  findTag("TrkSTLayoutT",datafile);
  ErrMsg(routine) << name() << " reading file " << filename << endmsg;
  datafile >> _radius;
  datafile >> _nsegs;
// delete and remake arrays
  _thickness.clear();  _thickness.resize(_nsegs);
  _materials.clear(); _materials.resize(_nsegs);
  _stends.clear(); _stends.resize(_nsegs+1);
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
    datafile >> _stends[iseg];
  ostream& os = ErrMsg(routine);
  os << name() << endl;
  print(os);
  os << endmsg;
}

// Destructor

TrkSTLayoutT::~TrkSTLayoutT()
{
}


//
// CopyFromBase (protected)
//
// Allocate and copy all objects from a base class
//
void TrkSTLayoutT::copyFromBase( const TrkSTLayout * const other )
{
  _radius = other->radius();
  _nsegs = other->nSegments();
// delete and remake arrays
  _thickness.clear();  _thickness.resize(_nsegs);
  _materials.clear(); _materials.resize(_nsegs);
  _stends.clear(); _stends.resize(_nsegs+1);
  for(unsigned iseg=0;iseg<_nsegs;iseg++){
    _thickness[iseg] = other->thickness(iseg);
    _materials[iseg] = other->getSTMaterial(iseg);
    _stends[iseg] = other->zSTEnds(iseg,TrkSTLayout::lowZEnd);
  }
  _stends[_nsegs] = other->zSTEnds(_nsegs-1,TrkSTLayout::highZEnd);
}

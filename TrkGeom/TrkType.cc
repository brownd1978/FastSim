// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkType.cc 105 2010-01-15 12:14:11Z stroili $
//
//  Description:
//  DetType for describing passive tubes in the tracking volume (beampipe,
//  shields, support tube, etc.).  This also used as a crude model of the
//  DCH as cylindrical cylinders of compresssed gas.
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "TrkGeom/TrkType.hh"
#include "TrkGeom/TrkBPLayout.hh"
#include "TrkGeom/TrkSTLayout.hh"
#include "TrkGeom/TrkSvtSLayout.hh"
#include "DetectorModel/DetMaterial.hh"
#include <assert.h>
#include <stdio.h>
#include <fstream>
using std::endl;
using std::fstream;
using std::ostream;

#define nvertices 100
//
//  Construct from a 2-d perimeter array.  The array must descStrute points IN ORDER
//  clockwise around the Strut (the starting point is irrelevant).
//
TrkType::TrkType(unsigned idnum,const TrkBPLayout& layout) :
  DetSurfaceType("Beampipe",idnum),
  _nsegs(layout.nSegments())
{
  _materials.reserve(layout.nSegments());
  _thickness.reserve(layout.nSegments());
  _zends.reserve(layout.nSegments()+1);
// fill the arrays from the layout data
  for(unsigned iseg=0;iseg<_nsegs;iseg++){
    _thickness.push_back(layout.thickness(iseg));
    const DetMaterial* mat = gblEnv->getGen()->findDetMaterial(layout.getBPMaterial(iseg));
    ostream& os = ErrMsg(routine);
    os << name() << endl;
    os  << " Section " << iseg << " of the beam pipe is made of "
	<< layout.getBPMaterial(iseg) << endl;	
    mat->printAll(os);
    _materials.push_back(gblEnv->getGen()->findDetMaterial(layout.getBPMaterial(iseg)));
    _zends.push_back(layout.zBPEnds(iseg,TrkBPLayout::lowZEnd));
    os << "  " << "from z = " << layout.zBPEnds(iseg,TrkBPLayout::lowZEnd)
       << endl << endmsg;
  }
  _zends.push_back(layout.zBPEnds(_nsegs-1,TrkBPLayout::highZEnd));
//
//  Define the outline
//
  _nperimeter = 2*nvertices + 2;
  double phistep = 2*M_PI/nvertices;
  TwoDCoord* point;
  for(int iend=0;iend<2;iend++){
    double phi(0);
    for(int ipoint=0;ipoint<nvertices+1;ipoint++){
      point = new TwoDCoord(phi,iend==0?_zends.front() :_zends.back());
      myPointStore()->push_back(point);
      myOutline()->push_back(point);
      phi += phistep;
    }
  }
// close the figure
  myOutline()->push_back(myPointStore()->front());
}
// Overload the creater, so that also the support tube can
// be created...
TrkType::TrkType(unsigned idnum,const TrkSTLayout& layout) :
  DetSurfaceType("Support Tube",idnum),
  _nsegs(layout.nSegments())
{
  _materials.reserve(layout.nSegments());
  _thickness.reserve(layout.nSegments());
  _zends.reserve(layout.nSegments()+1);
// fill the arrays from the layout data
  for(unsigned iseg=0;iseg<_nsegs;iseg++){
    _thickness.push_back(layout.thickness(iseg));
    const DetMaterial* mat = gblEnv->getGen()->findDetMaterial(layout.getSTMaterial(iseg));
    ostream& os = ErrMsg(routine);
    os << name() << endl;
    os << " Section " << iseg << " of the support tube is made of "
       << layout.getSTMaterial(iseg) << endl;	
    mat->printAll(os);
    _materials.push_back(gblEnv->getGen()->findDetMaterial(layout.getSTMaterial(iseg)));
    _zends.push_back(layout.zSTEnds(iseg,TrkSTLayout::lowZEnd));
    os << "  " << "from z = " << layout.zSTEnds(iseg,TrkSTLayout::lowZEnd)
		    << endl << endmsg;
  }
  _zends.push_back(layout.zSTEnds(_nsegs-1,TrkSTLayout::highZEnd));
//
//  Define the outline
//
  _nperimeter = 2*nvertices + 2;
  double phistep = 2*M_PI/nvertices;
  TwoDCoord* point;
  for(int iend=0;iend<2;iend++){
    double phi(0);
    for(int ipoint=0;ipoint<nvertices+1;ipoint++){
      point = new TwoDCoord(phi,iend==0?_zends.front() :_zends.back());
      myPointStore()->push_back(point);
      myOutline()->push_back(point);
      phi += phistep;
    }
  }
// close the figure
  myOutline()->push_back(myPointStore()->front());
}
//
// second overload, for the svt shield
//
TrkType::TrkType(unsigned idnum,const TrkSvtSLayout& layout,
		 unsigned ielem) :
  DetSurfaceType("Svt Shield",idnum),
  _nsegs(layout.nSegments())
{
  _materials.reserve(layout.nSegments());
  _thickness.reserve(layout.nSegments());
  _zends.reserve(layout.nSegments()+1);
// fill the arrays from the layout data
  for(unsigned iseg=0;iseg<_nsegs;iseg++){
    _thickness.push_back(layout.thickness(ielem,iseg));
    const DetMaterial* mat = gblEnv->getGen()->findDetMaterial(layout.getSvtSMaterial(ielem,iseg));
    ostream& os = ErrMsg(routine);
    os << name() << endl;
    os << " Section " << iseg << " of the Svt shield is made of "
       << layout.getSvtSMaterial(ielem,iseg) << endl;
    mat->printAll(os);
    _materials.push_back(gblEnv->getGen()->findDetMaterial(layout.getSvtSMaterial(ielem,iseg)));
    _zends.push_back(layout.zSvtSZEnds(ielem,iseg,TrkSvtSLayout::lowZEnd));
    os << " shield " << "from z = " << layout.zSvtSZEnds(ielem,iseg,TrkSvtSLayout::lowZEnd)
		    << endl << endmsg;
  }
  _zends.push_back(layout.zSvtSZEnds(ielem,_nsegs-1,TrkSvtSLayout::highZEnd));
//
//  Define the outline
//
  _nperimeter = 2*nvertices + 2;
  double phistep = 2*M_PI/nvertices;
  TwoDCoord* point;
  for(int iend=0;iend<2;iend++){
    double phi(0);
    for(int ipoint=0;ipoint<nvertices+1;ipoint++){
      point = new TwoDCoord(phi,iend==0?_zends.front() :_zends.back());
      myPointStore()->push_back(point);
      myOutline()->push_back(point);
      phi += phistep;
    }
  }
// close the figure
  myOutline()->push_back(myPointStore()->front());
}
//  Destructor
//  Destructor
TrkType::~TrkType(){;}
//
//
bool
TrkType::physicalMaterial(const TypeCoord* coord) const
{
//
//  Check if the point is on the cylinder
//
  double zval = (*coord)[1];
  return zval >= _zends.front() && zval <= _zends.back();
}

const
DetMaterial&
TrkType::material(const TypeCoord* coord) const
{
  double zval = (*coord)[1];
  int iseg = segment(zval);
  assert(iseg >= 0);
  return *_materials[iseg];
}

double
TrkType::thickness(const TwoDCoord* coord) const
{
  double zval = (*coord)[1];
  int iseg = segment(zval);
  assert(iseg >= 0);
  return _thickness[iseg];
}

int
TrkType::segment(double zval) const
{
  int retval(-1);
// Z should be within range
  if(zval >= _zends.front() && zval <= _zends.back()){
    for(unsigned iseg=0;iseg<_nsegs;iseg++){
      if(zval>=_zends[iseg] && zval<= _zends[iseg+1]){
	retval = iseg;
	break;
      }
    }
// logic error
    if(retval<0){
      ErrMsg(error) << " Unable to find Segment for Z = " << zval
		    << " in type " << typeName() << endmsg;
      retval = (_zends.size()-2)/2;
    }
  } else {
// out-of-range.  This can happen in 'normal' circumstances.
    ErrMsg(routine) << "Z value " << zval
		    << " in type " << typeName()
		    << " is outside limits "
		    << _zends.front() << " to "
		    << _zends.back() << " , truncating" << endmsg;
    if(zval<_zends.front())
      retval = 0;
    else
      retval = _zends.size()-2;
  }
  return retval;
}

// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkPassive.cc 124 2010-05-11 10:41:40Z stroili $
//
//  Description:
//  Functions for building up the beampipe, support tube, and the
//  Svt inner and outer shield material models.  This defines
//  the master object which owns the full detector description heirarchy for
//  the these structures.  The single (default) constructor for this class creates the object,
//  and builds the whole geometry/material tree.   
//  It also forces a rebuild of the whole tree any time that information changes.
//  
// Copyright Information:
//	Copyright (C) 2001	Colorado State University
//
//  Authors: Johan Blouw, May-2001
//           (Copy from SvtDetector class)
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include <assert.h>
#include "BbrStdUtils/BbrCollectionUtils.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Geometry/Transformation.h"
#include "CLHEP/Utilities/CLHEP.h"
#include "DetectorModel/DetCylinder.hh"
#include "TrkGeom/TrkType.hh"
#include "TrkGeom/TrkSimpVolume.hh"
#include "TrkGeom/TrkPassive.hh"
#include "TrkGeom/TrkPassiveTubeType.hh"
#include "TrkGeom/TrkBPLayout.hh"
#include "TrkGeom/TrkSTLayout.hh"
#include "TrkGeom/TrkSvtSLayout.hh"
using std::endl;
using std::ostream;
#define BUF 0.1 // 1mm buffer inside materials to define volumes
//
//  The constructor, from a TrkPassive 'database' object (and contained sub-objects).
//  This builds only the nominal geometry.
//
TrkPassive::TrkPassive(const TrkBPLayout& layout, 
		       const TrkSTLayout& stlayout,
		       const TrkSvtSLayout& svtslayout ) : _trkset("Trk-Passive",0)
{
//
// make a BeamPipe type object
//
  DetSurfaceType* BPtype = new TrkType(1,layout);
// create an element from it  
  HepTransformation NoTrans;
// create surface type of beampipe
  const DetCylinder bpsurf(NoTrans, layout.radius());
// create new element.
  DetElem* BPelem = new DetSurfaceElem(BPtype,"Beampipe",101,bpsurf);
  _trkset+=(*BPelem);
// add beampipe to tracking volume(?)
  _types.push_back(BPtype);
//
// And now the Support Tube:
//
// Create the support tube object.
  DetSurfaceType* STtype = new TrkType(1,stlayout);
// create surface type of support tube
  const DetCylinder stsurf(NoTrans, stlayout.radius()); 
// create new element.
  DetElem* STelem = new DetSurfaceElem(STtype,"Support Tube",102,stsurf);
  _trkset+=(*STelem);
// add support tube to tracking volume(?)
  _types.push_back(STtype);
//
// And finally the Svt Shields:
  unsigned nelems = svtslayout.nElements();
  static const char* names[2] = {"Svt Shield","Svt Bag"};
  for(unsigned ielem=0;ielem<nelems;ielem++){
//
// make the svt outer shield type
    DetSurfaceType* svttype = new TrkType(ielem,svtslayout,ielem);
// create surface type
    const DetCylinder svtsurf(NoTrans,svtslayout.radii(ielem));
// create new element
    DetElem* svtelem = new DetSurfaceElem(svttype,names[ielem],103+ielem,svtsurf);
    _trkset += (*svtelem);
    _types.push_back(svttype);
  }
//
//  Build the volume
// Note: layout.Radius() is the inner radius of the structure.
  _trkBPVolume.reset( new TrkCentralVolume("Beampipe Volume",layout.radius()-BUF,
                                                layout.zBPEnds(0,TrkBPLayout::lowZEnd),
                                                layout.zBPEnds(layout.nSegments()-1,TrkBPLayout::highZEnd)) );
  _trkSTVolume.reset( new TrkCentralVolume("Support Tube Volume",stlayout.radius()-BUF,
                                                stlayout.zSTEnds(0,TrkSTLayout::lowZEnd),
                                                stlayout.zSTEnds(stlayout.nSegments()-1,TrkSTLayout::highZEnd)));
//  Lock the sets
//
  _trkset.setLock();
}

//  Basic printout
//surface
void
TrkPassive::print(ostream& os) const {
  os << " TrkPassive contains the following elements and subsets: " << endl;
  _trkset.print(os);
}
//
void
TrkPassive::printAll(ostream& os) const {
  os << " TrkPassive contains the following elements and subsets: " << endl;
  _trkset.printAll(os);
}

//
//  Destructor; delete all the elements and sets
//
TrkPassive::~TrkPassive()
{
//  First delete all elements
  DetElemList elemlist;
  _trkset.select(elemlist,dsMatchAll,0,0,0);
  std::for_each(elemlist.begin(),elemlist.end(),babar::Collection::DeleteObject());

//  Remove the types
  std::for_each(_types.begin(),_types.end(),babar::Collection::DeleteObject());
  _types.clear();
}


//----------------------------------------
// Temporary solution to the fact that the support
// structure is not yet properly coded (as is the beampipe)
//----------------------------------------

void 
TrkPassive::createST() {
//
//  Build types and elements for the support tubes, and add them to the set
//
    HepTransformation notrans;
//
// Support tube.  This is shortened as we only care about its tracking effect
// new support tube, based on geant model.  Only the Carbon fiber section is
// modeled DNB 03/01/00
  double supportends[2] = {-44.418,77.616};
  const DetMaterial* carbonfiber = gblEnv->getGen()->
    findDetMaterial("dch-CFiber");
// DNB 7/20/00 updated thickness to W. Dunwoodie numbers 
  DetSurfaceType* sttype = new TrkPassiveTubeType(0.2083,supportends,carbonfiber,2);
  _types.push_back(sttype);
  DetCylinder stsurf(notrans,21.736);
  DetElem* stelem = new DetSurfaceElem(sttype,"Support Tube",102,stsurf);
  _trkset += *stelem;
}

void 
TrkPassive::createSvtShield() {
// this creates both the inner and outer shields
  HepTransformation notrans;
// outer shield
  double outerends[2] = {-44.418,77.616};
  const DetMaterial* bag = gblEnv->getGen()->
    findDetMaterial("svt-Bag");
  DetSurfaceType* outertype = new TrkPassiveTubeType(0.0102,outerends,bag,3);
  _types.push_back(outertype);
  DetCylinder outershield(notrans,20.0);
  DetElem* outer = new DetSurfaceElem(outertype,"Svt Gas Bag",103,outershield);
  _trkset += *outer;

  double innerends[2] = {-13.47,15.73};
  const DetMaterial* shield = gblEnv->getGen()->
    findDetMaterial("svt-Shield");
  DetSurfaceType* innertype = new TrkPassiveTubeType(0.0125,innerends,shield,4);
  _types.push_back(innertype);
  DetCylinder innershield(notrans,3.2);
  DetElem* inner = new DetSurfaceElem(innertype,"Svt Inner Shield",104,innershield);
  _trkset += *inner;
}

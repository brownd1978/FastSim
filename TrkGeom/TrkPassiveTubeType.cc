// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkPassiveTubeType.cc 105 2010-01-15 12:14:11Z stroili $
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
#include "ErrLogger/ErrLog.hh"
#include "TrkGeom/TrkPassiveTubeType.hh"
#include "DetectorModel/DetMaterial.hh"
#define nvertices 100
//
//  Construct from a 2-d perimeter array.  The array must descStrute points IN ORDER
//  clockwise around the Strut (the starting point is irrelevant).
//
TrkPassiveTubeType::TrkPassiveTubeType(double thick,double zend[2],
				       const DetMaterial* themat,int idnum) :
  DetSurfaceType("Trk Passive Tube",idnum),  _thick(thick),
  _thematerial(themat)
{
  _zend[0] = zend[0];
  _zend[1] = zend[1];
//
//  Define the outline
//
  _nperimeter = 2*nvertices + 2;
  double phistep = 2*M_PI/nvertices;
  TwoDCoord* point;
  for(int iend=0;iend<2;iend++){
    double phi(0);
    for(int ipoint=0;ipoint<nvertices+1;ipoint++){
      point = new TwoDCoord(phi,_zend[iend]);
      myPointStore()->push_back(point);
      myOutline()->push_back(point);
      phi += phistep;
    }
  }
// close the figure
  myOutline()->push_back(myPointStore()->front());
}
//  Destructor
TrkPassiveTubeType::~TrkPassiveTubeType(){;}
//
//
bool
TrkPassiveTubeType::physicalMaterial(const TypeCoord* thispoint) const {
//
//  Check if the point is on the strut
//
  double zval = (*thispoint)[1];
  return zval >= _zend[0] && zval <= _zend[1]; 
}

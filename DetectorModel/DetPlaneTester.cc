//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DetPlaneTester.cc,v 1.2 2004/08/06 05:58:31 bartoldu Exp $
//
// Description:
//	Class DetPlaneTester, patterned after DetConeTester
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Dave Brown, 12/29/98  Original author
//
// Copyright Information:
//	Copyright (C) 1998 LBL
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//---------------
// C++ Headers --
//---------------

#include <iostream>
#include <math.h>
//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "DetectorModel/DetPlane.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Transformation.h"
using std::cout;
using std::endl;

int main(int argc, char* argv[]){
  HepTransformation null;
  DetPlane theplane(null);
// find the normal
  HepPoint expoint(1.0,1.0,17.0);
  Hep3Vector normal;
  SurfacePoint spoint;
  double dist = theplane.normalTo(expoint,normal,spoint);
  cout << "Exterior normal distance = " << dist
       << " direction = " << normal
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
// direction
  Hep3Vector para(-1.0,1.0,0.0);
  DetSurface::intertype itype =
    theplane.distanceTo(expoint,para,dist,spoint);
  cout << "Exterior parallel intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
  Hep3Vector radial(1.0,1.0,1.0);
  itype = theplane.distanceTo(expoint,radial,dist,spoint);
  cout << "Exterior radial intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
// interior point
  HepPoint inpoint(-1.0,-1.0,-11.0);
  dist = theplane.normalTo(inpoint,normal,spoint);
  cout << "Interior normal distance = " << dist
       << " direction = " << normal
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
// direction
  itype = theplane.distanceTo(inpoint,para,dist,spoint);
  cout << "Interior parallel intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
  itype = theplane.distanceTo(inpoint,radial,dist,spoint);
  cout << "Interior radial intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
  return 0;
}

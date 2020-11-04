//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DetCylinderTester.cc,v 1.5 2004/08/06 05:58:29 bartoldu Exp $
//
// Description:
//	Class DetCylinderTester, patterned after DetConeTester
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

#include "DetectorModel/DetCylinder.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Transformation.h"
using std::cout;
using std::endl;
int main(int argc, char* argv[]){
  HepTransformation dummyTrans;
  DetCylinder mycyl(dummyTrans,0.5);
// find the normal
  HepPoint expoint(1.0,1.0,17.0);
  Hep3Vector normal;
  SurfacePoint spoint;
  double dist = mycyl.normalTo(expoint,normal,spoint);
  cout << "Exterior normal distance = " << dist
       << " direction = " << normal
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
// direction
  Hep3Vector para(-1.0,1.0,20.0);
  DetSurface::intertype itype =
    mycyl.distanceTo(expoint,para,dist,spoint);
  cout << "Exterior parallel intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
  Hep3Vector radial(1.0,1.0,-0.5);
  itype = mycyl.distanceTo(expoint,radial,dist,spoint);
  cout << "Exterior radial intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
// interior point
  HepPoint inpoint(0.25,0.25,-11.0);
  dist = mycyl.normalTo(inpoint,normal,spoint);
  cout << "Interior normal distance = " << dist
       << " direction = " << normal
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
// direction
  para = Hep3Vector(0.0,0.0,20.0);
  itype = mycyl.distanceTo(inpoint,para,dist,spoint);
  cout << "Interior parallel intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
  itype = mycyl.distanceTo(inpoint,radial,dist,spoint);
  cout << "Interior radial intersection retval = " << itype
       << " distance = " << dist
       << " surface point = " << spoint[0] << ","
       << spoint[1] << endl;
  return 0;
}




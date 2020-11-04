//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DetConeTester.cc,v 1.6 2004/08/06 05:58:29 bartoldu Exp $
//
// Description:
//	Class DetConeTester
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	<Author1>		<originator/contributor etc.>
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1996	<Institution>
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

#include <iostream>
#include <math.h>
#include <string.h>
#include <assert.h>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "DetectorModel/DetCone.hh"
#include "DetectorModel/DetSCone.hh"
#include "BaBar/Constants.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Geometry/Transformation.h"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

enum UseWhat {
  useDetCone, useDetSCone
};

inline static void
printMyCone(UseWhat type, const DetSurface& cone)
{
  const DetSurface *coneptr = &cone;

  switch (type) {
  case useDetCone:
    cout << "tan theta = " << ((DetCone*) coneptr)->tanTheta()
        << ", vertex z value =" << ((DetCone*) coneptr)->vertex();
    break;
  case useDetSCone:
    cout << "tan alpha = " << ((DetSCone*) coneptr)->tanAlpha()
        << ", z0 value = " << ((DetSCone*) coneptr)->z0();
    break;
  default:
    assert(0);
  }
}

inline static const char*
myName(UseWhat type)
{

  switch (type) {
  case useDetCone:
    return "DetCone";
  case useDetSCone:
    return "DetSCone";
  default:
    assert(0);
    return "UnknownCone";
  }
}

inline static DetSurface*
newCone(UseWhat type, double tanA, double z0)
{
  switch (type) {
  case useDetCone:
  default:
    return new DetCone(HepTransformation(), tanA, z0);
  case useDetSCone:
    if (z0 == 0.0) {
      // use of z0==0 is not allowed for DetSCone, use transformation instead
      return new DetSCone(HepTransformation(Hep3Vector(0, 0, -10.0),
          Hep3Vector(0, 0, 1), // z axis
          Hep3Vector(1, 0, 0)), // x axis
          tanA, 10.0);
    } else {
      return new DetSCone(HepTransformation(), tanA, z0);
    }
  }
}

inline static DetSurface*
newCone(UseWhat type, double z1, double z2, double r1, double r2)
{
  switch (type) {
  case useDetCone:
  default:
    return new DetCone(HepTransformation(), z1, z2, r1, r2);
  case useDetSCone:
    if (z1 * (r2 - r1) - r1 * (z2 - z1) == 0.0) {
      // use of z0==0 is not allowed for DetSCone, use transformation instead
      return new DetSCone(HepTransformation(Hep3Vector(0, 0, -10.0),
          Hep3Vector(0, 0, 1), // z axis
          Hep3Vector(1, 0, 0)), // x axis
          z1 + 10.0, z2 + 10.0, r1, r2);
    } else {
      return new DetSCone(HepTransformation(), z1, z2, r1, r2);
    }
  }
}

int
main(int argc, char* argv[])
{
  // Main???? What kind of heresy is this????

  UseWhat whatCone =
      (argc > 1 && ::strcmp(argv[1], "scone") == 0) ? useDetSCone : useDetCone;

  // Very simple cone whose axis is the BaBar z axis, whose half angle is 
  // 45 degrees and whose vertex is the origin.

  DetSurface& myFirstCone(*newCone(whatCone, 1.0, 0.0));

  cout << myName(whatCone) << " declared with ";
  printMyCone(whatCone, myFirstCone);
  cout << endl;

  // Set up a given surface point for use.
  double piBy4 = Constants::pi / 4.0;

  //*****************************************************
  // First check the basic "surface defining" functions
  //*****************************************************

  struct TestSurfPoint {
    const char *description;
    double phi, r;
  };

  TestSurfPoint listSurfPointTests[] = {
      { "--- Surface point r>0, phi<pi", piBy4, 1.0 },
      { "--- Surface point r<0, phi<pi", piBy4, -1.0 },
      { "--- Surface point r>0, phi>pi", 5.0 * piBy4, 1.0 },
      { "--- Surface point r<0, phi>pi", 5.0 * piBy4, -1.0 } };

  int i;

  for (i = 0; i < sizeof(listSurfPointTests) / sizeof(TestSurfPoint); i++) {

    TestSurfPoint& test = listSurfPointTests[i];

    switch (whatCone) {
    case useDetSCone:
      if (test.r < 0) continue; // negative r are not allowed for DetSCone
    case useDetCone:
      ; // nothing specific
    }

    cout << endl << test.description << endl;
    SurfacePoint aSurfacePoint(test.phi, test.r);

    // Check DetCone::normal() function
    cout << "Normal to the surface point with coords " << aSurfacePoint[0]
        << ", " << aSurfacePoint[1] << ": " << endl;

    Hep3Vector theNormal = myFirstCone.normal(aSurfacePoint);
    cout << "(" << theNormal.x() << "," << theNormal.y() << ","
        << theNormal.z() << ")" << endl;

    // Check DetCone::surfaceDirection function
    // First in phi direction
    cout << "Phi surface direction at the surface point with coords "
        << aSurfacePoint[0] << ", " << aSurfacePoint[1] << ": " << endl;

    Hep3Vector thePhiSurfaceDirection = myFirstCone.surfaceDirection(
        aSurfacePoint, 0);
    cout << "(" << thePhiSurfaceDirection.x() << ","
        << thePhiSurfaceDirection.y() << "," << thePhiSurfaceDirection.z()
        << ")" << endl;
    // Then in R direction
    cout << "R surface direction at the surface point with coords "
        << aSurfacePoint[0] << ", " << aSurfacePoint[1] << ": " << endl;
    Hep3Vector theRSurfaceDirection = myFirstCone.surfaceDirection(
        aSurfacePoint, 1);
    cout << "(" << theRSurfaceDirection.x() << "," << theRSurfaceDirection.y()
        << "," << theRSurfaceDirection.z() << ")" << endl;

    // Check than we can figure out where a surface point is.
    cout << "Location of surface point with coords " << aSurfacePoint[0]
        << ", " << aSurfacePoint[1] << ": " << endl;
    HepPoint theLocation = myFirstCone.spacePoint(aSurfacePoint);
    cout << "(" << theLocation.x() << "," << theLocation.y() << ","
        << theLocation.z() << ")" << endl;

    // And more importantly where a space point is.
    SurfacePoint returnedPoint;
    int returnCode = myFirstCone.surfacePoint(theLocation, returnedPoint);
    cout << "Surface point relating to location ";
    cout << "(" << theLocation.x() << "," << theLocation.y() << ","
        << theLocation.z() << ")" << endl;
    cout << " is " << returnedPoint[0] << "," << returnedPoint[1] << endl;
    cout << "Return code was " << returnCode << endl;
  }

  //**************************************************************
  // Now check some of the more difficult distance type functions
  //**************************************************************


  cout << endl << "***** Checks for distance relations *****" << endl;

  const Hep3Vector theDirection(1.0, 0.0, -1.0);

  struct TestOnePoint {
    const char *description;
    double x, y, z;
  };

  TestOnePoint listOnePointTests[] = { { "--- Point inside the cone, x>0, z>0",
      1.0, 0.0, 3.0 },
      { "--- Point inside the cone, x>0, z<0", 1.0, 0.0, -3.0 }, {
          "--- Point inside the cone, x<0, z>0", -1.0, 0.0, 3.0 }, {
          "--- Point inside the cone, x<0, z<0", -1.0, 0.0, -3.0 }, {
          "--- Point outside the cone, x>0, z>0", 3.0, 0.0, 1.0 }, {
          "--- Point outside the cone, x>0, z<0", 3.0, 0.0, -1.0 }, {
          "--- Point outside the cone, x<0, z>0", -3.0, 0.0, 1.0 }, {
          "--- Point outside the cone, x<0, z<0", -3.0, 0.0, -1.0 }, {
          "--- Point on the axis, z>0", 0.0, 0.0, 3.0 }, {
          "--- Point on the axis, z<0", 0.0, 0.0, -3.0 }
  // ,{ "--- Point at the vertex", 0.0, 0.0, 0.0 } // crashes (FPE) DetCone
      };

  for (i = 0; i < sizeof(listOnePointTests) / sizeof(TestOnePoint); i++) {

    TestOnePoint& test = listOnePointTests[i];

    cout << test.description << endl;

    HepPoint thePoint(test.x, test.y, test.z);

    SurfacePoint returnedPoint;
    Hep3Vector theReturnedNormal;
    double myDistance = myFirstCone.normalTo(thePoint, theReturnedNormal,
        returnedPoint);
    cout << "The distance of the point (" << thePoint.x() << ","
        << thePoint.y() << "," << thePoint.z() << ") from the cone is "
        << myDistance << endl;
    cout << "The normal to the surface at this distance is ("
        << theReturnedNormal.x() << "," << theReturnedNormal.y() << ","
        << theReturnedNormal.z() << ")" << endl;

    // Check DetCone::firstDeriv function 
    double firstDeriv = myFirstCone.firstDeriv(thePoint, theDirection);
    cout << "The first derivative of the distance from the point ("
        << thePoint.x() << "," << thePoint.y() << "," << thePoint.z()
        << ") from the cone in the direction of (" << theDirection.x() << ","
        << theDirection.y() << "," << theDirection.z() << ") is " << firstDeriv
        << endl;

    // Check distance to function
    int returnCode = myFirstCone.distanceTo(thePoint, theDirection, myDistance,
        returnedPoint);
    cout << "The distance from the point (" << thePoint.x() << ","
        << thePoint.y() << "," << thePoint.z()
        << ") from the cone in the direction of (" << theDirection.x() << ","
        << theDirection.y() << "," << theDirection.z() << ") is " << myDistance
        << endl << "The return code was " << returnCode
        << " and the  surface coords of intersection were ("
        << returnedPoint[0] << "," << returnedPoint[1] << ")." << endl << endl;
  }

  //******************************************
  // Check te closest approach function works
  //******************************************

  cout << endl << "***** Checks segmentMinMax *****" << endl;

  struct TestTwoPoints {
    const char *description;
    double x1, y1, z1;
    double x2, y2, z2;
  };

  TestTwoPoints listTwoPointsTests[] = { { "--- Points inside the cone, z>0",
      1.0, 0, 3.0, -1.0, 0, 3.0 }, { "--- Points inside the cone, z<0", 1.0, 0,
      -3.0, -1.0, 0, -3.0 }, { "--- Points inside the cone, z alters sign",
      1.0, 0, 3.0, 1.0, 0, -3.0 },
      { "--- Points outside the cone, z>0, intersect", 3.0, 0, 1.0, -3.0, 0,
          1.0 }, { "--- Points outside the cone, z>0, not intersect", 3.0, 0,
          1.0, 0, 3.0, 1.0 }, { "--- Points outside the cone, z<0", 3.0, 0,
          -1.0, -3.0, 0, -1.0 }, {
          "--- Points outside the cone, z alters sign", 3.0, 0, 1.0, 3.0, 0,
          -1.0 }, };

  for (i = 0; i < sizeof(listTwoPointsTests) / sizeof(TestTwoPoints); i++) {

    TestTwoPoints& test = listTwoPointsTests[i];

    cout << test.description << endl;
    HepPoint thePoint(test.x1, test.y1, test.z1);
    HepPoint anotherPoint(test.x2, test.y2, test.z2);

    double maxdist, mindist;
    myFirstCone.segmentMinMax(thePoint, anotherPoint, mindist, maxdist);
    cout << "The closest approach of the line connecting (" << thePoint.x()
        << "," << thePoint.y() << "," << thePoint.z() << ") and the point ("
        << anotherPoint.x() << "," << anotherPoint.y() << ","
        << anotherPoint.z() << ") is " << mindist
        << ". The maximum separation is " << maxdist << endl;
  }

  //*************************************************************
  // Check that the alternative way of declaring the cone works 
  //*************************************************************

  cout << endl << "***** Checks alternative constructor *****" << endl;

  struct TestAlter {
    double z1, z2;
    double r1, r2;
  };

  TestAlter listAlterTests[] = { { 5.0, 4.0, 3.0, 4.0 },
      { 5.0, 4.0, 4.0, 3.0 }, { -5.0, -4.0, 3.0, 4.0 },
      { -5.0, -4.0, 4.0, 3.0 } };

  for (i = 0; i < sizeof(listAlterTests) / sizeof(TestAlter); i++) {
    TestAlter& test = listAlterTests[i];

    DetSurface* anotherCone = newCone(whatCone, test.z1, test.z2, test.r1,
        test.r2);
    cout << "--- " << myName(whatCone) << " defined with" << " z1=" << test.z1
        << " z2=" << test.z2 << " r1=" << test.r1 << " r2=" << test.r2
        << " has ";
    printMyCone(whatCone, *anotherCone);
    cout << endl;

    delete anotherCone;
  }

  //*************************************************************
  // Check clone (copyOf)
  //*************************************************************

  cout << endl << "***** Checks copyOf cloning *****" << endl;

  double listCloneTests[] = { 0.0, 4.0, -4.0 };

  for (i = 0; i < sizeof(listCloneTests) / sizeof(double); i++) {
    double shift = listCloneTests[i];

    DetSurface* anotherCone = myFirstCone.copyOf(shift);

    cout << "--- " << myName(whatCone) << " shifted on " << shift << "cm"
        << " has ";
    printMyCone(whatCone, *anotherCone);
    cout << endl;

    delete anotherCone;
  }

  return 0;
}


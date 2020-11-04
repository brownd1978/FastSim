//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DetAlignElemTester.cc,v 1.4 2004/08/06 05:58:28 bartoldu Exp $
//
// Description:
//	Program DetAlignElemTest
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Dave Brown, 1/14/02  Original author
//
// Copyright Information:
//	Copyright (C) 2002 LBL
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//---------------
// C++ Headers --
//---------------

#include <iostream>
#include <iomanip>
//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "DetectorModel/DetAlignElem.hh"
#include "DetectorModel/DetAEAdder.hh"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Matrix/SymMatrix.h"
using std::cout;
using std::endl;
using std::setprecision;

int
main(int argc, char* argv[])
{

  HepSymMatrix cov(6, 1);
  cov(1, 1) = 1.0e-6;
  cov(2, 2) = 2.0e-6;
  cov(3, 3) = 3.0e-6;
  cov(4, 4) = 4.0e-6;
  cov(5, 5) = 5.0e-6;
  cov(6, 6) = 6.0e-6;
  HepVector vec(6);
  vec(1) = 0.001;
  vec(2) = 0.002;
  vec(3) = 0.003;
  vec(4) = 0.004;
  vec(5) = 0.005;
  vec(6) = 0.006;
  cout << "original vector and covariance" << setprecision(10) << vec << endl
      << cov << endl;
  DetAlignElem elem(vec, cov, "Test Alignment Element", 1);
  cout << "DetAlignElem vector and covariance " << setprecision(10)
      << elem.parameterVector() << endl << elem.parameterCovariance() << endl;
  // now, test combining alignments
  HepSymMatrix cov2(6, 1);
  cov2(1, 1) = 1.0e-7;
  cov2(2, 2) = 2.0e-7;
  cov2(3, 3) = 3.0e-7;
  cov2(4, 4) = 4.0e-7;
  cov2(5, 5) = 5.0e-7;
  cov2(6, 6) = 6.0e-7;
  HepVector vec2(6);
  vec2(1) = 0.0001;
  vec2(2) = 0.0002;
  vec2(3) = 0.0003;
  vec2(4) = 0.0004;
  vec2(5) = 0.0005;
  vec2(6) = 0.0006;
  cout << "second vector and covariance" << setprecision(10) << vec2 << endl
      << cov2 << endl;
  DetAlignElem elem2(vec2, cov2, "Test Alignment Element", 1);
  // combine
  DetAlignElem elem3;
  DetAEAdder adder;
  adder.combine(elem, elem2, elem3);
  cout << "Combined DetAlignElem vector and covariance " << setprecision(10)
      << elem3.parameterVector() << endl << elem3.parameterCovariance() << endl;
  return 0;
}

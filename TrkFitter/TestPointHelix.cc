//-----------------------------------------------------------------------------
//
// File and Version Information:
// 	$Id: TestPointHelix.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:  TestPointHelix: tests the parameter and covariance translation
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	David Brown  9/1/97
//
// Copyright Information:
//	Copyright (C) 1997	        Lawrence Berkeley Laboratory
//
//-----------------------------------------------------------------------------

//-------------
// C Headers --
//-------------
#include "BaBar/BaBar.hh"
#include <iostream>
#include <unistd.h>
#include <string>
#include "TrkBase/TrkDifPoca.hh"
#include "TrkBase/HelixTraj.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "difAlgebra/DifNumber.hh"
#include "difAlgebra/DifPoint.hh"
#include "difAlgebra/DifVector.hh"
using std::cin;
using std::cout;
using std::endl;
using std::flush;

int main( int argc, char* argv[] )
{
//
// get user info
//
  double d0val,phi0val,omegaval,z0val,tandval;
  double d0err,phi0err,omegaerr,z0err,tanderr;
  bool testtraj = true;
  std::string answer("Yes");
  std::string trajtypes[2];
  trajtypes[0] = "Unshifted ";
  trajtypes[1] = "Shifted   ";
  while(testtraj){
    cout << "Enter parameter vector (d0,phi0,omega,z0,tand): ";
    cout.flush();
    cin >> d0val >> phi0val >> omegaval >> z0val >> tandval;
    cout << "Enter parameter errors (d0,phi0,omega,z0,tand): ";
    cout.flush();
    cin >> d0err >> phi0err >> omegaerr >> z0err >> tanderr;
// build parameter vector/matrix
    HepVector parvec(_nhlxprm);
    parvec[_d0] = d0val;
    parvec[_phi0] = phi0val;
    parvec[_omega] = omegaval;
    parvec[_z0] = z0val;
    parvec[_tanDip] = tandval;
    HepMatrix parcov(_nhlxprm,_nhlxprm,0);
    parcov[_d0][_d0] = d0err*d0err;
    parcov[_phi0][_phi0] = phi0err*phi0err;
    parcov[_omega][_omega] = omegaerr*omegaerr;
    parcov[_z0][_z0] = z0err*z0err;
    parcov[_tanDip][_tanDip] = tanderr*tanderr;
// build origin point traj
    HelixTraj otraj(parvec,parcov);
    bool testpoint = true;
    while (testpoint){
      double pstart,pend;
      cout << "Enter start and end path length: ";
      cout.flush();
      cin >> pstart >> pend;
      HepPoint start = otraj.position(pstart);
      HepPoint end = otraj.position(pend);
      HepPoint newref(end);
// move the points slightly off the trajectory
      Hep3Vector sperp = otraj.delDirect(pstart).unit();
      Hep3Vector eperp = otraj.delDirect(pend).unit();
      Hep3Vector zaxis(0.0,0.0,1.0);
      start += sperp*0.0001;
      start += zaxis*0.0001;
      end += eperp*0.0001;
      end += zaxis*0.0001;
      cout << "Start point = " << start
	   << " end point   = " << end << endl;
// create a shifted trajectory
      HelixTraj shifttraj(otraj);
      shifttraj.changePoint(newref);
      HelixTraj* testtraj[2];
      testtraj[0] = &otraj;
      testtraj[1] = &shifttraj;
      double slen[2];
      slen[0] = pstart;
      slen[1] = pstart - pend;
      double elen[2];
      elen[0] = pend;
      elen[1] = 0;
      for(int itraj=0;itraj<2;itraj++){
//	cout << "Test trajectory ";
//	testtraj[itraj]->print();
// get position and error on position
	TrkDifPoca spoca(*testtraj[itraj],slen[itraj],start);
	double sdoca = spoca.doca();
	HepVector sderiv = spoca.derivs();
	HepMatrix sdocaerr = sderiv.T()*
	  testtraj[itraj]->parameters()->covariance()*
	  sderiv;
	double sderr = sqrt(sdocaerr[0][0]);
	cout << trajtypes[itraj]
	     << " has doca " 
	     << sdoca << " error " << sderr 
	     << " at fltlen " << spoca.flt1()
	     << endl;
//
	TrkDifPoca epoca(*testtraj[itraj],elen[itraj],end);
	double edoca = epoca.doca();
	HepVector ederiv = epoca.derivs();
	HepMatrix edocaerr = ederiv.T()*
	  testtraj[itraj]->parameters()->covariance()*
	  ederiv;
	double ederr = sqrt(edocaerr[0][0]);
	cout << trajtypes[itraj]
	     << " has doca "
	     << edoca << " error " << ederr 
	     << " at fltlen " << epoca.flt1()
	     << endl;
      }
//
      cout << "more points ? ";
      cout.flush();
      cin >> answer;
      answer.resize(1);
      testpoint = (answer=="y" || answer=="Y" );
      }
    cout << "more trajectories ? ";
    cout.flush();
    cin >> answer;
    answer.resize(1);
    testtraj = (answer=="y" || answer=="Y" );
  }
  return 0;
}

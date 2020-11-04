#include "BaBar/BaBar.hh"
#include "TrkBase/TrkDifPieceTraj.hh"
#include "TrkBase/HelixTraj.hh"
#include "TrkBase/TrkPoca.hh"
#include "TrkBase/TrkCompTrk.hh"
#include "BbrGeom/BbrPointErr.hh"
#include "BbrGeom/BbrVectorErr.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/Matrix.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <vector>
using std::cout;
using std::endl;

void usage(char*);

int main( int argc, char* argv[] ) {
//  BbrPointErr pointerr;
//  BbrVectorErr vecerr;
//  HepMatrix xpcov;
//  TrkCompTrk test(pointerr,vecerr,xpcov,1,1.0,1,0);
  if(argc==10){
    double d0,phi0,omega,z0,tandip,minflt1,maxflt1,minflt2,maxflt2;
    if(! ( sscanf(argv[1],"%lf",&d0) &&
	   sscanf(argv[2],"%lf",&phi0) &&
	   sscanf(argv[3],"%lf",&omega) &&
	   sscanf(argv[4],"%lf",&z0) &&
	   sscanf(argv[5],"%lf",&tandip) &&
	   sscanf(argv[6],"%lf",&minflt1) &&
	   sscanf(argv[7],"%lf",&maxflt1) &&
	   sscanf(argv[8],"%lf",&minflt2) &&
	   sscanf(argv[9],"%lf",&maxflt2)
	   ) ) {
      usage(argv[0]);
      return 0;
    } else {
// create a helix traj
      HepVector parvec(5);
      parvec[0] = d0;
      parvec[1] = phi0;
      parvec[2] = omega;
      parvec[3] = z0;
      parvec[4] = tandip;
      HepSymMatrix error(5,1);
      HelixTraj*  helix1 = new HelixTraj(parvec,error,minflt1,maxflt1);
// tweak the parameters a bit to distinguish the 2 trajs
      parvec[0] += 0.001;
      parvec[1] += 0.0001;
      parvec[2] += 0.00000001;
      parvec[3] += 0.001;
      parvec[4] += 0.0001;
      HelixTraj*  helix2 = new HelixTraj(parvec,error,minflt2,maxflt2);
      cout << "First ";
      helix1->printAll(cout);
      cout << "Second ";
      helix2->printAll(cout);
// create the piecetraj from these
      std::vector<TrkSimpTraj*> trajlist;
      trajlist.push_back(helix1);
      trajlist.push_back(helix2);
      TrkDifPieceTraj ptraj(trajlist);
      cout << "Piecewise trajectory ";
      ptraj.printAll(cout);
// access piece traj at a few places
      double localx;
      double x[6];
      x[0] = minflt1-1.0;
      x[1] = (minflt1+maxflt1)/2.0;
      x[2] = (maxflt1+minflt2)/2.0-0.01;
      x[3] = (maxflt1+minflt2)/2.0+0.01;
      x[4] = (minflt2+maxflt2)/2.0;
      x[5] = maxflt2+1.0;
      unsigned ipiece;
      for(ipiece=0;ipiece<6;ipiece++){
	const TrkSimpTraj* piece = ptraj.localTrajectory(x[ipiece],localx);
	cout << "fltlen " << x[ipiece] << " gave local trajectory ";
	piece->printAll(cout);
	cout << " and local flight length " <<
	  localx << " which is ";
	if(!ptraj.locallyValid(x[ipiece]))
	   cout << "NOT ";
	cout << "valid" << endl;
      }
// try poca at a few places
      for(ipiece=0;ipiece<6;ipiece++){
	HepPoint point = ptraj.position(x[ipiece]) + Hep3Vector(0.01,0.01,0.01);
	TrkPoca poca(ptraj,x[ipiece],point);
	cout << "point poca to " << point;
	if(poca.status().success()){
	  cout << " succeeded, flight length = " << poca.flt1() 
	       << " , doca = " << poca.doca()  << endl;
	} else
	  cout << " FAILED" << endl;
      }
    }
  } else
    usage(argv[0]);
  return 0;
}


void
usage(char* name) {
  cout << "Usage: " << name << " d0 phi0 omega z0 tandip minflt1 maxflt1 minflt2 maxflt2" << endl;
}






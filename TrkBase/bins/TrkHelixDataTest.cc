#include "BaBar/BaBar.hh"
#include "TrkBase/TrkHelixData_001.hh"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

void usage(char*);

int main( int argc, char* argv[] ) {
  if(argc==9){
    double d0,phi0,omega,z0,tandip,err,minflt,maxflt;
    if(! ( sscanf(argv[1],"%lf",&d0) &&
	   sscanf(argv[2],"%lf",&phi0) &&
	   sscanf(argv[3],"%lf",&omega) &&
	   sscanf(argv[4],"%lf",&z0) &&
	   sscanf(argv[5],"%lf",&tandip) &&
	   sscanf(argv[6],"%lf",&err) &&
	   sscanf(argv[7],"%lf",&minflt) &&
	   sscanf(argv[8],"%lf",&maxflt) 
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
      error = error*err;
// set some off-diagonal terms
      for(unsigned ip=1;ip<=5;ip++)
	for(unsigned jp=ip+1;jp<=5;jp++)
	  error.fast(jp,ip) = err/float(ip*jp);
      HelixTraj helix(parvec,error,minflt,maxflt);
      cout << "HelixTraj with range " 
	   << helix.lowRange() <<" to " << helix.hiRange() << " and parameters "
	   << endl << helix.d0() << " " 
	   <<  helix.phi0() << " " 
	   << helix.omega() << " "
	   << helix.z0() << " "
	   << helix.tanDip() << endl;
      cout <<" and error matrix " << helix.parameters()->covariance() << endl;
// create a helixData from that
      TrkHelixData_001 helixd(&helix);
// reverse the process
      HelixTraj* newhelix = helixd.helix();
      cout << "Unpacked HelixTraj with range " 
	   << newhelix->lowRange() << " to " <<
	newhelix->hiRange() << " and parameters "
	   << endl << newhelix->d0() << " "
	   << newhelix->phi0() << " "
	   << newhelix->omega() << " "
	   << newhelix->z0() << " "
	   << newhelix->tanDip() << endl;
      cout <<" error matrix " << newhelix->parameters()->covariance() << endl;
// re-pack the unpacked helix, and compare the packed data
      TrkHelixData_001 helixd2(newhelix);
      for(unsigned iparm=0;iparm<HelixTraj::NHLXPRM;iparm++){
	HelixTraj::ParIndex parm = (HelixTraj::ParIndex)iparm;
	if(helixd.parameters(parm) ==
	   helixd2.parameters(parm))
	  cout << "Packing of parameter " << parm << " is reversible" << endl;
	else
	  cout << "Packing of parameter " << parm << " is _not_ reversible" 
	       << endl << "old= " << helixd.parameters(parm) 
	       << endl << "new= " << helixd2.parameters(parm) << endl;
      }
      for(unsigned icor=0;icor<3;icor++){
	if(helixd.correlations(icor) == helixd2.correlations(icor))
	  cout << "Packing of correlation " << icor << " is reversible " << endl;
	else
	  cout << "Packing of correlation " << icor << " is _not_ reversible " << endl
	       << "old= " << helixd.correlations(icor) << endl
	       << "new= " << helixd2.correlations(icor) << endl;
      }
      if(helixd.flightLength() == helixd2.flightLength())
	cout << "Packing of flightlength is reversible " << endl;
      else
	cout << "Packing of flightlength is _not_ reversible " << endl
	     << "old= " << helixd.flightLength() << endl
	     << "new= " << helixd2.flightLength() << endl;
      delete newhelix;
    }
  } else{  
    usage(argv[0]);
    return 0;
  }
  return 1;
}

void
usage(char* name) {
  cout << "Usage: " << name << " d0 phi0 omega z0 tandip diagerror" << endl
       << " minflt maxflt" << endl;
}






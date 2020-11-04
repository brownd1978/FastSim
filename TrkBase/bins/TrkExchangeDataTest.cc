#include "BaBar/BaBar.hh"
#include "TrkBase/TrkExchangeData.hh"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

void usage(char*);

int main( int argc, char* argv[] ) {
  if(argc==6){
    double d0,phi0,omega,z0,tandip;
    if(! ( sscanf(argv[1],"%lf",&d0) &&
	   sscanf(argv[2],"%lf",&phi0) &&
	   sscanf(argv[3],"%lf",&omega) &&
	   sscanf(argv[4],"%lf",&z0) &&
	   sscanf(argv[5],"%lf",&tandip) ) ) {
      usage(argv[0]);
      return 0;
    } else {
// create an exhange par
      HepVector parvec(5);
      parvec[0] = d0;
      parvec[1] = phi0;
      parvec[2] = omega;
      parvec[3] = z0;
      parvec[4] = tandip;
      TrkExchangePar helix(parvec);
      cout << "TrkExchangePar parameters "
	   << endl << helix.d0() << " " 
	   <<  helix.phi0() << " " 
	   << helix.omega() << " "
	   << helix.z0() << " "
	   << helix.tanDip() << endl;
// create a helixData from that
      TrkExchangeData helixd(&helix);
// reverse the process
      TrkExchangePar* newhelix = helixd.exchange();
      cout << "Unpacked TrkExchangePar with parameters "
	   << endl << newhelix->d0() << " "
	   << newhelix->phi0() << " "
	   << newhelix->omega() << " "
	   << newhelix->z0() << " "
	   << newhelix->tanDip() << endl;
// re-pack the unpacked helix, and compare the packed data
      TrkExchangeData helixd2(newhelix);
      for(unsigned iparm=0;iparm<TrkExchangePar::nParam;iparm++){
	if(helixd.parameters(iparm) ==
	   helixd2.parameters(iparm))
	  cout << "Packing of parameter " << iparm << " is reversible" << endl;
	else
	  cout << "Packing of parameter " << iparm << " is _not_ reversible" 
	       << endl << "old= " << helixd.parameters(iparm) 
	       << endl << "new= " << helixd2.parameters(iparm) << endl;
      }
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
  cout << "Usage: " << name << " d0 phi0 omega z0 tandip" << endl;
}






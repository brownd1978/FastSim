#include "BaBar/BaBar.hh"
#include "TrkBase/TrkHotListEmpty.hh"
#include "TrkBase/TrkHotListData.hh"
#include "TrkBase/TrkView.hh"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

void usage(char*);

int main( int argc, char* argv[] ) {
  if(argc==14){
    double startfound,endfound;
    unsigned nphi,nz,nax,nst;
    int firstdch,lastdch;
    unsigned short svtpat[5];
    if(! ( sscanf(argv[1],"%d",&nphi) &&
	   sscanf(argv[2],"%d",&nz) &&
	   sscanf(argv[3],"%d",&nax) &&
	   sscanf(argv[4],"%d",&nst) &&
	   sscanf(argv[5],"%lf",&startfound) &&
	   sscanf(argv[6],"%lf",&endfound) &&
	   sscanf(argv[7],"%d",&firstdch) &&
	   sscanf(argv[8],"%d",&lastdch) &&
	   sscanf(argv[9],"%hu",&svtpat[0]) &&
	   sscanf(argv[10],"%hu",&svtpat[1]) &&
	   sscanf(argv[11],"%hu",&svtpat[2]) &&
	   sscanf(argv[12],"%hu",&svtpat[3]) &&
	   sscanf(argv[13],"%hu",&svtpat[4])
	   ) ){
      usage(argv[0]);
      return 0;
    } else {
// create a Hot list
      TrkView svtviews[5];
      std::vector< unsigned > aDummyVectorToGetThisToCompile;
      for(unsigned ilay=0;ilay<5;ilay++)
	svtviews[ilay] = TrkView(svtpat[ilay]);
      TrkHotListEmpty test(nphi,nz,nax,nst,startfound,endfound,
			   firstdch,lastdch,
			   svtviews,
			   aDummyVectorToGetThisToCompile);
// create a HotListData from that
      TrkHotListData hotld(test);
// reverse the process
      TrkHotList* newhotl = hotld.hotList();
// test
      for(int iview=-1;iview<3;iview++){
	TrkEnums::TrkViewInfo view = (TrkEnums::TrkViewInfo)iview;
	if(test.nHit(view) != newhotl->nHit(view))
	  cout << "NHit " << view << " don't match: test = " << test.nHit(view)
	       << " rebuilt = " << newhotl->nHit(view) << endl;
	if(test.nActive(view) != newhotl->nActive(view))
	  cout << "NActive " << view << " don't match: test = " << test.nActive(view)
	       << " rebuilt = " << newhotl->nActive(view) << endl;
	if(test.nSvt(view) != newhotl->nSvt(view))
	  cout << "NSvt " << view << " don't match: test = " << test.nSvt(view)
	       << " rebuilt = " << newhotl->nSvt(view) << endl;
	if(test.nDch(view) != newhotl->nDch(view))
	  cout << "NDch " << view << " don't match: test = " << test.nDch(view)
	       << " rebuilt = " << newhotl->nDch(view) << endl;
      }
      for(unsigned ilay=0;ilay<5;ilay++){
	if(test.svtView(ilay) != newhotl->svtView(ilay))
	  cout << "Svt pattern for layer " << ilay << " doesn't match: test = "
	       << test.svtView(ilay).viewData() << " rebuilt = " 
	       << newhotl->svtView(ilay).viewData() << endl;
      }
      if(test.firstDchLayer() != newhotl->firstDchLayer())
	cout << "First Dch Layer don't match: test = " << test.firstDchLayer()
	     << " rebuilt = " << newhotl->firstDchLayer() << endl;
      if(test.lastDchLayer() != newhotl->lastDchLayer())
	cout << "Last Dch Layer don't match: test = " << test.lastDchLayer()
	     << " rebuilt = " << newhotl->lastDchLayer() << endl;
      
      cout << "Start found range difference = "
	   << test.startFoundRange() - newhotl->startFoundRange() << endl;      
      cout << "End found range difference = "
	   << test.endFoundRange() - newhotl->endFoundRange() << endl;      
      
      delete newhotl;
    }
  } else{  
    usage(argv[0]);
    return 0;
  }
  return 1;
}

void
usage(char* name) {
  cout << "Usage: " << name << " nphi nz naxial nstereo startfound endfound firstdch "
       << "lastdch svtpat[5]" << endl;
}






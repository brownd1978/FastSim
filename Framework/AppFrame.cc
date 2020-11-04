//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppFrame.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	AppFrame Class Implementation
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List:
//	Marc Turcotte	                Original Author March '98
//      Marc Turcotte                   Added dispatch method 4/8/98
//      Marc Turcotte                   Added stackStop method 4/24/98
//      Marc Turcotte                   Modified to allow frame width 4/27/98
//      Marc Turcotte                   Modified for AppStatus 6/19/98
//      Marc Turcotte                   Moved dispatch to AppModule 6/20/98
//      Marc Turcotte                   Modified use of aStopTypeKey
//                                      not to cause a leak 9/21/98
//
// Copyright Information:
//	Copyright (C) 1998		University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

#include <assert.h>
#include <string>
using std::string;

//
// Collaborating class headers
//
#include "FrameUtil/APPList.hh"
#include "FrameUtil/APPListIterator.hh"
#include "Framework/AppStop.hh"
#include "Framework/AppStopType.hh"
//? #include "Framework/AppRecord.hh"
#include "Framework/AppModule.hh"
//? #include "Framework/AppMethodBase.hh"

//
// Class Header
//

#include "Framework/AppFrame.hh"
using std::cout;
using std::endl;

//
// Class interface implementation
//

AppFrame::AppFrame(){
  _theStops = new APPList<AppStop*>;
  
}

AppFrame::~AppFrame(){
  APPListDeleteAll(*_theStops);
  delete _theStops;
}

void
AppFrame::appendStop(AppStop* aStop){
  _theStops->append(aStop);
}

void
AppFrame::stackStop(AppStop* aStop, int stackSize) {

  assert(stackSize>=1); // filter ...
  assert(aStop!=0);     // ... nonsense out
  
  _theStops->append(aStop); // Stick stop in frame at the end of the list.
                            // But then make sure the stops don't stack up 
                            // to wider than the allowed frame width by deleting
                            // any instance outside the width. Use the fact that
                            // the list is inherently time ordered.
  
  APPListIterator<AppStop*> iter ( *_theStops );
  AppStop** storedStop;
  iter.skipAll(); // go one past beyond end of list
  int instances = 0;
  while ( ( storedStop=iter.prev() ) ) { // step backwards through the frame contents
    string theKey = (*storedStop)->stopType()->getStopTypeKey();
    cout <<
    "looking in the frame at stop type of hash key: "<<theKey<<endl;
    if ( (*storedStop)->stopType() == aStop->stopType() ) { // Same stop type value
      instances++;
      if (instances > stackSize) {
	cout <<
        "Frame width is: "<<stackSize<<" ; found an instance outside this width to delete..."<<endl;
        _theStops->remove(storedStop); // zap anything outside the frame width
      }
    }
  } 
}

APPList<AppStop*>* 
AppFrame::stops(){
  return _theStops;
}

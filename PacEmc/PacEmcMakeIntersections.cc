//--------------------------------------------------------------------
// PacEmcMakeIntersections
// 
//   Chih-hsiang Cheng @ Caltech   2009/12/22
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "PacEmc/PacEmcMakeIntersections.hh"
#include <assert.h>
#include <iostream>
#include <math.h>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "DetectorModel/DetSet.hh"
#include "DetectorModel/DetElem.hh"
#include "DetectorModel/DetElemList.hh"
#include "DetectorModel/DetIntersection.hh"
#include "PacGeom/PacDetector.hh"

#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkDifTraj.hh"
#include "AssocTools/AstSTLMap2.hh"
#include "EmcData/EmcTrkInterInfo.hh"
#include "AbsEvent/getTmpAList.hh"

using namespace std;

//----------------
// Constructors --
//----------------
PacEmcMakeIntersections::PacEmcMakeIntersections( const char* const theName, 
					const char* const theDescription )
  : AppModule( theName, theDescription )
  , _recoTrkListName("recoTrackListName", this, "PmcRecoTracks" )
  , _emcInterKey("EmcTrkInterMap", this, "Default" )
  , _detector(0), _emcfwd(0), _emcbarrel(0), _emcbwd(0)
{
  commands()->append(&_recoTrkListName);
  commands()->append(&_emcInterKey);
}

//--------------
// Destructor --
//--------------
PacEmcMakeIntersections::~PacEmcMakeIntersections()
{}

//--------------
// Operations --
//--------------
AppResult
PacEmcMakeIntersections::beginJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
PacEmcMakeIntersections::endJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
PacEmcMakeIntersections::event( AbsEvent* anEvent )
{
  if ( 0== _detector ) {
    _detector=Ifd<PacDetector>::get(gblPEnv, "Tracking Det" );
    if (! _detector) {
      ErrMsg(fatal) << "Can't get the detector model" << endmsg;
    }

    DetElemList elements; // list of DetectorElements
    _detector->detectorModel()->listAllElements(elements);
    
    DetElemList::const_iterator ielem = elements.begin();

    for (; ielem != elements.end(); ielem++ ) {
      if ( (*ielem)->elementNumber() == 0 ) { // Only take the first "layer"
        std::string ename= (*ielem)->elementName();
	if ( 0!= ename.compare(0,3,"Emc") ) continue;
	
	unsigned slen= ename.length();

	if ( 0== ename.compare(slen-3,3,"Fwd") ) _emcfwd= *ielem;
	else if ( 0== ename.compare(slen-6,6,"Barrel") ) _emcbarrel= *ielem;
	else if ( 0== ename.compare(slen-3,3,"Bwd") ) _emcbwd= *ielem;
      }
    }

    if ( !_emcbwd ) {
      ErrMsg(warning) << "Can't find the backward EMC" << endmsg;
    }

  }

  assert(_detector);
  assert(_emcbarrel);
  assert(_emcfwd);


  // Get the list of TrkRecoTrk
  HepAList<TrkRecoTrk>* trkList= Ifd< HepAList<TrkRecoTrk> >::get(anEvent, _recoTrkListName.value());
  assert(trkList!=0);


  // create empty output maps
  AstSTLMap2<TrkRecoTrk, EmcTrkInterInfo > *theInterMap = 
    new AstSTLMap2<TrkRecoTrk, EmcTrkInterInfo >;
  // put into the event
  if (! Ifd<AstSTLMap2<TrkRecoTrk, EmcTrkInterInfo > >::put(anEvent, theInterMap, _emcInterKey.value() ) ) {
    ErrMsg(fatal) << "Can't put AstSTLMap2<TrkRecoTrk, EmcTrkInterInfo >  \""
		  << _emcInterKey.value() << "\" to the event." << endmsg;
  }

  // An AList to store intersection info and put into event
  HepAList<EmcTrkInterInfo>* interlist;
  static const IfdStrKey defaultKey("Default");
  getTmpAList(anEvent, interlist, defaultKey);
  assert(interlist);
  assert(interlist->length()==0); // make sure it is newly created

  HepAListIterator<TrkRecoTrk> ittrk(*trkList);

  TrkRecoTrk* trk(0);
  while ( trk= ittrk() ) {
    
    const TrkFit* fit= trk->fitResult();
    if (!fit) continue;

    // Find the intersect between a trajectory and the center of a det element.
    // Check all three elements and keep the shortest path length.
    double pathlen(10000);
    if ( _emcfwd ) {
      DetIntersection dinter(_emcfwd, &fit->traj(), 175.,300.);
      int jflag=  _emcfwd->intersect(&fit->traj(), dinter);
      if ( jflag ) pathlen= dinter.pathlen;
    }
    if ( _emcbarrel ) {
      DetIntersection dinter(_emcbarrel, &fit->traj(), 88, 240);
      int jflag=  _emcbarrel->intersect(&fit->traj(), dinter);
      if ( jflag && dinter.pathlen < pathlen) pathlen= dinter.pathlen;
    }
    if ( _emcbwd ) {
      DetIntersection dinter(_emcbwd, &fit->traj(), 105, 250);
      int jflag=  _emcbwd->intersect(&fit->traj(), dinter);
      if ( jflag && dinter.pathlen < pathlen) pathlen= dinter.pathlen;
    }
    
    EmcTrkInterInfo *info= new EmcTrkInterInfo(pathlen,fit,trk);
    interlist->append(info);
    theInterMap->append(trk,info);

  }

  return AppResult::OK;
}


#include "BaBar/BaBar.hh"

#include <iostream>
#include <string>

#include <TTree.h>
#include <TFile.h>
#include <TObjString.h>

#include "PacDisplay/PacDisplay.hh"

#include "AbsEnv/AbsEnv.hh"
#include "BaBar/BaBar.hh"
#include "BField/BField.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "PacEnv/PacConfig.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/PdtPid.hh"
#include "ProxyDict/Ifd.hh"
#include "TrkEnv/TrkEnv.hh"
#include "AbsCalo/AbsRecoCalo.hh"

using std::cout;
using std::endl;

//----------------
// Constructors --
//----------------
PacDisplay::PacDisplay( const char* const theName,const char* const theDescription )
  : AppModule( theName, theDescription )
  , _saveGen("saveGen",this,false)
  , _saveSim("saveSim",this,false)
  , _saveRec("saveRec",this,false)
  , _saveDet("saveDet",this,false)
  , _saveVox("saveVox",this,false)
  , _saveHit("saveHit",this,false)
  , _saveEmc("saveEmc",this,false)
  , _resolution("resolution",this,200)
  , _fileName("fileName",this,"disp.root")
  , _recoCaloListName("recoCaloListName", this, "PmcRecoCalos")
  , _simTrackListName("simTrackListName", this, "Pmc PacSimTrack list")
  , _recoTrackListName("recoTrackListName", this, "PmcRecoTracks" )
  , _event(0)
{
	commands()->append(& _fileName);
	commands()->append(& _recoCaloListName);
	commands()->append(& _simTrackListName);
	commands()->append(& _recoTrackListName);
	commands()->append(& _saveGen);
	commands()->append(& _saveSim);
	commands()->append(& _saveRec);
	commands()->append(& _saveDet);
	commands()->append(& _saveVox);
	commands()->append(& _saveHit);
	commands()->append(& _saveEmc);
	commands()->append(& _resolution);
}
//--------------
// Destructor --
//--------------
PacDisplay::~PacDisplay( )
{
}

//--------------
// Operations --
//--------------
AppResult PacDisplay::beginJob(AbsEvent* anEvent)
{
	ErrMsg(routine) <<"Begin job" << endmsg;
  _display.init(_fileName.value().c_str(),_resolution.value(),_verbose.value());	
	return AppResult::OK;
}

AppResult PacDisplay::endJob(AbsEvent* anEvent) 
{
  _display.finalize();
	ErrMsg(routine) << "End job after " << _event <<  " events." << endmsg;
	return AppResult::OK;
}

AppResult PacDisplay::event(AbsEvent* anEvent)
{
// prep
  TrkEnv *theTrkEnv = gblEnv->getTrk();
  const BField* bfield = theTrkEnv->magneticField();
  assert(bfield!=0);
// draw detector on 1st event
	if(_event == 0 && _saveDet.value())
	  _display.drawDetector();
// reset event information in display
  _display.reset();
// Get list of PacSimTrack objects created by PmcSimulate			
  HepAList<PacSimTrack>* simList =  Ifd< HepAList<PacSimTrack> >::get(anEvent, _simTrackListName.value() );
  if (_verbose.value()) cout << name() << " PacSimList contains " << simList->length() << " entries " << endl;
  assert(simList!=0);
// Loop over sim track list and create trajectory
  const PacSimTrack* simtrk(0);
  for(unsigned istrk=0;istrk<simList->length();istrk++){
    simtrk = (*simList)[istrk];
    if (_verbose.value()) cout<< name () << " reading simtrk "<< simtrk->getTraj() << " " << istrk << endl;
    if(_saveGen.value()) {
      const GTrack* gentrk = simtrk->getGTrack();
      double range = simtrk->lastHit()->globalFlight();
      _display.drawGTrack(gentrk,range,bfield);
    }
    if(_saveSim.value()){
      _display.drawSimTrack(simtrk);
    }
    if(_saveHit.value()) {
      _display.drawSimHits(simtrk,istrk);
    }
  }
// if we're writing tracks, save them
  if(_saveRec.value()) {
      // Get list of TrkRecoTrk objects created by PmcReconstruct			
    HepAList<TrkRecoTrk>* recList =  Ifd< HepAList<TrkRecoTrk> >::get(anEvent, _recoTrackListName.value() );
    if (_verbose.value()) cout << name() << " PacRecoTrack contains " << recList->length() << " entries " << endl;
    assert(recList!=0);
      // Loop over rec track list and create trajectory
    const TrkRecoTrk* rectrk(0);
    for(int irtrk = 0;irtrk<recList->length();irtrk++){
      rectrk = (*recList)[irtrk];
      if (_verbose.value()) cout<< name () << " reading rectrk "<< irtrk << endl;
      _display.drawRecTrack(rectrk);
    }
  }

  if(_saveEmc.value()) {
        // Get a list of AbsRecoCalo
    HepAList<AbsRecoCalo>* calolist= Ifd< HepAList<AbsRecoCalo> >::get(anEvent, _recoCaloListName.value());
    assert(calolist!=0);
    if (_verbose.value()) cout << name() << " AbsRecoCalo list " << _recoCaloListName.value() << " contains " << calolist->length() << " entries " << endl;
    HepAListIterator<AbsRecoCalo> itcalo(*calolist);
      // Loop over calo list and store info to TTree
    int i = 1;
    const AbsRecoCalo *calo(0);
    while( calo = itcalo() ) {
      _display.drawCluster(calo);
      i++;
    }
  }
// fill the trees for this even
  _display.fillTrees();
  _event++;
  return AppResult::OK;
}


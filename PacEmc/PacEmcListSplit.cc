//--------------------------------------------------------------------------
// PacEmcListSplit
//
// Description:
//	Module PacEmcListSplit.  Split the input emc list into charged and 
//      neutral lists (copy much of the code from Babar's EmcReco/EmcListSplit)
//
// Chih-hsiang Cheng   Caltech   2009/03/17
//---------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacEmc/PacEmcListSplit.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcTMInfo.hh"
#include "EmcData/EmcTrkInterInfo.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "AssocTools/AstSTLMatch.hh"
#include "AssocTools/AstSTLMap2.hh"
#include "AssocTools/AstUniqMap2.hh"
#include "ProbTools/Consistency.hh"
#include "ErrLogger/ErrLog.hh"
#include <assert.h>

PacEmcListSplit::PacEmcListSplit ( const char* const theName, const char* const theDescription ) :
  AppModule(theName,theDescription)
  , _trkMatchType("TrackClusterMatchType",this)
  , _trkMatchCut("TrackClusterMatchCut",this,1.0e-4)
  , _trkOkNDchCut("TrackClusterTrkOkNDch",this,-1)
  , _emcInputList("emcInputList", this, "PmcRecoCalos")
  , _emcTMMap("EmcTrackMatchMap",this,"PacTMMap")
  , _emcTrkMapKey("EmcTrkMap",this,"Default")
  , _bestneutbumps("BestNeutralBumps",this,"bestneutbumps")
  , _chargedbumps("ChargedBumps",this,"chargedbumps")
  , _bestbumptrks("BestBumpTrks",this,"bestbumptrks")
{
  commands()->append( &_trkMatchType);
  commands()->append( &_trkMatchCut);
  commands()->append( &_trkOkNDchCut);
  commands()->append( &_emcInputList);
  commands()->append( &_emcTrkMapKey);
  commands()->append( &_emcTMMap);
  commands()->append( &_bestneutbumps);
  commands()->append( &_chargedbumps);
  commands()->append( &_bestbumptrks);
  _trkMatchType.addItem("Consistency",consistency); // default
  _trkMatchType.addItem("LogLikelihoodDifference",likelihood);

}

PacEmcListSplit::~PacEmcListSplit ( )
{}

AppResult
PacEmcListSplit::event ( AbsEvent* anEvent ){

  _theTrkMatch = 
    Ifd< AstSTLMatch<AbsRecoCalo,  TrkRecoTrk, PacEmcTMInfo > >::get(anEvent, _emcTMMap.value());
  HepAList< AbsRecoCalo >* emcInputList = 
    Ifd< HepAList<AbsRecoCalo> >::get(anEvent, _emcInputList.value());

  if (emcInputList != 0 && _theTrkMatch != 0 ) {

    // Create the output: final track-cand map
    _emcTrkMap = 
      Ifd<std::map<const TrkRecoTrk*,std::pair<const AbsRecoCalo*,double> > >::get(anEvent,_emcTrkMapKey.value());
    if (_emcTrkMap == 0) {
      _emcTrkMap = 
	new std::map<const TrkRecoTrk*,std::pair<const AbsRecoCalo*,double> >();
      putData<std::map<const TrkRecoTrk*,std::pair<const AbsRecoCalo*,double> > >(anEvent,_emcTrkMap,_emcTrkMapKey.value());
    } else {
      ErrMsg(fatal) << "Track-cand map already exists!  Aborting" << endmsg;
    }

    // Build the lists using only the Track-match info.
    // Put these into the event with the correct keys
    HepAList< AbsRecoCalo>* bestneutbumps = new HepAList< AbsRecoCalo>;
    assert(bestneutbumps != 0);
    putData<HepAList<AbsRecoCalo> >(anEvent,bestneutbumps,_bestneutbumps.value());

    HepAList< AbsRecoCalo>* chargedbumps = new HepAList< AbsRecoCalo>;
    assert(chargedbumps != 0);
    putData<HepAList<AbsRecoCalo> >(anEvent,chargedbumps,_chargedbumps.value());

    HepAList< TrkRecoTrk>* bestbumptrks = new HepAList< TrkRecoTrk>;
    assert(bestbumptrks != 0);
    putData<HepAList<TrkRecoTrk> >(anEvent,bestbumptrks,_bestbumptrks.value());

    // the following function also fills the track->best cand map
    fillNeutralLists(*emcInputList, *bestneutbumps,*chargedbumps,*bestbumptrks);

    if ( verbose() ) {
      ErrMsg(routine) << "input : " << _emcInputList.value() << " length= "
			<< emcInputList->length() << endmsg;
      ErrMsg(routine) << "output : " << _bestneutbumps.value() << " length= "
			<< bestneutbumps->length() << endmsg;
      ErrMsg(routine) << "       : " << _chargedbumps.value() << " length= "
			<< chargedbumps->length() << endmsg;
      ErrMsg(routine) << "       : " << _bestbumptrks.value() << " length= "
			<< bestbumptrks->length() << endmsg;
    }

  }
  return AppResult::OK;
}

void
PacEmcListSplit::fillNeutralLists (const HepAList<AbsRecoCalo>& emcInputList,
				   HepAList<AbsRecoCalo>& bestneutbumps,
				   HepAList<AbsRecoCalo>& chargedbumps,
				   HepAList<TrkRecoTrk>& bestbumptrks) {
  
  // For each emc bump, look for the best matched track with a good consistency
  unsigned ncands = emcInputList.length();
  for (unsigned icand=0; icand<ncands; ++icand) {
    AbsRecoCalo* theEmcCand = emcInputList[icand];

    if ( theEmcCand== 0 ) continue;

    const std::map<TrkRecoTrk*, PacEmcTMInfo*, BbrPtrLess> * bumpmap =
      _theTrkMatch->findMatchSet(theEmcCand);

    // Fill the best-track map with the best match
    if (bumpmap != 0 && bumpmap->size() > 0 ) {

      std::map<TrkRecoTrk*, PacEmcTMInfo*, BbrPtrLess>::const_iterator imap = bumpmap->begin();
      while (imap != bumpmap->end()) {
	TrkRecoTrk* trk = imap->first;
	const PacEmcTMInfo *theInfo = imap->second;

	if (trk != 0 && theInfo!=0) {
	  bool trkOK = trk->fitResult()!=0 && trk->fitResult()->nDch() > _trkOkNDchCut.value();
	  const Consistency theCons = theInfo->getConsistency();
	  
	  if (trkOK && theCons.status()==Consistency::OK) {
	    // Compute matching quantity
	    double matchq(-1.0e6);
	    if (_trkMatchType.value() == consistency) {
	      matchq = theCons.consistency();
	    } else if (_trkMatchType.value() == likelihood) {
	      matchq = log(theCons.likelihood());
	    } else {
	      ErrMsg(error) << "Unknown match type " << _trkMatchType.value() << endmsg;
	    }
	    
	    if (matchq > _trkMatchCut.value()) {
	      std::map<const TrkRecoTrk*,std::pair<const AbsRecoCalo*,double> >::iterator current = _emcTrkMap->find(trk);
	      // If no match exists for this track, or if the new match is 
	      // better than the old, set the new match
	      if (current == _emcTrkMap->end() || current->second.second < matchq) {
		(*_emcTrkMap)[trk] = std::pair<const AbsRecoCalo*,double>(theEmcCand,matchq);
	      }
	    }
	  }
	}
	++imap;
      }
    }
  }

  // Now fill the 'traditional' neutral lists that rely only on the best match
  // First, find the bumps associated with the best track match
  std::vector<const AbsRecoCalo*> bestchargedbumps;
  std::map<const TrkRecoTrk*,std::pair<const AbsRecoCalo*,double> >::iterator imatch = _emcTrkMap->begin();
  while (imatch != _emcTrkMap->end()) {
    const AbsRecoCalo* theEmcCand = imatch->second.first;
    if (theEmcCand!=0) {
      bestchargedbumps.push_back(theEmcCand);
      bestbumptrks.append(const_cast<TrkRecoTrk*>(imatch->first));
    } else {
      ErrMsg(error) << "Non-bump cand is best track match!!" << endmsg;
    }
    ++imatch;
  }

  // Now pick out the EmcCands which _don't_ use these bumps and call them neutral
  for (unsigned icand=0; icand<ncands; ++icand) {
    AbsRecoCalo* theEmcCand = emcInputList[icand];
    if ( theEmcCand==0 ) continue;

    if (std::find(bestchargedbumps.begin(),bestchargedbumps.end(),theEmcCand) == bestchargedbumps.end()) {
      bestneutbumps.append(theEmcCand);
    } else {
      chargedbumps.append(theEmcCand);
    }
  }
}

template <class A>
void
PacEmcListSplit::putData (AbsEvent* anEvent, A* evtobj, const IfdKey& key) {
// put the object into the event
  IfdDataProxy< A >* proxy =  new IfdDataProxy< A >(evtobj);
  assert(proxy != 0);
  if (Ifd< A >::put(anEvent,proxy,key) == 0){
    ErrMsg(error) << "Cant put proxy into event " << endmsg;
    delete proxy;
    delete evtobj;
  }
}

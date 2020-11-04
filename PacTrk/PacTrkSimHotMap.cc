#include "BaBar/BaBar.hh"
#include "PacTrk/PacTrkSimHotMap.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacSim/PacSimHit.hh"
#include <assert.h>

PacTrkSimHotMap::PacTrkSimHotMap()
{ }

PacTrkSimHotMap::~PacTrkSimHotMap()
{ }

void PacTrkSimHotMap::Clear()
{
// delete any unused hots
  for(SimTrkToHotsMapType::iterator iter = _simTrkToHotsMap.begin();iter != _simTrkToHotsMap.end();iter++){
    if(!simTrkUsed(iter->first)){
      for(std::vector<const PacHitOnTrk*>::iterator ihot = iter->second.begin();ihot != iter->second.end();ihot++){
        delete ( const_cast<PacHitOnTrk*>(*ihot));
      }
    }
  }
// clear the maps
  _simToHotMap.clear();
  _hotToSimMap.clear();
  _simHitUsed.clear();
  _pixelPairMap.clear();
  _simTrkToRecoMap.clear();
  _simTrkToHotsMap.clear();
  _hots.clear();
}

void PacTrkSimHotMap::addToMap(const PacSimHit* hit, const PacHitOnTrk* hot)
{  
  _hots.push_back(const_cast<PacHitOnTrk*>(hot));
  _simToHotMap.insert(std::make_pair(hit, hot));
  _hotToSimMap.insert(std::make_pair(hot, hit));

  // Create an entry in the Used SimHit Array if one doesn't already exist
  if (_simHitUsed.find(hit) == _simHitUsed.end())
    _simHitUsed.insert(std::make_pair(hit, false));
// insert to track map
  addToMap(hit->getSimTrack(),hot);
}

void PacTrkSimHotMap::addToMap(const PacSimTrack* simtrk, const PacHitOnTrk* hot)
{
  _simTrkToHotsMap[simtrk].push_back(hot);
 
}


void PacTrkSimHotMap::addToMap(const PacSimTrack* sTrk, const TrkRecoTrk* rTrk)
{
  _simTrkToRecoMap.insert(std::make_pair(sTrk, rTrk));
}

const TrkRecoTrk* PacTrkSimHotMap::getRecoTrk(const PacSimTrack* sTrk) const
{
  SimTrkToRecoMapType::const_iterator pos;
  pos = _simTrkToRecoMap.find(sTrk);
  if (pos != _simTrkToRecoMap.end())
    return pos->second;
  else
    return 0;
}

bool PacTrkSimHotMap::simHitUsed(const PacSimHit* hit)
{
  SimHitUsedType::const_iterator pos = _simHitUsed.find(hit);
  if (pos == _simHitUsed.end())
    // If hit wasn't found it hasn't been used
    return false;
  else
    return pos->second; 
}

const PacSimHit* PacTrkSimHotMap::getSimHit(const PacHitOnTrk* hot) const
{
  HotToSimMapType::const_iterator pos;
  pos = _hotToSimMap.find(hot);
  if (pos != _hotToSimMap.end()) {
    return pos->second;
  }
  else {
    return 0;
  }
}

const PacSimHit* PacTrkSimHotMap::getSimHit(const TrkHitOnTrk* hot) const
{
  return getSimHit(dynamic_cast<const PacHitOnTrk*>(hot));
}

std::vector<const PacHitOnTrk*> PacTrkSimHotMap::getHots(const PacSimHit* hit) const {

  std::vector<const PacHitOnTrk*> retVal;
  
  SimToHotMapType::const_iterator pos;
  
  std::pair<SimToHotMapType::const_iterator, SimToHotMapType::const_iterator> range;
  range = _simToHotMap.equal_range(hit);
  for (pos = range.first; pos != range.second; ++pos)
    retVal.push_back(pos->second);

  return retVal;
}

const std::vector<const PacHitOnTrk*>& PacTrkSimHotMap::getHots(const PacSimTrack* sTrk) const {
  SimTrkToHotsMapType::const_iterator pos = _simTrkToHotsMap.find(sTrk);
  if(pos != _simTrkToHotsMap.end())
    return pos->second;
  else {
    static std::vector<const PacHitOnTrk*> empty;
    return empty;
  }
  
}


void PacTrkSimHotMap::markHotUsed(const PacHitOnTrk* hot) {

  // Get SimHit associated with this hot
  const PacSimHit* simHit = getSimHit(hot);

  // Should always be there!
  assert(0 != simHit);

  _simHitUsed[simHit] = true;

  return;
}

void PacTrkSimHotMap::markSimTrkUsed(const PacSimTrack* strk) {
  _simTrkUsedMap[strk] = true;
}

bool PacTrkSimHotMap::simTrkUsed(const PacSimTrack* strk) {
  SimTrkUsedMapType::const_iterator istrk = _simTrkUsedMap.find(strk);
  return (istrk != _simTrkUsedMap.end() && istrk->second );
}


void PacTrkSimHotMap::removeHot(const PacHitOnTrk* hot) {

  _hotToSimMap.erase(hot);

  SimToHotMapType::iterator pos;
  for (pos = _simToHotMap.begin(); pos != _simToHotMap.end(); ) {
    if (pos->second == hot) {
      _simToHotMap.erase(pos++);
    }
    else {
      ++pos;
    }
  }

  // Remove this hot from the pixel pair map, too
  _pixelPairMap.erase(hot);
  // Need to handle removing element if "hot" is the value, not the key
  PixelPairMapType::iterator pixIter;
  for (pixIter = _pixelPairMap.begin(); pixIter != _pixelPairMap.begin(); ) {
    if (pixIter->second == hot) {
      _pixelPairMap.erase(pixIter++);
    }
    else {
      ++pixIter;
    }
  }
  // remove from hotlist map
  SimTrkToHotsMapType::iterator ishots;
  for(ishots = _simTrkToHotsMap.begin(); ishots != _simTrkToHotsMap.end();ishots++){
    std::vector<const PacHitOnTrk*>::iterator ihot = std::find(ishots->second.begin(),ishots->second.end(),hot);
    if(ihot != ishots->second.end())ishots->second.erase(ihot);
  }

  return;
}

void PacTrkSimHotMap::addPixelPair(const PacHitOnTrk* hot1, const PacHitOnTrk* hot2) {

  // I know this double the size of the map, but makes retrival easier
  _pixelPairMap[hot1] = hot2;
  _pixelPairMap[hot2] = hot1;

  return;
}

const PacHitOnTrk* PacTrkSimHotMap::getPixelPair(const PacHitOnTrk* hot) const {

  const PacHitOnTrk* retval = 0;

  PixelPairMapType::const_iterator pos;
  pos = _pixelPairMap.find(hot);

  if (pos != _pixelPairMap.end())
    retval = pos->second;

  return retval;
}

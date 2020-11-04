#ifndef PacTrkSimHotMap_HH
#define PacTrkSimHotMap_HH

#include <map>
#include <vector>

class PacSimHit;
class PacHitOnTrk;
class TrkHitOnTrk;
class PacSimTrack;
class TrkRecoTrk;

typedef std::vector<PacHitOnTrk*> HotStorage;
typedef std::multimap<const PacSimHit*, const PacHitOnTrk*> SimToHotMapType;
typedef std::map<const PacHitOnTrk*, const PacSimHit*> HotToSimMapType;
typedef std::map<const PacSimHit*, bool> SimHitUsedType;
typedef std::map<const PacHitOnTrk*, const PacHitOnTrk*> PixelPairMapType;
typedef std::map<const PacSimTrack*, const TrkRecoTrk*> SimTrkToRecoMapType;
typedef std::map<const PacSimTrack*, std::vector<const PacHitOnTrk*> > SimTrkToHotsMapType;
typedef std::map<const PacSimTrack*, bool> SimTrkUsedMapType;

class PacTrkSimHotMap {

public:
  PacTrkSimHotMap();
  ~PacTrkSimHotMap();

public:
  void Clear();

  void addToMap(const PacSimHit* hit, const PacHitOnTrk* hot);

  const PacSimHit* getSimHit(const PacHitOnTrk* hot) const;
  
  const PacSimHit* getSimHit(const TrkHitOnTrk* hot) const;

  std::vector<const PacHitOnTrk*> getHots(const PacSimHit* hit) const;

  const std::vector<const PacHitOnTrk*>& getHots(const PacSimTrack* sTrk) const;

  void markHotUsed(const PacHitOnTrk* hot);

  bool simHitUsed(const PacSimHit* hit);

  bool simTrkUsed(const PacSimTrack* strk);

  void markSimTrkUsed(const PacSimTrack* strk);
  
  void removeHot(const PacHitOnTrk* hot);

  void addPixelPair(const PacHitOnTrk* hot1, const PacHitOnTrk* hot2);

  const PacHitOnTrk* getPixelPair(const PacHitOnTrk* hot) const;

  void addToMap(const PacSimTrack* sTrk, const TrkRecoTrk* rTrk);
  
  const TrkRecoTrk* getRecoTrk(const PacSimTrack* sTrk) const;
  
  const HotStorage& hots() { return _hots;}
private:
  void addToMap(const PacSimTrack* simtrk, const PacHitOnTrk* hot);

private:
  HotStorage _hots;
  SimToHotMapType _simToHotMap;
  HotToSimMapType _hotToSimMap;
  SimHitUsedType _simHitUsed;
  PixelPairMapType _pixelPairMap;
  SimTrkToRecoMapType _simTrkToRecoMap;
  SimTrkToHotsMapType _simTrkToHotsMap;
  SimTrkUsedMapType _simTrkUsedMap;
};


#endif

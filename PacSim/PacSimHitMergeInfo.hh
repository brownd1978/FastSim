//--------------------------------------------------------------------
//
//--------------------------------------------------------------------

#ifndef PacSimHitMergeInfo_HH
#define PacSimHitMergeInfo_HH

#include "CLHEP/Geometry/HepPoint.h"
#include <vector>

class PacSimHit;
class PacSimHitMergeInfo {

public:

  enum mergeCode {noMerge=0, mergedAndUse, mergedAndDontUse};

  PacSimHitMergeInfo();

  // Copy constructor
  PacSimHitMergeInfo(const PacSimHitMergeInfo&);

  // Assignment
  PacSimHitMergeInfo& operator=(const PacSimHitMergeInfo&);

  // Destructor
  ~PacSimHitMergeInfo();

  mergeCode mergeStatus() const { return _mergeStatus; }
  const HepPoint& position() const { return _pos; }
  const int nUpdates() const { return _nUpdates; }

  void setMergeStatus(mergeCode code) { _mergeStatus = code; }
  void setPosition(HepPoint position) { _pos = position; _nUpdates++;}

  void addSimHitToMergeList(PacSimHit* sHit) { _simHitMergeList.push_back(sHit); }
  const std::vector<PacSimHit*> getSimHitMergeList() const {return _simHitMergeList;}

  void addSimHitToPatrecList(PacSimHit* sHit) {_simHitPatrecList.push_back(sHit);}
  const std::vector<PacSimHit*> getSimHitPatrecList() const {return _simHitPatrecList;}
private:

  mergeCode _mergeStatus;

  // Position of the merged hit
  HepPoint _pos;

  // Keep track of how many times this hit gets its position updated
  int _nUpdates;

  // Keep a list of the other SimHits that this hit was merged with
  std::vector<PacSimHit*> _simHitMergeList;

  // Keep a list of the other SimHits to be considered for PatRec confusion
  std::vector<PacSimHit*> _simHitPatrecList;
};

#endif

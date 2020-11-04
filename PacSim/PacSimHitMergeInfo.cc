//--------------------------------------------------------------------
//
//--------------------------------------------------------------------

#include "PacSim/PacSimHitMergeInfo.hh"

PacSimHitMergeInfo::PacSimHitMergeInfo() :
  _mergeStatus(noMerge),_pos(0,0,0),_nUpdates(0)
{

}

PacSimHitMergeInfo::PacSimHitMergeInfo(const PacSimHitMergeInfo& other) :
  _mergeStatus(other._mergeStatus),_pos(other._pos),
  _nUpdates(other._nUpdates), _simHitMergeList(other._simHitMergeList),
  _simHitPatrecList(other._simHitPatrecList)
{

}

PacSimHitMergeInfo&
PacSimHitMergeInfo::operator = (const PacSimHitMergeInfo& other)
{
  if (this != &other) {
    _mergeStatus = other._mergeStatus;
    _pos = other._pos;
    _nUpdates = other._nUpdates;
    _simHitMergeList = other._simHitMergeList;
    _simHitPatrecList = other._simHitPatrecList;
  }

  return *this;
}

PacSimHitMergeInfo::~PacSimHitMergeInfo() {

}

//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkMergeMap.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkMergeMap.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkId.hh"
#include "ErrLogger/ErrLog.hh"
#include <utility>

TrkMergeMap::TrkMergeMap()
{
  _lastRequest = _map.end();
}

TrkMergeMap::~TrkMergeMap()
{}

void
TrkMergeMap::insert(TrkRecoTrk* trk,
                    TrkRecoTrk* p1, TrkRecoTrk* p2)
{
  map_t::value_type p(trk->id(),std::make_pair(p1,p2));
  std::pair<map_t::iterator,bool> x = _map.insert( p );
  _lastRequest = x.first;
  if (!x.second) {
         ErrMsg(warning) << "you tried to insert a track twice; don't! -- ignored" << endmsg;
  }
}

void
TrkMergeMap::remove(const TrkRecoTrk* trk)
{
  map_t::iterator i = _map.find(trk->id());
  if (i!=_map.end()) {
       _map.erase(i);
       _lastRequest = _map.end(); // invalidate iterator!
  }
}

TrkRecoTrk*
TrkMergeMap::track1(const TrkRecoTrk* trk) const
{
  if (_map.empty()) return 0;
  if (_lastRequest == _map.end() || trk->id() != _lastRequest->first) {
    _lastRequest = _map.find(trk->id()); 
  }
  return _lastRequest==_map.end()?0:_lastRequest->second.first;
}

TrkRecoTrk*
TrkMergeMap::track2(const TrkRecoTrk* trk) const
{
  if (_map.empty()) return 0;
  if (_lastRequest == _map.end() || trk->id() != _lastRequest->first) {
    _lastRequest = _map.find(trk->id()); 
  }
  return _lastRequest==_map.end()?0:_lastRequest->second.second;
}

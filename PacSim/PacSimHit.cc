//--------------------------------------------------------------------------
// Name:
//   PacSimHit
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 23 APR 2008
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "PacSim/PacSimHit.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacShowerInfo.hh"
#include "PacSim/PacDKChainInfo.hh"
#include "PacSim/PacSimHitMergeInfo.hh"
#include "PacGeom/PacDetElem.hh"
#include "DetectorModel/DetMaterial.hh"
#include "PDT/PdtEntry.hh"

PacSimHit::PacSimHit() :
  _strk(0),_globtraj(0),_globlen(0.0),_pos(0,0,0),_time(-1.0),_pin(0,0,0),_pout(0,0,0),_effect(none),
  _shower(0),_decay(0),_merge(0),_delem(0)
{
}

PacSimHit::PacSimHit(const PacSimTrack* strk,const DetIntersection& dinter,
		const HepPoint& pos, const Hep3Vector& inmom,const Hep3Vector& outmom,
		effect effect) :
	_strk(strk),_dinter(dinter),_globtraj(0),_globlen(0.0),_pos(pos),_time(-1.0),_pin(inmom),_pout(outmom),_effect(effect),
	_shower(0),_decay(0),_merge(0),_delem(0)
{
}

PacSimHit::PacSimHit(const PacSimTrack* strk,const DetIntersection& dinter,
		const HepPoint& pos, const Hep3Vector& inmom,const Hep3Vector& outmom,
		     effect effect, double hitTime) :
	_strk(strk),_dinter(dinter),_globlen(0.0),_pos(pos),_time( hitTime ),_pin(inmom),_pout(outmom),_effect(effect),
	_shower(0),_decay(0),_merge(0),_delem(0)
{
}

PacSimHit::PacSimHit(const PacSimHit& other)
  : _strk(other._strk),_dinter(other._dinter),_globtraj(other._globtraj),_globlen(other._globlen),_pos(other._pos),_time(other._time),
    _pin(other._pin),_pout(other._pout),_effect(other._effect),_shower(0),_decay(0),_merge(0),_delem(0)
{
  if(other._shower != 0)_shower = new PacShowerInfo(*other._shower);
  if(other._decay != 0)_decay = new PacDKChainInfo(*other._decay);
  if(other._merge != 0) _merge = new PacSimHitMergeInfo(*other._merge);
}
PacSimHit&
PacSimHit::operator = (const PacSimHit& other) {
	if(this != &other)  {
		_strk = other._strk;
		_dinter = other._dinter;
    _globtraj = other._globtraj;
    _globlen = other._globlen;
		_pos = other._pos;
		_time = other._time;
		_pin = other._pin;
		_pout = other._pout;
		_effect = other._effect;
    _delem = other._delem;
		delete _shower; _shower=0;
		delete _decay; _decay=0;
		delete _merge; _merge = 0;
		
    if(other._shower != 0)_shower = new PacShowerInfo(*other._shower);
    if(other._decay != 0)_decay = new PacDKChainInfo(*other._decay);
    if(other._merge != 0) _merge = new PacSimHitMergeInfo(*other._merge);

	}
	return *this;
}

PacSimHit::~PacSimHit() {
  delete _shower;
  delete _decay;
  delete _merge;
}

const PdtEntry*
PacSimHit::pdt() const {
  return _strk->pdt();
}


double
PacSimHit::energyChange() const {
// if this object has shower information, use that
  if(_shower != 0)
    return _shower->energyDeposit();
  else if(_decay != 0)
    return DetMaterial::particleEnergy(_pin.mag(),_strk->pdt()->mass());
  else {
// compute the energy change based on the momentum change
    double mass = _strk->pdt()->mass();
    double m2 = mass*mass;
    double ein = sqrt(_pin.mag2() + m2);
    double eout = sqrt(_pout.mag2() + m2);
    return ein - eout;
  }
}

const HepPoint&
PacSimHit::position() const {

  // Return the position as the merged hit position.  Access to the
  // unmerged position is via ::originalPosition()
  if (0 != _merge && _merge->mergeStatus() == PacSimHitMergeInfo::mergedAndUse)
    return _merge->position();
  else
    return _pos;

}


bool
PacSimHit::addShowerInfo(const PacShowerInfo& showerinfo) {
// verify this is a shower type SimHit
  if(_effect >= shower){
    delete _shower;
    _shower = new PacShowerInfo(showerinfo);
    return true;
  } else
    return false;
}

bool
PacSimHit::addDecayInfo(PacDKChainInfo* decayinfo) {
// verify this is a decay type SimHit
  if(_effect == decay){
    delete _decay;
    _decay = decayinfo;
    return true;
  } else
    return false;
}

void
PacSimHit::addMergeInfo(PacSimHitMergeInfo* mergeinfo) {
  delete _merge;
  _merge = mergeinfo;
  return;
}

void PacSimHit::print( std::ostream& o )
{
  o << "PacSimHit details:" << std::endl;
  o << "\t position " << _pos << " @ time " << _time << std::endl;
  o << "\t effect = " << detEffectName(_effect) << std::endl;
  o << "\t Momenta: in = " << _pin << " of magnitude = " << _pin.mag()
    << std::endl
    << "\t         out = " << _pout << "of magnitude = " << _pout.mag()
    << std::endl;
}

const char*
PacSimHit::effectNames[neffects] = {"none","creation","mark","brems","compton","normal","stop","interact",
	"convert","decay","shower","hadshower"};

const char*
PacSimHit::detEffectName(PacSimHit::effect eff) {
	return effectNames[eff];
}

const PacDetElem*
PacSimHit::detElem() const {
  if(0 == _delem)
    _delem = dynamic_cast<const PacDetElem*>(_dinter.delem);
  return _delem;
}


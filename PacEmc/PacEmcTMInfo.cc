//-------------------------------------------------------------------------
// PacEmcTMInfo
//
//  Class PacEmcTMInfo. This class contains information generated during
//      track matching.  A simplified version of Babar's EmcTMInfo.
//
//  Chih-hsiang Cheng   Caltech 2009/03/05
//-------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "PacEmc/PacEmcTMInfo.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <vector>
#include <map>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include <iostream>
#include <iterator>

#include "EmcData/EmcTrkInterInfo.hh"
#include "AssocTools/AstAssociation.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "ErrLogger/ErrLog.hh"
#include "TrkBase/TrkFit.hh"
#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkPocaBase.hh"

using std::ostream;
using std::ostream_iterator;

//----------------
// Constructors --
//----------------

PacEmcTMInfo::PacEmcTMInfo () :
  _entryPoint(0,0,0),
  _deltaXY( 0 ),
  _deltaZ( 0 ),
  _trkInterInfo(0),
  _track( 0 ),
  _charge( 0 ),
  _calo( 0 ),
  _poca(0)
{}

PacEmcTMInfo::PacEmcTMInfo (HepPoint theEntrance, int charge,
			    const EmcTrkInterInfo interinfo, 
			    const AbsRecoCalo * calo,
			    double deltaXY, double deltaZ ) :
  _entryPoint( theEntrance ),
  _deltaXY( deltaXY ),
  _deltaZ( deltaZ ),
  _trkInterInfo(interinfo),
  _track( interinfo.recoTrack() ),
  _charge( 0 ),
  _calo( calo ),
  _poca(0)
{
  if ( _track ) {
    const TrkFit * tf = _track->fitResult();
    if ( tf ) {
      _charge = tf->charge();
    }
  }
}

PacEmcTMInfo::PacEmcTMInfo( const PacEmcTMInfo & o ):
  _entryPoint( o._entryPoint ),
  _deltaXY( o._deltaXY ),
  _deltaZ( o._deltaZ ),
  _trkInterInfo( o._trkInterInfo ),
  _track( o._track ),
  _charge( o._charge ),
  _calo( o._calo ),
  _consistency( o.getConsistency() ),
  _poca(o._poca != 0 ? o._poca->clone() : 0)
{
}

//--------------
// Destructor --
//--------------
PacEmcTMInfo::~PacEmcTMInfo()
{
  _deltaXY = 0;
  _deltaZ = 0;
  delete _poca;
}

//-------------
// Methods   --
//-------------

//-------------
// Operators --
//-------------

bool
PacEmcTMInfo::operator==( const PacEmcTMInfo& other) const {

  if ( _deltaXY != other._deltaXY ) return false;
  if ( _deltaZ != other._deltaZ ) return false;
  if ( !(_trkInterInfo == other.getTrkInterInfo()) ) return false;
  if ( !(_consistency == other.getConsistency()) ) return false;
  
  return true;
}

const Consistency
PacEmcTMInfo::getConsistency() const {
  return _consistency;
}

void
PacEmcTMInfo::setConsistency(const Consistency &c) {
  _consistency= c;
}

const HepPoint
PacEmcTMInfo::getEntrancePoint() const
{
  return _entryPoint;
}

void
PacEmcTMInfo::setPoca(const TrkPocaBase& poca) {
  delete _poca;
  _poca = poca.clone();
}

void
PacEmcTMInfo::print( ostream &o ) const {

  o << "Calo= "<<(void*)_calo<<"  Trk= "<<(void*)_track<<"\n"
    << "EntryPoint= " << _entryPoint << "\n";
  o<<" deltaXY= "<<_deltaXY<<" deltaZ= "<<_deltaZ<<"\n";
  this->getConsistency().print(o);
//   o<<" consistency= " << this->getConsistency().consistency() << "\n";
//   o<<" likelihood= " << this->getConsistency().likelihood() << "\n";
}

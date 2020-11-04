//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TrkEnv.cc 127 2010-05-19 07:12:48Z stroili $
//
// Description:
//	Class TrkEnv
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Dave Brown
//
// Copyright Information:
//	Copyright (C) 1995, 1996, 1997	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "TrkEnv/TrkEnv.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "AbsEnv/AbsEnv.hh"


#include "ErrLogger/ErrLog.hh"

#include "GenEnv/GenEnv.hh"

#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/AbsArgVal.hh"

#include "TrkBase/TrkVolume.hh"
#include "TrkEnv/TrkTimeAdjuster.hh"

#include <algorithm>
using std::cout;
using std::endl;
using std::ostream;


static const char rcsid[] = "$Id: TrkEnv.cc 127 2010-05-19 07:12:48Z stroili $";

//
//  Default constructor
//
TrkEnv::TrkEnv() : _theOrigin(0.0,0.0,0.0),
  _theKalContext(0),_useNominalField(false),_nominalFieldValue(0.0)
{;}
//
//--------------
// Destructor --
//--------------
TrkEnv::~TrkEnv( )
{
  delete _theKalContext;
}
//-------------
// Operators --
//-------------
//
//  Selection function
//

const BField* 
TrkEnv::magneticField(const char* fieldname) const{
  return Ifd<BField>::get(gblPEnv, fieldname);
}


const TrkIdManager* 
TrkEnv::idManager(const char* name) const{
  return Ifd<TrkIdManager>::get(gblPEnv, name);
}

//
const TrkVolume*
TrkEnv::findTrkVolume( TrkVolumeHandle::trkvolumes vol ) const
{
  std::vector< TrkVolumeHandle >* vols =
    Ifd< std::vector< TrkVolumeHandle > >::get( gblPEnv,"Tracking Volumes" );
  if(vols != 0){
    TrkVolumeHandle in(0,vol);
    std::vector<TrkVolumeHandle>::iterator i=std::find(vols->begin(),vols->end(),in);
    if (i!=vols->end()) return i->volume();
  }
  return 0;
}

//  List volumes
void
TrkEnv::listTrkVolumes(ostream& os) const{
  os << "Currently recognized track volumes as follows: " << endl;
  std::vector< TrkVolumeHandle >* vols =
    Ifd< std::vector< TrkVolumeHandle > >::get( gblPEnv,"Tracking Volumes" );
  if(vols != 0){
//    unsigned nvol = vols->size();
    typedef std::vector<TrkVolumeHandle> TrkVolumeHandles;
    for(TrkVolumeHandles::iterator ivol=vols->begin(); ivol!=vols->end();++ivol)
      cout << ivol->volume()->name() << endl;
  }
}

// detector set function
const DetSet*
TrkEnv::trkDetSet() const {
  return Ifd< DetSet >::get( gblPEnv,"Tracking Set" );
}
//
//  statics used to control detector useage in proxys
//
bool
TrkEnv::_usedets[TrkEnv::nDetectors] = {false,false,false,false,false,false};

void
TrkEnv::useDetector(TrkEnv::detectors thedet) {
  _usedets[thedet] = true;
}

bool
TrkEnv::detectorUsed(TrkEnv::detectors thedet) {
  return _usedets[thedet];
}

void 
TrkEnv::setToUseNominalField(double nominalFieldValue){
  _useNominalField = true;
  _nominalFieldValue = nominalFieldValue;
}


const TrkTimeAdjuster* 
TrkEnv::getTrkTimeAdjuster(const char* keyname) const {
  if(keyname == 0)
    return Ifd< TrkTimeAdjuster >::get( gblPEnv);
  else
    return Ifd< TrkTimeAdjuster >::get( gblPEnv, keyname);
}

const TrkPassive*
TrkEnv::getTrkPassive(const char* keyname) const {
  if(keyname == 0)
    return Ifd< TrkPassive >::get( gblPEnv);
  else
    return Ifd< TrkPassive >::get( gblPEnv, keyname);
}

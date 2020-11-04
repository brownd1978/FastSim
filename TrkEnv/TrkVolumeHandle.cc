
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TrkVolumeHandle.cc 127 2010-05-19 07:12:48Z stroili $
//
// Description:  TrkVolumeHandle
//
// Environment:
//	Software developed for the BaBar Volume at the SLAC B-Factory.
//
// Author List:
//        David Brown 10-5-98
//
// Copyright Information:
//	Copyright (C) 1998	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "TrkEnv/TrkVolumeHandle.hh"

TrkVolumeHandle::TrkVolumeHandle() : _volume(0), _volid(none)
{;}

TrkVolumeHandle::TrkVolumeHandle(const TrkVolume* vol,trkvolumes volid) :
  _volume(vol), _volid(volid)
{;}

TrkVolumeHandle::TrkVolumeHandle(const TrkVolumeHandle& other) :
  _volume(other._volume), _volid(other._volid)
{;}

TrkVolumeHandle::~TrkVolumeHandle(){;}

TrkVolumeHandle&
TrkVolumeHandle::operator = (const TrkVolumeHandle& other) {
  if(&other != this) {
    _volume = other._volume;
    _volid = other._volid;
  }
  return *this;
}


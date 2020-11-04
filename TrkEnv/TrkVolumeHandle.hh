//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: TrkVolumeHandle.hh 103 2010-01-15 12:12:27Z stroili $
//
// Description:  TrkVolumeHandle
//      Small class to keep track of TrkVolumes
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

#ifndef TRKVOLUMEHANDLE_HH
#define TRKVOLUMEHANDLE_HH

class TrkVolume;

class TrkVolumeHandle {
public:
// enum to define the track volumes (logically)
  enum trkvolumes{none=0,beampipe,svt,dch,drc,emc,ifr};
  TrkVolumeHandle();
  TrkVolumeHandle(const TrkVolume* vol,trkvolumes volid);
  TrkVolumeHandle(const TrkVolumeHandle&);
  ~TrkVolumeHandle();
  TrkVolumeHandle& operator = (const TrkVolumeHandle& other);
  const TrkVolume* volume() const { return _volume; }
  trkvolumes volumeId() const { return _volid; }
// rw-required functions
  int operator == (const TrkVolumeHandle& other) const {
    return _volid == other.volumeId(); }
  int operator != (const TrkVolumeHandle& other) const {
    return _volid != other.volumeId(); }
  int operator < (const TrkVolumeHandle& other) const {
    return _volid < other.volumeId(); }
private:
  const TrkVolume* _volume; // the volume pointed to by this handle
  trkvolumes _volid; // whose volume it is
};

#endif

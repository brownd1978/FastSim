// ------------------------------------------------------------------------------
// File and Version Information:
//   PacDetSet: subclass of DetSet for PacDetSets
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14July2009
// ------------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "PacGeom/PacDetSet.hh"
#include "PacGeom/PacDetElem.hh"
#include "PacEnv/EdmlDetVolume.hh"
#include "ErrLogger/ErrLog.hh"

PacDetSet::PacDetSet(const EdmlDetVolume* evol, int index) :
  DetSet(evol->name().c_str(),index),
  _zshift(evol->zShift())
{}

PacDetSet::~PacDetSet() {
}

void
PacDetSet::append(DetElem* elem){
  PacDetElem* pelem = dynamic_cast<PacDetElem*>(elem);  
  if(pelem != 0){
    pelem->applyZShift(_zshift);
  } else {
    ErrMsg(error) << "Element is not a known Pac type!  No volume parameters applied" << endmsg;
  }
// append to the base class
  *this += *elem;
}

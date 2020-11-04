// ------------------------------------------------------------------------------
// File and Version Information:
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacPlaneDetType.hh"
#include "DetectorModel/DetMaterial.hh"
//
PacPlaneDetType::PacPlaneDetType(double thick,const char* name,const DetMaterial* themat,int idnum) :
  DetSurfaceType(name,idnum),_thick(thick),_thematerial(themat)
{}
//  Destructor
PacPlaneDetType::~PacPlaneDetType(){;}

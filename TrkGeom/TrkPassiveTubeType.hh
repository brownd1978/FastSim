// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkPassiveTubeType.hh 105 2010-01-15 12:14:11Z stroili $
//
//  Description:
//  DetType for describing passive tubes in the tracking volume (beampipe,
//  shields, support tube, etc.).  This also used as a crude model of the
//  DCH as cylindrical cylinders of compresssed gas.
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 3/20/97
// ------------------------------------------------------------------------------
#ifndef TRKPASSIVETUBETYPE_HH
#define TRKPASSIVETUBETYPE_HH
//
//  Includes
//
#include "DetectorModel/DetSurfaceType.hh"

class DetMaterial;
//
//  Define the class
//
class TrkPassiveTubeType: public DetSurfaceType {
public:
//
//  Constructor; build from the primitive
//
  TrkPassiveTubeType(double thick,double zends[2],
		     const DetMaterial*,int);
//  Destructor
  virtual ~TrkPassiveTubeType();
//
//  real versions of the virtual functions
//
  bool physicalMaterial(const TypeCoord*) const ;
  const DetMaterial& material(const TypeCoord*) const { return *_thematerial;}
  double thickness(const TwoDCoord*) const { return _thick; }
//
//  Access
//
private:
  double _thick; // tube wall thickness
  double _zend[2]; // z position of the std::ends
//
//  Approximate outline, in 2-D type coordinates
//
  int _nperimeter;
//
//  Material description.  This is a hack, ultimately this should point
//  back to a database object
//
  const DetMaterial* _thematerial; // material of this type
};
#endif

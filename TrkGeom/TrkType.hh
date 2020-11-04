// ------------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkType.hh 105 2010-01-15 12:14:11Z stroili $
//
//  Description:
//  DetType for describing the beampipe as a passive tubes in the tracking volume divided into
//  annular regions (in Z).
//
// Copyright Information:
//	Copyright (C) 2001	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 7/11/01
// ------------------------------------------------------------------------------
#ifndef TRKBPTYPE_HH
#define TRKBPTYPE_HH
//
//  Includes
//
#include "DetectorModel/DetSurfaceType.hh"
#include <vector>

class DetMaterial;
class TrkBPLayout;
class TrkSTLayout;
class TrkSvtSLayout;

//
//  Define the class
//
class TrkType: public DetSurfaceType {
public:
//
//  Constructor; build from the layout
//
  TrkType(unsigned idnum,const TrkBPLayout& layout);
  TrkType(unsigned idnum,const TrkSTLayout& layout);
  TrkType(unsigned idnum,const TrkSvtSLayout& layout,unsigned ielem);
//  Destructor
  virtual ~TrkType();
//
//  real versions of the virtual functions
//
  bool physicalMaterial(const TypeCoord*) const;
  const DetMaterial& material(const TypeCoord*) const;
  double thickness(const TwoDCoord*) const;
//
//  Access
//
private:
  unsigned _nsegs;
  std::vector<double> _thickness;
  std::vector<const DetMaterial*> _materials;
  std::vector<double> _zends;
// helper function
  int segment(double) const;
  const char* name() {return "TrkType";}

//
//  Approximate outline, in 2-D type coordinates
//
  int _nperimeter;
// disallow
  TrkType(const TrkType&);
  TrkType& operator = (const TrkType&);

  friend bool testCdb(const TrkType*, const TrkType*);

};
#endif

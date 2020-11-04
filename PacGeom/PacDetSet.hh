// ------------------------------------------------------------------------------
// File and Version Information:
//   PacDetSet: subclass of DetSet for PacDetElems
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14July2009
// ------------------------------------------------------------------------------

#ifndef PacDetSet_HH
#define PacDetSet_HH
#include "DetectorModel/DetSet.hh"
class EdmlDetVolume;

class PacDetSet : public DetSet {
  public:
// construct from an edml volume
	PacDetSet(const EdmlDetVolume*,int);
// destructor
  virtual ~PacDetSet();
// append elements
  void append(DetElem* elem);
// accessors
  double zShift() const { return _zshift;}
protected:
  double _zshift; // shift elements by this amount in Z, applied when elements are added
};

#endif

//--------------------------------------------------------------------------
// Name:
//   PacPieceTraj
// Description:
//   Piecewise helix, but removing all the 'diff' stuff.
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#ifndef PacPieceTraj_HH
#define PacPieceTraj_HH
#include "BbrGeom/TrkPieceTraj.hh"
#include "BbrGeom/TrkGeomTraj.hh"

class PacPieceTraj : public TrkPieceTraj {
public:
	PacPieceTraj();
	PacPieceTraj(const PacPieceTraj&);
  PacPieceTraj* clone() const;
  virtual ~PacPieceTraj();
// append a piece.  If this isn't the first piece, it's
// initial point must fit on the trajectory at flightlength flt
// note this object -TAKES OWNERSHIP- of the trajs passed to it
  void append(const double& flt,TrkGeomTraj*);
};
#endif


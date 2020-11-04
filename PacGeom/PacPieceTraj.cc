//--------------------------------------------------------------------------
// Name:
//   PacPieceTraj
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 23 APR 2008
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacGeom/PacPieceTraj.hh"
#include "CLHEP/Geometry/HepPoint.h"

PacPieceTraj::PacPieceTraj()
{}

PacPieceTraj::PacPieceTraj(const PacPieceTraj& other) : TrkPieceTraj(other) {}

PacPieceTraj* 
PacPieceTraj::clone() const
{ return new PacPieceTraj(*this);}

PacPieceTraj::~PacPieceTraj()
{}

void
PacPieceTraj::append(const double& flt,TrkGeomTraj* piece ){
	if(_traj.size() == 0) {
// first piece: just append, taking this piece as the dfn. of flightrange
		_traj.push_back(std::make_pair(piece->lowRange(),piece));
		flightrange[0] = piece->lowRange();
	} else {
// truncate the previous piece
		double locflt;
		TrajIter iend = trajIndex(flt,locflt);
		TrkGeomTraj* last = _traj.back().second;
		if(iend->second == last){
			double locrange[2] = {last->lowRange(),locflt};	
			last->setFlightRange(locrange);
		} else {
			std::cout << "appending before last trajectory" << std::endl;
			double fltrange[2] = {lowRange(),flt};
			setFlightRange(fltrange);
		}
		_traj.push_back(std::make_pair(flt,piece));
// check
		HepPoint pos = position(flt);		
		HepPoint ppos = piece->position(piece->lowRange());
    HepPoint lpos = last->position(last->hiRange());
		if(pos.distanceTo(ppos) > 1e-3 || pos.distanceTo(lpos) > 1e-3)
			std::cerr << "New piece doesn't match to end" << std::endl;
	}
}

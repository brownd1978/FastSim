//
// $Id: GTrimMarker.cc 430 2010-01-14 15:20:24Z stroili $
//
// GTrimMarker
// -----------
// Stores information on a set of tracks (of common ancestory)
// generated in the simulation but not saved in GTrack objects.
//
// This object belongs to the framework, and is usually stored
// in a HepAlist.
//
// In Bogus, each generated particle is represented in the 
// framework by one and only one of the following:
//         1. In its own GTrack
//         2. As a member of a GTrimMarker
// In the case of (1), the particle was considered important enough
// to preserve into its own GTrack. In the case of (2), the particle
// was not saved in GTrack to save space and because it doesn't carry
// any important information (an example would be particles
// created inside a shower).
//
// --------------------------------------------------
// Copyright (c) 2000      BaBar Collaboration
//
#include "BaBar/BaBar.hh"
#include <iomanip>

#include "G3Data/GTrimMarker.hh"

#include "G3Data/GVertex.hh"
using std::ostream;
using std::setprecision;
using std::setw;

//
// ostream format
//
//         Origin  ------------- Four Momentum ---------------   Number
//  Index  Vertex         X          Y          Z          E       Part  Type
//1234567 1234567   123456789  123456789  123456789  123456789  1234567  1234
//

const char *GTrimMarker::ostreamHeader
	= "         Origin  ------------- Four Momentum ---------------   Number\n  Index  Vertex         X          Y          Z          E       Part  Type";

//
// Constructor
//
GTrimMarker::GTrimMarker()
	: _fourMomentum(0),
	  _vertex(0),
	  _numParticle(0),
	  _index(unassigned),
	  _type(unknown)       {;}

//
// iostream globals
//
ostream &operator<<(ostream &stream, const GTrimMarker &mark)
{
	if (mark.index() == GTrimMarker::unassigned)
		stream << "     --";
	else
		stream << setw(7) << mark.index();

	if (mark.vertex())
		stream <<  setw(8) << mark.vertex()->index();
	else
		stream << "      --";

	stream << " "
	       << setw(11) << setprecision(4) << mark.p4().x()
	       << setw(11) << setprecision(4) << mark.p4().y()
	       << setw(11) << setprecision(4) << mark.p4().z()
	       << setw(11) << setprecision(4) << mark.p4().e();

	stream << setw(9) << mark.numParticle();
	
	stream << setw(6) << mark.type();
	
	return stream;
}

ostream &operator<<(ostream &stream, const GTrimMarker *mark)
{
	stream << *mark;
	return stream;
}

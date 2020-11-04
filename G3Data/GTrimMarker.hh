//
// $Id: GTrimMarker.hh 430 2010-01-14 15:20:24Z stroili $
//
// GTrimMarker
// ------
// Stores information on a set of tracks (of common ancestory)
// generated in the simulation but not saved in a GTrack object.
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
// was not saved in GTrack to save space (these would be particles
// created, for example, inside a shower).
//
// Units are cm, GeV, second
//
// --------------------------------------------------
// Copyright (c) 2000      BaBar Collaboration
//

#ifndef GTrimMarker_HH
#define GTrimMarker_HH

#include <iostream>

#include "AbsEvent/AbsEvtObj.hh"

#include "CLHEP/Vector/LorentzVector.h"
using CLHEP::HepLorentzVector;

class GVertex;

class GTrimMarker : public AbsEvtObj {
	public:
	//
	// Default constructor.
	// Note that we rely on the compiler generated copy constructor
	// and assignment operator, which is safe for the data types
	// stored in this class.
	//
	GTrimMarker();
	~GTrimMarker() {;}
	
	//
	// Constants
	//
	enum { unassigned = -1 };
	
	enum MarkerType {
		unknown,
		standard,
		showerParamaterization
	};
	
	//
	// Equivalence operator, for compatibility with containers
	//
	bool operator==( const GTrimMarker &other ) const { 
		if ( _index == unassigned ) return false;
		return _index == other._index; 
	};
	
	//
	// =========== Print methods
	//
	virtual void printAll (std::ostream &o = std::cout) const {o << *this; }   
	virtual void print (std::ostream &o = std::cout) const {o << *this; }
	
	friend std::ostream &operator<<(std::ostream &, const GTrimMarker&);
	friend std::ostream &operator<<(std::ostream &, const GTrimMarker*);
	
	//
	// A nice string for printing a header for the << operators
	// For example:
	//        cout << GTrimMarker::ostreamHeader << endl;
	//        cout << marker << endl;
	//
	static const char *ostreamHeader;
	
	//
	// Return four momentum at origin.
	//
	HepLorentzVector p4() const { return _fourMomentum; }
	void setP4( const HepLorentzVector &p4 ) { _fourMomentum = p4; }
	
	//
	// Access vertex associated with the interaction
	// that is responsible for all member particles
	//
	GVertex *vertex() const { return _vertex; }
	void setVertex( GVertex *v ) { _vertex = v; };
	
	//
	// Access index
	//
	signed long index() const { return _index; }
	void setIndex( signed long newIndex ) { _index = newIndex; }
	
	//
	// Access number particles
	//
	unsigned long numParticle() const { return _numParticle; }
	void setNumParticle( unsigned long n ) { _numParticle = n; }
	void incrementNumParticle() { _numParticle++; }
	void decrementNumParticle() { _numParticle--; }
	
	//
	// Access type
	//
	GTrimMarker::MarkerType type() const { return _type; }
	void setType( GTrimMarker::MarkerType newType ) { _type = newType; }
	
	
	protected:
	
	//
	// _fourMomentum: the four-momentum of the particle in the
	// lab frame at the point of origin (as stored in GVertex)
	//
	HepLorentzVector _fourMomentum;
	
	//
	// _vertex: the GVertex object representing the origin
	// of this particle. Not owned by GTrack.
	//
	GVertex *_vertex;
	
	//
	// _numParticle: the number particles this marker is
	// representing
	//
	unsigned long _numParticle;

	//
	// _index: the index of this object in the owning HepAList,
	// or GTrimMarker::unassigned
	//
	signed long _index;
	
	//
	// _type: the type of marker
	//
	MarkerType _type;
};

#endif

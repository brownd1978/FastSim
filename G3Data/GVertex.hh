//
// $Id: GVertex.hh 521 2010-02-03 20:35:12Z brownd $
//
// GVertex
// -------
// Stores information on the interactions in a simulated event.
//
// This object belongs to the framework, and is usually stored
// in a HepAlist.
//
// In general, only the following vertices are saved in GVertex:
//        1. If the vertex marks the start or end of a 
//           particle stored in GTrack.
//        2. If the vertex marks the start of a GTrimMarker.
//
// Units are cm, GeV, second
//
// --------------------------------------------------
// Copyright (c) 2000      BaBar Collaboration
//

#ifndef GVertex_HH
#define GVertex_HH

#include <iostream>

#include "CLHEP/Geometry/HepPoint.h"

#include "AbsEvent/AbsEvtObj.hh"

class GTrack;
class GTrimMarker;

class GVertex : public AbsEvtObj {
	public:
	GVertex();
	~GVertex() {;}
	
	//
	// Constants
	//
	enum { unassigned = -1 };
	
	//
	// There is some attempt to preserve the ordering
	// of Cause to group different types of processes together.
	// Try to follow this method if you add new entries.
	//
	// For example, the following application code would be supported:
	//        if (vertex->Cause >= GVertex::comptonScattering &&
	//            vertex->Cause <= GVertex::emOther                ) {
	//		cout << "Interaction was electro-magnetic" << endl;
	//	  }
	//
	enum Cause {
		unknown=0,		// Unknown -- either BBSIM or probably a program error
		generator=10,		// Was made by the generator (e.g. GenFwkInt)
		
		preassignedDecay=11,    // A decay performed by GEANT but in a decay mode
					//     preassigned by the generator
		
		decay=100,		// Decay of a particle by GEANT (using G4's decay tables)
		
		comptonScattering=200,	// EM type effects
		gammaConversion,
		ionization,			// (including delta-ray production)
		photoElectricEffect,
		bremsstrahlung,
		eplusAnnihilation,
		muonPairProduction,
		emOther=299,
		
		hadronInelastic=300,	// Hadronic type effects
		hadronElastic,
		hadronCapture,			// (including annihilation, fission)
		gammaNuclear,			// gamma-induced nuclear interactions
		electroNuclear,			// e+/e- induced nuclear interactions
		muonNuclear,			// Muon interactions
		hadronOther=399,
		
		gammaAbsorption=500,	// Optical effects
		gammaRayleigh,
		opticalOther=599,
		unknownBackground=600, // backgrounds
		beamstrahlung=601,
		radBhabha=602,
		Bhabha=603,
		pair=604,
		Touschek=605,
		beamneutron=606,		
		
		minimumEnergy=900,	// Was stopped because the particle had
		                        //     too little energy
		outOfTime,		// Was stopped because particle is out of sensitive
					//     time of detector
		neutrino,		// Was stopped because neutrinos aren't tracked
		stopOther=999,
					
		paramAlbedo=1000,	// Simulated albedo from a parameterized shower
		paramOther=1099,

		runAway=10000,		// Was killed because the particle took too
					//     many steps in the simulation

		looperDeath,		// Was stopped by BgsLooperDeath
		
		trackingError=10100	// Oops
	};

	
	//
	// Equivalence operator, for compatibility with containers
	//
	bool operator==( const GVertex &other ) const { 
		if ( _index == unassigned ) return false;
		return _index == other._index; 
	};
	
	//
	// =========== Print methods
	//
	virtual void printAll (std::ostream &o = std::cout) const {o << *this; }   
	virtual void print (std::ostream &o = std::cout) const {o << *this; }
	
	friend std::ostream &operator<<(std::ostream &, const GVertex&);
	friend std::ostream &operator<<(std::ostream &, const GVertex*);

	//
	// A nice string for printing a header for the << operators
	// For example:
	//        cout << GVertex::ostreamHeader << endl;
	//        cout << vertex << endl;
	//
	static const char *ostreamHeader;
	
	//
	// Return string description of cause
	//
	static const char *causeName( Cause cause );
	
	//
	// =========== Standard accessors
	//
	GVertex::Cause cause() const { return _cause; }
	void setCause( GVertex::Cause newCause ) { _cause = newCause; }	

	HepPoint position() const { return _position; }
	void setPosition( const HepPoint &p ) { _position = p; }
	
	HepDouble time() const { return _time; }
	void setTime( const HepDouble &t ) { _time = t; }

        bool parentIsImmediate() const { return _parentIsImmediate; }
        void setParentIsImmediate( bool newStat ) { _parentIsImmediate = newStat; }
	
	GTrack *parentTrack() const { return _parent; }
	void setParentTrack( GTrack *newParent ) { _parent = newParent; }
	
	GTrimMarker *parentTrimMarker() const { return _trimParent; }
        void setParentTrimMarker( GTrimMarker *newMark );
	
	bool terminal() const { return _terminal; }
	void setTerminal( bool wasTerminal ) { _terminal = wasTerminal; }
	
	signed long index() const { return _index; }
	void setIndex( signed long newIndex ) { _index = newIndex; }
	
	
	//
	// ===========  Depreciated methods
	// These are retained for backward
	// compatibility with the old dbio generated ones.
	//
	// Applications should migrate to the newer accessors above.
	//
	// Please note that n() counts from 1 (FORTRAN style),
	// whereas index, above, counts from 0.
	//
	public: 
	int n() const { return _index+1; }
	void setN(const int i) { _index = i-1; }
	
	int parent() const;
	void setParent(const int i) {;}		// Do nothing!
	
	float x() const { return _position.x(); }
	void setX(const float x) { _position.setX(x); }
	
	float y() const { return _position.y(); }
	void setY(const float y) { _position.setY(y); }
	
	float z() const { return _position.z(); }
	void setZ(const float z) { _position.setZ(z); }
	
	float tof() const { return _time; }
	void setTof(const float t) { _time = t; }
	

	protected:	

	//
	// _cause: reason why this vertex came into being
	//
	Cause _cause;
	
	//
	// _pos: position of this vertex
	//
	HepPoint _position;
	
	//
	// _time: relative time of this vertex
	//
	HepDouble _time;

	//
	// _parent: oldest progenitor stored in a GTrack.
	// If _trimParent is zero, this is also the particle
	// that created the interaction.
	//
	GTrack *_parent;
	
  bool _parentIsImmediate;

	// 
	// _trimParent: if this vertex was caused by a particle that
	// was not saved to GTrack, then this pointer points to
	// the associated GTrimMarker. Otherwise, this pointer is zero.
	//
	GTrimMarker *_trimParent;
	
	//
	// _terminal: true if this vertex killed its parent particle
	//
	bool _terminal;

	//
	// _index: the index of this object in the owning
	// HepAList. Counting from zero. Set to GVertex::unassigned
	// if this object is not in a list.
	//
	// In BBSIM, we expect:
	//      HepAList<GTrack> gtracks;
	//      gtracks[n]->n() == n+1;
	// However, it is not clear if this is guaranteed!
	// See, for example, SimUtils/G3accessor
	//
	signed long _index;
};

#endif

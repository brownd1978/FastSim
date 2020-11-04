//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: GTrack.cc 430 2010-01-14 15:20:24Z stroili $
//
// Description:
//      Class GTrack implementation file
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Torre Wenaus                 LLNL
//      Ed Frank                     University of Pennsylvania
//      David Williams               UC Santa Cruz
//
// History:
//      ?? ??? 95   T. Wenaus        create
//      23 Jan 96   Ed Frank         add *Store.dcc C++->FORTRAN copyback(dbio)
//      18 May 00   DCW              Rewrite
//      02 Mar 02   Bill Lockman     Added int  numDaughters()
//                                   void addDaughters()
//                                   void removeAllDaughters()
//                                   const GTrack* daughter(k) const
//                                   std::vector<const GTrack *> _daughterList
//      22 Oct 03   Tim Barrass      added parentIsImmediate()
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include <iomanip>

#include "G3Data/GTrack.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "G3Data/GVertex.hh"
using std::ios;
using std::ostream;
using std::resetiosflags;
using std::setiosflags;
using std::setprecision;
using std::setw;

//
// ostream format
//
//                        Origin    Term  ---------- Four Momentum (GeV) ------------
//  Index Particle        Vertex  Vertex         X          Y          Z          E
//1234567 12345678901234 1234567 1234567   123456789  123456789  123456789  123456789
//

const char *GTrack::ostreamHeader
	= "                        Origin    Term  ---------- Four Momentum (GeV) ------------\n  Index Particle        Vertex  Vertex         X          Y          Z          E";

//
// Default constructor
//
GTrack::GTrack()
  : _fourMomentum(0)
  , _vertex(0)
  , _terminalVertex(0)
  , _pdt(0)
  , _index( GTrack::unassigned )
  , _hepid(0) 
  , _daughterList(0)
{;}

//
// Constructor taking a four vector
//
GTrack::GTrack( const HepLorentzVector &v )
  : _fourMomentum(v)
  , _vertex(0)
  , _terminalVertex(0)
  , _pdt(0)
  , _index( GTrack::unassigned )
  , _hepid(0)
  , _daughterList(0)
{;}

//
// Destructor
//
GTrack::~GTrack(){;}

//
//
//
// iostream globals
//
ostream &
operator<<(ostream &stream, const GTrack &track)
{
  if (track.index() == GTrack::unassigned) 
    stream << "     -- ";
  else
    stream << setw(7) << track.index() << " ";
  
  stream << setiosflags(ios::left);
  stream << setw(14) << (track.pdt() ? track.pdt()->name() : "(unknown)");
  stream << resetiosflags(ios::left);
  
  if (track.vertex())
    stream << setw(8) << track.vertex()->index();
  else
    stream << "      --";
  
  if (track.terminalVertex())
    stream << setw(8) << track.terminalVertex()->index();
  else
    stream << "      --";
  
  stream << " " << setiosflags(ios::fixed)
	 << setprecision(4) << setw(11) << track.p4().x()
	 << setprecision(4) << setw(11) << track.p4().y()
	 << setprecision(4) << setw(11) << track.p4().z()
	 << setprecision(4) << setw(11) << track.p4().e();
  
  return stream;
}

ostream &
operator<<(ostream &stream, const GTrack *track)
{
  stream << *track;
  return stream;
}

//
// parentIsImmediate
//
// If the parent particle was stored in a GTrack then
// it is regarded as immediate.
//
bool GTrack::parentIsImmediate() const
{
  if ( _vertex == 0 ) return true;
  return _vertex->parentIsImmediate();
}

//
// parentTrack
//
// Return the youngest parent saved to a GTrack. We locate
// this track by accessing the vertex.
//
GTrack* GTrack::parentTrack() const
{
  if (_vertex == 0) return 0;
  return _vertex->parentTrack();
}

//
// parentTrimMarker
//
// As in parentTrack, we access the trim marker via the vertex.
//
GTrimMarker *GTrack::parentTrimMarker() const
{
	if (_vertex == 0) return 0;
	
	return _vertex->parentTrimMarker();
}

//
// ultimateParent
//
// Search backwards until we find a vertex that
// was created by the generator. The daughter of
// this vertex will be the answer.
//
// In principle, we should never need to return zero.
//
const GTrack*
GTrack::ultimateParent() const
{
  const GTrack *answer(this);
  
  for(;;) {
    const GVertex *pVertex = answer->vertex();
    if (pVertex == 0) break;
    
    if (pVertex->cause() > GVertex::unknown &&
	pVertex->cause() <= GVertex::generator) break;
    
    const GTrack *pTrack = pVertex->parentTrack();
    if (pTrack == 0) break;
    
    answer = pTrack;
  }
  return answer;
}

//
// ivtx
//
// Implement backwards compatible method
//
int 
GTrack::ivtx() const
{
	if (_vertex == 0) return 0;
	
	return _vertex->index()+1;
}


//
// id
//
// Implement almost backward compatible method
//
int 
GTrack::id() const
{
	return _pdt ? _pdt->geantId() : 0;
}

//
// setId
//
// Implement backward compatible method
//
void 
GTrack::setId( const int odd )
{
  // Comments from original GTrack.hh, whose behavior we must now preserve
  // ----------------------------------------------------------------------------
  // that's a bit dirty:
  // pdg code (used by bogus) has sign information. to be able to differentiate 
  // from geant code (used by bbsim) a 500000 offset has been added.
  // we test from 250000 as a security because for D2*- = -20413 for example
  // the id = 500000 - 20413 = 479687 and what if we discover new excited states 
  // with babar and have to give them a pdg number ?? 
  // This limit is then safe enough (I believe)
  // This can be simplified when BBsim will be abandonned !
  // But don't forget to remove the offset in BgsFwkModule/BfmGTransformer
  // ----------------------------------------------------------------------------
  if ( odd > 250000 ) { 
    _pdt = Pdt::lookup(Pdt::lundId(odd-500000)); 
  }
  else { // geant code (bbsim) 
    _pdt = Pdt::lookup(Pdt::geantId(odd)); 
  } 
}

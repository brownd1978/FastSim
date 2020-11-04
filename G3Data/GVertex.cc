//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: GVertex.cc 703 2010-09-13 11:47:05Z stroili $
//
// Description:
//      Class GVertex implementation file
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      ??
//      Ed Frank                     University of Pennsylvania
//      David Williams               UC Santa Cruz
//
// History:
//      ?? ??? ??   ??               create
//      23 Jan 96   Ed Frank         add *Store.dcc C++->FORTRAN copyback(dbio)
//      18 May 00   DCW              rewrite
//      22 Oct 03   Tim Barrass      added check for immediate parent
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include <iomanip>

#include "G3Data/GVertex.hh"
#include "G3Data/GTrack.hh"
#include "G3Data/GTrimMarker.hh"
using std::ios;
using std::ostream;
using std::resetiosflags;
using std::setiosflags;
using std::setprecision;
using std::setw;

//
// ostream format
//
//          Track    Trim  --------- Vertex (cm) ----------
//  Index  Parent  Parent         X          Y          Z         Time    Cause
//1234567 1234567 1234567   123456789  123456789  123456789   123456789  *123456
//

const char
    *GVertex::ostreamHeader =
        "          Track    Trim  --------- Vertex (cm) ----------\n  Index  Parent  Parent         X          Y          Z        Time     Cause";

//
// Constructor
//
GVertex::GVertex() :
  _cause(unknown), _position(0), _time(0), _parent(0),
      _parentIsImmediate(true), _trimParent(0), _terminal(false), _index(
          unassigned)
{
  ;
}

//
// iostream globals
//
ostream &
operator<<(ostream &stream, const GVertex &vert)
{
  if (vert.index() == GVertex::unassigned) stream << "     --";
  else stream << setw(7) << vert.index();

  if (vert.parentTrack()) stream << setw(8) << vert.parentTrack()->index();
  else stream << "      --";

  if (vert.parentTrimMarker()) stream << setw(8)
      << vert.parentTrimMarker()->index();
  else stream << "      --";

  stream << " " << setiosflags(ios::fixed) << setprecision(4) << setw(11)
      << vert.position().x() << setprecision(4) << setw(11)
      << vert.position().y() << setprecision(4) << setw(11)
      << vert.position().z() << " " << setiosflags(ios::scientific)
      << setprecision(4) << setw(11) << vert.time() << resetiosflags(
      ios::scientific);

  stream << (vert.terminal() ? "  *" : "   ");

  stream << setiosflags(ios::left) << GVertex::causeName(vert.cause())
      << resetiosflags(ios::left);

  return stream;
}

ostream &
operator<<(ostream &stream, const GVertex *vert)
{
  stream << *vert;
  return stream;
}

const char *
GVertex::causeName(GVertex::Cause cause)
{
  static const char *name;
  switch (cause) {
  case unknown:
    name = "unknown/error";
    return name;
    break;
  case generator:
    name = "generator decay";
    return name;
    break;
  case preassignedDecay:
    name = "preassigned decay";
    return name;
    break;
  case decay:
    name = "decay";
    return name;
    break;
  case comptonScattering:
    name = "compton scattering";
    return name;
    break;
  case gammaConversion:
    name = "gamma conversion";
    return name;
    break;
  case ionization:
    name = "ionization";
    return name;
    break;
  case photoElectricEffect:
    name = "photoelectric effect";
    return name;
    break;
  case bremsstrahlung:
    name = "bremsstrahlung";
    return name;
    break;
  case eplusAnnihilation:
    name = "e+ annihilation";
    return name;
    break;
  case muonPairProduction:
    name = "muon pair production";
    return name;
    break;
  case emOther:
    name = "other EM effect";
    return name;
    break;
  case hadronInelastic:
    name = "inelastic hadronic";
    return name;
    break;
  case hadronElastic:
    name = "elastic hadronic";
    return name;
    break;
  case hadronCapture:
    name = "hadron capture";
    return name;
    break;
  case gammaNuclear:
    name = "gamma nuclear";
    return name;
    break;
  case electroNuclear:
    name = "electro nuclear";
    return name;
    break;
  case muonNuclear:
    name = "muon nuclear";
    return name;
    break;
  case hadronOther:
    name = "other hadronic effect";
    return name;
    break;
  case gammaAbsorption:
    name = "gamma absorption";
    return name;
    break;
  case gammaRayleigh:
    name = "rayleigh scattering";
    return name;
    break;
  case opticalOther:
    name = "other optical effect";
    return name;
    break;
  case unknownBackground:
    name = "unknown background";
    return name;
    break;
  case beamstrahlung:
    name = "beamstrahlung";
    return name;
    break;
  case minimumEnergy:
    name = "energy too small";
    return name;
    break;
  case outOfTime:
    name = "out of time";
    return name;
    break;
  case neutrino:
    name = "neutrino not tracked";
    return name;
    break;
  case stopOther:
    name = "other stopping effect";
    return name;
    break;
  case paramAlbedo:
    name = "param shower albedo";
    return name;
    break;
  case paramOther:
    name = "other param effect";
    return name;
    break;
  case runAway:
    name = "run away";
    return name;
    break;
  case looperDeath:
    name = "looper";
    return name;
    break;
  case trackingError:
    name = "geant4 error";
    return name;
    break;
  default:
    name = "(unrecognized)";
    return name;
    break;
  }

  static const char *oops = "(unrecognized)";

  return oops;
}

//
// parent()
//
// Placed here to avoid including GTrack.hh in the header file
//
int
GVertex::parent() const
{
  return _parent ? _parent->index() : 0;
}

//
// setParentTrimMarker
//
// A little more complex than the other set methods: 
// as the presence of a TrimMarker means that  the parent
// particle was not immediate, we may need to change 
// _parentIsImmediate. HOWEVER! Some objects send 0,
// so we need to make sure we don't get fooled into
// thinking it's a real trim marker...
//
void
GVertex::setParentTrimMarker(GTrimMarker *newMark)
{
  _trimParent = newMark;
  if (_trimParent == 0) {
    setParentIsImmediate(true);
    //cout << "Set to true" << endl; 
  } else {
    setParentIsImmediate(false);
    //cout << "Set to false" << endl;
  }
}


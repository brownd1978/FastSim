//--------------------------------------------------------------------------
// Name:
//   PacMatInt
//  Description:
//  Class to describe discrete material interactions in Pacrat
// Environment:
//   Software developed for PACRAT / SuperB
// Copyright Information:
//	 Copyright (C) 2009	Lawrence Berkeley Laboratory
// Author List:
//   Dave Brown 2 June 2009
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include <algorithm>

#include "PacSim/PacMatInt.hh"
#include "PacSim/PacSimulate.hh"
#include "PacSim/PacSimTrack.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandPoisson.h"
#include "G3Data/GTrack.hh"
#include "DetectorModel/DetMaterial.hh"
#include "PacEnv/PacConfig.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtId.hh"
#include "GfiBase/GfiRandomManager.hh"
#include "EvtGenBase/EvtGenKine.hh"
#include "EvtGenBase/EvtVector3R.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include <algorithm>
using std::min;
using std::max;


// these numbers should come from configuration.  FIXME!!!
PacMatInt::PacMatInt() :
 _randengine(0) {
// initialize the daughter multiplicity parameters
  _offset = gconfig.getdouble("PacInteraction.nhaddau_offset",-999.);
  _slope =  gconfig.getdouble("PacInteraction.nhaddau_slope",-999.);
  _maxndau =gconfig.getint("PacInteraction.max_nhaddau",-999);
  _minmom_gamma = gconfig.getdouble("PacShower.minmom_gamma",-1.0);
// initialize the nucleon probability
  _nprob.push_back( gconfig.getdouble("PacInteraction.nucprob_0",-999.));
  _nprob.push_back(gconfig.getdouble("PacInteraction.nucprob_1",-999.));
  _nprob.push_back(gconfig.getdouble("PacInteraction.nucprob_2",-999.));
// require initialization
  assert(_offset > -1 && _slope > -1 && _maxndau > -1 && _nprob[0] > -1 && _nprob[1] > -1 && _nprob[2] > -1);
}
PacMatInt::~PacMatInt()
{}

bool
PacMatInt::isMatInt(PacSimHit::effect eff) { return
  eff == PacSimHit::brems ||
  eff == PacSimHit::compton ||
  eff == PacSimHit::interact ||
  eff == PacSimHit::convert;
}

bool
PacMatInt::isTerminal(PacSimHit::effect eff) { return
  eff == PacSimHit::interact ||
  eff == PacSimHit::convert;
}

void
PacMatInt::makeInteraction(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const {
  switch (hit.detEffect()) {
    case PacSimHit::brems:
    return brems(hit,vertex,dautrks);
    case PacSimHit::convert:
    return convert(hit,vertex,dautrks);
    case PacSimHit::compton:
    return compton(hit,vertex,dautrks);
    case PacSimHit::interact:
    return hadronic(hit,vertex,dautrks);
    default:
    ErrMsg(error) << "Unknown interaction effect " << hit.detEffect() << endmsg;
  }
}

void
PacMatInt::brems(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const {
// brems photons are produced parallel with the electron.  We don't worry about individual photons here
  Hep3Vector gammom = -hit.momentumChange();
  if(gammom.mag() > _minmom_gamma){
    HepLorentzVector gamp4(gammom,gammom.mag());
// daughter photon type, gets set first time this function is called
    static const PdtEntry* gamtype = Pdt::lookup(PdtPdg::gamma);
// generate a vertex.  This is non-terminal
    vertex = PacSimulate::makeGVertex(const_cast<GTrack*>(hit.getSimTrack()->getGTrack()),hit.position(),hit.time(),GVertex::bremsstrahlung,false);
    GTrack* gamtrk = PacSimulate::makeGTrack(vertex,gamp4,gamtype);
    dautrks.push_back(gamtrk);
  }
}

void
PacMatInt::convert(const PacSimHit& hit,GVertex*& vertex,std::vector<GTrack*>& dautrks ) const {
// conversion electrons energy fraction distribution is effectively flat
  Hep3Vector gammom = hit.momentumIn();
// available kinetic energy is the gamma energy minus twice the electron mass
  static const PdtEntry* etype[2]= {Pdt::lookup(PdtPdg::e_plus),Pdt::lookup(PdtPdg::e_minus)};
  static const double emass = etype[0]->mass();
  double egam = gammom.mag();
  double eenrg[2];
  eenrg[0] = RandFlat::shoot(_randengine,emass,egam - emass);
  eenrg[1] = egam-eenrg[0];
// electron direction is almost parallel to gamma.  Smear by a small amount
  static const double convanglerms(0.0062); // from full simulation
  double convangle[2];
  RandGaussQ::shootArray(_randengine,2,convangle,0.0,convanglerms);
// define a basis perpendicular to the gamma direction
  static Hep3Vector zaxis(0.0,0.0,1.0);
  Hep3Vector phidir = zaxis.cross(gammom).unit();
  Hep3Vector thetadir = phidir.cross(gammom).unit();
  static const double sign[2] = {1,-1};
// create the vertex
  vertex = PacSimulate::makeGVertex(const_cast<GTrack*>(hit.getSimTrack()->getGTrack()),hit.position(),hit.time(),GVertex::gammaConversion,true);
// create tthe electron gtracks
  for(unsigned iele=0;iele<2;iele++){
    double emommag = DetMaterial::particleMomentum(eenrg[iele],emass);
    Hep3Vector emom = gammom.unit() + sign[iele]*convangle[0]*phidir + sign[iele]*convangle[1]*thetadir;
    emom.setMag(emommag);
    HepLorentzVector ep4(emom, eenrg[iele]);
    GTrack* etrk = PacSimulate::makeGTrack(vertex,ep4,etype[iele]);
    dautrks.push_back(etrk);  
  }
}

void
PacMatInt::compton(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const {
  ErrMsg(fatal) << "Compton scattering not yet implemented! " << endmsg;
}

void
PacMatInt::hadronic(const PacSimHit& hit,GVertex*& vertex, std::vector<GTrack*>& dautrks ) const {
// storage for EvtGen
  static const int maxndau(10);
  static EvtVector4R p4[maxndau+1];
  static double mass[maxndau+1];
  static const PdtEntry* dtype[maxndau+1];
// Pdt entries for daughters
  static const PdtEntry* ntype[2]= {Pdt::lookup(PdtPdg::p_plus),Pdt::lookup(PdtPdg::n0)};
  static const PdtEntry* pitype[3]= {Pdt::lookup(PdtPdg::pi_plus),Pdt::lookup(PdtPdg::pi0),Pdt::lookup(PdtPdg::pi_minus)};
  double mommag = hit.momentumIn().mag();
// daughter multiplicity is described by a Poisson distribution whose mean depends on the incident particle momentum
  double lambda = _offset+mommag*_slope;
// limit the number of daughters: the phasespace generator becomes extremely inefficient for large numbers of daughters
  int ndau = std::min((int)RandPoisson::shoot(_randengine,lambda), _maxndau);
  if(ndau > 0) {
// Find the material
    const DetMaterial* mat(0);
    if(hit.detIntersection().delem != 0)
      mat = &(hit.detIntersection().delem->material(hit.detIntersection()));
    else {
      ErrMsg(error) << " material interaction hit has no material!!!" << endmsg;
      return;
    }
// take the nucleus mass as the nucleon mass times effective atomic number
    double mnucleus = mat->aeff()*ntype[0]->mass();
// compute the boost into the COM, and the kinetic energy in the COM
    const PdtEntry* pdt = hit.getSimTrack()->pdt();
    double ein = DetMaterial::particleEnergy(mommag,pdt->mass());
    double bval = mommag/(ein+mnucleus);
    Hep3Vector boost = hit.momentumIn().unit()*bval;
    EvtVector3R evtboost(boost.x(),boost.y(),boost.z());
    double ekin = sqrt(mnucleus*mnucleus + pdt->mass()*pdt->mass() + 2*mnucleus*ein) - mnucleus - pdt->mass();
//  Assign daughter masses; First, see if we're adding a nucleon
    int idau=0;
    bool nucleon(false);
    if(RandFlat::shoot(_randengine) < _nprob[std::min(ndau,2)]){
// choose nucleon species according to Z/A
      int inuc(1); // assume neutron
      if(RandFlat::shoot(_randengine) < mat->zeff()/mat->aeff())
        inuc = 0;
      dtype[idau] = ntype[inuc];
      mass[idau] = dtype[idau]->mass();
      idau++;
      nucleon = true;
    }
// loop over the remaining daughters assigning pion types.
// We try to balance pion charge (not nucleon, that comes from the nucleus).
    int charge = (int)rint(pdt->charge());
    while(idau < ndau){
      int ipi(1); // assume pi0
      if(charge!=0){
        ipi = charge>0 ? 2 : 0;
      } else if( ndau-idau>1){
// if charge is already balanced and there are at least 2 daughters left, pick the next pion based on isospin  
        ipi = (int)floor(RandFlat::shoot(_randengine,0.0,3.0));
      }
      dtype[idau] = pitype[ipi];
      mass[idau] = dtype[idau]->mass();
      charge += (int)rint(dtype[idau]->charge());
      idau++;
    }
// add a daughter for the nucleus recoil.  If there's a nucleon, subtract out its mass. NB: we should
// smear this to account for nuclear binding energy FIXME!!!!
    mass[idau] = mnucleus;
    if(nucleon)
      mass[idau] -= mass[0];
// total the energy by adding all the masses and the kinetic energy
    double ecom = ekin;
    for(int idau=0;idau<ndau+1;idau++)
      ecom += mass[idau];
// generate 4-vectors for these daughters (plus the recoil nucleus).  This is done in the COM
// Tell GfiRandomManager that we are generating events, so it's safe
// to generate random numbers:
    GfiRandomManager::setEventGeneration(true);
    EvtGenKine::PhaseSpace(ndau+1,mass,p4,ecom);
    GfiRandomManager::setEventGeneration(false);
// create the vertex
    GVertex::Cause cause = nucleon ? GVertex::hadronInelastic : GVertex::hadronElastic;
    vertex = PacSimulate::makeGVertex(const_cast<GTrack*>(hit.getSimTrack()->getGTrack()),hit.position(),hit.time(),cause,true);
// boost the real particles back to the lab frame, and convert to GTracks.  Note the difference in 4-vector
//  convention!
    for(int jdau=0;jdau<ndau;jdau++){
      p4[jdau].applyBoostTo(evtboost);
      HepLorentzVector dp4(p4[jdau].get(1),p4[jdau].get(2),p4[jdau].get(3),p4[jdau].get(0));
      GTrack* dtrk = PacSimulate::makeGTrack(vertex,dp4,dtype[jdau]);
      dautrks.push_back(dtrk);
    }
  }
}

//--------------------------------------------------------------------------
// Name:
//   PacWriteParticles
// Environment:
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "CLHEP/Random/RandFlat.h"
#include "PacSim/PacWriteParticles.hh"
#include "PacSim/PacSimTrack.hh"
#include "PacSim/PacSimHit.hh"
#include "PacEnv/PacConfig.hh"
#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

PacWriteParticles::PacWriteParticles()  :_nevtwrote(0),_nparwrote(0),_particles("TParticle",10000),
_evtnum(0),_nsum(1),_npar(0){
// get config information  
  _evtwt = gconfig.getfloat("ParticleOutput.weight",1.0);
  _fileName = gconfig.getcstr("ParticleOutput.filename","FastSimOutput.root");
  _treeName = gconfig.getcstr("ParticleOutput.treename","Particles");
  _statuscode = gconfig.getint("ParticleOutput.statuscode",100);
  _active = gconfig.getbool("ParticleOutput.active",false);
  _verbose = gconfig.getbool("ParticleOutput.verbose",false);
  _writeempty = gconfig.getbool("ParticleOutput.writeempty",false);
// setup the output file and its tree and branches
  if(_active){
    _output = new TFile(_fileName,"RECREATE");
    _tree = new TTree(_treeName,_treeName);
    _tree->Branch("EvtNr",&_evtnum);
    _tree->Branch("EvtW",&_evtwt);
    _tree->Branch("NEvt",&_nsum);
    _tree->Branch("NPar",&_npar);
    _tree->Branch("Particles",&_particles);
  }
}

PacWriteParticles::~PacWriteParticles()
{
// close the output
  if(_active && _output != 0){
    _output->cd();
    _tree->Write();
    _output->Write();
    _output->Close();
    delete _output; _output=0;
    std::cout <<  "PacWriteParticles wrote " << _nevtwrote << " events, " 
      << _nparwrote << " particles to file " << _fileName << std::endl;
  }
}

void
PacWriteParticles::writeParticle(const PacSimHit& pshit){
  if(_active){
// create the particle memory with placement new!!
    TParticle* part = new(_particles[_npar++]) TParticle();
// fill particle properties
    part->SetPdgCode(pshit.pdt()->pdgId());
    double mass = pshit.pdt()->mass();
  // take the momentum before the effect of this hit
    const Hep3Vector& mom = pshit.momentumIn();
    double energy = sqrt(mass*mass+mom.mag2());
    part->SetMomentum(mom.x(),mom.y(),mom.z(), energy);
    HepPoint pt = pshit.position();
// shift position a tiny bit to the negative to make sure we can find the intersections
    Hep3Vector momdir = mom.unit();
    static const double backstep(-1.0e-3); // 10 microns
    pt += momdir*backstep;
    part->SetProductionVertex(pt.x(),pt.y(),pt.z(),pshit.time());
    part->SetWeight(1.0); // all particles have same weight
    part->SetStatusCode(_statuscode);
    if(_verbose) cout << "Wrote particle # " << _npar 
    <<  " type " << pshit.pdt()->name() 
    << " mom " << mom.mag() << endl;
  }
}

void
PacWriteParticles::fillTree() {
  if(_active && (_writeempty || _particles.GetEntriesFast() > 0) ){
    _tree->Fill();
    _nparwrote += _particles.GetEntriesFast();
    _particles.Clear();
    _nevtwrote++;
    _nsum = 0;
    _npar = 0;
  }
}

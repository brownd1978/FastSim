//--------------------------------------------------------------------------
// Name:
//   PacWriteParticles
// Description:
//   Write out true particles in a TClonesArray of TParticles
// Environment:
//   Software developed for FastSim
// Copyright Information:
//	Copyright (C) 2011	Lawrence Berkeley Laboratory
// Author List:
//      Dave Brown 28 Feb 2011
//------------------------------------------------------------------------

#ifndef PacWriteParticles_HH
#define PacWriteParticles_HH

#include "PacSim/PacSimHit.hh"
#include "PacSim/PacDetEffect.hh"
#include "PacSim/PacMatInt.hh"
#include "PDT/PdtEntry.hh"
#include "TParticle.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

class PacWriteParticles {
public:
// configuration from config
  PacWriteParticles();
  virtual ~PacWriteParticles();
// test activity
  bool isActive() const { return _active;}
// add a particle to the output tree
  void writeParticle(const PacSimHit& pshit);
// close the tree for this event
  void fillTree();
private:
  const char* _fileName;
  const char* _treeName;
  int _statuscode;
  int _nevtwrote;
  int _nparwrote;
  TFile* _output;
  TTree* _tree;
  bool _active;
  bool _verbose;
  bool _writeempty;
// branches
  TClonesArray _particles;
  Int_t _evtnum;
  Float_t _evtwt;
  UInt_t _nsum;
  UInt_t _npar;
};

#endif // PacWriteParticles_HH

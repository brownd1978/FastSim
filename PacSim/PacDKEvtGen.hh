//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  
//------------------------------------------------------------------------

#ifndef PACDKEVTGEN_HH
#define PACDKEVTGEN_HH

#include "EvtGenBase/EvtPDL.hh"

class EvtParticle;
class EvtRandomEngine;
#include "CLHEP/Vector/LorentzVector.h"
class EvtVector4R;
class EvtStdHep;
class EvtSpinDensity;
class EvtAbsRadCorr;
class EvtParticle;
class EvtDecayBase;

class PacDKEvtGen{

public:

  PacDKEvtGen(const char* const decayName);
  ~PacDKEvtGen();
  //  void initDecay(EvtParticle *p, EvtDecayBase *decayer );
void initDecay(EvtParticle *p);
  void generateDecay(EvtParticle *p);
  void decayLongLivedParticle(EvtParticle* p);
private:
};



#endif


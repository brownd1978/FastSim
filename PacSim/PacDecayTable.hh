//--------------------------------------------------------------------------
//
// Environment:
//      This software is part of the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.  
//------------------------------------------------------------------------

#ifndef PACDECAYTABLE_HH
#define PACDECAYTABLE_HH

#include "EvtGenBase/EvtDecayBase.hh"
#include "EvtGenBase/EvtParticleDecayList.hh"
#include <vector>

class EvtId;


typedef EvtDecayBase* EvtDecayBasePtr;

class PacDecayTable{

public:

  static int getNMode(int ipar);

  static EvtDecayBase* getDecay(int ipar, int imode);

  static void readDecayFile(const std::string dec_name, bool verbose=true);

  static int findChannel(EvtId parent,std::string model,int ndaug, 
			 EvtId *daugs,
			 int narg, std::string *args);
  
  static int inChannelList(EvtId parent, int ndaug, EvtId *daugs);

  static EvtDecayBase *getDecayFunc(EvtParticle *);

  static void printSummary();

  static void checkConj();

  static const std::vector<EvtParticleDecayList> & decaytable() {return _decaytable;};

private:

  static std::vector<EvtParticleDecayList> _decaytable;

};

#endif



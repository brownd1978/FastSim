//--------------------------------------------------------------------
// PacTrkClusterMatch
//
//  For each TrkRecoTrk, find a AbsRecoCalo within acceptable DOCA
//  add fill up a map with a vector of (AbsRecoCalo,separation) pairs.
//
//   Chih-hsiang Cheng @ Caltech   2009/02/27
//------------------------------------------------------------------------
#ifndef PACTRKCLUSTERMATCH_HH
#define PACTRKCLUSTERMATCH_HH

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmDouble.hh"

class TrkRecoTrk;
class AbsRecoCalo;
class BField;

#include <map>

class PacTrkClusterMatch : public AppModule {

public:

  // Constructor
  PacTrkClusterMatch( const char* const theName, const char* const theDescription );
  
  // Destructor
  virtual ~PacTrkClusterMatch();

  // Operations
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );
  
private:

  AbsParmIfdStrKey _recoTrkListName;
  AbsParmIfdStrKey _recoCaloListName;
  AbsParmIfdStrKey _emcmapkey;
  AbsParmDouble    _minConsistency;
  AbsParmDouble    _minCaloEnergy;
  AbsParmDouble    _maxDThetaCrude; // crude cut on track emc polar angle difference

  bool passTrkSelection(const TrkRecoTrk* tr, const AbsRecoCalo* calo);
  bool passCaloSelection(const AbsRecoCalo* cl);

};
#endif

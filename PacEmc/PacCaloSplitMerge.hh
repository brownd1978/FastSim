//------------------------------------------------------------------------
// $Id$
//   PacCaloSplitMerge
//
//      Split PacEmcCluster generated from a hadron if necessary
//      and merge clusters if two are too close to each other, and
//      adjust the list accordingly.
//
//    Chih-hsiang Cheng @ Caltech   2009/02/05
//------------------------------------------------------------------------
#ifndef PMCCALOSPLITMERGE_HH
#define PMCCALOSPLITMERGE_HH

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "Framework/AbsParmGeneral.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"
#include "PDT/Pdt.hh"

class PacEmcCluster;
class PacEmcModel;

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class PacCaloSplitMerge : public AppModule {

public:

  // Constructor
  PacCaloSplitMerge( const char* const theName, const char* const theDescription );
  
  // Destructor
  virtual ~PacCaloSplitMerge();
  
  // Operations
  virtual AppResult beginJob( AbsEvent* anEvent );
  virtual AppResult event( AbsEvent* anEvent );
  virtual AppResult endJob  ( AbsEvent* anEvent );
  virtual bool allowMerge(PacEmcCluster* clus1, PacEmcCluster* clus2) const ;

private:

  AbsParmIfdStrKey _recoCaloListName;

  // Any closer, we will try merging
  //AbsParmDouble _maxDistance; // in the unit of Moliere radius

  // Parameters for PacEmcClusterSplitter
  AbsParmDouble _bumpTolerance;
  AbsParmGeneral<int> _maxIterations;
  AbsParmDouble _splitFuncExponent;
  AbsParmBool _doSplit;
  AbsParmBool _doMerge;
  AbsParmBool _mipThreshold;
  AbsParmDouble _minValley;

  PacEmcModel *_emcmodel;

};
#endif

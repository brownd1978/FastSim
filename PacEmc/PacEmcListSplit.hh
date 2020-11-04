//--------------------------------------------------------------------------
// PacEmcListSplit
//
// Description:
//	Module PacEmcListSplit.  Split the input emc list into charged and 
//      neutral lists (copy much of the code from Babar's EmcReco/EmcListSplit)
//
// Chih-hsiang Cheng   Caltech   2009/03/17
//---------------------------------------------------------------------------

#ifndef PacEmcListSplit_HH
#define PacEmcListSplit_HH

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "AbsParm/AbsParmNamedValue.hh"
#include "Framework/AbsParmDouble.hh"
#include "Framework/AbsParmBool.hh"

class TrkRecoTrk;
class PacEmcCluster;
class PacEmcTMInfo;
class AbsRecoCalo;

template <class A> class HepAList;
template <class A, class B, class C> class AstSTLMatch;
//template <class A, class B > class AstSTLMap2;

//		---------------------
// 		-- Class Interface --
//		---------------------
class PacEmcListSplit : public AppModule {
public:
  enum emcMatchType {consistency=0,likelihood};

// Constructors
  PacEmcListSplit ( const char* const theName, const char* const theDescription );
// Destructor
  virtual ~PacEmcListSplit ( );
// Operations
  virtual AppResult event ( AbsEvent* anEvent );  

private:

  // config variables
  AbsParmNamedValue<emcMatchType> _trkMatchType;
  AbsParmDouble _trkMatchCut;
  AbsParmDouble _trkOkNDchCut;

  // input list
  AbsParmIfdStrKey _emcInputList;
  // input maps
  //AbsParmIfdStrKey _clusterBumpMap;
  AbsParmIfdStrKey _emcTMMap;
// output map
  AbsParmIfdStrKey _emcTrkMapKey;
// output lists
  AbsParmIfdStrKey _bestneutbumps;
  AbsParmIfdStrKey _chargedbumps;
  AbsParmIfdStrKey _bestbumptrks;

  void
  fillNeutralLists (const HepAList<AbsRecoCalo>& emcUniqueList,
		    HepAList<AbsRecoCalo>& bestneutbumps,
		    HepAList<AbsRecoCalo>& chargedbumps,
		    HepAList<TrkRecoTrk>& bestbumptrks);
  // event data, referenced here for convenience
  //AstSTLMap2<EmcCluster, EmcBump>* _theClusterBumpMap;
  AstSTLMatch<  AbsRecoCalo, TrkRecoTrk, PacEmcTMInfo >* _theTrkMatch;
  std::map<const TrkRecoTrk*,std::pair<const AbsRecoCalo*,double> >* _emcTrkMap;
  // utility
  template <class A> 
  static void putData (AbsEvent* anEvent,A* evtobj,const IfdKey& key);

};
#endif

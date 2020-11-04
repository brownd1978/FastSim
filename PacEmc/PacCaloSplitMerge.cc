//------------------------------------------------------------------------
// File
//   PacCaloSplitMerge
//
//    Chih-hsiang Cheng @ Caltech   2009/02/05
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "PacEmc/PacCaloSplitMerge.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>
//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <math.h>
#include <string>
#include <algorithm>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "AbsEvent/AbsEvent.hh"
#include "AbsCalo/AbsRecoCalo.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"

//#include "G3Data/GTrack.hh"
//#include "G3Data/GVertex.hh"
//#include "PacSim/PacSimTrack.hh"

#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"

#include "AbsEvent/getTmpAList.hh"
//#include "RecoData/RecoCompGMatchList.hh"

#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcClusterSplitter.hh"

#include <vector>
#include <list>
#include <cassert>

using namespace std;

//typedef std::pair<const GTrack*, double> GTEpair;
//bool compare(const GTEpair p1, const GTEpair p2);

//----------------
// Constructors --
//----------------
PacCaloSplitMerge::PacCaloSplitMerge ( const char* const theName, 
				const char* const theDescription )
  : AppModule( theName, theDescription )
  ,_recoCaloListName("recoCaloListName", this, "PmcRecoCalosRaw")
    //  ,_maxDistance("maxDistanceRM", this, 3.5)// in the unit of Moliere radius
  ,_bumpTolerance("splittedBumpTolerance", this, 0.1)
  ,_maxIterations("splitterMaxIterations", this, 10)
  ,_splitFuncExponent("splitFuncExponent", this, -2.5)
  ,_doSplit("doSplit", this, true)
  ,_doMerge("doMerge", this, true)
  ,_mipThreshold("mipThreshold", this, 0.1)
  ,_minValley("minValley", this, 0.5)
  ,_emcmodel(0)
{
  commands()->append(&_recoCaloListName);
  //  commands()->append(&_maxDistance);
  commands()->append(&_bumpTolerance);
  commands()->append(&_maxIterations);
  commands()->append(&_splitFuncExponent);
  commands()->append(&_doSplit);
  commands()->append(&_doMerge);
  commands()->append(&_mipThreshold);
  commands()->append(&_minValley);
}

//--------------
// Destructor --
//--------------
PacCaloSplitMerge::~PacCaloSplitMerge ()
{
}
//--------------
// Operations --
//--------------
AppResult
PacCaloSplitMerge::beginJob(AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
PacCaloSplitMerge::endJob(AbsEvent* anEvent )
{
  ErrMsg(routine) << name() << "  End job" << endmsg;
  return AppResult::OK;
}

AppResult
PacCaloSplitMerge::event(AbsEvent* anEvent )
{

  // Get a list of RecoCalo
  HepAList<AbsRecoCalo>* caloList= Ifd< HepAList<AbsRecoCalo> >::get(anEvent, _recoCaloListName.value());
  assert(caloList!=0);

  // Get Emc model
  _emcmodel= PacEmcModel::getModel();
 
  // Split clusters if necessary
  HepAListIterator<AbsRecoCalo> itcalo(*caloList);
  PacEmcClusterSplitter cSplitter;
  cSplitter.setTolerance(_bumpTolerance.value());
  cSplitter.setMaxIterations(_maxIterations.value());
  cSplitter.setExponent(_splitFuncExponent.value());
  HepAList<AbsRecoCalo> splittedCaloList;
  std::list<AbsRecoCalo*> removeCaloList;
  AbsRecoCalo* calo(0);

  if ( _doSplit.value() ) {
    while ( calo = itcalo() ) {
      PacEmcCluster *cluster= dynamic_cast<PacEmcCluster*>(calo);
      cluster->searchLocalMax();
      if ( _verbose.value() ) {
	ErrMsg(warning) << "Cluster has " << cluster->nBumps() << " bumps"
			<< endmsg;
      }
      if ( cluster->nBumps() > 1 ) {
	const PacEmcDigi *maxdigi= cluster->maxima();
	if ( ! maxdigi ) {
	  ErrMsg(warning) << "Can't find the cluster's maxima? Remove it!" << endmsg;
	  cluster->print(cout);
	  removeCaloList.push_back(calo);
	  continue;
	}

	double rMo= _emcmodel->rMoliere(*(maxdigi->tci()));
	cSplitter.setRMo(rMo);
	
	std::vector< PacEmcCluster* > splittedClusters;
	
	cSplitter.splitCluster(cluster,splittedClusters);

	for ( unsigned k=0; k<splittedClusters.size(); k++) {
	  if ( 0!=splittedClusters[k] ) {
	    splittedCaloList.append( splittedClusters[k] );
	  }
	}
	
	//removeIdx.push_back(k);
	removeCaloList.push_back(calo);
	
	cSplitter.clear();
	
      }
    }

    // Remove clusters that have been splitted from the original caloList
    std::list<AbsRecoCalo*>::iterator di= removeCaloList.begin();
    std::list<AbsRecoCalo*>::iterator dend= removeCaloList.end();
    for ( ; di!= dend; ++di) {
      caloList->remove(*di);
      delete (*di);
    }
    removeCaloList.clear();
    
    // Append the splitted clusters
    caloList->append(splittedCaloList);
  }


  if ( _doMerge.value() ) {
    // ===  Merge Clusters. Check pairs for distances ===  
    int length= caloList->length();
    
    if (_verbose.value() ) {
      ErrMsg(routine) << "There are " << length << " clusters after splitting"
		      << endmsg;
    }
    
    for ( int i=0; i< length-1; ++i) {
      AbsRecoCalo *calo1= (*caloList)[i];
      if ( std::find(removeCaloList.begin(), removeCaloList.end(), calo1) 
	   != removeCaloList.end() ) {
	// This cluster has been merged by one of the previous clusters.
	continue;
      }
      
      PacEmcCluster* clus1= dynamic_cast<PacEmcCluster*>(calo1);
      if ( clus1->numberOfDigis() <= 0 ) continue;
      
      for ( int j=i+1; j< length; ++j) {
	AbsRecoCalo *calo2= (*caloList)[j];
	
	if ( std::find(removeCaloList.begin(), removeCaloList.end(), calo2) 
	     != removeCaloList.end() ) {
	  // This cluster has been merged by one of the previous clusters.
	  continue;
	}
	
	PacEmcCluster* clus2= dynamic_cast<PacEmcCluster*>(calo2);
	if ( clus2->numberOfDigis() <= 0 ) continue;

	// Check if we want to merge them
	if ( ! this->allowMerge(clus1, clus2) ) continue;
	
	clus1->mergeCluster(clus2);

	removeCaloList.push_back(calo2);
	
      }
      
    }

    // Remove clusters that have been mearged to others
    std::list<AbsRecoCalo*>::iterator di= removeCaloList.begin();
    std::list<AbsRecoCalo*>::iterator dend= removeCaloList.end();
    
    for ( ; di!= dend; ++di) {
      caloList->remove(*di);
      delete (*di);
    }
    removeCaloList.clear();
  }  // end of doMerge


  if ( verbose() ) {
    ErrMsg(routine) << _recoCaloListName.value() << " list has " 
		    << caloList->length() << " entries after split/merge." 
		    << endmsg;
  }

  return AppResult::OK;
}

//==============================================================
// Check if they should be merged.
bool
PacCaloSplitMerge::allowMerge(PacEmcCluster* clus1, PacEmcCluster* clus2) const {

  bool allow= false;

  if ( clus1->where().distanceTo(clus2->where()) > clus1->radius()+clus2->radius() + 3) {
    // too far
    return false;
  }

  if (! clus1->isConnectedTo( clus2 ) ) {
    // not connected
    return false;
  }

  // Test merging
  PacEmcCluster *testcl= new PacEmcCluster(clus1->centroidAlgo());
  testcl->mergeCluster(clus1);
  testcl->mergeCluster(clus2);
  if ( testcl->nBumps() < clus1->nBumps()+clus2->nBumps() ) {
    allow= true;
  } else {
    allow= false;
  }

  if ( _verbose.value() ) {
    if ( allow ) ErrMsg(routine) << "Will merge" << endmsg;
    else ErrMsg(routine) << "Will not merge" << endmsg;
    testcl->print(cout,1);
  }

  delete testcl;
  return allow;
}


//bool
//compare(const GTEpair p1, const GTEpair p2) {
//  return p1.second > p2.second ;
//}

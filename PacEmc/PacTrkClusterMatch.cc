//--------------------------------------------------------------------
// PacTrkClusterMatch
//
//   Chih-hsiang Cheng @ Caltech   2009/02/27
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "PacEmc/PacTrkClusterMatch.hh"
#include <assert.h>
#include <iostream>
#include <math.h>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Alist/AIterator.h"
#include "BaBar/Constants.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsCalo/AbsRecoCalo.hh"
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/IfdStrKey.hh"
#include "ProxyDict/IfdKey.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Vector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "BField/BField.hh"

#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkPocaXY.hh"
#include "TrkBase/TrkExchangePar.hh"

#include "AssocTools/AstSTLMatch.hh"
#include "PacEmc/PacEmcTMInfo.hh"
#include "ProbTools/ChisqConsistency.hh"

#include "PacEmc/PacEmcModel.hh"

using namespace std;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//----------------
// Constructors --
//----------------
PacTrkClusterMatch::PacTrkClusterMatch( const char* const theName, 
					const char* const theDescription )
  : AppModule( theName, theDescription )
  , _recoTrkListName("recoTrackListName", this, "PmcRecoTracks" )
  , _recoCaloListName("recoCaloListName", this,"PmcRecoCalos")
  , _emcmapkey("EmcTrackMatchMap",this,"PacTMMap")
  , _minConsistency("minConsistency", this, 0.005)
  , _minCaloEnergy("minCaloEnergy", this, 0.001)
  , _maxDThetaCrude("maxDThetaCrude", this, 0.5)
{
  commands()->append(&_recoTrkListName);
  commands()->append(&_recoCaloListName);
  commands()->append(&_emcmapkey);
  commands()->append(&_minConsistency);
  commands()->append(&_minCaloEnergy);
  commands()->append(&_maxDThetaCrude);
}
//--------------
// Destructor --
//--------------
PacTrkClusterMatch::~PacTrkClusterMatch() 
{
}

//--------------
// Operations --
//--------------
AppResult
PacTrkClusterMatch::beginJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
PacTrkClusterMatch::endJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
PacTrkClusterMatch::event( AbsEvent* anEvent )
{

  static double Pi = Constants::pi;
  static double TwoPi = Constants::twoPi;

  const PacEmcModel *emcmodel= PacEmcModel::getModel();

  // Get a list of RecoCalo
  HepAList<AbsRecoCalo>* caloList= Ifd< HepAList<AbsRecoCalo> >::get(anEvent, _recoCaloListName.value());
  assert(caloList!=0);

  // Get a list of TrkRecoTrk
  HepAList<TrkRecoTrk>* trkList= Ifd< HepAList<TrkRecoTrk> >::get(anEvent, _recoTrkListName.value());
  assert(trkList!=0);

  if ( _verbose.value() ) {
    ErrMsg(routine) << "#tracks(" << _recoTrkListName.value() << ")= " 
		    << trkList->length() << endmsg;
    ErrMsg(routine) << "#calos(" << _recoCaloListName.value() << ") = "
		    << caloList->length() << endmsg;    
  }


  // create empty output object
  AstSTLMatch<AbsRecoCalo, TrkRecoTrk, PacEmcTMInfo> *emcTrkMap = 
    new AstSTLMatch<AbsRecoCalo, TrkRecoTrk, PacEmcTMInfo>;
  AstSTLMatch<TrkRecoTrk, AbsRecoCalo, PacEmcTMInfo> *trkEmcMap =
    new AstSTLMatch<TrkRecoTrk, AbsRecoCalo, PacEmcTMInfo>;

  if (! Ifd<AstSTLMatch<AbsRecoCalo, TrkRecoTrk, PacEmcTMInfo> >::put(anEvent, emcTrkMap, _emcmapkey.value() ) ) {
    ErrMsg(fatal)<< "Can't put AstSTLMatch<AbsRecoCalo, TrkRecoTrk, PacEmcTMInfo>  \"" << _emcmapkey.value() << "\" to the event." << endmsg;
  }
  
  if (! Ifd<AstSTLMatch<TrkRecoTrk, AbsRecoCalo, PacEmcTMInfo> >::put(anEvent, trkEmcMap, _emcmapkey.value() ) ) {
    ErrMsg(fatal)<< "Can't put AstSTLMatch<TrkRecoTrk, AbsRecoCalo, PacEmcTMInfo>  \"" << _emcmapkey.value() << "\" to the event." << endmsg;
  }
  
  
  HepAListIterator<AbsRecoCalo> itcalo(*caloList);
  HepAListIterator<TrkRecoTrk> ittrk(*trkList);

  // For each AbsRecoCalo, find all matched TrkRecoTrk within sqrt(chisq) cut
  AbsRecoCalo *calo(0);
  while ( calo= itcalo() ) {

    if (! passCaloSelection(calo) ) continue;

    ittrk.rewind();

    // The position is defined at a certain shower depth
    HepPoint poscalo= calo->position();

    PacEmcCluster *clus= dynamic_cast<PacEmcCluster*>(calo);
    const HepSymMatrix m2mat= clus->secondMomentMatrix();
    int ierr=0;
    const HepSymMatrix invm2mat = m2mat.inverse(ierr);
    if ( ierr!= 0 || isnan(m2mat.determinant()) || m2mat.determinant()<=0 ) {
      ErrMsg(warning) << "The cluster's secondMomentMatrix is not positive-definite:\n" << m2mat 
		      << "\n No track will be matched."
		      << endmsg;
      clus->print(cout, 1);
      continue;
    } 

    // Radial resolution is set by the configuration file.
    const PacEmcDigi* maxdigi= clus->maxima();
    if ( !maxdigi ) {
      ErrMsg(warning) << "Can't find max digi? " << endmsg;
      clus->print(cout);
      continue;
    }
    const double dRSig= emcmodel->radialResolution(maxdigi->theta());
    if ( dRSig <= 0 ) {
      ErrMsg(fatal) << "Found radialResolution for a cluster at itheta= "
		    << maxdigi->theta() << " to be " << dRSig
		    << ". This is likely to be a configuration error."
		    << " Check radialResolution in your PacEmcGeom*.xml"
		    << endmsg;
    }
    const double dRSig2= dRSig*dRSig;

    TrkRecoTrk *trk(0);
    while ( trk= ittrk() ) {
      if (! passTrkSelection(trk, calo) ) continue;

      const TrkFit* fit= trk->fitResult();
      TrkPocaXY poca(fit->traj(), fit->endFoundRange(),poscalo);

      double flightlen= poca.flt1();
      HepPoint trkPos= fit->traj().position(flightlen);

      double deltaTheta= trkPos.theta() - poscalo.theta();
      double deltaPhi= trkPos.phi() - poscalo.phi();
      while ( deltaPhi > Pi ) deltaPhi-= TwoPi;
      while ( deltaPhi < -Pi ) deltaPhi+= TwoPi;
      while ( deltaTheta > Pi ) deltaTheta-= TwoPi;
      while ( deltaTheta < -Pi ) deltaTheta+= TwoPi;
      HepVector da(2);  // angular difference between poca and emc
      da[0]= deltaTheta;
      da[1]= deltaPhi;
      // The chi^2 of the separation (in angular space)
      double chisq = invm2mat.similarity(da);

//       cout << "m2mat: \n" << m2mat << endl;
//       cout << "invm2mat: \n" << invm2mat << endl;
//       cout << "da: " << da << endl;
//       cout << "chisq= " << chisq << endl;
	
      // The difference in R of trk's poca and the cluster
      double deltaR= trkPos.mag()- poscalo.mag();
      // Add deltaR contribution to the chisq
      chisq+= deltaR*deltaR/dRSig2;

      if ( isnan(chisq) ) {
	ErrMsg(warning) << "chisq isnan, why?" << endmsg;
	ErrMsg(warning) << "m2mat: \n" << m2mat << "\n"
			<< "invm2mat: \n" << invm2mat << "\n"
			<< "da: " << da << "\n"
			<< "deltaR= " << deltaR << "\n"
			<< "dRSig2= " << dRSig2 << endmsg;
	ErrMsg(fatal) << "abort here" << endmsg;
	continue;
      }


      ChisqConsistency con(chisq,2);


      if ( _verbose.value() ) {
	ErrMsg(routine) << "cluster  =========" << endmsg;
	clus->print(cout);
	ErrMsg(routine) << "track   flightlen= " << flightlen << endmsg;
	fit->helix(flightlen).print(cout);
	ErrMsg(routine) << "\ntrkPos= " << trkPos << endmsg;
	ErrMsg(routine) << "cluster matrix " << m2mat << endmsg ;
	ErrMsg(routine) << "chisq= " << chisq << "  consistency= "
			<< con.consistency() << endmsg;
      }



      if ( con.consistency() > _minConsistency.value() ) {

	EmcTrkInterInfo interinfo(flightlen,fit,trk);

	PacEmcTMInfo* tminfo= new PacEmcTMInfo(trkPos,fit->charge(), interinfo, calo, deltaPhi, deltaTheta);
	tminfo->setConsistency(con);
	tminfo->setPoca(poca);
//   	cout << "PacTrkClusterMatch =====================================" << endl;
// 	cout << "trkPos = " << trkPos << endl;
// 	clus->print(std::cout);
// 	cout << "m2mat = " << clus->secondMomentMatrix() << endl;
//  	cout << "chisq = " << chisq << endl;
//  	cout << "consistency = " << con.consistency() << endl;
//   	tminfo->print(std::cout);
//  	cout << endl;
	emcTrkMap->insertMatch(calo,trk, tminfo);
	
      }
    }
  }

  // Build track->calo match
  std::map<AbsRecoCalo*, std::map<TrkRecoTrk*, PacEmcTMInfo*, BbrPtrLess>*,BbrPtrLess>::const_iterator
    nextCaloDict = emcTrkMap->getTheMatchDictionary()->begin();
  
  while ( nextCaloDict != emcTrkMap->getTheMatchDictionary()->end()) {

    AbsRecoCalo* theCalo = nextCaloDict->first;

    std::map<TrkRecoTrk*, PacEmcTMInfo*, BbrPtrLess>::iterator
      nextTrkMatch = nextCaloDict->second->begin();
    while (nextTrkMatch != nextCaloDict->second->end()) {
      TrkRecoTrk* theTrack = nextTrkMatch->first;
      PacEmcTMInfo* theTMInfo = nextTrkMatch->second->clone();
      trkEmcMap->insertMatch(theTrack, theCalo, theTMInfo);
      ++nextTrkMatch;
    }
    ++nextCaloDict;
  }

  return AppResult::OK;
}

// Crude track selection
bool
PacTrkClusterMatch::passTrkSelection(const TrkRecoTrk* trk, const AbsRecoCalo* calo) {

  bool retval= true;
  const TrkFit* fit= trk->fitResult();
  // check track and recocalo are in the same general direction
  // for large-ish momentum tracks.
  if ( 1./ fabs(fit->helix(fit->startFoundRange()).omega()) > 70 ) {

    // Check the polar angle, but first need to adjust cluster's polar
    // angle with respect to the z0 of the track
    double z0= fit->helix(fit->startFoundRange()).z0();
    if ( fabs(fit->momentum().theta()- calo->position(HepPoint(0,0,z0)).theta())
	 > _maxDThetaCrude.value() ) {
      retval= false;
    } else {
      double dphi= fabs(fit->momentum().phi() - calo->position().phi());
      while ( dphi > Constants::pi ) dphi-= Constants::twoPi;
      if ( fabs(dphi) > Constants::pi/2. ) {
	retval= false;
      }
    }
  }

  return retval;
}

bool
PacTrkClusterMatch::passCaloSelection(const AbsRecoCalo* cl) {

  if ( cl->energy() < _minCaloEnergy.value() ) return false;
  return true;
}

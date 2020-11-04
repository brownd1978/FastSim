#include "BaBar/BaBar.hh"
#include "PacTrk/PacTrkQual.hh"

#include "BetaMicroAdapter/BtaTrkQual.hh"
#include "Beta/BtaCandidate.hh"

#include "TrkBase/TrkRecoTrk.hh"
#include "TrkBase/TrkHotList.hh"
#include "TrkBase/TrkFit.hh"
#include "TrkBase/TrkFitStatus.hh"
#include "PDT/PdtPid.hh"
#include "ProbTools/ChisqConsistency.hh"
#include "TrkBase/TrkView.hh"
#include "ErrLogger/ErrLog.hh"


#include <iostream>
using namespace std;

PacTrkQual::PacTrkQual() {
  if(ErrLogging(trace))
    cout << "create PacTrkQual" << endl;
}

PacTrkQual::~PacTrkQual() {
}

void PacTrkQual::makeTrkQual(BtaTrkQual* trkQual, const TrkRecoTrk* recotrk){


  if ( !trkQual ) return;
  if ( !recotrk ) return;

  PdtPid::PidType hypo = recotrk->defaultType();
  trkQual->setPidHypo(hypo);
  const TrkFit* fit = recotrk->fitResult(hypo);
  const TrkFitStatus* trkStatus = recotrk->status(hypo);
  bool fitStatus =  trkStatus != 0 && trkStatus->fitValid() ;
  //    && trkStatus->fitCurrent();
  trkQual->setFitStatus( fitStatus );
  if(ErrLogging(trace))
    cout<<"fit status"<<fitStatus<<endl;//debug
  int nDch=0;
  if ( fit != 0) {
    nDch=fit->nDch();
    trkQual->setDchHits( nDch ); 
    trkQual->setNDof( fit->nDof() );
    trkQual->setChi2( fit->chisq() );
    trkQual->setProb( fit->chisqConsistency().consistency() );
    trkQual->setSvtHits( fit->nSvt() );
    double stFound  =   fit->startFoundRange() ;
    double tLen  =  fabs( fit->endFoundRange()-stFound );
    trkQual->setTLen( tLen );
    trkQual->setStFound( stFound );
    if(ErrLogging(trace)){
      cout<<"fit found"<<endl;
    //check : fit status is always zero
    //    if (fitStatus) {  
      cout<<"filling fit qual"<<endl;
      cout<<"ndch"<<nDch<<endl;//debug
      cout<<"nsvt"<<fit->nSvt()<<endl;//debug
    }    
  }else {
    trkQual->setDchHits( 0 );
    trkQual->setNDof( 0 );
    trkQual->setChi2( 0. );
    trkQual->setProb( 0. );
    trkQual->setSvtHits( 0 );
    trkQual->setTLen( 0. );
    trkQual->setStFound( 0. );
  }
  
  const TrkHotList* hotList = recotrk->hots(hypo);
  
  if ( hotList != 0)  {
    unsigned fDch = hotList->firstDchLayer();
    if(nDch == 0) fDch = 0;
    // maintain compatibility with the micro
    if(fDch > 0)
      trkQual->setDchFirstHit( fDch );
    else
      trkQual->setDchFirstHit( 50 );
    trkQual->setDchLastHit( hotList->lastDchLayer() );

    // layer numbers start at 1!!!
    unsigned svtPattern(0);
    for(unsigned ilay=0;ilay<5;ilay++){
      TrkView view = hotList->svtView(ilay+1);
      if(view.contains(TrkEnums::xyView))
        svtPattern |= (1<<ilay);
      if(view.contains(TrkEnums::zView))
        svtPattern |= (1<<(ilay+5));
    }    
    trkQual->setSvtPattern( svtPattern );
    if(ErrLogging(trace)){
      cout<<"filling hot qual"<<endl;
      cout << " svt pattern = " << svtPattern << endl;
      cout << " first Dch layer = " << fDch << endl;      
    }
  }else{
    trkQual->setDchFirstHit( 50 );
    trkQual->setDchLastHit( 0 );
    trkQual->setSvtPattern( 0 );
  }
}

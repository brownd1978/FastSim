//--------------------------------------------------------------------------
//
// Environment:
//      This software is inspired by the EvtGen package developed jointly
//      for the BaBar and CLEO collaborations.
//
//------------------------------------------------------------------------
// 
#include "BaBar/BaBar.hh"

#include "Experiment/Experiment.hh"
#include <stdio.h>
#include <fstream>
#include <math.h>
#include "EvtGenBase/EvtComplex.hh"
#include <stdlib.h>
#include "PacSim/PacDKEvtGen.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "EvtGenBase/EvtVectorParticle.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtScalarParticle.hh"
#include "PacSim/PacDecayTable.hh"
#include "EvtGenBase/EvtDecayTable.hh"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtStdHep.hh"
#include "EvtGenBase/EvtSecondary.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include "EvtGenBase/EvtSimpleRandomEngine.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtSpinDensity.hh"
#include "CLHEP/Vector/LorentzVector.h"
//#include "EvtGenModels/EvtModelReg.hh"
#include "EvtGenBase/EvtStatus.hh"
#include "EvtGenBase/EvtAbsRadCorr.hh"
#include "EvtGenBase/EvtRadCorr.hh"
#include "EvtGenModels/EvtPHOTOS.hh"
using std::endl;
using std::fstream;
using std::ifstream;


PacDKEvtGen::~PacDKEvtGen(){

}


PacDKEvtGen::PacDKEvtGen(const char* const decayName){
  report(INFO,"PacDKEvtGen") << "Main decay file name  :"<<decayName<<endl;
  PacDecayTable::readDecayFile(decayName,false);
};

void PacDKEvtGen::generateDecay(EvtParticle *p){

  int times=0;
  do{
    times+=1;
    EvtStatus::initRejectFlag();

    //    p->decay();
    decayLongLivedParticle(p);
    //ok then finish.
    if ( EvtStatus::getRejectFlag()==0 ) { 
      times=0;
    }
    else{   
      int ii;
      for (ii=0;ii<p->getNDaug();ii++){
	EvtParticle *temp=p->getDaug(ii);
	temp->deleteTree();
      }
      p->resetFirstOrNot();
      p->resetNDaug();
      
    }

    if ( times==10000) {
      report(ERROR,"PacDKEvtGen") << "Your event has been rejected 10000 times!"<<endl;
      report(ERROR,"PacDKEvtGen") << "Will now abort."<<endl;
      ::abort();
      times=0;
    }
  } while (times);

}




void PacDKEvtGen::decayLongLivedParticle(EvtParticle* p){
  //P is particle to decay, typically 'this' but sometime
  //modified by mixing 
  initDecay(p);
  EvtDecayBase *decayer;
  // this crashes when trying to decay to unstable long lived charged particles
  decayer = PacDecayTable::getDecayFunc(p);
  //decayer = EvtDecayTable::getDecayFunc(p);

  // figure out the masses
  // GS comment this out because this calls EvtParticle::initDecay
  //  if ( p->getNDaug() == 0 ) p->generateMassTree();
  if ( decayer != 0) {
    decayer->makeDecay(p);
  }
  else{
    //    p->_rhoBackward.setDiag(p->getSpinStates());
    EvtSpinDensity rho(p->getSpinDensityBackward());
    rho.setDiag(p->getSpinStates());
    p->setSpinDensityBackward(rho);
  }
  return;  
}


//void PacDKEvtGen::initDecay(EvtParticle *p, EvtDecayBase *decayer ){
void PacDKEvtGen::initDecay(EvtParticle *p){
  bool useMinMass=false;
  EvtParticle* par=p->getParent();
  double parMass=-1.;
  if ( par != 0 ) {
    if ( par->hasValidP4() ) parMass=par->mass();
    int i;
    for ( i=0;i<par->getNDaug();i++) {
      EvtParticle *tDaug=par->getDaug(i);
      if ( p != tDaug )
	parMass-=EvtPDL::getMinMass(tDaug->getId());
    }
  }

  EvtDecayBase *decayer;
  // this crashes when trying to decay to unstable long lived charged particles
  decayer = PacDecayTable::getDecayFunc(p);
  //decayer = EvtDecayTable::getDecayFunc(p);
  if ( decayer ) {
    p->makeDaughters(decayer->nRealDaughters(),decayer->getDaugs());

    //then loop over the daughters and init their decay
    int i;
    for(i=0;i<p->getNDaug();i++){
      //      std::cout << EvtPDL::name(p->getDaug(i)->getId()) << " " << i << " " << p->getDaug(i)->getSpinType() << " " << EvtPDL::name(p->getId()) << std::endl;
      if ( EvtPDL::getWidth(p->getDaug(i)->getId()) > 0.0000001)
	p->getDaug(i)->initDecay(useMinMass);
      else p->getDaug(i)->setMass(EvtPDL::getMeanMass(p->getDaug(i)->getId()));
    }
  }
  
  //take care of setting the mass of the parent (I'm not sure I understand 
  //how this is done exaclty GS)
  int j;
  EvtId *dauId=0;
  double *dauMasses=0;
  int nDaugT=p->getNDaug();
  if ( nDaugT > 0) {
    dauId=new EvtId[nDaugT];
    dauMasses=new double[nDaugT];
    for (j=0;j<nDaugT;j++) { 
      dauId[j]=p->getDaug(j)->getId();
      dauMasses[j]=p->getDaug(j)->mass();
    }
  }

  EvtId *parId=0;
  EvtId *othDauId=0;
  EvtParticle *tempPar=p->getParent();
  if (tempPar) {
    parId=new EvtId(tempPar->getId());
    if ( tempPar->getNDaug()==2 ) {
      //      if ( tempPar->getDaug(0) == this ) othDauId=new EvtId(tempPar->getDaug(1)->getId());
      if ( tempPar->getDaug(0) == p ) othDauId=new EvtId(tempPar->getDaug(1)->getId());
      else othDauId=new EvtId(tempPar->getDaug(0)->getId());
    }
  }
  if ( p->getParent() && p->hasValidP4()==false ) {
    if ( !useMinMass ) p->setMass(EvtPDL::getRandMass(p->getId(),parId,p->getNDaug(),dauId,othDauId,parMass,dauMasses));
    else p->setMass(EvtPDL::getMinMass(p->getId()));
  }
  if ( parId) delete parId;
  if ( othDauId) delete othDauId;
  if ( dauId) delete [] dauId;
  if ( dauMasses) delete [] dauMasses;
}

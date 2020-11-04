#include "BaBar/BaBar.hh"
#include "PacEmc/PacCalQual.hh"

#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcClusterEnergySums.hh"
#include "PacEmc/PacEmcXClMoments.hh"

#include <iostream>
using namespace std;

PacCalQual::PacCalQual() {
  cout << "create PacCalQual" << endl;
}

PacCalQual::~PacCalQual() {
}

void PacCalQual::makeCalQual(BtaCalQual* calQual, const PacEmcCluster* cluster){

  if ( !calQual ) return;
  if ( !cluster ) return;
  //  cout << "makeCalQual   " << cluster->numberOfDigis() << endl;
  if ( cluster->numberOfDigis() == 0 ) return;

  calQual->setNBumps(cluster->nBumps());
  calQual->setNCry(cluster->numberOfDigis());
  calQual->setLat(cluster->Xmoments().lat());
  calQual->setS9S25(cluster->esums().e9e25());
  calQual->setS1S9(cluster->esums().e1e9());
  calQual->setAbsZern42(cluster->Xmoments().absZernikeMoment(4,2));
  calQual->setAbsZern31(cluster->Xmoments().absZernikeMoment(3,1));
  calQual->setAbsZern20(cluster->Xmoments().absZernikeMoment(2,0));
  calQual->setSecondMomentTP(cluster->Xmoments().secondMomentTP());
  calQual->setERaw(cluster->rawEnergy());
  calQual->setECal(cluster->energy());
  calQual->setCentroid(cluster->where());
  calQual->setCovMat(cluster->errorMatrix());
  calQual->setStatus(0);


}

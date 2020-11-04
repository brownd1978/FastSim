//-----------------------------------------------------------------------
// PacEmcCalibrator
//
//    Emc cluster energy calibrator.
//
//    Chih-hsiang Cheng     Caltech   2009/04/08
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "ErrLogger/ErrLog.hh"
#include "PacEmc/PacEmcCalibrator.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcCluster.hh"
#include <cmath>

using namespace std;

PacEmcCalibrator::PacEmcCalibrator () 
  : PacEmcAbsCalibrator()
{
}

PacEmcCalibrator::PacEmcCalibrator (const char* name)
  : PacEmcAbsCalibrator(name)
{
}

PacEmcCalibrator::PacEmcCalibrator (const PacEmcAbsCalibrator &o) 
  : PacEmcAbsCalibrator(o)
{
}

PacEmcCalibrator::~PacEmcCalibrator () 
{
}

void
PacEmcCalibrator::calibrate (PacEmcCluster *cluster) const {
  if ( cluster->isCalibrated() ) {
    return;
  }

  if ( _p.size() < 4 ) {
    ErrMsg(fatal) << "Need four parameters" << endmsg;
  }
  
  const double x= cluster->energy();
  // pull= mean of (cluster->energy() - gtrack.E() )/ gtrack.E() 
  const double arg= _p[2]*x;
  double pull= _p[0];
  if ( arg > 0 ) pull+= _p[1]*(log(arg)+ _p[3])*exp(-arg);

  cluster->setCalibFactor( 1.0/(1+pull) );

}

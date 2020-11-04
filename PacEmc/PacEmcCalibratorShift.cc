//-----------------------------------------------------------------------
// PacEmcCalibratorShift
//
//    Emc cluster energy calibrator.
//
//    Chih-hsiang Cheng     Caltech   2009/07/07
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "ErrLogger/ErrLog.hh"
#include "PacEmc/PacEmcCalibratorShift.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcCluster.hh"
#include <cmath>

using namespace std;

PacEmcCalibratorShift::PacEmcCalibratorShift () 
  : PacEmcAbsCalibrator()
{
}

PacEmcCalibratorShift::PacEmcCalibratorShift (const char* name) 
  : PacEmcAbsCalibrator(name)
{
}

PacEmcCalibratorShift::PacEmcCalibratorShift (const PacEmcAbsCalibrator &o)
  : PacEmcAbsCalibrator(o)
{
}

PacEmcCalibratorShift::~PacEmcCalibratorShift () 
{
}

// Shift the cluster energy by _p[0]
void
PacEmcCalibratorShift::calibrate (PacEmcCluster* cluster) const {
  if ( cluster->isCalibrated() ) {
    return;
  }
  const double e= cluster->energy();
  cluster->setCalibFactor( (e+_p[0])/e );
}

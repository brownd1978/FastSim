//-----------------------------------------------------------------------
// PacEmcCalibratorShift
//
//    Emc cluster energy calibrator.
//
//    Chih-hsiang Cheng     Caltech   2009/07/07
//-----------------------------------------------------------------------
#ifndef PacEmcCalibratorShift_HH
#define PacEmcCalibratorShift_HH

#include "PacEmc/PacEmcAbsCalibrator.hh"
#include <vector>
#include <string>

class PacEmcCluster;

class PacEmcCalibratorShift : public PacEmcAbsCalibrator {

public:

  PacEmcCalibratorShift();
  PacEmcCalibratorShift(const char* name);
  PacEmcCalibratorShift(const PacEmcAbsCalibrator& o);
  virtual ~PacEmcCalibratorShift();

  virtual void calibrate(PacEmcCluster* cluster) const ;

private:

};
#endif

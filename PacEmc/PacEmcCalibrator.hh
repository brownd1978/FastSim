//-----------------------------------------------------------------------
// PacEmcCalibrator
//
//    Emc cluster energy calibrator.
//
//    Chih-hsiang Cheng     Caltech   2009/04/08
//-----------------------------------------------------------------------
#ifndef PacEmcCalibrator_HH
#define PacEmcCalibrator_HH

#include "PacEmc/PacEmcAbsCalibrator.hh"
#include <vector>
#include <string>

class PacEmcCluster;

class PacEmcCalibrator : public PacEmcAbsCalibrator {

public:

  PacEmcCalibrator();
  PacEmcCalibrator(const char* name);
  PacEmcCalibrator(const PacEmcAbsCalibrator& o);
  virtual ~PacEmcCalibrator();

  virtual void calibrate(PacEmcCluster* cluster) const;

private:

};
#endif

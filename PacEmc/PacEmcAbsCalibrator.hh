//-----------------------------------------------------------------------
// PacEmcAbsCalibrator
//
//    Emc cluster energy calibrator.
//
//    Chih-hsiang Cheng     Caltech   2009/07/07
//-----------------------------------------------------------------------
#ifndef PacEmcAbsCalibrator_HH
#define PacEmcAbsCalibrator_HH

#include <vector>
#include <string>

class PacEmcCluster;

class PacEmcAbsCalibrator {

public:

  PacEmcAbsCalibrator();
  PacEmcAbsCalibrator(const char* name);
  PacEmcAbsCalibrator(const PacEmcAbsCalibrator& o);
  virtual ~PacEmcAbsCalibrator();

  virtual void calibrate(PacEmcCluster* cluster) const = 0;
  virtual void setParams(const std::vector<double> p);

protected:

  std::string _name;
  std::vector<double> _p;


};
#endif

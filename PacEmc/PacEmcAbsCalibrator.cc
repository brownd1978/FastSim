//-----------------------------------------------------------------------
// PacEmcAbsCalibrator
//
//    Emc cluster energy calibrator.
//
//    Chih-hsiang Cheng     Caltech   2009/07/07
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "ErrLogger/ErrLog.hh"
#include "PacEmc/PacEmcAbsCalibrator.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcCluster.hh"
#include <cmath>

using namespace std;

PacEmcAbsCalibrator::PacEmcAbsCalibrator () :
  _name("")
{
}

PacEmcAbsCalibrator::PacEmcAbsCalibrator (const char* name) :
  _name(name)
{
}

PacEmcAbsCalibrator::PacEmcAbsCalibrator (const PacEmcAbsCalibrator &o) :
  _name(o._name), _p(o._p)
{
}

PacEmcAbsCalibrator::~PacEmcAbsCalibrator () 
{
}

void PacEmcAbsCalibrator::setParams (const std::vector<double> p)
{
  _p = p;
}

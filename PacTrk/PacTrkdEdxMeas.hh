// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkdEdxMeas.hh; create dE/dx values for tracking hits
//
// Copyright Information:
//	Copyright (C) 2009    INFN, Pisa
//
//  Authors: John Walsh, 25May2009
// ------------------------------------------------------------------------------
#ifndef PacTrkdEdxMeas_HH
#define PacTrkdEdxMeas_HH

#include <vector>
#include "TrkBase/TrkEnums.hh"
#include "PacGeom/PacMeasurement.hh"
#include "PacTrk/PacTrkHitMeas.hh"
class DetMaterial;

class PacTrkdEdxMeas : public PacMeasurement {
public:

  PacTrkdEdxMeas(PacTrkHitMeas::HitType type, std::vector<double>& pars);
  virtual ~PacTrkdEdxMeas();

  virtual void get_dedx(const PacSimHit& simhit, double& dedx, double& ededx) const;
  virtual double getdEdxAve(double mommag, double mass,const DetMaterial& mat) const;
  virtual double truncFrac() const {return _trunc_frac;}
 

protected:

  PacTrkHitMeas::HitType _type;

  double _dedx_par1;
  double _dedx_par2;
  double _dedx_par3;
  double _trunc_frac;

  double _svt_thresh;   // in fraction of MIP, not used for DCH

  double _svt_fit_p0;  // fitted pars describing expected dE/dx function
  double _svt_fit_p1;
  double _svt_fit_p2;
  double _svt_fit_p3;
  double _svt_fit_p4;
  

};

#endif


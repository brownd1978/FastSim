// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkdEdxMeas
// Copyright Information:
//	Copyright (C) 2009	INFN, Sezione di Pisa
//
//  Authors: John Walsh, 25 September 2009
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacTrk/PacTrkHitMeas.hh"
#include "PacTrk/PacTrkdEdxMeas.hh"
#include "PacTrk/PacHit.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacSim/PacSimHit.hh"
#include "PacSim/PacSimTrack.hh"

#include "PacEnv/PacConfig.hh"
#include "GenEnv/GenEnv.hh"
#include "AbsEnv/AbsEnv.hh"
#include "DetectorModel/DetMaterial.hh"


#include "DetectorModel/DetIntersection.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "TrkBase/TrkHotList.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include <assert.h>
#include <iostream>
#include <math.h>

PacTrkdEdxMeas::PacTrkdEdxMeas(PacTrkHitMeas::HitType type, std::vector<double>& pars) : 
    PacMeasurement(PacMeasurement::dEdx),_type(type) {

  if (_type == PacTrkHitMeas::Wire) {
    assert(pars.size() == 4);
    _svt_thresh = -99.;
    _svt_fit_p0 = -99.;
    _svt_fit_p1 = -99.;
    _svt_fit_p2 = -99.;
    _svt_fit_p3 = -99.;
    _svt_fit_p4 = -99.;
  }
  else if (_type == PacTrkHitMeas::DoubleSiStrip) {
    assert(pars.size() == 10);

    _svt_thresh = pars[4];

    _svt_fit_p0 = pars[5];
    _svt_fit_p1 = pars[6];
    _svt_fit_p2 = pars[7];
    _svt_fit_p3 = pars[8];
    _svt_fit_p4 = pars[9];
  }
  else
    assert(0);

  // these parameters are common to svt and dch
  _dedx_par1 = pars[0];
  _dedx_par2 = pars[1];
  _dedx_par3 = pars[2];
  _trunc_frac = pars[3];


}

PacTrkdEdxMeas::~PacTrkdEdxMeas() {
}
  
double PacTrkdEdxMeas::getdEdxAve(double mommag, double mass,const DetMaterial& mat) const {
  
  double density = mat.density();
  assert (density>0);
  double dEdx = mat.dEdx(mommag,(DetMaterial::dedxtype) 0,mass);
  
  return dEdx/density;
}

void PacTrkdEdxMeas::get_dedx(const PacSimHit& simhit, double& dedx, double& ededx) const {

  double mommag = simhit.momentum().mag();
  double mass = simhit.getSimTrack()->pdt()->mass();
  
  const DetMaterial& material = simhit.detIntersection().delem->material(simhit.detIntersection());
  if(&material != 0){
    double dedx_ave = fabs(getdEdxAve(mommag,mass,material));
    double deltax =  simhit.detIntersection().pathLength();

    double mipDedx = 1.622e-3;
    ededx = _dedx_par1*pow(dedx_ave/mipDedx,_dedx_par2)*pow(deltax,_dedx_par3);

    HepRandomEngine* rng = HepRandom::getTheEngine();

    dedx = RandGauss::shoot(rng,dedx_ave,ededx);
  }
  return;
  
}

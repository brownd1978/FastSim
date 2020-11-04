//-----------------------------------------------------------------------
// File and Version Information:
//     PacEmcClusterMeas
//     Concrete class implementing PacMeasure for Emc cluster
//
//  Copyright Information:
//      Copyright (C) 2008      Caltech
//
//  Authors:  Chih-hsiang Cheng    2008/06/17
//-----------------------------------------------------------------------
#ifndef PACEMCCLUSTERMEAS_HH
#define PACEMCCLUSTERMEAS_HH

#include "PacGeom/PacMeasurement.hh"
#include "AbsEnv/TwoCoordIndex.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "AbsEnv/TwoCoordIndex.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacEmc/PacEmcDigi.hh"
#include <string>

class DetIntersection;
class PacEmcCluster;
class GTrack;

class PacEmcClusterMeas : public PacMeasurement {

public:

  enum FillMethod {GRPansatz, Gaussian, tripleGaussian, Line};
  enum ShowerType {EMShower, HadShower};

  PacEmcClusterMeas();
  ~PacEmcClusterMeas();

  virtual void createEmcCluster(const PacSimHit& hit, PacEmcCluster *cluster,
      bool verbose) const;

private:

  void init();

  void randomizeShape(PacEmcCluster *cluster, const TwoCoordIndex* tci0) const ;
  void createEMShower(const PacSimHit& hit, PacEmcCluster *cluster, const TwoCoordIndex* tci0,
      double scale) const;
  void createMIPCluster(const PacSimHit& hit, PacEmcCluster *cluster,
      const TwoCoordIndex* tci0) const;
  void createShower(const PacSimHit& hit, PacEmcCluster *cluster, const TwoCoordIndex* tci0,
      ShowerType type ) const;

  void recursiveFillCluster(PacEmcCluster* cluster, const TwoCoordIndex* tci,
      const PacSimHit& hit, double scale, FillMethod method,
      double RMxp, double RMxm, double RMyp, double RMym) const;

  void randomWalkFillCluster(PacEmcCluster* cluster, PacEmcCluster* temp,
      const TwoCoordIndex* tci, const PacSimHit& hit, double scale) const;

  bool addDigiToCluster(const TwoCoordIndex* tci, PacEmcCluster* cluster,
      double energy, const GTrack* gtrk, double forceAdd= -1) const;

  double gaussIntegral(double x1, double x2) const;

  double tripleGaussIntegral(double x1, double x2, double RM) const;

  double frGRPansatzIntegral(double x1, double x2, double y1, double y2) const;

  double frGRPansatzFunc(double x, double y) const;

  double energyFraction(const PacSimHit& hit, const TwoCoordIndex* tci, const HepPoint &ptci,
      double theta, double phi, double RMxp, double RMxm, double RMyp, double RMym,
      double entryth, double entryphi, FillMethod method= Gaussian) const;
  
  double energyDeposition(const PacSimHit& hit) const;
  MeasurementTime inTime(double time,double& fraction) const;

  PacEmcModel *_emcmodel;
  mutable bool _newcluster;
  //mutable double _hitEnergyChange;
  mutable double _hitEnergyDeposit;
  mutable PacEmcModel::EmcRegion _hitregion;
  mutable PacEmcDigi::WaveFormTime _wftime;
  mutable double _wfmax;
  mutable double _wfattp;

};
#endif


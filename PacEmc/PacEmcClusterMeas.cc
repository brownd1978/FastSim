//-----------------------------------------------------------------------
// File and Version Information:
//     PacEmcClusterMeas
//
//  Copyright Information:
//      Copyright (C) 2008      Caltech
//
//  Authors:  Chih-hsiang Cheng    2008/06/17
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"
#include "PacEmc/PacEmcClusterMeas.hh"
#include "AbsEnv/NeighbourStore.hh"
#include "AbsEnv/AbsEnv.hh"
#include "ProxyDict/Ifd.hh"
#include "G3Data/GTrack.hh"
#include "G3Data/GVertex.hh"
#include "PDT/PdtEntry.hh"
#include "PDT/Pdt.hh"
#include "PDT/PdtPdg.hh"
#include "PacEmc/PacEmcShowerLib.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcDigi.hh"
#include "PacEmc/PacEmcModel.hh"
#include "PacGeom/PacDetector.hh"
#include "PacSim/PacSimHit.hh"
#include "PacSim/PacSimTrack.hh"
#include "DetectorModel/DetIntersection.hh"
#include "DetectorModel/DetSurfaceType.hh"
#include "TrajGeom/TrkLineTraj.hh"

#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandPoisson.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Vector/ThreeVector.h"

#include <assert.h>
#include <iostream>
#include <cmath>
using namespace std;

PacEmcClusterMeas::PacEmcClusterMeas() :
  PacMeasurement(PacMeasurement::Calor), _emcmodel(0), _newcluster(true),
      _hitEnergyDeposit(0), _hitregion(PacEmcModel::Unknown), _wftime(
          PacEmcDigi::InSigWin), _wfmax(1.0), _wfattp(1.0)
{
  init();
}

PacEmcClusterMeas::~PacEmcClusterMeas()
{
}

void
PacEmcClusterMeas::init()
{

  cout << "PacEmcClusterMeas::init()" << endl;

  if (!_emcmodel) _emcmodel = PacEmcModel::getModel();
  _newcluster = true;

}

void
PacEmcClusterMeas::createEmcCluster(const PacSimHit& hit,
    PacEmcCluster *cluster, bool verbose) const
{
  _newcluster = false;
  if (cluster->numberOfDigis() == 0) {
    _newcluster = true;
  }

  //  _hitEnergyChange= hit.energyChange();
  _hitEnergyDeposit = energyDeposition(hit);
  if (_hitEnergyDeposit <= 0) return; // No energy deposit

  PacEmcCluster newcluster(cluster->centroidAlgo());

  // Find the entering crystal
  const TwoCoordIndex* tci0 = _emcmodel->realCoordToTci(hit.position());

  if (!cluster->shapePars()->init()) {
    randomizeShape(cluster, tci0);
  }
  newcluster.shapePars()->copyPars(cluster->shapePars());

  if (0 == tci0) {
    // cout << "PacEmcClusterMeas::createEmcCluster:  Error, can't find the tci: "
    //      	 << hit.position()
    //      	 << "  theta= " << hit.position().theta()
    //      	 << "  phi= " << hit.position().phi()
    //      	 << endl;
    return;
  }

  if (isnan(hit.energyChange())) {
    ErrMsg(warning) << "hit.energyChange() is nan! " << hit.energyChange()
        << "  hit.position= " << hit.position() << "  hit.detEffect()= "
        << hit.detEffect() << "\n  hit.momentumIn()= " << hit.momentumIn()
        << "  hit.momentumOut()= " << hit.momentumOut() << "\n  elem name= "
        << hit.detIntersection().delem->elementName() << "  # "
        << hit.detIntersection().delem->elementNumber() << endmsg;
    const GTrack *gtrk = hit.getSimTrack()->getGTrack();
    ErrMsg(warning) << "GTrack: " << *gtrk << endmsg;
    if (gtrk->parentTrack()) ErrMsg(warning) << "parent: \n"
        << *gtrk->parentTrack() << endmsg;
    if (gtrk->vertex()) {
      ErrMsg(warning) << "vertex: " << gtrk->vertex()->position() << endmsg;
      ErrMsg(warning) << "   cause is " << gtrk->vertex()->cause() << endmsg;
    }
    if (gtrk->terminalVertex()) ErrMsg(warning) << "terminal: "
        << gtrk->terminalVertex() << endmsg;

    return;
  }

  if (isnan(_hitEnergyDeposit)) {
    ErrMsg(warning) << "energyDeposition is nan! " << _hitEnergyDeposit
        << "  hit.position= " << hit.position() << "  hit.detEffect()= "
        << hit.detEffect() << endmsg;
    return;
  }

  switch (hit.detEffect()) {
  case PacSimHit::hadshower:
    if (verbose) ErrMsg(warning) << "  create hadshower" << endmsg;
    createShower(hit, &newcluster, tci0, HadShower);
    // Record the simtrack energy change
    newcluster.setSimTrkEnergyChange(_hitEnergyDeposit);
    break;
  case PacSimHit::shower:
    if (verbose) ErrMsg(warning) << "  create emshower" << endmsg;
    createShower(hit, &newcluster, tci0, EMShower);
    // Record the simtrack energy change
    newcluster.setSimTrkEnergyChange(_hitEnergyDeposit);
    break;
  case PacSimHit::normal:
  case PacSimHit::stop:
    if (verbose) ErrMsg(warning) << "  create MIP" << endmsg;
    createMIPCluster(hit, &newcluster, tci0);
    // Record the simtrack energy change
    newcluster.setSimTrkEnergyChange(_hitEnergyDeposit);
    break;
  case PacSimHit::interact:
  case PacSimHit::brems:
  case PacSimHit::compton:
  case PacSimHit::convert:
  case PacSimHit::none:
  case PacSimHit::mark:
  default:
    break;
  }

  if (verbose) {
    ErrMsg(warning) << endmsg;
    newcluster.print(cout);
  }

  if (isnan(newcluster.energy())) {
    ErrMsg(warning) << "newcluster.energy() = nan ?" << "  hit.position= "
        << hit.position() << "  hit.detEffect()= " << hit.detEffect() << endmsg;
    newcluster.print(cout, 1);
    return;
  }

  if (newcluster.numberOfDigis() > 0 && newcluster.energy() > 0) {
    newcluster.setFirstSimhitTime(hit.time());
    cluster->mergeCluster(&newcluster);
  }

  return;
}

void
PacEmcClusterMeas::randomizeShape(PacEmcCluster *cluster,
    const TwoCoordIndex* tci0) const
{
  // get random engine
  HepRandomEngine* rng = HepRandom::getTheEngine();

  // Fluctuate Moliere radius
  const double RM0 = _emcmodel->rMoliere(*tci0);
  const double fRM = _emcmodel->rMoliereFluc(*tci0);
  const double RM = RM0 * (1 + RandGauss::shoot(rng, 0, fRM));

  const double hadSigma = _emcmodel->hadShowerSigma(*tci0); // The 2D Gaussian profile sigma

  // Fluctuate essentricity
  double ess = RandGauss::shoot(rng, _emcmodel->emEssentricity(*tci0),
      _emcmodel->emEssenRMS(*tci0));
  if (ess < -0.9) ess = -0.9; // capped at +-0.9
  else if (ess > +0.9) ess = +0.9;
  double ratio = sqrt((1 + ess) / (1 - ess));
  double RMx = RM / ratio;
  double RMy = RM * ratio;

  double hSx = hadSigma / ratio;
  double hSy = hadSigma * ratio;
  cluster->shapePars()->setHadSigma(hSx, hSy);

  // Make asymmetry
  double amx = RandGauss::shoot(rng, 0, _emcmodel->emAsymmetry(*tci0));
  double amy = RandGauss::shoot(rng, 0, _emcmodel->emAsymmetry(*tci0));
  // capped at 0.9
  const double acap = 0.9;
  if (amx > acap) amx = acap;
  else if (amx < -acap) amx = -acap;
  if (amy > acap) amy = acap;
  else if (amy < -acap) amy = -acap;

  const double RMxp = RMx * (1 + amx);
  const double RMxm = RMx * (1 - amx);
  const double RMyp = RMy * (1 + amx);
  const double RMym = RMy * (1 - amx);

  cluster->shapePars()->setRM(RMxp, RMxm, RMyp, RMym);

  // EM fraction in hadron shower
  double emfrac = _emcmodel->hadShowerEMfrac();
  double emfracsig = _emcmodel->hadShowerEMfracSigma();
  double frac = RandGauss::shoot(rng, emfrac, emfracsig);
  if (frac > 1) frac = 1;
  if (frac < 0) frac = 0;
  cluster->shapePars()->setHadEMFrac(frac);

  // EM fraction in em shower
  emfrac = _emcmodel->emShowerEMfrac();
  emfracsig = _emcmodel->emShowerEMfracSigma();
  frac = RandGauss::shoot(rng, emfrac, emfracsig);
  if (frac > 1) frac = 1;
  if (frac < 0) frac = 0;
  cluster->shapePars()->setEmEMFrac(frac);

  // Noise occupancy
  const double occmean = _emcmodel->noiseOccupancy(*tci0);
  const double occsigma = _emcmodel->noiseOccSigma(*tci0);
  double occ = RandGauss::shoot(rng, occmean, occsigma);
  if (occ < 0) occ = 0;
  cluster->shapePars()->setNoiseOcc(occ);

  cluster->shapePars()->init(true);
}

void
PacEmcClusterMeas::createEMShower(const PacSimHit& hit, PacEmcCluster *cluster,
    const TwoCoordIndex* tci0, double scale) const
{

  recursiveFillCluster(cluster, tci0, hit, scale, GRPansatz,
      cluster->shapePars()->RMxp(), cluster->shapePars()->RMxm(),
      cluster->shapePars()->RMyp(), cluster->shapePars()->RMym());
}

void
PacEmcClusterMeas::createMIPCluster(const PacSimHit& hit,
    PacEmcCluster *cluster, const TwoCoordIndex* tci0) const
{

  // Only for charged particle
  if (hit.getSimTrack()->getGTrack()->pdt()->charge() == 0) {
    return;
  }

  recursiveFillCluster(cluster, tci0, hit, 1, Line, 0, 0, 0, 0);
}

void
PacEmcClusterMeas::createShower(const PacSimHit& hit, PacEmcCluster *cluster,
    const TwoCoordIndex* tci0, ShowerType type) const
{

  double emfrac(0);
  if (type == EMShower) {
    emfrac = cluster->shapePars()->emEMFrac();
  } else if (type == HadShower) {
    emfrac = cluster->shapePars()->hadEMFrac();
  } else {
    ErrMsg(fatal) << "Unknown shower type " << type << endmsg;
  }

  if (emfrac > 0) {
    // create EM component
    createEMShower(hit, cluster, tci0, emfrac);
  }

  if (emfrac < 1) { // Hadronic component
    // create a cluster and fill it with a Gaussian profile
    PacEmcCluster tmpcl;
    recursiveFillCluster(&tmpcl, tci0, hit, 1, Gaussian,
        cluster->shapePars()->hadSx(), cluster->shapePars()->hadSx(),
        cluster->shapePars()->hadSy(), cluster->shapePars()->hadSy());

    if (tmpcl.energy() <= 0) return;

    PacEmcCluster hadcomp;
    // Do random walk
    randomWalkFillCluster(&hadcomp, &tmpcl, tci0, hit, 1 - emfrac);

    // Merge hadronic part
    cluster->mergeCluster(&hadcomp);
  }

}

void
PacEmcClusterMeas::recursiveFillCluster(PacEmcCluster* cluster,
    const TwoCoordIndex* tci, const PacSimHit& hit, const double scale,
    FillMethod method, double RMxp, double RMxm,
    double RMyp, double RMym) const
{

  // RMxp, RMxm : effective Moliere radius in positive/negative x direction
  // RMyp, RMym : effective Moliere radius in positive/negative y direction

  if (cluster->getDigi(*tci) != 0) { // This has been done.
    return;
  }

  // energy loss
  if (_hitEnergyDeposit <= 0) return;
  double energy = _hitEnergyDeposit;

  // Element ID
  const int elemId = hit.detIntersection().delem->elementNumber();
  HepPoint tcipos = _emcmodel->whereLocal(*tci, elemId);

  double dtheta = _emcmodel->deltaTheta(*tci);
  double dphi = _emcmodel->deltaPhi(*tci);

  // Unit vector of momentum
  const Hep3Vector umom = hit.momentumIn().unit();
  // and the project on to theta and phi. If umom is parallel to hitpos,
  // the projections will be zero.
  // We will use this to change shower lateral shape
  // Unit vectors in phi and theta directions
  // Unit vector in z
  static const Hep3Vector zhat(0, 0, 1);
  // hit position relative to the EMC detector origin
  const HepPoint hitpos = hit.position() - _emcmodel->origin();
  const Hep3Vector vhitpos(hitpos.x(), hitpos.y(), hitpos.z());
  const Hep3Vector uphi = (zhat.cross(vhitpos)).unit();
  const Hep3Vector uthe = (uphi.cross(vhitpos)).unit();
  const double entryth = umom.dot(uthe);
  const double entryphi = umom.dot(uphi);
  double ei = scale * energy * energyFraction(hit, tci, tcipos, dtheta, dphi,
      RMxp, RMxm, RMyp, RMym, entryth, entryphi, method);

  if (false == addDigiToCluster(tci, cluster, ei, hit.getSimTrack()->getGTrack())) {
    // Energy is too low. Stop here.
    return;
  }

  // Go on to fill up its neighbors.
  std::set<AbsDetIndex*>::const_iterator iter = tci->itsNeighbours()->begin();
  while ((iter != tci->itsNeighbours()->end())) {
    const TwoCoordIndex* tci2 = dynamic_cast<const TwoCoordIndex*> (*iter);
    recursiveFillCluster(cluster, tci2, hit, scale, method, RMxp, RMxm, RMyp,
        RMym);
    ++iter;
  }

}

void
PacEmcClusterMeas::randomWalkFillCluster(PacEmcCluster* cluster,
    PacEmcCluster* temp, const TwoCoordIndex* tci, const PacSimHit& hit,
    double scale) const
{
  // get random engine
  HepRandomEngine* rng = HepRandom::getTheEngine();

  const int elemId = hit.detIntersection().delem->elementNumber();
  const double etotal = _hitEnergyDeposit * scale;

  const double hadShowerMaxR = _emcmodel->hadShowerMaxR(*tci); //
  const double quanta = _emcmodel->hadShowerQuanta(*tci); // in GeV
  const double localsmear = _emcmodel->hadShowerSmear(*tci); // in GeV
  const double hadShowerMinE = _emcmodel->hadShowerMinE(*tci); // minimum E in a digi


  HepPoint origpos = _emcmodel->whereLocal(*tci, elemId);
  const TwoCoordIndex* atci = tci;

  double ediff = cluster->energy() - etotal;
  bool hasneighbors(true);
  while (hasneighbors && ediff < -1e-6) {

    // expected energy
    const PacEmcDigi *digi = temp->getDigi(*atci);
    if (digi) {

      double ei = digi->energy();

      ei = RandPoisson::shoot(rng, ei / quanta) * quanta;
      ei += RandGauss::shoot(rng, 0, localsmear);
      ei *= scale;
      if (ei <= 0) ei = 0.0001; // FIXME?? hard-coded number

      ediff += ei;
      if (ediff > 0) ei = etotal - cluster->energy();

      // Fill a digi with energy ei and add to the cluster
      //addDigiToCluster(atci,cluster,ei, 0, 0, 0, hadShowerMinE);
      addDigiToCluster(atci, cluster, ei, hit.getSimTrack()->getGTrack(), hadShowerMinE);
    }

    // Random walk to a neighboring crystal
    // Probability proportional to energy profile
    double esum[20];
    unsigned ns(0);
    hasneighbors = false;
    std::set<AbsDetIndex*>::const_iterator iter =
        atci->itsNeighbours()->begin();
    while ((iter != atci->itsNeighbours()->end())) {
      const TwoCoordIndex* tci2 = dynamic_cast<const TwoCoordIndex*> (*iter);
      double dene = 0;
      const PacEmcDigi *tmpdigi = temp->getDigi(*tci2);
      if (tmpdigi) {
        dene = tmpdigi->energy();
        hasneighbors = true;
      }
      esum[ns] = (ns == 0) ? dene : esum[ns - 1] + dene;
      //      if ( ns == 0 ) esum[ns]= dene;
      //      else esum[ns]= esum[ns-1]+ dene;
      ns++;
      iter++;
    }

    //   Get a random number between 0 and total neighbor energy sum
    double upper = esum[ns - 1] * RandFlat::shoot(rng);

    //   Shift the iterator 
    iter = atci->itsNeighbours()->begin();
    for (int k = 0; k < ns && esum[k] < upper; k++)
      iter++;

    const TwoCoordIndex* tci2 = dynamic_cast<const TwoCoordIndex*> (*iter);
    atci = tci2;

    HepPoint newpos = _emcmodel->whereLocal(*atci, elemId);

    if ((newpos - origpos).mag() > hadShowerMaxR) break; // running outside

  }

}

bool
PacEmcClusterMeas::addDigiToCluster(const TwoCoordIndex* tci,
    PacEmcCluster* cluster, double energy, const GTrack* gtrk,
    double forceAdd) const
{

  bool retval(false);

  const double cutoff = _emcmodel->ecutoff(*tci);

  double e(energy);
  if (e < forceAdd) {
    e = forceAdd;
  }

  if (e < cutoff) { //  below cutoff, no digi is added
    retval = false;
  } else {
    PacEmcDigi adigi(e, *tci);
    adigi.setWaveFormPeakE(e * _wfmax / _wfattp);
    adigi.setWaveFormInTime(_wftime);
    adigi.setSimEnergy(e);
    if (gtrk) adigi.addGTrackWeight(gtrk, 1.0);
    cluster->addDigi(adigi, true);
    retval = true;
  }
  return retval;
}

double
PacEmcClusterMeas::energyFraction(const PacSimHit& hit,
    const TwoCoordIndex* tci, const HepPoint &ptci, double dtheta,
    double dphi, double RMxp, double RMxm,
    double RMyp, double RMym, double entryth,
    double entryphi, FillMethod method) const
{

  // path length and then project onto the detector surface
  //  assume flat
  double flt = hit.detIntersection().pathrange[1]
      - hit.detIntersection().pathrange[0];
  double fltx = fabs(flt * entryth);
  double flty = fabs(flt * entryphi);
  double fltxy = sqrt(fltx * fltx + flty * flty);

  // Hit position relative to the EMC origin
  const HepPoint phit = hit.position() - _emcmodel->origin();
  double rhit = phit.mag();
  double rp1 = sqrt(phit.x() * phit.x() + phit.y() * phit.y());

  // low edge of this crystal in theta relative to the hit center
  double x1 = rhit * tan(ptci.theta() - dtheta / 2. - phit.theta());
  // high edge of this crystal in theta relative to the hit center
  double x2 = rhit * tan(ptci.theta() + dtheta / 2. - phit.theta());
  // low edge of this cryster in phi relative to the hit center
  double dphi1 = ptci.phi() - dphi / 2 - phit.phi();
  while (dphi1 > Constants::pi)
    dphi1 -= Constants::twoPi;
  while (dphi1 < -Constants::pi)
    dphi1 += Constants::twoPi;
  double y1 = rp1 * tan(dphi1);
  // high edge of this cryster in phi relative to the hit center
  double dphi2 = ptci.phi() + dphi / 2 - phit.phi();
  while (dphi2 > Constants::pi)
    dphi2 -= Constants::twoPi;
  while (dphi2 < -Constants::pi)
    dphi2 += Constants::twoPi;
  double y2 = rp1 * tan(dphi2);

  const double halfgap = 0.5 * _emcmodel->crystalGap(*tci);
  x1 += halfgap;
  x2 -= halfgap;
  y1 += halfgap;
  y2 -= halfgap;

  // Asymmetric RM
  double RMx1(0), RMx2(0), RMy1(0), RMy2(0);

  if (method == GRPansatz || method == Gaussian || method == tripleGaussian) {
    RMx1 = (x1 > 0) ? RMxp : RMxm;
    //    if ( x1>0 ) RMx1= RMxp;
    //    else RMx1= RMxm;
    RMx2 = (x2 > 0) ? RMxp : RMxm;
    //    if ( x2>0 ) RMx2= RMxp;
    //    else RMx2= RMxm;
    RMy1 = (y1 > 0) ? RMyp : RMym;
    //    if ( y1>0 ) RMy1= RMyp;
    //    else RMy1= RMym;
    RMy2 = (y2 > 0) ? RMyp : RMym;
    //    if ( y2>0 ) RMy2= RMyp;
    //    else RMy2= RMym;

    // inflate the RM if a particle comes in in an angle
    RMx1 = sqrt(RMx1 * RMx1 + fltx * fltx);
    RMx2 = sqrt(RMx2 * RMx2 + fltx * fltx);
    RMy1 = sqrt(RMy1 * RMy1 + flty * flty);
    RMy2 = sqrt(RMy2 * RMy2 + flty * flty);
  }

  double retval(0);
  switch (method) {
  case GRPansatz:
    retval = frGRPansatzIntegral(x1 * 3 / RMx1, x2 * 3 / RMx2, y1 * 3 / RMy1,
        y2 * 3 / RMy2);
    break;
  case Gaussian:
    retval = gaussIntegral(x1 / RMx1, x2 / RMx2) * gaussIntegral(y1 / RMy1, y2
        / RMy2);
    break;
  case tripleGaussian:
    retval = tripleGaussIntegral(x1, x2, 0.5 * (RMxp + RMxm))
        * tripleGaussIntegral(y1, y2, 0.5 * (RMyp + RMym));
    break;
  case Line:

    // Everything here has been projected onto a 2D plane.
    if (fltxy < 0.1) {
      // if hit is within the crystal, return 1
      if (x1 * x2 < 0 && y1 * y2 < 0) retval = 1;
      else retval = 0;
    } else {

      // Define the total flight segment. Remember that the fastsim layer
      // radius is at the middle of the supposedly thick detector component.
      double xseg = fltx / 2.;
      double yseg = flty / 2.;

      // Figure out the overlap between the flight segment and the 
      // rectangle (x1,y1)(x2,y2)
      double xr = min(max(xseg, -xseg), x2);
      double xl = max(min(xseg, -xseg), x1);
      if (xr < x1 || xl > x2) { // without overlap
        retval = 0;
        break;
      }
      double yup = min(max(yseg, -yseg), y2);
      double ylo = max(min(yseg, -yseg), y1);
      if (yup < y1 || ylo > y2) { // without overlap
        retval = 0;
        break;
      }

      if (fabs(xseg) < 0.01) {
        retval = (yup - ylo) / fltxy;
      } else if (fabs(yseg) < 0.01) {
        retval = (xr - xl) / fltxy;
      } else {
        double slp = yseg / xseg;
        if (slp > 0) {
          yup = min(yup, xr * slp);
          ylo = max(ylo, xl * slp);
        } else {
          yup = min(yup, xl * slp);
          ylo = max(ylo, xr * slp);
        }
        retval = (yup - ylo) / flty;
      }
    }

    break;

  default:
    break;
  }

  return retval;
}

double
PacEmcClusterMeas::gaussIntegral(double x1, double x2) const
{

  static const double sqrt2 = 1.41421356237309515;

  int sign = 1; // same sign
  if ((x1 > 0 && x2 < 0) || (x1 < 0 && x2 > 0)) sign = -1; // opposite sign

  double sx1 = fabs(x1);
  double sx2 = fabs(x2);

  double x1int = sx1 > 5 ? 1.0 : erf(sx1 / sqrt2);
  double x2int = sx2 > 5 ? 1.0 : erf(sx2 / sqrt2);

  return fabs(0.5 * (x2int - sign * x1int));
}

double
PacEmcClusterMeas::tripleGaussIntegral(double x1, double x2,
    double RM) const
{

  // To account for the angle between track entry momentum and hit position 
  // vector in order to modify the lateral shower shape
  // ==>> Need to know the longitudinal shower length (FIXME)

  // The radial distribution of an EM shower can be fit with a
  // empirical function
  //     2*r / (1+r^2)^2
  // The integral from 0 to 3 equals to 0.9, so Moliere radius = 3
  // We can fit this distribution to a sum of 3 Gaussians and rescale
  // the length unit. The triple-Gaussian parameters are (in the unit of
  // Moliere radius) :
  const double sigma_1 = 0.1917 * RM;
  const double sigma_2 = 0.4403 * RM;
  const double sigma_3 = 1.2352 * RM;
  const double frac_2 = 0.3931;
  const double frac_3 = 0.0837;

  double sx11 = x1 / sigma_1;
  double sx12 = x1 / sigma_2;
  double sx13 = x1 / sigma_3;
  double sx21 = x2 / sigma_1;
  double sx22 = x2 / sigma_2;
  double sx23 = x2 / sigma_3;

  double g1 = gaussIntegral(sx11, sx21);
  double g2 = gaussIntegral(sx12, sx22);
  double g3 = gaussIntegral(sx13, sx23);

  return (1 - frac_2 - frac_3) * g1 + frac_2 * g2 + frac_3 * g3;

}

double
PacEmcClusterMeas::frGRPansatzFunc(double x, double y) const
{

  // function for R=RM/3=1
  // The integral is pi; will be scaled down after the integral
  //  const double dem= x*x+y*y+1;
  return 1. / (pow(x * x + y * y + 1, 2));
}

double
PacEmcClusterMeas::frGRPansatzIntegral(double x1, double x2,
    double y1, double y2) const
{

  // Integrate the shower profile over [x1,x2][y1,y2], assuming
  // the radial profile satisfies the ansatz [NIM A290,469]
  // f(r)dr = 2r R^2 / (r^2+R^2)^2 dr
  //
  // Integral of r from 0 to R equal to 0.5
  // Integral of r from 0 to 3R equal to 0.9  ==> RM = 3R
  //
  // x and y are must be rescaled so that R= RM/3 = 1. I.e. x-> x*3/RM
  //
  // Transform to dxdy
  // \int f(r)dr = R^2/(pi) * \int (x^2+y^2+R^2)^{-2} dx dy
  //
  // Here we use the 2D Simpson rule for the integral.
  //
  // 20090602: A tail that falls off slower is added
  //------------------------------------------------------------------

  double dx = (x2 - x1);
  double dy = (y2 - y1);

  double x0 = x1 + dx / 2;
  double y0 = y1 + dy / 2;
  double prevsum = dx * dy * frGRPansatzFunc(x0, y0) / Constants::pi;

  double precision;
  if (prevsum < 1e-4) precision = 1e-1;
  else if (prevsum < 1e-3) precision = 3e-2;
  else if (prevsum < 1e-2) precision = 1e-2;
  else precision = 1e-3;

  prevsum *= 12. * Constants::pi; // Scaled to be compared to the integral (sum).

  int nb(0);
  double sum(0);
  const double maxiter = 10;
  double err = 1;

  while (fabs(err) > precision && nb < maxiter) {
    if (nb > 0) prevsum = sum;
    ++nb;

    dx = (x2 - x1) / nb;
    dy = (y2 - y1) / nb;
    sum = 0;

    for (int i = 0; i < nb; ++i) {
      double xa = x1 + i * dx;
      double xb = xa + dx;
      x0 = xa + dx / 2;
      for (int j = 0; j < nb; ++j) {
        double ya = y1 + j * dy;
        double yb = ya + dy;
        y0 = ya + dy / 2;
        //  some code speedup?
        // 	sum+= frGRPansatzFunc(xa,ya);
        // 	sum+= frGRPansatzFunc(xa,yb);
        // 	sum+= frGRPansatzFunc(xb,ya);
        // 	sum+= frGRPansatzFunc(xb,yb);
        // 	sum+= 8*frGRPansatzFunc(x0,y0);
        sum += frGRPansatzFunc(xa, ya) + frGRPansatzFunc(xa, yb)
            + frGRPansatzFunc(xb, ya) + frGRPansatzFunc(xb, yb) + 8
            * frGRPansatzFunc(x0, y0);
      }
    }

    sum *= (dx * dy);
    err = (sum - prevsum) / sum;
  }

  sum /= (12. * Constants::pi);

  return sum;

}

//---------------------------------------------------------------------
// Signal shape height in the timing window
double
PacEmcClusterMeas::energyDeposition(const PacSimHit& hit) const
{

  double e0 = hit.energyChange();
  //  return e0;
  double dist = (hit.position() - _emcmodel->origin()).mag() / Constants::c;  // in sec.
  double t0 = hit.time() - dist;  // adjust for the distance from the origin

  _hitregion = _emcmodel->region(hit.position());

  _wftime = PacEmcDigi::OutOfSigWin;
  _wfmax = 1.0;
  _wfattp = 1.0;

  double fraction = 0;
  if (inTime(t0, fraction) == PacMeasurement::OutOfTime) return 0;

  if (isnan(e0 * fraction)) {
    ErrMsg(warning) << "hit.position= " << hit.position() << "  e0 = " << e0
        << "   t0 = " << t0 << " region = " << _hitregion << "   fraction =  "
        << fraction << endmsg;
  }

  // Set pulse maximum in accepted time region
  if ( t0 >= _emcmodel->signalTLo(_hitregion) && t0 < _emcmodel->signalTHi(_hitregion) ) {
    _wftime= PacEmcDigi::InSigWin;
    _wfmax= 1.0;
    _wfattp= 1.0;
    fraction= 1;
  } else if ( t0 < _emcmodel->signalTLo(_hitregion) && t0 >= _emcmodel->acceptWindowLo(_hitregion) ) {
    _wftime= PacEmcDigi::OutOfSigWin;
    _wfmax= 1.0;
    _wfattp= _emcmodel->pulseShapeNorm(_emcmodel->pulseTshift(_hitregion),t0,_hitregion);
  } else if ( t0 < _emcmodel->acceptWindowLo(_hitregion) ) {
    _wftime= PacEmcDigi::OutOfSigWin;
    _wfmax= _emcmodel->pulseShapeNorm(_emcmodel->acceptWindowLo(_hitregion)+_emcmodel->pulseTshift(_hitregion),t0,_hitregion);
    _wfattp= _emcmodel->pulseShapeNorm(_emcmodel->pulseTshift(_hitregion),t0,_hitregion);
  }

  return e0 * fraction;

}

PacMeasurement::MeasurementTime
PacEmcClusterMeas::inTime(double time, double& fraction) const
{

  fraction = _emcmodel->signalFraction(time, _hitregion);
  if (fraction <= 0) return PacMeasurement::OutOfTime;
  return PacMeasurement::Mixed;
}


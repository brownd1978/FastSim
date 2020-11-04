// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkHitViewDch
// Copyright Information:
//	Copyright (C) 2008	Lawrence Berkeley Laboratory
//
//  Authors: Dave Brown, 14May2008
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "BbrGeom/BbrAngle.hh"
#include "PacTrk/PacTrkHitMeas.hh"
#include "PacTrk/PacTrkHitViewDch.hh"
#include "PacTrk/PacHitOnTrk.hh"
#include "PacSim/PacSimHit.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "PacEnv/PacConfig.hh"
#include "GenEnv/GenEnv.hh"
#include "AbsEnv/AbsEnv.hh"
#include "DetectorModel/DetMaterial.hh"
#include "DetectorModel/DetSurfaceElem.hh"

#include "G3Data/GTrack.hh"

#include "DetectorModel/DetSurfaceType.hh"
#include "PacGeom/PacPieceTraj.hh"
#include <iostream>
#include "PacGeom/PacCylDetType.hh"

#include "ErrLogger/ErrLog.hh"

using namespace std;

// axial (sangle=0) and stereo wire constructor. Note that the hitview definitions don't really make sense, FIXME!!!
PacTrkHitViewDch::PacTrkHitViewDch(const PacHitRes& hres,double ngfrac, double ngfactor,
				   double eff_par0, double eff_par1, 
				   double cell_size, double sangle) : 
	PacTrkHitView(sangle == 0.0 ? TrkEnums::xyView : TrkEnums::zView),
	_hres(hres),_ngfrac(ngfrac),_ngfactor(ngfactor),
	_eff_par0(eff_par0),
	_eff_par1(eff_par1),
	_cell_size(cell_size),
	_sangle(sangle)
{
  // fetch parameters.  These should be keyed to the measurement name  FIXME!!!
  _mincpath = gconfig.getdouble("mincellpath",0.0);
  // drift velocity should come from the measurement: FIXME!!!!
  _vdrift = 5e6; // 50 um/nsec in cm/sec
  // set cache values
	_sins = sin(_sangle);
	_coss = cos(_sangle);
  // correct RMS for non-Gausian factors
  _ngcor = sqrt(1.0-_ngfrac + _ngfrac*_ngfactor*_ngfactor);
}

PacTrkHitViewDch::~PacTrkHitViewDch()
{}

void
PacTrkHitViewDch::getHitInfo(const PacSimHit& simhit,std::vector<PacRecHitInfo>& hvec) const {
	// find normal to surface
  const DetSurfaceElem* delem = simhit.detElem()->surfElem();
  assert(delem != 0);
  const DetSurface* surf = delem->surface();
  assert(surf != 0);
  SurfacePoint sp;
  // the following fills the sp, don't remove the call
  if(0 != surf->surfacePoint(simhit.position(),sp) )
    ErrMsg(error) << "hit point not on surface ! " << endmsg;
  Hep3Vector norm = surf->normal(sp);
  // wires are along local 'y'
  Hep3Vector localy = surf->surfaceDirection(sp,1);
  Hep3Vector localx = surf->surfaceDirection(sp,0);    
// stereo wires are tilted in the local coordinate plane, forced to lie on the surface
// compute the axis
	Hep3Vector wdir = _coss*localy + _sins*localx;
// measurement direction is perp to this and the normal
  Hep3Vector mdir = wdir.cross(norm);
// compute the cross-product of this view's direction with the track; that is the direction
// constrained by this hit (poca)
  Hep3Vector tdir = simhit.momentum();
  Hep3Vector pdir = wdir.cross(tdir).unit();
// determine how many cells were crossd.  Due to curvature, this can be >1
// base this on the pathlength transverse to the wire, divided by the cells (transverse) size
  double pathlen = simhit.detIntersection().pathrange[1]-simhit.detIntersection().pathrange[0];
  Hep3Vector tperp = (tdir.unit()).perpPart(wdir);
  double cpath = pathlen*tperp.mag()/_cell_size;
// count the # of integral cells crossed, and the remainder cell fraction
  unsigned ncells = (unsigned)floor(fabs(cpath - _mincpath));
  double cfrac = cpath-ncells*_cell_size;
// get random engine
  HepRandomEngine* rng = HepRandom::getTheEngine();
// if the cell fraction is large enough, allow it to generate a hit
  if(cfrac - _mincpath > RandFlat::shoot(rng)) ncells++;
// require at least 1 cell
  ncells = std::max((unsigned)1,ncells);
// start and step for each hit
  double pstep = pathlen/ncells;
  double pstart = -0.5*pathlen*(ncells-1)/ncells;
// generate random positions and resolutions for each cell
  for(unsigned icell=0;icell<ncells;icell++){
// create a hit info object for this hit
    PacRecHitInfo hinfo;
// generate uniformly-spaced hits across the flightlength
    hinfo._flentrue = simhit.globalFlight() + pstart+icell*pstep;
// generate a hit position randomly smeared by the resolution about this point, in the measurment direction
    hinfo._simpos = simhit.globalTraj()->position(hinfo._flentrue);
// note we do not recompute the directions for each hit as that is a 2nd order correction
    hinfo._wdir = wdir; // this is the same for all hits from this simhit
    hinfo._mdir = mdir; // this is the same for all hits from this simhit
    hinfo._pdir = pdir; // this is the same for all hits from this simhit
// sample resolution function for this hit, and smear the 'reconstructed' position along the measurment direction
    double x = RandFlat::shoot(rng);
    double deltax = RandGauss::shoot(rng,0,hinfo._sres);
    hinfo._hitpos = hinfo._simpos + deltax*pdir;
// randomize the error assigned to this hit by the same amount.  Remember that this is relative to the cell size
    double smearx = std::min(1.0,std::max(0.0,x + deltax/_cell_size));
    hinfo._herr = _hres.resFun(smearx)*_ngcor;
// compute the smearing, including non-Gaussian effects
    hinfo._sres = _hres.resFun(x);
    if(_ngfrac > RandFlat::shoot(rng))
      hinfo._sres *= _ngfactor;
// store these informations
    hvec.push_back(hinfo);
  }
  return;
}

double 
PacTrkHitViewDch::efficiency(const PacSimHit& simhit) const {
  double dx =  simhit.detIntersection().pathLength();
  const PacCylDetType* ctype = static_cast<const PacCylDetType*>(simhit.detIntersection().delem->detectorType());
  double h = ctype->thick();//cell height [cm]
  double eff = _eff_par0*(1-pow(1-_eff_par1,dx/h));

  return eff;
}

double
PacTrkHitViewDch::measurementAngle() const {
  return _sangle;
}

double
PacTrkHitViewDch::sensorSeparation() const {
  return _cell_size;
}

double
PacTrkHitViewDch::sensorSize() const {
// wires are quasi-infinite
  return 1e8;
}

#include "BaBar/BaBar.hh"
#include "BaBar/Constants.hh"
#include "PacSim/PacDetEffect.hh"
#include "PacSim/PacShowerInfo.hh"
#include "PacSim/PacSimTrack.hh"
#include "CLHEP/Geometry/HepPoint.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandGaussQ.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandExponential.h"
#include "CLHEP/Random/RandPoisson.h"
#include "CLHEP/Random/RandLandau.h"
#include "DetectorModel/DetMaterial.hh"
#include "PDT/PdtEntry.hh"
#include "PacEnv/PacConfig.hh"

// random number
using namespace std;
#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

// statics

const Hep3Vector PacDetEffect::_nullmom(0.0,0.0,0.0);

// constructor

PacDetEffect::PacDetEffect() :
  _smallangle(0.01)
  {
	// Get the random number enginae
	_randengine = HepRandom::getTheEngine();
	// get landau configuration
	_lanshift = gconfig.getdouble("PacMaterial.lanshift",-999.);
	_lanrms = gconfig.getdouble("PacMaterial.lanrms",-1);
	assert((_lanshift != -999.) && (_lanrms >= 0.));
	
	// DetMaterial parameters
  double ione = gconfig.getdouble("PacMaterial.IonLossEnergy",-1.0);
  double minkappa = gconfig.getdouble("PacMaterial.IonLossMinKappa",-1.0);
  assert(ione>=0.0 && minkappa >= 0);
  
  _maxnsingle = gconfig.getdouble("PacMaterial.maxnsingle",10.0);
  _maxradfrac1 = gconfig.getdouble("PacMaterial.maxradfrac1",0.1);
  _maxradfrac2 = gconfig.getdouble("PacMaterial.maxradfrac2",10.);
	
// energy loss formulas
  DetMaterial::setEnergyLossScale(ione);
  DetMaterial::setMinimumKappa(minkappa);
		
	// shower properties
	_nshower = gconfig.getint("PacShower.nshower",-1);
  _showerradfrac = gconfig.getdouble("PacShower.showerradfrac",-1);
  
  assert(_nshower>0 );
  
  // discrete vs shower interaction thresholds
  _thinfract = gconfig.getdouble("PacShower.thinfract",-1.0);
  
  // minimum momenta

  _minmom_e = gconfig.getdouble("PacShower.minmom_e",-1.0);
  _minmom_gamma = gconfig.getdouble("PacShower.minmom_gamma",-1.0);
  _minmom_had = gconfig.getdouble("PacShower.minmom_had",-1.0);
  _mine = gconfig.getdouble("PacShower.mine",-1.0);
  
  assert(_thinfract>0.0 && _minmom_e > 0.0 && _minmom_gamma > 0.0 && _minmom_had > 0.0 && _mine > 0.0);
}

// compute effect of this detector elment on a particle, including possible showering.
PacSimHit::effect
PacDetEffect::detEffect(PacSimHit::effect oldeff,double pathlen,const DetMaterial* mat,const PdtEntry* pdt,
  const PacShowerInfo* oldshower, Hep3Vector& mom, PacShowerInfo& sinfo) const {
  PacSimHit::effect eff(PacSimHit::none);
  if(mat != 0){
// find the effect
// check if we've already started to shower
    if(oldeff >= PacSimHit::shower ){
      eff = oldeff;
    } else if(isHadron(pdt)) {
      eff = hadEffect(mat,pathlen,pdt,mom);
    } else if(isEM(pdt)){
      eff = emEffect(mat,pathlen,pdt,mom);
    } else {
// Here we must have muons.  Compute momentum change due to scattering and energy loss
      eff = normal(mat,pathlen,pdt,mom);
    }
// compute the shower effect.
    if(eff >= PacSimHit::shower){
//  First, find the shower model associated to this material/particle
      const PacShowerModel* smodel = showerModel(mat,pdt);
// if the shower already started, used the old information, otherwise start from scratch
      if(oldshower != 0)
        smodel->showerStep(mat,pathlen,pdt,*oldshower,sinfo);
      else {
// compute a random starting point for the shower.  This updates the pathlen (start position)
// and potentially momentum (due to ionization loss for charged particles).
        findShowerStart(mat,pdt,pathlen,mom);
// initialize the shower with this incoming energy
        PacShowerInfo init(DetMaterial::particleEnergy(mom.mag(),pdt->mass()));
        smodel->showerStep(mat,pathlen,pdt,init,sinfo);
      }
// update momentum
      double newmag = DetMaterial::particleMomentum(sinfo.energyOut(),pdt->mass());
      if(newmag > 0.0)
        mom.setMag(newmag);
      else
        mom = _nullmom;
    }
  } else if(pdt->charge() != 0){
// if there's no material, we may just be marking the passage.  This is only relevant for
// charged (position) measurments
    eff = PacSimHit::mark;
  } else
    eff = PacSimHit::none;
  return eff;
}
   
// compute effect of this detector elment on a particle.  In addition to the return value, the momentum
// and input hit are updated.
PacSimHit::effect
PacDetEffect::detEffect(const DetIntersection& dinter,const HepPoint& pos,Hep3Vector& mom,
  PacSimTrack* strk) const {
// get information from last hit (if it exists)
  PacSimHit::effect oldeff(PacSimHit::none);
  const PacShowerInfo* oldshower(0);
  if(strk->getHitList().size() > 0){
    const PacSimHit& lasthit = strk->getHitList().back();
    oldeff = lasthit.detEffect();
    oldshower = lasthit.showerInfo();
  }
  Hep3Vector oldmom(mom);
  const PdtEntry* pdt = strk->pdt();
  double pathlen = dinter.pathLength();
  if(pathlen < 0.0){
    ErrMsg(error) << "negative pathlength through element!!! " << endmsg;
    return PacSimHit::stop;
  }
  //	find material.  We have to deal with the broken DetectorModel interface
  const DetMaterial* mat = &(dinter.delem->material(dinter));
// compute effect
  PacShowerInfo sinfo;
  PacSimHit::effect eff = detEffect(oldeff,pathlen,mat,pdt,oldshower,mom,sinfo);
// if there's a new effect, create a new hit and push it into the PacSimTrack
  if(eff > PacSimHit::none){
    // NA: add time information
    PacSimHit newhit(strk,dinter,pos,oldmom,mom,eff,strk->timeOfLastHit());
// add shower decorator if necessary
    if(eff >= PacSimHit::shower)
      newhit.addShowerInfo(sinfo);
    strk->addHit(newhit);
  }
  return eff;
}

PacSimHit::effect
PacDetEffect::normal(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,
	Hep3Vector& mom) const {
// default effect of charged particles in material is 'normal' ionization energy loss and multiple Coulomb scattering.
// default for neutrals is nothing.
    bool charged = pdt->charge() != 0;
    if(!charged)
      return PacSimHit::none;
// kinematics
		double mass = pdt->mass();
		double mommag = mom.mag();
		double energy = DetMaterial::particleEnergy(mommag,mass);
		double newmommag;
// compute most probable energy loss.  Note that this will never
// be more than the available kinetic energy, so test for stopping BEFORE
// smearing
		double elossmean = fabs(mat->energyLoss(mommag,pathlen,mass));
		if(energy-elossmean-mass< _mine){
// particle stops completely
			mom = _nullmom;
			return PacSimHit::stop;		  
		}
// compute energy loss RMS.
		double elossrms = mat->energyLossRMS(mommag,pathlen,mass);
// landau curve.  The most probable value is slightly shifted,
// and the (restricted) sigma is wider than 1.  The negative sigma is =1.
    double ranlan = RandLandau::shoot(_randengine);
//    double eloss = std::max(0.0,elossmean + (ranlan+_lanshift)*elossrms/_lanrms);
// landau model is causing problems: revert to Gaussian
    double eloss = 	std::max(RandGaussQ::shoot(_randengine,elossmean,elossrms),0.0);
		energy -= eloss;
// make sure the energy loss is physical
		if(energy - mass > _mine){
			newmommag = DetMaterial::particleMomentum(energy,mass);
			mom.setMag(newmommag);
		} else {
// particle stops completely
			mom = _nullmom;
			return PacSimHit::stop;
		}
// use average momentum to compute scattering
		double avgmom = 0.5*(mommag+newmommag);
// Compute 2 2-dimensional scattering angles using prescription in CPC 141 (2001) 230-246, Fruhwirth etal
    double scat[2];
    scatter(scat,mat,avgmom,pathlen,mass);
// compute the scattering basis vectors WRT the particle: phi, and theta perp to the track
    Hep3Vector phihat, thetahat;
// If the tracks is very close to z, use x and y axes
    if(fabs(mom.cosTheta())< 0.99 ){
		  double pt = mom.perp();
		  double invpt = 1.0/pt;
		  double zscale = mom.z()*invpt/newmommag;
		  phihat = Hep3Vector(-mom.y()*invpt,mom.x()*invpt,0.0); // points orhtogonal to mom, z axis
		  thetahat = Hep3Vector(mom.x()*zscale,mom.y()*zscale,-pt/newmommag);
	  } else {
      static Hep3Vector xaxis(1.0,0.0,0.0);
      static Hep3Vector yaxis(0.0,1.0,0.0);
      phihat = mom.cross(xaxis).unit();
      thetahat = mom.cross(phihat).unit();
    }
// compute multiple scattering angle in space
// rotate the momentum vector; if the angle is small, simply use vector addition.
// if it's large, use a rotation
    double sangle = sqrt(scat[0]*scat[0] + scat[1]*scat[1]);
		if(sangle < _smallangle){
			mom += avgmom*(scat[0]*phihat + scat[1]*thetahat);
		} else {
// use the total scatter angle to rotate the track.  This results in a scater 90degrees
// from the one above, but since the direction is random this doesn't matter
			Hep3Vector scataxis = scat[0]*phihat + scat[1]*thetahat;
			HepRotation srot(scataxis,sangle);
			mom = srot(mom);
		}
// Add effect of BField inhomogeneity someday
		return PacSimHit::normal;
}

PacSimHit::effect
PacDetEffect::hadEffect(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,Hep3Vector& mom) const {
  PacSimHit::effect retval;
	double numint = pathlen/mat->intLength();
  double intprob = exp(-numint);
  if(intprob >  RandFlat::shoot(_randengine))
    retval =  normal(mat,pathlen,pdt,mom);
	else {
// for thin materials, interact.  Require a minimum energy to produce a daughter
	  if(numint < _thinfract && mom.mag() > _minmom_had ){
  	  mom = _nullmom;
		  return PacSimHit::interact;
    } else {
// otherwise, shower.
      retval = PacSimHit::hadshower;
    }
  }
  return retval;
}

PacSimHit::effect
PacDetEffect::emEffect(const DetMaterial* mat,double pathlen,const PdtEntry* pdt,
  Hep3Vector& mom) const {
// compute the fraction of a radiation length traversed by this particle
  double radfrac = mat->radiationFraction(pathlen);
// if radfrac is very large, this is always a shower
  if(radfrac > _showerradfrac){
    return PacSimHit::shower;
  }
  PacSimHit::effect retval = PacSimHit::none;
// kinetic energy (approximately)
  double energy = mom.mag();
// separate behavior of photons and electrons
  bool charged = pdt->charge() != 0;
  if(charged) {
// Electrons loose energy due to bremsstrahlung.  The following updates
// the energy, even if nbrems = 0 (nbrems counts high-energy photons)
    unsigned nbrems = _brems.nBrems(radfrac,energy);
// if we have more than 1 brems photons, consider this to be the start of a shower.
    if(nbrems > _nshower ){
      retval = PacSimHit::shower;
// otherwise, model the discrete effect of the brems photons
    } else {
// update the momentum
      mom.setMag(energy);
// also account for ionization loss
      retval = normal(mat,pathlen,pdt,mom);
// if there are high-energy brems, set this as the return value
      if(nbrems > 0)
        retval = PacSimHit::brems;
    }
// if there are high-energy brems, override the return value
  } else {
// model low-momentum photons as total absorption
    if(mom.mag() < _minmom_gamma){
// fixed cross-section: FIXME!!!
      static double xsect=1.0; // in units of gm/cm^2
      double absorblen = xsect/mat->density();
      double alen = RandExponential::shoot(_randengine,absorblen);
      if( pathlen > alen){
        retval = PacSimHit::stop;
        mom = _nullmom;
      }
    } else {
// This could be either compton scatter or pair conversion.  Conversion
// dominates at BaBar energies.
// The mean free path of a photon is 9/7 the radiation length (see PDG 27.4).  This means the probability
// of conversion as
      const double convfactor = 7.0/9.0;
      double convprob = 1.0 - exp(-convfactor*radfrac);
      if(convprob > RandFlat::shoot(_randengine)){
// Allow the electrons brem in this material.  If either one does, consider
// this to be the start of a shower.  choose a random conversion point and random energy sharing.
// count the conversion itself as one daughter
        double eradfrac = RandFlat::shoot(_randengine)*radfrac;
        double e1 = RandFlat::shoot(_randengine)*energy;
        double e2 = energy - e1;
        unsigned nbrems = _brems.nBrems(eradfrac,e1)+_brems.nBrems(eradfrac,e2);
        if(nbrems > _nshower - 1 ){
// consider this to be the start of a shower
          retval = PacSimHit::shower;
        } else {
// otherwise the photon loses all its energy to the electrons
          retval = PacSimHit::convert;
          mom = _nullmom;
        }
      }
    }
//    } else {
//  To model Compton scattering we need a PDF for the energy spectrum, which is not provided by PDG.
//  this is a minor effect at BaBar/SuperB energies, so ignore for now
//  double compfrac = pow(energy*1000,-0.87); // formula approximated from PDG figure 27.14
//  if(compac < RandFlat::shoot(_randengine))      
//  retval = PacSimHit::compton;
//  double momfrac = RandExponential::shoot(_randengine,xrad);
  }
  return retval;
}

bool
PacDetEffect::isHadron(const PdtEntry* pdt) const {
  return abs((int)pdt->pdgId()) > 100;
}

bool
PacDetEffect::isEM(const PdtEntry* pdt) const {
  return abs((int)pdt->pdgId()) == (int)PdtPdg::e_minus || 
	pdt->pdgId() == PdtPdg::gamma;
}

void
PacDetEffect::findShowerStart(const DetMaterial* mat,const PdtEntry* pdt,double& pathlen,Hep3Vector& mom) const {

  if ( isHadron(pdt) ) return;  // Hadron, do nothing

  if ( isEM(pdt) ) {

    double radfrac = mat->radiationFraction(pathlen);

    double convfactor;

    bool charged = pdt->charge() != 0;

    if ( charged ) {
      convfactor = 1;
    } else {
      convfactor = 7.0/9.0;
    }

    // Is this the correct way? FIXME??
    // Shower should start between 0 and pathlen with a probability 
    // proportional to exp(-x/mean_free_path)
    double u= RandFlat::shoot(_randengine, exp(-radfrac*convfactor) , 1);
    double xstart= -log(u);  // in the unit of mean free path

    // shorten the pathlength by the depth where the shower starts.
    pathlen-= (mat->radiationLength()/mat->density()/convfactor *  xstart) ;

    assert(pathlen>0);

  }

}

const PacShowerModel*
PacDetEffect::showerModel(const DetMaterial* mat,const PdtEntry* pdt) const{
// for now, simply return EM shower model for gammas/electrons, had for everything else
  if(isEM(pdt))
    return &_emshower;
  else
    return &_hadshower;
}

void 
PacDetEffect::setRandomEngine(HepRandomEngine* engine){
  _randengine = engine;
  _emshower.setRandomEngine(engine);
  _hadshower.setRandomEngine(engine);
  _brems.setRandomEngine(engine);
}

void
PacDetEffect::scatter(double scatter[2],const DetMaterial* mat,double mom, double pathlen, double mass) const {
  scatter[0] = scatter[1] = 0.0;
// find # of single scatters
  double nsinglescat = mat->nSingleScatter(mom,pathlen,mass);
// for very thin materials, just add the effect of a random number of individual scatters.
  if(nsinglescat < _maxnsingle ){
    fewScatter(scatter,nsinglescat,mat,mom);
  } else {
    double radfrac = pathlen*mat->inverseX0();
// If the rad. frac. is small, use the semi-Gauss approximation (Gaussian + hard scatter tail)
    if(radfrac < _maxradfrac1 ) {
      semiGaussScatter(scatter,mat,nsinglescat,mom,pathlen,mass);      
    } else if( radfrac < _maxradfrac2 ){
// use the multi-Gaussian approximation
      multiGaussScatter(scatter,mat,mom,pathlen,mass);
    } else {
// single gaussian model
      double sigma = mat->highlandSigma(mom,pathlen,mass);
      gaussScatter(scatter,sigma);
    }
  }
}

void
PacDetEffect::fewScatter(double scatter[2], double nsinglescat,const DetMaterial* mat,double mom) const {
// use Poissonian to pick actual # of single scatters
  unsigned nss = RandPoisson::shoot(_randengine,nsinglescat);
// compute the single scatter terms once for efficiency
  double aparam = mat->aParam(mom);
  double bparam = mat->bParam(mom);
// add these angles incoherently
  for(unsigned iss=0;iss<nss;iss++){
    singleScatter(scatter,aparam, bparam);
  }
}

void
PacDetEffect::singleScatter(double scatter[2],double aparam, double bparam) const {
// compute the space angle and direction separately, then project
  double rangle = RandFlat::shoot(_randengine);
  double sangle = aparam*bparam*sqrt((1.0-rangle)/(rangle*bparam*bparam + aparam*aparam));
  projectScatter(scatter,sangle);
}

void
PacDetEffect::projectScatter(double scatter[2],double sangle) const {
// given an angle in space, project into 2-D
  double sphi = RandFlat::shoot(_randengine,0.0,Constants::twoPi);
  scatter[0] += sangle*cos(sphi);
  scatter[1] += sangle*sin(sphi);
}

void
PacDetEffect::semiGaussScatter(double scatter[2],const DetMaterial* mat,double nsinglescatter, double mom,double pathlen,double mass) const {
// setup the parameters
  double rho = 41000/pow(mat->zeff(),2.0/3.0);
  double bpar = rho/sqrt(nsinglescatter*(log(rho)-0.5));  
  double n = pow(mat->zeff(),0.1)*log(nsinglescatter);
  double var1 = (5.783e-4*n+3.803e-2)*n+1.827e-1;
  double apar =(((-4.590e-5*n+1.330e-3)*n-1.355e-2)*n+9.828e-2)*n+2.822e-1;
  double epsilon = std::max(0.0,(1-var1)/(pow(apar,2)*(log(bpar/apar)-0.5)-var1));
  double hscat = mat->highlandSigma(mom,pathlen,mass);
// check if we're in the core or tail
  if(RandFlat::shoot(_randengine) > epsilon ){
// core; must scale by the standard scattering sigma
    double sigma = sqrt(var1)*hscat;
    gaussScatter(scatter,sigma);
  } else {
    singleScatter(scatter,apar,bpar);
// must scale to highland
    scatter[0] *= hscat;
    scatter[1] *= hscat;
  }
}

void
PacDetEffect::gaussScatter(double scatter[2],double sigma) const {
  double sangle = sigma*sqrt(-2.0*log(RandFlat::shoot(_randengine)));
  projectScatter(scatter,sangle);
}

void
PacDetEffect::multiGaussScatter(double scatter[2],const DetMaterial* mat,double mom,double pathlen,double mass) const {
  double beta = mom/sqrt(pow(mom,2)+pow(mass,2));
  double b2 = beta*beta;
  double radfrac = pathlen*mat->inverseX0();
// compute parameters
  double d1 = log(radfrac/b2);
  double d2 = log(pow(mat->zeff(),2.0/3.0)*radfrac/b2);
  double epsi;
  if( d2<0.5 ) {
    epsi=(6.096e-4*d2+6.348e-3)*d2+4.841e-2;
  } else {
    epsi=(-5.729e-3*d2+1.106e-1)*d2-1.908e-2;
  }
  double var1 = (-1.843e-3*d1+3.347e-2)*d1+8.471e-1;
  if(RandFlat::shoot(_randengine) > epsi ){
// core
    gaussScatter(scatter,sqrt(var1)*mat->highlandSigma(mom,pathlen,mass));
  } else {
// tail
    double var2 = (1-(1-epsi)*var1)/epsi;
    gaussScatter(scatter,sqrt(var2)*mat->highlandSigma(mom,pathlen,mass));
  }
}


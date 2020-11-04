//-----------------------------------------------------------------------
// File and Version Information:
//     PacEmcCluster;
//     Class for PacEmcCluster for fast simulation, a stripped down version
//     from Babar's EmcCluster.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/13)
//
//-----------------------------------------------------------------------

#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------

#include "PacEmc/PacEmcCluster.hh"

//---------------
// C++ Headers --
//---------------

#include <iostream>
#include <utility>
#include <iomanip>
#include <math.h>
#include <algorithm>
#include <float.h>
#include <assert.h>

using namespace std;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "AbsEnv/AbsEnv.hh"
#include "AbsEnv/NeighbourStore.hh"
#include "BaBar/Constants.hh"
#include "ErrLogger/ErrLog.hh"
#include "BbrGeom/BbrError.hh"
//#include "BbrStdUtils/CollectionUtils.hh"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Matrix/Vector.h"
#include "ErrLogger/ErrLog.hh"
//#include "G3Data/GTrack.hh"
#include "TrajGeom/TrkLineTraj.hh"
#include "difAlgebra/DifNumber.hh"
#include "difAlgebra/DifArray.hh"
#include "PDT/PdtEntry.hh"

#include "PacEmc/PacEmcClusterEnergySums.hh"
#include "PacEmc/PacEmcXClMoments.hh"

#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandPoisson.h"
#include "CLHEP/Random/RandExponential.h"

using std::endl;
using std::ios;
using std::ostream;
using std::setw;

bool high2low(const PacEmcDigi* d1, const PacEmcDigi* d2);

//----------------
// Constructors --
//----------------

PacEmcCluster::PacEmcCluster(CentroidMethod algo) 
  : AbsRecoCalo()
  ,_energyValid( false )
  ,_rawenergy( 0 )
  ,_calibFactor(1.0)
  ,_whereValid( false )
  ,_regionValid(false)
  ,_rangeValid(false)
  ,_foundLocalMax(false)
  ,_calibrated(false)
  ,_extendedTciValid(false)
  ,_radiusValid(false)
  ,_simtrkEnergyChange(0)
  ,_firstSimhitTime(999)
  ,_recordTime(999), _recordTimeErr(999)
  ,_nbumps(1)
  ,_region(PacEmcModel::Unknown)
  ,_theClusEnergySums( 0 )
  ,_theClusXClMoments( 0 )
  ,_centroidAlgo(algo)
  ,_tciThetaLow(-1)
  ,_tciThetaHi(-1)
  ,_tciPhiLow(-1)
  ,_tciPhiHi(-1)
  ,_shapePars(new CluShapePars())
{
}

//--------------
// Destructor --
//--------------
PacEmcCluster::~PacEmcCluster()
{

  clear();

  if ( _theClusEnergySums != 0) delete _theClusEnergySums;
  if ( _theClusXClMoments != 0) delete _theClusXClMoments;

  _theClusEnergySums =0;
  _theClusXClMoments = 0;

  delete _shapePars;

}

void
PacEmcCluster::clear() {

  PacEmcDigiIterator iter= _pacEmcDigis.begin();
  PacEmcDigiIterator iend= _pacEmcDigis.end();
  for (; iter!= iend; ++iter) {
    delete (*iter);
  }
  _pacEmcDigis.clear();
  _localMaxima.clear();
}


void
PacEmcCluster::setCalibFactor(const double x) {
  _calibFactor= x;
  _calibrated= true;
}

const std::vector< PacEmcDigi* >*
PacEmcCluster::pacEmcDigis() const {
  return &_pacEmcDigis;
}

const std::vector< PacEmcDigi* >*
PacEmcCluster::localMaxima() {
  if ( !_foundLocalMax ) searchLocalMax();
  return &_localMaxima;
}

PacEmcDigi*
PacEmcCluster::getDigi(const TwoCoordIndex& tci) const {
  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  PacEmcDigiConstIterator iend= _pacEmcDigis.end();
  for (; iter!=iend; ++iter) {
    if ( (*iter)->tciIndex() == tci.itsIndex() ) {
      return (*iter);
    }
  }
  return 0;
}

PacEmcDigi*
PacEmcCluster::getDigi(const long &itheta, const long &iphi) const {
  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  PacEmcDigiConstIterator iend= _pacEmcDigis.end();
  for (; iter!=iend; ++iter) {
    if ( (*iter)->theta()== itheta &&  (*iter)->phi()== iphi ) {
      return (*iter);
    }
  }
  return 0;
}

double
PacEmcCluster::energy(const HepPoint& from, const PdtEntry* particle ) const {

  return this->rawEnergy() * this->calibFactor();
  
}

double
PacEmcCluster::calibFactor() const {
  return _calibFactor;
//  if ( _calibrated ) {
//    return _calibFactor;
//  } else {
//    return 1.0;
//  }
}

double
PacEmcCluster::rawEnergy() const {

  if  ( ! _energyValid ) {
    double sum=0;
    
    PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
    while ( (iter != _pacEmcDigis.end() ) ) {
      sum+= (*iter)->energy();
      iter++;
    }
    
    _rawenergy = sum;
    _energyValid = true;
  }
  
  return _rawenergy;

}


double
PacEmcCluster::theta() const
{
  return where().theta();
}

double
PacEmcCluster::phi() const
{
  return where().phi();
}

// Theta and Phi in index space
double
PacEmcCluster::thetaIndex() const {
  PacEmcModel *model= PacEmcModel::getModel();
  const TwoCoordIndex* tci= model->realCoordToTci( where() );
  const double delta= model->deltaTheta(*tci);
  const double theta0= model->tciToRealThetaLocal(*tci);
  return tci->itsXCoord() + ( (where()-model->origin()).theta() - theta0 )/delta;
}

int
PacEmcCluster::thetaIndexInt() const {
  PacEmcModel *model= PacEmcModel::getModel();
  const TwoCoordIndex* tci= model->realCoordToTci( where() );
  return tci->itsXCoord();
}

double
PacEmcCluster::phiIndex() const {
  PacEmcModel *model= PacEmcModel::getModel();
  const TwoCoordIndex* tci= model->realCoordToTci( where() );
  const double delta= model->deltaPhi(*tci);
  const double phi0= model->tciToRealPhiLocal(*tci);
  return tci->itsYCoord() + ( (where()-model->origin()).phi()-phi0)/delta;
}

int
PacEmcCluster::phiIndexInt() const {
  PacEmcModel *model= PacEmcModel::getModel();
  const TwoCoordIndex* tci= model->realCoordToTci( where() );
  return tci->itsYCoord();
}

double
PacEmcCluster::thetaFromIndex() const {
  return 0; // FIXME
}

double
PacEmcCluster::phiFromIndex() const {
  return 0; // FIXME
}


double
PacEmcCluster::major_axis() const {
  return 0; // FIXME
}

double
PacEmcCluster::radius() const {
  if (_radiusValid) return _radius;

  _radius= 0;
  PacEmcModel *model= PacEmcModel::getModel();

  for ( int i=0; i< _pacEmcDigis.size(); i++ ) {
    const PacEmcDigi *digi= _pacEmcDigis[i];
    // distance between digi center to cluster centroid
    double dist= digi->where().distanceTo( this->where() );
    // add half the crystal diagonal
    double dth= model->deltaTheta( *digi->tci() )/2.;
    double dph= model->deltaPhi( *digi->tci() )/2.;
    HepPoint pdigi= digi->where()- model->origin();

    double drho2= pdigi.x()*pdigi.x()+pdigi.y()*pdigi.y();
    double dR2= drho2 +pdigi.z()*pdigi.z();

    dist+= sqrt(dR2*dth*dth + drho2*dph*dph);

    if ( dist > _radius ) _radius= dist;
  }

  _radiusValid= true;
  return _radius;
}


HepSymMatrix
PacEmcCluster::secondMomentMatrix() const {
  
  if ( _secondMoment.num_col() == 2 && _secondMoment[0][0]>0 ) {
    return _secondMoment;
  }

  static double Pi = Constants::pi;
  static double TwoPi = Constants::twoPi;

  double mxx(0), myy(0), mxy(0);  // x= theta;  y=phi;
  double esum(0);
  PacEmcDigiConstIterator diter= _pacEmcDigis.begin();

  while ( diter!= _pacEmcDigis.end() ) {

    double x= (*diter)->thetaReal() - where().theta();
    double y= (*diter)->phiReal() - where().phi();

    while ( x > Pi )  x-= TwoPi;
    while ( x < -Pi ) x+= TwoPi;
    while ( y > Pi )  y-= TwoPi;
    while ( y < -Pi ) y+= TwoPi;

    double xw = (*diter)->deltaTheta();
    double yw = (*diter)->deltaPhi();

    double energy= (*diter)->energy();
    esum+= energy;

    mxx+= energy* (x*x + xw*xw/12.);
    myy+= energy* (y*y + yw*yw/12.);
    mxy+= energy* x*y;
    
    //cout << x << "  " << y << "  " << xw << "  " << yw << "  " << energy << endl;

    diter++;
  }
  
  _secondMoment= HepSymMatrix(2,0);
  _secondMoment[0][0]= mxx/esum;
  _secondMoment[1][1]= myy/esum;
  _secondMoment[1][0]= mxy/esum;

  //cout << _secondMoment << endl;

  return _secondMoment;
}


const PacEmcDigi*
PacEmcCluster::maxima() const {

  double max=-9;
  PacEmcDigi *biggest=0;
  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  while ( iter != _pacEmcDigis.end() ) {
    if ( max < (*iter)->energy() ) {
      max = (*iter)->energy();
      biggest= *iter;
    }
    iter++;
  }
  
  return biggest;
}

HepPoint
PacEmcCluster::position( const HepPoint &p,
			 const PdtEntry* ) const {
  return this->where() - Hep3Vector(p.x(), p.y(), p.z());
}

HepPoint
PacEmcCluster::rawPosition( const HepPoint &p,
			    const PdtEntry* ) const {
  return this->position() - Hep3Vector(p.x(), p.y(), p.z());
}

BbrError
PacEmcCluster::errorMatrix( const HepPoint &from, const PdtEntry* particle ) const {
  
  PacEmcModel *model= PacEmcModel::getModel();

  double clEnergy= this->rawEnergy();
  PacEmcModel::EmcRegion theregion = this->region();
  //  double clTheta= this->theta();
  //  PacEmcModel::EmcRegion region = model->region(clTheta);

  // Energy resolution
  double pp = model->energyResEPower(theregion);
  double fa = (model->energyResCoef(theregion)) / pow(clEnergy, pp) ;
  double fb = model->energyResConst(theregion);

  double errEnergy = clEnergy* sqrt(fa*fa + fb*fb);

  // Angular resolution
  double errTheta = model->thetaResolution(theregion);
  double errPhi = model->phiResolution(theregion);

  // Third, set arbitrary radial error
  // ---------------------------------
  double errR = 10;  

  BbrError theError(4);

  theError[0][0] = errEnergy*errEnergy ; 
  theError[1][1] = errTheta*errTheta ; 
  theError[2][2] = errPhi*errPhi ; 
  theError[3][3] = errR*errR ; 
                       
  return theError ;
}


BbrError
PacEmcCluster::errorMatrixXYZ( const HepPoint &from, const PdtEntry* particle ) const {
  
  const BbrError errors( errorMatrix( from, particle ) );
  const HepPoint point ( position( from, particle ) );
  DifNumber eDF(energy(from, particle),1,4), tDF(point.theta(),2,4), pDF(point.phi(),3,4), rDF(point.mag(),4,4);
  DifArray pars(4,4);
  pars[0]   =  rDF*sin(tDF)*cos(pDF);
  pars[1]   =  rDF*sin(tDF)*sin(pDF);
  pars[2]   =  rDF*cos(tDF);
  pars[3]   =  eDF;
  BbrError result(errors.similarity(pars.jacobian()));
  return result;

}


double
PacEmcCluster::mass() const {
  return this->fourMomentum().mag();
}

HepPoint
PacEmcCluster::where() const {
  if ( !_whereValid ) {

    switch ( _centroidAlgo ) {
    case gravity :
      _where= gravWhere(this);
      break;
      
    case logarithmic :
      _where= privateLogWhere(this);
      break;
      
    case neighbour:
    case lilo:
    default:
      cout << "PacEmcCluster::selectCentroidMethod. "
	   << " method " << _centroidAlgo << " is not implemented or defined."
	   << endl;
      abort();
      break;
    }
    _whereValid = true;
  }
  
  return _where;
}

PacEmcModel::EmcRegion PacEmcCluster::region() const {
  
  if ( _pacEmcDigis.empty() ) {
    _region = PacEmcModel::Unknown;
    _regionValid = false;
    return _region;
  }
  
  if ( !_regionValid ) {
    // Define the cluster region as the crystal's region
    const PacEmcDigi *max= this->maxima();
    if(max == 0)max = _pacEmcDigis[0];
    _region = PacEmcModel::getModel()->region(max->theta());
    _regionValid = true;
  }
  
  return _region;
}


double
PacEmcCluster::x() const {
  return where().x();
}

double
PacEmcCluster::y() const {
  return where().y();
}

double
PacEmcCluster::z() const {
  return where().z();
}

//-------------
// Modifiers --
//-------------

void
PacEmcCluster::addDigi(const PacEmcDigi& theDigi, bool merge ) {

  // Check if the location has already a digi
  PacEmcDigi *digi= getDigi(*theDigi.tci());

  if (!merge && 0!=digi ) { // this location has been done
    ErrMsg(warning) << "(PacEmcCluster::addDigi) you are trying to add a digi to a cluster at the location where a digi has been added. " << endmsg;
    return;
  }

  invalidateCache();
  
  if ( 0!= digi ) {
    // Add new digi energy to the existing digi
    *digi += theDigi;
  } else {
    _pacEmcDigis.push_back(new PacEmcDigi(theDigi));
  }

}

void
PacEmcCluster::addDigi(const TwoCoordIndex* tci, const double &energy , bool merge ) {

  // Check if the location has already a digi
  PacEmcDigi *digi= getDigi(*tci);

  if (!merge && 0!=digi ) { // this location has been done
    ErrMsg(warning) << "(PacEmcCluster::addDigi) you are trying to add a digi to a cluster at the location where a digi has been added. " << endmsg;
    return;
  }

  invalidateCache();
  
  if ( 0!= digi ) {
    // Add new digi energy to the existing digi
    digi->addEnergy(energy);
  } else {
    _pacEmcDigis.push_back(new PacEmcDigi(energy, *tci));
  }

}

void
PacEmcCluster::setDigiEnergy(const TwoCoordIndex& tci, double e, double se) {

  PacEmcDigi *digi= getDigi(tci);
  if ( digi ) {
    digi->setEnergy(e);
    if ( se>=0 ) digi->setSimEnergy(se);
    invalidateCache();
  } else {
    ErrMsg(warning) << "Cannot find a digi at this TCI ("
		    << tci.itsXCoord() << " , " 
		    << tci.itsYCoord() << ")" << endmsg;
  }
}

void
PacEmcCluster::addNoise() {

  if ( numberOfDigis() == 0 ) return;

  PacEmcModel *model= PacEmcModel::getModel();
  HepRandomEngine* rng = HepRandom::getTheEngine();

  findExtendedTcis();
  bool added(false);
  std::set<const TwoCoordIndex*>::const_iterator iter= _extendedTcis.begin();
  while ( iter!= _extendedTcis.end() ) {
    const TwoCoordIndex* tci= (*iter);
    
    if ( getDigi(*tci) == 0 ) { // do not add to existing digis
      const double occupancy= this->shapePars()->noiseOcc();
      if ( RandPoisson::shoot(rng, occupancy) >= 1 ) { // fire
	const double econst= model->noiseExpConst(*tci);
	double e= model->digiThreshold(*tci) + RandExponential::shoot(rng,econst);
	addDigi(tci,e,false);
	added= true;
      }
    }
    iter++;
  }

  if (added) searchLocalMax();
    
}

void
PacEmcCluster::removeNoise(NoiseType type, const double ecut) {

  if ( numberOfDigis() == 0 ) return;

  bool rm(false);

  PacEmcModel *model= PacEmcModel::getModel();

  double emin= ecut;
  if ( emin <= 0 ) {  // use value from configuration
    emin= model->digiThreshold( *(this->maxima()->tci()) );
  }

  PacEmcDigiIterator iter= _pacEmcDigis.begin();
  while ( (iter!= _pacEmcDigis.end()) ) {

    bool tocut= false;
    if ( type== LowEnergy || type== EnergyTime ) {
      if ( (*iter)->energy() < emin ) {
	tocut= true;
      }
    }
    if ( !tocut && ( type== BadTiming || type== EnergyTime ) ) {
      if ( (*iter)->waveFormInTime() != PacEmcDigi::InSigWin )  {
	tocut= true;
      }
    }
    
    if ( tocut ) {
      this->addSimTrkEnergyChange( -(*iter)->simenergy() );
      delete *iter;
      iter= _pacEmcDigis.erase(iter);
      rm= true;
    } else {
      iter++;
    }
  }

  if ( rm ) {
    invalidateCache();
    searchLocalMax();
  }

}

void
PacEmcCluster::addCluster(PacEmcCluster* cluster) {
  // FIXME
  //_myRefs->push_back((AbsRecoCalo*)cluster);
}

void
PacEmcCluster::mergeCluster(const PacEmcCluster* other) {

  invalidateCache();

  PacEmcDigiConstIterator iter= other->pacEmcDigis()->begin();
  
  while ( (iter!= other->pacEmcDigis()->end()) ) {
    this->addDigi(*(*iter), true );
    iter++;
  }

  _simtrkEnergyChange+= other->simtrkEnergyChange();

  // Hit time
  if ( other->firstSimhitTime() < this->firstSimhitTime() ) {
    this->setFirstSimhitTime(other->firstSimhitTime());
  }
  if ( other->recordTime() < this->recordTime() ) {
    this->setRecordTime(other->recordTime());
  }
  if ( other->recordTimeErr() > this->recordTimeErr() ) {
    this->setRecordTimeErr(other->recordTimeErr());
  }

  searchLocalMax();
}


// Scale each digi by the same factor
void
PacEmcCluster::scale(double x) {

  invalidateCache(true);
  
  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  
  while ( (iter!= _pacEmcDigis.end()) ) {
    (*iter)->scale(x);
    iter++;
  }
  
}


const unsigned PacEmcCluster::numberOfDigis() const { 
  return _pacEmcDigis.size(); 
}



double
PacEmcCluster::distanceToCentre( const HepPoint& aPoint ) const
{
  return  ( where() - aPoint ).mag();
}

double
PacEmcCluster::distanceToCentre( const PacEmcDigi* aDigi ) const
{
  return  ( where() - aDigi->where() ).mag();
}


void
PacEmcCluster::sortDigis() {
  // FIXME
  // Sort digis in cluster with distance from centre of cluster

}

const unsigned
PacEmcCluster::nBumps() const {
  if ( !_foundLocalMax ) {
    ErrMsg(fatal) << "The bumps haven't been determined, or have been invalidated. You need to do cluster->searchLocalMax() after the cluster is created or modified." << endmsg;
  }
  return _nbumps;
}

void
PacEmcCluster::setNBumps(unsigned n) {
  _nbumps=n;
}

void
PacEmcCluster::invalidateCache(bool keepLocalMax){
  _energyValid = false;
  _whereValid = false;
  _regionValid = false;
  _rangeValid = false;
  _radiusValid = false;

  if ( _foundLocalMax && !keepLocalMax ) {
    _localMaxima.clear();
    _foundLocalMax = false;
  }

  delete _theClusEnergySums;   _theClusEnergySums = 0;
  delete _theClusXClMoments;  _theClusXClMoments = 0;

  _secondMoment = HepSymMatrix(0,0);

}


const PacEmcClusterEnergySums&
PacEmcCluster::esums() const
{
  if (_theClusEnergySums == 0)
    _theClusEnergySums = new PacEmcClusterEnergySums( *this );
  return *_theClusEnergySums;
}


const PacEmcXClMoments&
PacEmcCluster::Xmoments() const
{
  if (_theClusXClMoments == 0)
    _theClusXClMoments = new PacEmcXClMoments( *this );
  return *_theClusXClMoments;
}


//              -----------------------------------------------
//              -- Static Data & Function Member Definitions --
//              -----------------------------------------------

HepPoint
PacEmcCluster::gravWhere( const PacEmcCluster* me ) {

  Hep3Vector aVector(0,0,0);

  if ( me->rawEnergy() > 0 ) {

    PacEmcDigiConstIterator iter= me->pacEmcDigis()->begin();
    
    while ( (iter!= me->pacEmcDigis()->end()) ) {
      HepPoint aPoint( (*iter)->where() );
      aVector += Hep3Vector( aPoint.x(), aPoint.y(), aPoint.z() ) * (*iter)->energy();
      iter++;
    }
    
    aVector *= 1./me->rawEnergy();
  }

  return HepPoint( aVector.x(), aVector.y(), aVector.z() );
  
}

HepPoint
PacEmcCluster::privateLogWhere( const PacEmcCluster* me ) {

  Hep3Vector aVector(0,0,0);

  //The Offset constant below has been tuned using single photon MC in BABAR
  //and the best value was found to be fairly insensitive to
  //the photon energy (S.Willocq).
  //One purpose of the Offset is to ignore low energy hits, which are far from
  //the true photon location.
  const double Offset(4.0);

  if ( me->rawEnergy() > 0 ) {
    double weightsum(0.),weight,enorm(1./me->rawEnergy());

    PacEmcDigiConstIterator iter= me->pacEmcDigis()->begin();

    int ndigis = 0;

    while ( (iter!= me->pacEmcDigis()->end()) ) {
      HepPoint aPoint( (*iter)->where() );
      if ( (*iter)->energy() > 0 ) {
	weight=Offset+log(((*iter)->energy())*enorm);
      } else {
	weight=0;
      }

      if(weight > 0.){
	ndigis++;
	aVector += Hep3Vector(aPoint.x(),aPoint.y(),aPoint.z()) * weight;
	weightsum+=weight;
      }
      //else the block is ignored.
      iter++;
    }

    // If the cluster does not contain a sufficient amount of digis
    // with weight > 0, then revert to the linear weighting method
    if (ndigis < 1) {
      return me->gravWhere(me);
    } else {
      aVector *= 1./weightsum;
    }
  }

  return HepPoint( aVector.x(), aVector.y(), aVector.z() );

}

void
PacEmcCluster::searchLocalMax() {

  if ( _foundLocalMax ) return;
  if ( this->numberOfDigis() == 0 ) {
    _foundLocalMax= true;
    return;
  }

  _localMaxima.clear();

  const PacEmcDigi *absmax(0);

  std::map<const PacEmcDigi*, bool> notLocalMax;

  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  for ( ; iter!= _pacEmcDigis.end(); iter++ ) {
    const PacEmcDigi *digi= (*iter);
    if ( !absmax ) absmax= digi;
    else if ( digi->energy() > absmax->energy() ) absmax= digi;

    if ( notLocalMax[digi] ) { continue; }

    bool isLocalMax= true;
    int nneighbors=0;
    double emaxneighbor=0;

    // search neighbors
    std::set<AbsDetIndex*>::const_iterator niter= digi->tci()->itsNeighbours()->begin();
    while ( (niter!= digi->tci()->itsNeighbours()->end()) ) {
      const TwoCoordIndex* tci2= dynamic_cast<const TwoCoordIndex*>(*niter);
      const PacEmcDigi* nbdigi= getDigi(*tci2);
      if ( nbdigi ) {
	if ( digi->energy() > nbdigi->energy() ) {
	  notLocalMax[nbdigi] = true;
	  
	  if ( nbdigi->energy() > 0.002 ) {
	    nneighbors++;
	    if ( nbdigi->energy() > emaxneighbor ) {
	      emaxneighbor= nbdigi->energy();
	    }
	  }

	} else {
	  notLocalMax[digi] = true;
	  isLocalMax= false;
	  break;
	}
      }
      niter++;
    }

    if ( !isLocalMax ) continue;
    // ** See BaBar detector NIMA 479, 1, sec. 9.6 for the second condition **
    // Note, if it has only one neighbor, it won't be a local max unless
    // it is the absolute max.
    if ( nneighbors<= 0 ||  // isolated, must be a maximum
	 0.5*(nneighbors-2.5) > emaxneighbor/digi->energy() ) { 
      _localMaxima.push_back((PacEmcDigi*)digi);
    } 
  }
  
  if ( std::find(_localMaxima.begin(), _localMaxima.end(), absmax)==_localMaxima.end()) {
    // The absolute maxima did not pass local max check. Add it!
    _localMaxima.push_back((PacEmcDigi*)absmax);
  }

  // Sort local maxima from high energy to low
  std::sort( _localMaxima.begin(), _localMaxima.end(), high2low );
  
  // sanity check
  if ( absmax->energy() < _localMaxima[0]->energy() ) {
    ErrMsg(fatal) << "local maxima sorting failed?" << endmsg;
  }

  _nbumps= _localMaxima.size();
  _foundLocalMax= true;

}

// Find the theta and phi index ranges.
// Because the periodic nature of the phi index, this 
// method is not correct for clusters that corss
// the phi=0 boundary.
void
PacEmcCluster::digiTciRange() {
  if ( _rangeValid ) return;

  _tciThetaLow = 10000;
  _tciThetaHi = -1;
  _tciPhiLow = 10000;
  _tciPhiHi = -1;  

  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  while ( iter!= _pacEmcDigis.end() ) {
    const PacEmcDigi *digi= (*iter);
    if ( digi->theta() < _tciThetaLow ) {
      _tciThetaLow = digi->theta();
    } else if ( digi->theta() > _tciThetaHi ) {
      _tciThetaHi = digi->theta();
    }
    if ( digi->phi() < _tciPhiLow ) {
      _tciPhiLow = digi->phi();
    } else if ( digi->phi() > _tciPhiHi ) {
      _tciPhiHi = digi->phi();
    }

    iter++;
  }
  _rangeValid= true;

}

long PacEmcCluster::tciThetaLow() {
  if ( !_rangeValid ) digiTciRange();
  return _tciThetaLow;
}
long PacEmcCluster::tciThetaHi() {
  if ( !_rangeValid ) digiTciRange();
  return _tciThetaHi;
}
long PacEmcCluster::tciPhiLow() {
  if ( !_rangeValid ) digiTciRange();
  return _tciPhiLow;
}
long PacEmcCluster::tciPhiHi() {
  if ( !_rangeValid ) digiTciRange();
  return _tciPhiHi;
}

void PacEmcCluster::fluctuateDigis() {

  // Fluctuate the digis but in the end keep the cluster energy the same

  if ( numberOfDigis() == 0 ) return;

  const double e0= this->energy();

  if ( e0 <= 0 ) {
    ErrMsg(routine) << "fluctuateDigis: this cluster's energy is " << e0
		    << ". Do nothing." << endmsg;
    this->print(ErrMsg(routine));
    ErrMsg(routine) << endmsg;
    return;
  }

  // Assume all digis are in the same region
  PacEmcModel *emcmodel= PacEmcModel::getModel();
  const PacEmcDigi *maxdigi= this->maxima();
  if ( ! maxdigi ) {
    ErrMsg(warning) << "fluctuateDigis: can't find the maximum digi. Do nothing." << endmsg;
    this->print(cout);
    return;
  }
  PacEmcModel::EmcRegion theregion= emcmodel->region(maxdigi->theta());
  if ( theregion == PacEmcModel::Unknown || theregion== PacEmcModel::NRegions ) {
    ErrMsg(warning) << "fluctuateDigis: the maximum is in an unknown region "
		    << theregion << ". Do nothing  " << endmsg;
    this->print(cout);
    return;
  }

  double cutoff = emcmodel->ecutoff(*maxdigi->tci());
  double fa= emcmodel->energyResCoef(theregion);
  double fb= emcmodel->energyResConst(theregion);
  double ep= emcmodel->energyResEPower(theregion);
  double cexp= emcmodel->energyResExpCoef(theregion);
  double dexp= emcmodel->energyResExpConst(theregion);
  double pexp= emcmodel->energyResExpEPower(theregion);
  
  PacEmcDigiIterator iter= _pacEmcDigis.begin();
  while ( iter!= _pacEmcDigis.end()) {
    fluctuateDigi(*iter, fa,fb,ep,cexp,dexp,pexp);
    iter++;
  }

  if ( this->energy() > 0 ) {
    // Rescale the cluster energy to the original energy.
    this->scale( e0/this->energy() );
  }

  this->removeNoise(LowEnergy, cutoff);
}

void PacEmcCluster::fluctuateDigi(PacEmcDigi* digi, const double& fa, const double& fb, const double& ep, const double cexp, double dexp, double pexp) {

  if ( ! digi ) {
    ErrMsg(warning) << "fluctuateDigi: Found a null digi! Do nothing." << endmsg;
    return;
  }

  double energy(digi->energy());
  if ( energy <= 0 ) {
    return;
  }

  if ( fa>0 || fb>0 || cexp>0) {
    invalidateCache();

    double de= smearE(energy,fa,fb,ep,cexp,dexp,pexp);

    if ( 1+de <= 0 ) {
      digi->setEnergy(0);
    } else {
      digi->scale(1+de);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Fluctuate the cluster's energy based on the resolution model, without
// changing the energy distribution among digis. Note here we assume the
// cluster's energy before smearing is very close to the true total energy
// deposition.
void PacEmcCluster::fluctuateCluster() {

  if ( numberOfDigis() == 0 ) return;

  PacEmcModel *emcmodel= PacEmcModel::getModel();
  const PacEmcDigi *maxdigi= this->maxima();
  if ( ! maxdigi ) {
    ErrMsg(routine) << "fluctuateCluster: can't find the maximum digi. Do nothing." << endmsg;
    this->print(ErrMsg(routine));
    ErrMsg(routine) << endmsg;
    return;
  }

  PacEmcModel::EmcRegion theregion= emcmodel->region(maxdigi->theta());
  if ( theregion == PacEmcModel::Unknown || theregion== PacEmcModel::NRegions ) {
    ErrMsg(warning) << "fluctuateCluster: the maximum is in an unknown region "
                    << theregion << ". Do nothing  " << endmsg;
    this->print(cout);
    return;
  }

  double fa= emcmodel->energyResCoef(theregion);
  double fb= emcmodel->energyResConst(theregion);
  double ep= emcmodel->energyResEPower(theregion);
  double cexp= emcmodel->energyResExpCoef(theregion);
  double dexp= emcmodel->energyResExpConst(theregion);
  double pexp= emcmodel->energyResExpEPower(theregion);

  double de= smearE(this->energy(),fa,fb,ep,cexp,dexp,pexp);

  if ( 1+de <= 0 ) {
    this->scale(0);
  } else {
    this->scale(1+de);
  }

  // this->removeNoise();
}

double PacEmcCluster::smearE(const double& energy, const double& fa, const double& fb, const double& ep, double cexp, double dexp, double pexp) {

  // get random engine
  HepRandomEngine* rng = HepRandom::getTheEngine();
  // energy resolution:   sigma(E)/E = fa/E^(ep) o+ fb
  //    convoluted with an exponential tail, tau= cexp/E^(pexp) o+ dexp
  double fluc= fa/pow(energy, ep);
  fluc= sqrt(fluc*fluc + fb*fb);
  double etau= cexp/pow(energy, pexp);
  etau= sqrt(etau*etau + dexp*dexp);
  double de= RandGauss::shoot(rng,0,fluc) - RandExponential::shoot(rng,etau);
  return de;
}

void
PacEmcCluster::simulateHitTime() {
  PacEmcModel *model= PacEmcModel::getModel();
  _recordTimeErr= model->timeResolution(this->region());

  // get random engine
  HepRandomEngine* rng = HepRandom::getTheEngine();
  _recordTime= this->firstSimhitTime() + RandGauss::shoot(rng,0,_recordTimeErr);

}

bool
PacEmcCluster::overlap(PacEmcCluster &other) const {

  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  PacEmcDigiConstIterator iend= _pacEmcDigis.end();
  for (; iter!=iend; ++iter) {
    if ( 0!= other.getDigi(*(*iter)->tci())) {
      return true;
    }
  }
  return false;
}

void
PacEmcCluster::print( std::ostream& o, bool verbose) {

  this->searchLocalMax();

  o << "PacEmcCluster: ";
  o << "nDigis= " << numberOfDigis();
  o << " nbumps= " << nBumps() ;
  o << ", E= " << energy();
  o << " @" << where(); 
  o << " simtrkE dep= " << this->simtrkEnergyChange();
  o << "\n";

  if ( verbose ) {
    o << "  (theta, phi)= " << theta() << " , " << phi() << "\n";
    o << "  digis \n";
    for ( unsigned i=0; i< _pacEmcDigis.size(); i++) {
      _pacEmcDigis[i]->print(o);
    }
    o << "  absolute maximum is \n";
    if ( this->maxima() ) this->maxima()->print(o);
    else o << "not found\n";
    //o << "       nBumps = " << nBumps() << "\n";
    o << "     local maxima are " << "\n";
    for ( unsigned i=0; i< _localMaxima.size(); i++ ) {
      _localMaxima[i]->print(o);
    }
    o << "Second moment matrix\n ";
    o << this->secondMomentMatrix() << "\n";
    o << "Theta range = " << tciThetaLow() << " -- " << tciThetaHi() << endl;
    o << "Phi range   = " << tciPhiLow() << " -- " << tciPhiHi() << endl;
  }
  o << endl;

}

void
PacEmcCluster::findExtendedTcis() {
  if ( _extendedTciValid ) return;

  _extendedTcis.clear();

  // store all the digis' tcis and their neighbors'
  PacEmcDigiConstIterator iter= _pacEmcDigis.begin();
  while ( iter!= _pacEmcDigis.end() ) {
    std::set<AbsDetIndex*>::const_iterator niter= (*iter)->tci()->itsNeighbours()->begin();
    while ( (niter!= (*iter)->tci()->itsNeighbours()->end()) ) {
      const TwoCoordIndex* tci= dynamic_cast<TwoCoordIndex*>(*niter);
      if ( tci ) _extendedTcis.insert(tci);
      niter++;
    }
    iter++;
  }
  _extendedTciValid=true;

}

bool
PacEmcCluster::isConnectedTo(const PacEmcCluster* other) const {

  const double emin= 0.001;

  std::pair< const TwoCoordIndex*, const TwoCoordIndex* > bestpair;
  double closest=1e6;
  bool connected= false;
  for ( int i=0; i< this->pacEmcDigis()->size(); i++) {
    const PacEmcDigi* d1= (*this->pacEmcDigis())[i];
    if ( d1->energy() < emin ) continue;
    for ( int j=0; j< other->pacEmcDigis()->size(); j++) {
      const PacEmcDigi* d2= (*other->pacEmcDigis())[j];
      if ( d2->energy() < emin ) continue;

      if ( d1->tciIndex() == d2->tciIndex() ) {
	connected= true;  // overlap
	break;
      } else {
	double dist= d1->where().distanceTo( d2->where() );
	if ( dist < closest ) {
	  closest= dist;
	  bestpair= std::make_pair( d1->tci(), d2->tci() );
	}
      }
    }
    if ( connected ) break;
  }

  if ( closest<10 && !connected ) {
    // not overlap, check if the closest pair is neighbors 
//    if ( bestpair.first->itsNeighbours()->find( const_cast<AbsDetIndex*>(bestpair.second) ) !=
//	 bestpair.first->itsNeighbours()->end() ) {
    if ( bestpair.first->isNeighbour( bestpair.second ) ) {
      connected = true;
    }
  }

  return connected;
}


bool
high2low(const PacEmcDigi* d1, const PacEmcDigi* d2) {
  return d1->energy() > d2->energy();
}

//--------------------------------------------------------------------------

#include "BaBar/BaBar.hh"

#include <assert.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include "AbsEnv/TwoCoordIndex.hh"
#include "G3Data/GTrack.hh"
#include "PacEmc/PacEmcDigi.hh"
#include "PacEmc/PacEmcModel.hh"
#include "ErrLogger/ErrLog.hh"
#include <utility>
using std::pair;
using std::make_pair;

using std::cout;
using std::endl;
using std::ios;
using std::ostream;
using std::setw;

//----------------
// Constructors --
//----------------
PacEmcDigi::PacEmcDigi() :
  _energy(0), _simenergy(0), _tciIndex(0), _thetaIdx(0), _phiIdx(0),
  _thetaReal(0), _phiReal(0), _deltaTheta(0), _deltaPhi(0), _where(0,0,0),
  _whereLocal(0,0,0), _waveFormPeakE(0), _wtime(InSigWin)
{
  init();
}

PacEmcDigi::PacEmcDigi(double e, const TwoCoordIndex& tci) :
  _energy(e), _simenergy(e), _tciIndex(tci.itsIndex()), 
  _thetaIdx(0), _phiIdx(0),
  _thetaReal(0), _phiReal(0), _deltaTheta(0), _deltaPhi(0), _where(0,0,0),
  _whereLocal(0,0,0), _waveFormPeakE(e), _wtime(InSigWin)
{
  init();
}

PacEmcDigi::PacEmcDigi(const PacEmcDigi& other) :
  _energy(other.energy()), _simenergy(other.simenergy()),
  _tciIndex(other.tciIndex()), 
  _thetaIdx(other.theta()), _phiIdx(other.phi()),
  _thetaReal(other.thetaReal()), _phiReal(other.phiReal()),
  _deltaTheta(other.deltaTheta()), _deltaPhi(other.deltaPhi()),
  _where(other.where()),_whereLocal(other.whereLocal()),
  _gtrkWeights(*other.gtrkWeights()),
  _waveFormPeakE(other.waveFormPeakE()), _wtime(other.waveFormInTime())
{
  //  init();
}

PacEmcDigi::~PacEmcDigi() {
  _gtrkWeights.clear();
}


// Calculate where
void
PacEmcDigi::init() {
  
  const PacEmcModel *model= PacEmcModel::getModel();

  const TwoCoordIndex *tci= model->getTci(_tciIndex);
  if ( !tci ) {
    ErrMsg(fatal) << "Cannot create a PacEmcDigi with an out-of-range index"
		  << _tciIndex << endmsg;
  }

  _thetaIdx= tci->itsXCoord();
  _phiIdx= tci->itsYCoord();
      
  _where= model->where(*tci);
  _whereLocal= model->whereLocal(*tci);

  _thetaReal = _where.theta();
  _phiReal = _where.phi();
  _deltaTheta = model->deltaTheta(*tci);
  _deltaPhi = model->deltaPhi(*tci);

  _gtrkWeights.clear();
}

PacEmcDigi*
PacEmcDigi::clone() const {
  return new PacEmcDigi(*this);
}

//-------------
// Operators --
//-------------

bool
PacEmcDigi::operator==( const PacEmcDigi& otherDigi ) const {
//  bool equal = false;
  
  if ( this->theta() == otherDigi.theta() &&
       this->phi() == otherDigi.phi() &&
       this->energy() == otherDigi.energy() ) { 
//    equal=true;
    return true;
  }

//  return equal;
  return false;
}

bool
PacEmcDigi::operator!=( const PacEmcDigi& otherDigi ) const {
  return ! ( *this == otherDigi );
}

void 
PacEmcDigi::operator+=( const PacEmcDigi& otherDigi ) {
  // only limited to the same crystal
  if ( _tciIndex != otherDigi.tciIndex() ) {
    cout << "Error: PacEmcDigi::operator+=() : Cannot add two "
	 << "PacEmcDigi with different TwoCoordIndex.\n"
	 << *(this->tci()) << *otherDigi.tci() << endl;
    return;
  }

  double newenergy= _energy+otherDigi.energy();
  // scale down the original GTrack weights by old_energy/new_energy
  const double xf= _energy/newenergy;
  GTDouble::iterator iter= _gtrkWeights.begin();
  while ( iter!= _gtrkWeights.end() ) {
    iter->second*= xf;
    iter++;
  }

  // add GTrack weights of the otherDigi
  GTDouble::const_iterator oiter= otherDigi.gtrkWeights()->begin();
  while ( oiter!= otherDigi.gtrkWeights()->end() ) {
    const GTrack* gtrk= oiter->first;
    const double weight= (oiter->second) * (1-xf);
    this->addGTrackWeight(gtrk,weight);
    oiter++;
  }
  
  if ( _wtime== otherDigi.waveFormInTime() ) {
    _waveFormPeakE+= otherDigi.waveFormPeakE();
  } else {
    if ( _wtime== OutOfSigWin ) { // && _otherDigi.waveFormInTime()== InSigWin
      // this digi peak is out of signal; other is in.
      if ( newenergy > _waveFormPeakE ) {
	_waveFormPeakE= newenergy;
	_wtime= InSigWin;  // set merged digi's peak time to be "in".
      }
    } else {
      // this digi peak is in time; other is out.
      if ( newenergy < otherDigi.waveFormPeakE() ) {
	_waveFormPeakE= otherDigi.waveFormPeakE();
	_wtime= OutOfSigWin;  // set merged digi's peak time to be "out".
      }
    }
  }

  _energy= newenergy;
  _simenergy+= otherDigi.simenergy();
}

const TwoCoordIndex*
PacEmcDigi::tci() const {
  const PacEmcModel *model= PacEmcModel::getModel();
  return model->getTci(_tciIndex);
}


const HepPoint&
PacEmcDigi::where() const {
  return _where;
}

const HepPoint&
PacEmcDigi::whereLocal() const {
  return _whereLocal;
}

// Add the weight to the GTrack map
void
PacEmcDigi::addGTrackWeight(const GTrack *gtrk, const double weight) {
  GTDouble::iterator iter= _gtrkWeights.find(gtrk);
  if ( iter != _gtrkWeights.end() ) {
    (iter->second) += weight;
  } else {
    _gtrkWeights.insert( make_pair(gtrk, weight) );
  }
}

void
PacEmcDigi::print( ostream& o ) const
{
  
  o << "PacEmcDigi:";
  o << " (" << theta() << ", " << phi() << ")" ;
  o << "  = (" << _where.theta() << "," << _where.phi() << ") @ " ;
  o << _where << "  ";
  ios::fmtflags f = o.flags();
  o.precision( 6 );
  o << ", Energy = " << energy();
  o << ", wf peak= " << _waveFormPeakE;
  o << ", timewindow= " << _wtime;
  o << endl;
  GTDouble::const_iterator iter= _gtrkWeights.begin();
  while ( iter!= _gtrkWeights.end() ) {
    o << *iter->first << " : w= " << iter->second << endl;
    iter++;
  }
  o.setf( ios::scientific );
  o.flags( f );

}


//==================================================================

double PacEmcDigi::_rescaleFactor = 1.0;
double PacEmcDigi::_positionDepth = 12.5; // in cm



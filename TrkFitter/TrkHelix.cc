//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkHelix.cc 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------

#include "BaBar/BaBar.hh"
#include "TrkFitter/TrkHelix.hh"
#include "TrkBase/TrkExchangePar.hh"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"
using std::endl;
using std::ostream;

//------------------------------------------------------------------------
TrkHelix::TrkHelix() {
//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
TrkHelix::TrkHelix(const d_Float vec[5], const d_Float cov[15]) {
//------------------------------------------------------------------------
  for (int i=0; i<5; i++) _paramVec[i]= vec[i];
  for (int i=0; i<15; i++) _paramCov[i]= _paramCov[i];
}

//------------------------------------------------------------------------
TrkHelix::~TrkHelix() {
//------------------------------------------------------------------------
}

//------------------------------------------------------------------------
TrkHelix::TrkHelix(const TrkHelix& rhs) {
//------------------------------------------------------------------------
  int i;
  for (i = 0; i < 5; i++) {
    _paramVec[i] = rhs._paramVec[i];
  }
  for (i = 0; i < 15; i++) {
    _paramCov[i] = rhs._paramCov[i];
  }
}

//------------------------------------------------------------------------
TrkHelix&   
TrkHelix::operator= (const TrkHelix& rhs) {
//------------------------------------------------------------------------
  if (this == &rhs) return *this;
  int i;
  for (i = 0; i < 5; i++) {
    _paramVec[i] = rhs._paramVec[i];
  }
  for (i = 0; i < 15; i++) {
    _paramCov[i] = rhs._paramCov[i];
  }
  return *this;
}

//------------------------------------------------------------------------
void
TrkHelix::fill(const TrkExchangePar& parIn) {
//------------------------------------------------------------------------
  const HepSymMatrix& covtmp = parIn.covariance();
  const HepVector& partmp = parIn.params();
  int index = 0;   // index into flat storage of cov matrix
  int npar = 5;
  for (int ipar = 0; ipar < npar; ipar++) {
    _paramVec[ipar] = partmp[ipar];
    for (int jcol = 0; jcol <= ipar; jcol++) {
      _paramCov[index] = covtmp(ipar+1,jcol+1);
      index++;
    }
  }
}

//------------------------------------------------------------------------
void
TrkHelix::fillParamVec(d_Float vec[5]) {
//------------------------------------------------------------------------
  for (int i=0; i<5; i++) vec[i]= _paramVec[i];
}

//------------------------------------------------------------------------
void
TrkHelix::fillParamCov(d_Float cov[15]) {
//------------------------------------------------------------------------
  for (int i=0; i<15; i++) cov[i]= _paramCov[i];
}

//------------------------------------------------------------------------
TrkExchangePar 
TrkHelix::output() const {
//------------------------------------------------------------------------
  HepSymMatrix covtmp(5);
  HepVector partmp(5);  
  int index = 0;   // index into flat storage of cov matrix
  int npar = 5;
  for (int ipar = 0; ipar < npar; ipar++) {
    partmp[ipar] = _paramVec[ipar];
    for (int jcol = 0; jcol <= ipar; jcol++) {
      covtmp(ipar+1,jcol+1) = _paramCov[index];
      index++;
    }
  }
  return TrkExchangePar(partmp, covtmp);
}

//------------------------------------------------------------------------
void 
TrkHelix::printAll(ostream& os) const {
//------------------------------------------------------------------------
  int iD0 = TrkExchangePar::ex_d0;
  int iPhi0 = TrkExchangePar::ex_phi0;
  int iOmega = TrkExchangePar::ex_omega;
  int iZ0 = TrkExchangePar::ex_z0;
  int iCt = TrkExchangePar::ex_tanDip;

  os << "Parameters: " << endl;
  os << "  d0: " << _paramVec[iD0] << "  phi0: " << _paramVec[iPhi0] 
     << "  omega: " << _paramVec[iOmega] << "  z0: " << _paramVec[iZ0] 
     << "  ct: " << _paramVec[iCt] << endl;
  os << "Covariance matrix: " << endl;
  os << _paramCov[0] << endl;
  os << _paramCov[1] << "  " << _paramCov[2] << endl;
  os << _paramCov[3] << "  " << _paramCov[4] << "  " << _paramCov[5] << endl;
  os << _paramCov[6] << "  " << _paramCov[7] << "  " << _paramCov[8] 
     << "  " << _paramCov[9] << endl;
  os << _paramCov[10] << "  " << _paramCov[11] << "  " << _paramCov[12] 
     << "  " << _paramCov[13] << _paramCov[14] << endl;

  return;
}


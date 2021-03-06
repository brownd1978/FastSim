//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: DifIndepPar.cc 501 2010-01-14 12:46:50Z stroili $
//
// Description:
//	Class Implementation for |DifIndepPar| 
//      What do i do ?
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A. Snyder
//
// Copyright Information:
//	Copyright (C) 1996	SLAC
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "difAlgebra/DifIndepPar.hh"
#include "difAlgebra/DifNumber.hh"
#include "difAlgebra/DifArray.hh"
using std::endl;
using std::ostream;

DifIndepPar::DifIndepPar(int ndim):
  parvec(ndim,0), 
  parcov(ndim,1)
{}

DifIndepPar::DifIndepPar(const CLHEP::HepVector& a):
  parvec(a),
  parcov(nPar(),1)
{}

DifIndepPar::DifIndepPar(const DifIndepPar& rhs):
  parvec(rhs.parameter()),
  parcov(rhs.covariance())
{}

DifArray DifIndepPar::difParameters()const{
  DifArray pars(nPar(),nPar());
  for(int i=1; i<=nPar(); i++){
    pars(i)=difPar(i);
  }
  return pars;
}

DifIndepPar::DifIndepPar(const CLHEP::HepVector& pvec, const CLHEP::HepSymMatrix& pcov) :
  parvec(pvec), parcov(pcov) {
}
//void DifIndepPar::setDerives(){
//  for(int i=0; i<nPar(); i++){
//    _parameters[i].zeroDerivatives();
//    _parameters[i].setDerivative(i+1,1.0);
//    _parameters[i].setIndepPar(this);  
//  }
//}

DifNumber DifIndepPar::difPar(int i)const{
  return DifNumber(parvec(i),i,this);
}

//--------------------------------------
void 
DifIndepPar::print(ostream& os) const {
//--------------------------------------
  os << "DifIndepPar Parameter Vector = " << parvec;
}

//--------------------------------------
void 
DifIndepPar::printAll(ostream& os) const {
//--------------------------------------
  os << "DifIndepPar Parameter Vector = " << parvec << endl;
  os << "DifIndepPar Covariance Matrix = " << parcov << endl;
}

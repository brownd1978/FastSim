//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedCovariance.cc 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackedCovariance 
//      Class to format 5D covariance matrices in a storable format:
//      the matrix is first nomalized in a range set by ComFitRangePack
//      then its eigenvalues are found together with the rotation angles 
//      that allow to reconstruct the rotation and get the original matrix back
//      Algorithm copied from the Aleph mini-DST packing
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      R.Faccini                   Originator. 
//
// Copyright Information:
//      Copyright (C) 1999          U.C.S.D.
//
//
//--------------------------------------------------------------------------
//#include "BaBar/BaBar.hh"
#include "BaBar/BaBar.hh"

//
// C includes
//
#include <stddef.h> 
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
//
// C++ includes
//
#include <iostream>

//
// Bass class includes
//
//? #include "CommonUtils/ComPackUFloat.hh"
#include "ErrLogger/ErrLog.hh"
#include "BaBar/Constants.hh"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"
using CLHEP::HepSymMatrix;
using CLHEP::HepMatrix;
using CLHEP::HepVector;


//
// This class
//
#include "CommonUtils/ComPackedCovariance.hh"

ComPackedCovariance::~ComPackedCovariance(){
}

ComPackedCovariance::ComPackedCovariance(const HepSymMatrix& c):HepSymMatrix(c){}

ComPackedCovariance::ComPackedCovariance(const HepVector& eigenValues,const HepVector& angles):HepSymMatrix(eigenValues.num_row())
{
  const int nDim=num_row() ;
  assert (angles.num_row() == nDim*(nDim-1)/2);
  // restore the diagonal part
  HepSymMatrix diag(nDim,0);
  for(int l=1;l<=nDim;l++)diag(l,l)=eigenValues[l-1];
  // build up the rotation
  HepMatrix rot(nDim,nDim,1);
  for(int i=nDim-1;i>0;i--){
    for(int j=i+1;j<=nDim;j++){
      double phi=angles[ j*(j-1)/2-i]*Constants::pi;
      double co = cos(phi);
      double si = sin(phi);
      for (int k=1;k<=nDim;k++){
	double a1 = co*rot(k,j-1)-si*rot(k,j);
	double a2 = si*rot(k,j-1)+co*rot(k,j);
	rot(k,j-1) = a1;
	rot(k,j) = a2;
      }
    }
  }
  HepSymMatrix::operator=(diag.similarity(rot)) ;
}


  
void
ComPackedCovariance::getRotation(HepSymMatrix& diag,HepVector& angles ) const
{
  const int nDim=num_row() ;
  diag = HepSymMatrix(*this);
  angles = HepVector(nDim*(nDim-1)/2);
  HepMatrix rot(diagonalize(&diag));
  if(rot.determinant()<0)
    for(int l=1;l<=nDim;l++)
      rot(l,1) *=-1;
  for(int i=1;i<nDim;i++){
    for(int j=nDim;j>i;j--){
      int index = j*(j-1)/2-i;
      double si(0);
      double co(0);
      if(fabs(rot(i,j)) <1e-30){
	if(rot(i,j-1)>=0){
	  angles[index] = 0;
	  co = 1;
	}else{
	  angles[index] = 1;
	  co =-1;
	}
      }else if(fabs(rot(i,j-1)) <1e-30){
	si = (rot(i,j)>0) ? 1 :-1;
	angles[index] = si*0.5;
      }else{
	 angles[index]=atan2(rot(i,j),rot(i,j-1));
	 co = cos(angles[index]);
	 si = sin(angles[index]);
	 angles[index]/=Constants::pi;
      }	 
      for(int k=1;k<=nDim;k++){
	double a1 = co*rot(k,j-1)+si*rot(k,j);
	double a2(0);
	if(k!=i)a2 = -si*rot(k,j-1)+co*rot(k,j);
	rot(k,j-1)= a1;
	rot(k,j) = a2;
      }
    }
  }
}

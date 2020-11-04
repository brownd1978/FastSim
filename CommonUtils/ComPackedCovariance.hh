//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedCovariance.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      - See class for  information
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      R.Faccini               Originator. 
//
// Copyright Information:
//      Copyright (C) 1999      U.C.S.D.
//
//
//--------------------------------------------------------------------------

#ifndef ComPackedCovariance_HH
#define ComPackedCovariance_HH

#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Matrix/Matrix.h"
#include "CLHEP/Matrix/Vector.h"

//
// Bass class includes
//
class HepString;

class ComFitRangePack{
private:
  double ranges[5][5];
public:
  ComFitRangePack(int chge){
    if(chge == 0){
      for(int j=0;j<4;j++)ranges[j][j]=1e3;
    }else{
      ranges[0][0]=10;
      ranges[1][1]=1e-3;
      ranges[2][2]=1e-4;
      ranges[3][3]=1e3;
      ranges[4][4]=0.1;
    }
  }
  double range(int i,int j){return ranges[i][j];}
};

class ComPackedCovariance: public HepSymMatrix {

public:

  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackedCovariance(const HepVector& eigenVectors,const HepVector& angles);
  ComPackedCovariance(const HepSymMatrix& orig);

  ///////////////////
  // Destructor:   //
  ///////////////////
  ~ComPackedCovariance();

  ///////////////////
  // Packers:      //
  ///////////////////

  void getRotation(HepSymMatrix& diag,HepVector& angle ) const;


private:
};

#endif





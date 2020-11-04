//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedDEDX.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      ComPackNonFlatFloat interface
//      - See class for more information
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
// Quote:
//      "It ain't the way that you do it, it's what you do"
//
//--------------------------------------------------------------------------

#ifndef ComPackedDEDX_HH
#define ComPackedDEDX_HH

#include "CommonUtils/ComPackBase.hh"

//
// Bass class includes
//
class HepString;

class ComPackedDEDX{

public:

  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackedDEDX(double value,int nSamples,double maxVal);
  ComPackedDEDX(const d_UShort& packedValue,double maxVal);

  ///////////////////
  // Destructor:   //
  ///////////////////
  ~ComPackedDEDX();

  ///////////////////
  // Packers:      //
  ///////////////////
  d_UShort packedValue () const;
  double value () const{ return _value;}
  int nSamples () const{return _nSamples;}


private:
  ComPackBase<double>*_packer;
  double _value;
  int _nSamples;
  
  // Limits
  enum {
    NHITLEN=6,
    FRACLEN=6,
    EXPLEN=4,
    BIAS=-5
  };

};

#endif





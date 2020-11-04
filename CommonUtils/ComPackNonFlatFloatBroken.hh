//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackNonFlatFloatBroken.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      ComPackNonFlatFloatBroken interface
//      - See class for more information
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//      Used only in old "micro" written before end of Y2K 
//
// Author List:
//      R.Faccini               Originator of ComPackNonFlatFloat. 
//      D.Aston   cloned from ComPackNonFlatFloat in CommonUtils V00-03-02. 
//
// Copyright Information:
//      Copyright (C) 1999      U.C.S.D.
//
// Quote:
//      "It ain't the way that you do it, it's what you do"
//
//--------------------------------------------------------------------------

#ifndef ComPackNonFlatFloatBroken_HH
#define ComPackNonFlatFloatBroken_HH

#include "CommonUtils/ComPackBase.hh"

//
// Bass class includes
//
class HepString;

class ComPackNonFlatFloatBroken {

public:
  // unsigned?
  enum signType{SIG,UNSIG,SIG_8,UNSIG_8};

  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackNonFlatFloatBroken(signType sigT=SIG);

  ///////////////////
  // Destructor:   //
  ///////////////////
  ~ComPackNonFlatFloatBroken();

  ///////////////////
  // Packers:      //
  ///////////////////
  ComPackBase<double>::StatusCode pack (double xmin, double xmax, double, d_UShort &) const;
  ComPackBase<double>::StatusCode pack (double xmin, double xmax, double, d_Octet &) const;

  bool packAndScream(double xmin, double xmax ,  double x, d_UShort&y, const char* name) ;
  bool packAndScream(double xmin, double xmax ,  double x, d_Octet&y, const char* name) ;

  ///////////////////
  // Unpackers:    //
  ///////////////////
  ComPackBase<double>::StatusCode unpack (double xmin,double xmax , d_UShort , double&) const;

  bool unpackAndScream(double xmin, double xmax, const  d_UShort&x, double &y, const char* name) const ;
  double getMinVal(){return _engine->getMinVal();}
  double getMaxVal(){return _engine->getMaxVal();}

  ///////////////////
  // Accessors:    //
  ///////////////////


private:
  // Limits
  enum {
    FRACLEN_8=5,
    EXPLEN_8=3,
    BIAS_8=-3,

    FRACLEN_16=10,
    EXPLEN_16=6,
    BIAS_16=-30
  };
  ComPackBase<double>*_engine;
};

#endif




//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedConsistencyBroken.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      ComPackedConsistencyBroken interface
//      - See class for more information
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//      Used only in old "micro" written before end of Y2K 
//
// Author List:
//      R.Faccini               Originator of ComPackedConsistency. 
//      D.Aston   cloned from ComPackedConsistency in CommonUtils V00-03-02. 
//
// Copyright Information:
//      Copyright (C) 1999      U.C.S.D.
//
// Quote:
//      "It ain't the way that you do it, it's what you do"
//
//--------------------------------------------------------------------------

#ifndef ComPackedConsistencyBroken_HH
#define ComPackedConsistencyBroken_HH

#include "ProbTools/Consistency.hh"
#include "CommonUtils/ComPackBase.hh"

//
// Bass class includes
//
class HepString;

class ComPackedConsistencyBroken: public Consistency {

public:

  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackedConsistencyBroken(const Consistency&, double scaleLike=1.);
  ComPackedConsistencyBroken(const d_UShort& value,const d_UShort& status=0);

  ///////////////////
  // Destructor:   //
  ///////////////////
  ~ComPackedConsistencyBroken();

  ///////////////////
  // Packers:      //
  ///////////////////
  d_UShort packedValue () const;
  d_UShort packedStatus() const; // note : only the first 4 bits used


private:
  // Limits
  enum {
    FRACLEN=4,
    EXPLEN=4,
    BIAS=-5
  };

};

#endif





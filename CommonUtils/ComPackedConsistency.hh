//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackedConsistency.hh 443 2010-01-14 12:24:42Z stroili $
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
//      D.Aston           More precision and range for 2001 microDST
//
// Copyright Information:
//      Copyright (C) 1999      U.C.S.D.
//      Copyright (C) 2000      SLAC
//
// Quote:
//      "It ain't the way that you do it, it's what you do"
//
//--------------------------------------------------------------------------

#ifndef ComPackedConsistency_HH
#define ComPackedConsistency_HH

#include "ProbTools/Consistency.hh"
#include "CommonUtils/ComPackBase.hh"

//
// Bass class includes
//
class HepString;

class ComPackedConsistency: public Consistency {

public:

  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackedConsistency(const Consistency&, double scaleLike=1.);
  ComPackedConsistency(const d_UShort& value,const d_UShort& status=0);

  ///////////////////
  // Destructor:   //
  ///////////////////
  ~ComPackedConsistency();

  ///////////////////
  // Packers:      //
  ///////////////////
  d_UShort packedValue () const; // likelihood (12 bits)
  d_UShort packedStatus() const; // consistency (12 bits) status (4 bits)


private:
  // Limits
  enum {
    FRACLEN=6,   // formerly 4   --  but the ComPackUFloat class
    EXPLEN=6,    // formerly 4   --  was buggy and incompatible 
    BIAS=-29     // formerly -5  --  (different unpacked values)
  };

};

#endif





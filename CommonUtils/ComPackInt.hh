//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackInt.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackInt
//      - allows one to construct limited "short ints"
// To Use:
//      Construct with the minimum, and the maximum value that you would like
//      to store. The number of bits required are calculated and stored 
//      internally. Provides pack() and unpack() methods.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      D.E.Azzopardi               Originator. 
//
// Copyright Information:
//      Copyright (C) 1999  DESVA Research & Design, for Hedgehog Concepts
//
// Additional information:
//      This class, and related classes, are in development right now...
//      so BEWARE!!!!
//   
// Quote:
//      "If at first you don't succeed, 
//             find out if the loser gets anything."
//
//--------------------------------------------------------------------------
#ifndef COMPACKINT_HH
#define COMPACKINT_HH

#include <stddef.h>
//
// Bass class includes
//
#include "CommonUtils/ComPackBase.hh"
#include "BaBar/BaBarODMGTypes.h"

class ComPackInt:public ComPackBase<int> {

public:
  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackInt(const int, const int); // small int type

  ///////////////////
  // Destructor:   //
  ///////////////////
  virtual ~ComPackInt();

  ///////////////////
  // Packers:      //
  ///////////////////
  StatusCode pack (const int, d_ULong &) const;

  ///////////////////
  // Unpackers:    //
  ///////////////////
  StatusCode unpack (const d_ULong, int &) const;

  /////////////////// 
  // Diagnostic:   //
  ///////////////////
  void testMe ( size_t, int & );

private:
};

#endif

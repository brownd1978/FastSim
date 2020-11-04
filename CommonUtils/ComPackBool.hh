//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackBool.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackBool
//      - trivial case - returns 0x1 for TRUE, 0x0 for FALSE
// To Use:
//      - instantiate, call pack with a bool...
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
//      "We are all worms, but I do believe that I am a glow-worm."
//                         - Winston Churchill 
//--------------------------------------------------------------------------
#ifndef COMPACKBOOL_HH
#define COMPACKBOOL_HH

//
// Bass class includes
//
#include "CommonUtils/ComPackBase.hh"

class ComPackBool:public ComPackBase<bool> {

public:
  ///////////////////
  // Constructors: //
  ///////////////////
  ComPackBool(); 

  ///////////////////
  // Destructor:   //
  ///////////////////
  virtual ~ComPackBool();

  ///////////////////
  // Packers:      //
  ///////////////////
  StatusCode pack (const bool, d_ULong &) const;

  ///////////////////
  // Unpackers:    //
  ///////////////////
  StatusCode unpack (const d_ULong, bool &) const;

  ///////////////////
  // Diagnostic:   //
  ///////////////////
  //void testMe ( size_t, int & );

private:
};
#endif

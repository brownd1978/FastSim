//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: ComPackRat.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//      Class ComPackRat.  Packs a rational number with no loss.
//      This differs from floating packing in that the bining is defined
//      so that a float which is really a rational will be packed/unpacked
//      to _exactly_ the same value, iff the denominator is the same as the
//      one given.  If the denominator of the rational value given is different
//      from the one in the packer, it will still do something sensible (ie
//      more appropriate than what's done for the float packer).
//      This class is intended for rational numbers with 'midrange' denominators
//      (~100), for huge demominators you should pack as a real float, for
//      tiny denominators you should pack as integers.
//      For best packing efficiency, choose the (numerator) range to have a value
//      2**N-1. (stored in exactly N bits).  Check the bitRange() function after
//      construction to see how many bits are actually used.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Authors: 8/17/01 David Brown
//
//      Copyright (C) 2001 Lawrence Berkeley National Lab
//--------------------------------------------------------------------------

#ifndef COMPACKRAT_HH
#define COMPACKRAT_HH

//
// Bass class includes
//
#include "CommonUtils/ComPackBase.hh"

class ComPackRat:public ComPackBase<float> {

public:
// specify the denominator, max and min numerator.  The number of bits
// will be allocated appropriately.  You can actually use this class to
// store rational numbers with other denominators, however they won't
// necessarily haveendpoint property (though 
  ComPackRat(unsigned denom, long minnum, long maxnum);

  virtual ~ComPackRat();
 
  StatusCode pack (const float, d_ULong &) const;

  StatusCode unpack (const d_ULong, float &) const;

private:
  double _denom; // denominator.  This is more efficient to
//  store as float since it gets promoted anyways
  double _scale; // cache to speed unpacking
};

#endif

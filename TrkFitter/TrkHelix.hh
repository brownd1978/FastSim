//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: TrkHelix.hh 104 2010-01-15 12:13:14Z stroili $
//
// Description:
//     Store data for single helix, using minimum space.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author(s): Steve Schaffner
//
//------------------------------------------------------------------------
#ifndef TRKHELIX_HH
#define TRKHELIX_HH

#include "BaBar/BaBarODMGTypes.h"

#include <iosfwd>
class TrkExchangePar;

// Class interface //
class TrkHelix {

public:
  TrkHelix();
  TrkHelix(const d_Float vec[5], const d_Float cov[15]);
  virtual ~TrkHelix();
  void fill(const TrkExchangePar&);
  TrkExchangePar output() const;
  TrkHelix&   operator= (const TrkHelix&);
  TrkHelix(const TrkHelix &);
  void printAll(std::ostream& os) const;

  void fillParamVec(d_Float vec[5]);
  void fillParamCov(d_Float cov[15]);
  
private:	
  d_Float _paramVec[5];
  d_Float _paramCov[15];
};

#endif


//-----------------------------------------------------------------------
// File and Version Information:
//     PacEmcShowerLib
//      Shower library interface, providing pure virtual methods which
//      are the interface to the underlying shower library.
//  Copyright Information:
//      Copyright (C) 2008      Caltech
//
//  Authors:  Chih-hsiang Cheng    2008/06/18
//-----------------------------------------------------------------------
#ifndef PACEMCSHOWERLIB_HH
#define PACEMCSHOWERLIB_HH

#include <vector>
#include <string>

class PacSimHit;
class PacEmcCluster;
class PdtEntry;

class PacEmcShowerLib {

public:

  PacEmcShowerLib();
  PacEmcShowerLib(const char* filename);
  virtual ~PacEmcShowerLib();
  
  
  virtual unsigned select_entry(const PacSimHit *simhit) const = 0;
  virtual PacEmcCluster *sample_cluster(const PacSimHit *simhit) const = 0;

private:

  std::string _filename;


};
#endif

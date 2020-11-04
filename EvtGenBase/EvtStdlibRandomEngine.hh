/*******************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: EvtGenBase
 *    File: $Id: EvtStdlibRandomEngine.hh 427 2010-01-14 13:25:53Z stroili $
 *  Author: Alexei Dvoretskii, dvoretsk@slac.stanford.edu, 2001-2002
 *
 * Copyright (C) 2002 Caltech
 *******************************************************************************/

/*
 * Interface to stdlib's random number generator
 */
                      
#ifndef EVT_STDLIB_RANDOM_ENGINE_HH
#define EVT_STDLIB_RANDOM_ENGINE_HH

#include <stdlib.h>
#include "EvtGenBase/EvtRandomEngine.hh"

class EvtStdlibRandomEngine : public EvtRandomEngine {
public:
  
  void setSeed(unsigned int seed)
  {
    srand(seed);
  }
  
  virtual double random()
  {
    double x = rand();
    double y = RAND_MAX;
    return x/y;
  }
};

#endif



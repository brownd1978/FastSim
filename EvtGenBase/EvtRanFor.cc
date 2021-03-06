//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: EvtRanFor.cc 427 2010-01-14 13:25:53Z stroili $
//
// Description:
//	subroutine emcranfor_.
//      Provides FORTRAN calable interface to EvtRandom::Flat()
//      Can be used as EVTRANFOR instead of RANLUX in FORTRAN programs
//      or as evtranfor_ instead of ranlux_ in C/C++ programs.
//      No header file is provided, as C++ programs should use EvtRandom
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Sven Menke
//
// Copyright Information: See EvtGen/COPYRIGHT
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "EvtGenBase/EvtRandom.hh"
extern "C" {
  void evtranfor_(float *rvec, int *len) 
  {
    for (int i=0;i<*len;i++)
      rvec[i] = EvtRandom::Flat();
  }
}


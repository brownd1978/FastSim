/*******************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: EvtGenBase
 *    File: $Id: EvtPointPred.hh 427 2010-01-14 13:25:53Z stroili $
 *  Author: Alexei Dvoretskii, dvoretsk@slac.stanford.edu, 2001-2002
 *
 * Copyright (C) 2002 Caltech
 *******************************************************************************/

// Predicate testing validity of a point. The point class must provide
// bool isValid() method

#ifndef EVT_POINT_PRED_HH
#define EVT_POINT_PRED_HH

template <class Point> class EvtPointPred {
public:
  
  typedef Point argument_type;
  typedef bool result_type;
  
  EvtPointPred() {}
  EvtPointPred(const EvtPointPred&) {}
  ~EvtPointPred() {}
  
  result_type operator()(argument_type x) 
  {
    return x.isValid();
  }
};


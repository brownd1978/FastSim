/*******************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: EvtGenBase
 *    File: $Id: EvtFlatAmp.hh 427 2010-01-14 13:25:53Z stroili $
 *  Author: Alexei Dvoretskii, dvoretsk@slac.stanford.edu, 2001-2002
 *
 * Copyright (C) 2002 Caltech
 *******************************************************************************/

// Flat amplitude

#ifndef EVT_FLAT_AMP_HH
#define EVT_FLAT_AMP_HH

#include "EvtGenBase/EvtAmplitude.hh"

template <class T>
class EvtFlatAmp : public EvtAmplitude<T> {  
public:

  EvtFlatAmp() {}
  EvtFlatAmp(const EvtFlatAmp<T>& other) : EvtAmplitude<T>(other) {}
  virtual ~EvtFlatAmp() {}

  virtual EvtAmplitude<T>* clone() const { return new EvtFlatAmp<T>(*this); }
  virtual EvtComplex amplitude(const T& p) const { return EvtComplex(1.,0.); }
}; 

#endif

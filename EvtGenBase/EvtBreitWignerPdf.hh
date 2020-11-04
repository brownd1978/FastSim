/*******************************************************************************
 * Project: BaBar detector at the SLAC PEP-II B-factory
 * Package: EvtGenBase
 *    File: $Id: EvtBreitWignerPdf.hh 427 2010-01-14 13:25:53Z stroili $
 *  Author: Alexei Dvoretskii, dvoretsk@slac.stanford.edu, 2001-2002
 *
 * Copyright (C) 2002 Caltech
 *******************************************************************************/

// Breit-Wigner PDF

#ifndef EVT_BREIT_WIGNER_PDF_HH
#define EVT_BREIT_WIGNER_PDF_HH

#include "EvtGenBase/EvtIntegPdf1D.hh"

class EvtBreitWignerPdf : public EvtIntegPdf1D {
  
public:
  
  EvtBreitWignerPdf(double min, double max, double m0, double g0);
  EvtBreitWignerPdf(const EvtBreitWignerPdf& other);
  virtual ~EvtBreitWignerPdf(); 
  
  double pdf(const EvtPoint1D& x) const;
  EvtPdf<EvtPoint1D>* clone() const
  {
    return new EvtBreitWignerPdf(*this);
  }

  double pdfIntegral(double m) const;
  double pdfIntegralInverse(double x) const;

  // accessors

  inline double m0() const { return _m0; }
  inline double g0() const { return _g0; }

private:

  double _m0;
  double _g0;
  
};


#endif


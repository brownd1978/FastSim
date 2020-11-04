// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkResModelSi.cc
// Copyright Information:
//	Copyright (C) 2009	INFN, Pisa
//
//  Authors: John Walsh, 25May2009
// ------------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "PacTrk/PacTrkResModelSi.hh"

#include <math.h>
#include <assert.h>

PacTrkResModelSi::PacTrkResModelSi(TrkEnums::TrkViewInfo view, int model, std::vector<double>& pars) : 
  _view(view),_model(model),_pars(pars)
{
  if (0 == _model) {  // flat resolution model
    assert(_pars.size() == 1);
  }
  else if (1 == _model) {
    assert(7 == _pars.size()) ;
  }
}	

PacTrkResModelSi::~PacTrkResModelSi()
{
}

double
PacTrkResModelSi::hitRes(double lambda) {

  double r=0;

  if (0 == _model) {  // flat resolution model
    r = _pars[0];
  }
  else if (1 == _model) {
    assert(7 == _pars.size()) ;

    double l = lambda;

    // in phi view, add average angle of incidence by hand. This is necessary due 
    // to cylindrical approximation of geometry
    if (_view == TrkEnums::xyView) l += _pars[6];

    // first calc resolution in detector plane
    double rplane = _pars[0] + _pars[1]*pow(fabs(l)-_pars[2],2) +
      _pars[3]*exp(-pow(fabs(l)-_pars[4],2)/2./pow(_pars[5],2));
    
    // project perp to track
    r = cos(l)*rplane;
    
  }
  return r;
}

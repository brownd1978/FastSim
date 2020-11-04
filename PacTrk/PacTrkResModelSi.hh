// ------------------------------------------------------------------------------
// File and Version Information:
//   PacTrkResModelSi.hh; model silicon point resolution
//
// Copyright Information:
//	Copyright (C) 2009    INFN, Pisa
//
//  Authors: John Walsh, 25May2009
// ------------------------------------------------------------------------------
#ifndef PacTrkResModelSi_HH
#define PacTrkResModelSi_HH

#include <vector>
#include "TrkBase/TrkEnums.hh"

class PacTrkResModelSi {
public:

  PacTrkResModelSi(TrkEnums::TrkViewInfo view, int model, std::vector<double>& pars);
  virtual ~PacTrkResModelSi();

  
  double hitRes(double lambda); 

  int resModel() {return _model;}

private:
  TrkEnums::TrkViewInfo _view;
  int _model;
  std::vector<double> _pars;

};

#endif


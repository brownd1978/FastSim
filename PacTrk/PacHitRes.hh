// ------------------------------------------------------------------------------
// File and Version Information:
//   PacHitRes; Struct to describe hit resolution
// Copyright Information:
//
//  Authors: Matteo Rama 28 jan 2009
// ------------------------------------------------------------------------------
#ifndef PacHitRes_HH
#define PacHitRes_HH

// polynomial representation of the integrated 'T2D' function, giving
// the proabability distribution of different resolutions across a cell

struct PacHitRes {
  PacHitRes(const std::vector<double>& params) : _params(params) {}
   
  std::vector<double> _params;
  double resFun(double xval) const {
    double arg(1.0);
    double retval(0.0);
    for(std::vector<double>::const_iterator ipar = _params.begin();ipar != _params.end(); ipar++){
      retval += arg*(*ipar);
      arg *= xval;
    }
    return retval;
  }
  
};

#endif
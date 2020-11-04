//-----------------------------------------------------------------------
// File and Version Information:
//   PacEmcClusterSplitter
//     Class for splitting a PacEmcCluster to several single bump
//     clusters if the original PacEmcCluster has multiple bumps.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2009/02/03)
//
//-----------------------------------------------------------------------
#ifndef PACEMCCLUSTERSPLITTER_HH
#define PACEMCCLUSTERSPLITTER_HH

#include <vector>
#include <map>
using std::vector;
using std::map;

#include "AbsEnv/TwoCoordIndex.hh"
#include "PacEmc/PacEmcCluster.hh"

class PacEmcClusterSplitter {

public:

  PacEmcClusterSplitter();
  
  virtual ~PacEmcClusterSplitter();

  void splitCluster(PacEmcCluster *cluster,std::vector< PacEmcCluster* >& splittedClusters);

  void setRMo(double x) { _rMo= x; }
  void setTolerance(double x) { _tolerance= x;}
  void setMaxIterations(unsigned k) {_maxIterations=k; }
  void setExponent(double x) { _exponent= x; }
  unsigned numIterations() { return _numIterations; }
  void clear();

private:

  double weightingFunc(const double energy, const double dR);
  double _rMo;
  unsigned _maxIterations;
  double _tolerance;
  double _exponent;
  unsigned _numIterations;
};
#endif

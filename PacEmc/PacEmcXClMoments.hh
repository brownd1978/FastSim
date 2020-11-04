//-----------------------------------------------------------------------
// File and Version Information:
//    PacEmcXClMoments
//      Modified from Babar's PacEmcXClMoments class.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/25)
//
//-----------------------------------------------------------------------
#ifndef PACEMCXCLMOMENTS_HH
#define PACEMCXCLMOMENTS_HH

//---------------
// C++ Headers --
//---------------

#include <iostream>
#include <vector>

//----------------------
// Base Class Headers --
//----------------------
#include "PacEmc/PacEmcAbsClusterProperty.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "CLHEP/Vector/ThreeVector.h"
class PacEmcDigi;
class PacEmcCluster;


struct PacEmcClEnergyDeposition
{
  double deposited_energy;
  double r;
  double phi;
};

class PacEmcXClMoments : public PacEmcAbsClusterProperty
{

public:

  // Constructors
  PacEmcXClMoments( const PacEmcCluster& );
  PacEmcXClMoments( const PacEmcXClMoments & );

  // Destructor

  virtual ~PacEmcXClMoments();


  // Second moment of energy about centroid
  virtual double secondMoment() const;
  virtual double secondMomentTP() const;
  virtual double secondMomentPhi() const;
  virtual double secondMomentTheta() const;

  // Absolute value Zernike-Moment of index(m,n); according to the
  // definition of the Zernike moments, n and m must satisfy
  // (a) n>=m  (b) n-m even  (c) n,m>=0
  // Otherwise this function returns -1;
  virtual double absZernikeMoment(int n, int m,double R0=15) const;

  // Parameter for description of the LATeral energydeposition
  // within the cluster; if cluster contains less than 3 digis,
  // the function return 0
  virtual double lat() const;

  void print( std::ostream& o = std::cout );

protected:

  // Helper functions

  void init();

  // explicit implementation of polynomial part of
  // Zernike-Functions for n<=5;

  double f00(double r) const;
  double f11(double r) const;
  double f20(double r) const;
  double f22(double r) const;
  double f31(double r) const;
  double f33(double r) const;
  double f40(double r) const;
  double f42(double r) const;
  double f44(double r) const;
  double f51(double r) const;
  double f53(double r) const;
  double f55(double r) const;

  // Calculation of Zernike-Moments for n<=5 :
  double fast_AbsZernikeMoment(int n, int m, double R0)  const;

  // Calculation of Zernike-Moments for general values of (n,m)
  double calc_AbsZernikeMoment(int n, int m, double R0)  const;

  double fak(int n) const;

private:

  // These should never get used
  PacEmcXClMoments();
  PacEmcXClMoments &operator=(const PacEmcXClMoments &);

  // Data members
  std::vector< PacEmcClEnergyDeposition > *_energyDistribution;
  int _clusterSize;
  double (PacEmcXClMoments::*_fcn[12])( double ) const;
  double (PacEmcXClMoments::*_fcn2)( double );

};

#endif 

//-----------------------------------------------------------------------
// File and Version Information:
//    PacEmcClusterEnergySums
//      Modified from Babar's EmcClusterEnergySums class.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/13)
//
//-----------------------------------------------------------------------
#ifndef PACEMCCLUSTERENERGYSUMS_HH
#define PACEMCCLUSTERENERGYSUMS_HH

//---------------
// C++ Headers --
//---------------

#include <iostream>

//----------------------
// Base Class Headers --
//----------------------
#include "PacEmc/PacEmcAbsClusterProperty.hh"

//              ---------------------
//              -- Class Interface --
//              ---------------------

class PacEmcClusterEnergySums : public PacEmcAbsClusterProperty
{

public:

  // Constructors
  PacEmcClusterEnergySums( const PacEmcCluster& );
  PacEmcClusterEnergySums( const PacEmcClusterEnergySums &);

  // Destructor

  virtual ~PacEmcClusterEnergySums();

  // Partial Energy of Cluster
  virtual double energy( int numberOfDigis ) const;
  virtual double energy( double distance ) const;
  virtual double e1() const;
  virtual double e9() const;
  virtual double e25() const;

  // Energy ratios of Cluster
  virtual double e1e9() const;
  virtual double e9e25() const;

  // Energies between ( and including )
  // -- theta rings
  //  virtual double energyTheta( int from, int to ) const;
  // -- phi sectors
  //  virtual double energyPhi( int from, int to ) const;

  // Printing
  //virtual void print( std::ostream& o ) const;


private:

  PacEmcClusterEnergySums();
  PacEmcClusterEnergySums &operator=(const PacEmcClusterEnergySums &);

};
#endif

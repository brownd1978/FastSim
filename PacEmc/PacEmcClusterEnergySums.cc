//-----------------------------------------------------------------------
// File and Version Information:
//    PacEmcClusterEnergySums
//      Modified from Babar's EmcClusterEnergySums class.
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/13)
//
//-----------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "PacEmc/PacEmcClusterEnergySums.hh"

#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "BaBar/Constants.hh"
#include "AbsEnv/TwoCoordIndex.hh"
#include "PacEmc/PacEmcCluster.hh"
#include "PacEmc/PacEmcDigi.hh"

using std::endl;
using std::ostream;

//----------------
// Constructors --
//----------------

PacEmcClusterEnergySums::PacEmcClusterEnergySums( const PacEmcCluster& toUse )
  : PacEmcAbsClusterProperty( toUse )
{
}

PacEmcClusterEnergySums::PacEmcClusterEnergySums(const PacEmcClusterEnergySums& other)
  : PacEmcAbsClusterProperty( other )
{}

//--------------
// Destructor --
//--------------

PacEmcClusterEnergySums::~PacEmcClusterEnergySums()
{}

//-------------
// Methods   --
//-------------

double
PacEmcClusterEnergySums::energy( int n ) const
{
  double sum = 0;
  const PacEmcDigi* current;

  const std::vector< PacEmcDigi* > *DigiList = myCluster().pacEmcDigis();

  // If asked for too many digis summed return the whole energy
  if ( n > myCluster().numberOfDigis() ) return ( myCluster().energy() );
  
  // DigiList must have been sorted, right?
  for ( int i=0; i<n; i++ ) {
    current = (*DigiList)[i];
    sum += current->energy();
  }

  return( sum );
}

double
PacEmcClusterEnergySums::energy( double distance ) const
{
  double sum = 0;

  PacEmcDigiConstIterator iter= myCluster().pacEmcDigis()->begin();
  
  while ( (iter != myCluster().pacEmcDigis()->end()) ) {
    if ( myCluster().distanceToCentre( (*iter) ) < distance )
      sum += (*iter)->energy();
    ++iter;
  }
  
  return( sum );
}


double
PacEmcClusterEnergySums::e1() const
{
  //  const EmcDigi* current = myCluster().centrum();
  const PacEmcDigi* current = myCluster().maxima();

  if ( current ) {
    return( current->energy() );
  } else {
    return 0;
  }
}


double
PacEmcClusterEnergySums::e9() const
{
  const PacEmcDigi* current = myCluster().maxima();
  if ( !current ) {
    return 0;
  }
  double sum(0);

  // Loop over all digis and find those whose x or y index is different from
  // the maxima digi by 1 or 0, and sum up energies.

  // FIXME: it doesn't work when it crosses the boundary like iphi=0 or where
  // the number of crystals in a ring changes
  PacEmcDigiConstIterator iter= myCluster().pacEmcDigis()->begin();
  while ( (iter != myCluster().pacEmcDigis()->end() ) ) {
    if ( abs( (*iter)->theta() - current->theta() ) <= 1 ) {
      if ( abs( (*iter)->phi() - current->phi() ) <= 1 ) {
	sum+= (*iter)->energy();
      }
    }
    
    iter++;
  }

  return sum;

}

double
PacEmcClusterEnergySums::e25() const
{
  const PacEmcDigi* current = myCluster().maxima();
  if ( ! current ) {
    return 0;
  }
  double sum(0);

  // Loop over all digis and find those whose x or y index is different from
  // the maxima digi by 0, 1 or 2, and sum up energies.
 
  // FIXME: it doesn't work when it crosses the boundary like iphi=0
  // the number of crystals in a ring changes
  PacEmcDigiConstIterator iter= myCluster().pacEmcDigis()->begin();
  while ( (iter != myCluster().pacEmcDigis()->end() ) ) {
    if ( abs( (*iter)->theta() - current->theta() ) <= 2 &&
	 abs( (*iter)->phi() - current->phi() ) <= 2 ) {
      sum+= (*iter)->energy();
    }
    
    iter++;
  }

  return sum;
}


double
PacEmcClusterEnergySums::e1e9() const
{
  double _e9 = e9();

  double e1e9 = ( _e9 != 0. )? e1()/_e9 : -1.;

  return( e1e9 );
}

double
PacEmcClusterEnergySums::e9e25() const
{
  double _e25 = e25();

  double e9e25 = ( _e25 != 0. )? e9()/_e25 : -1.;

  return( e9e25 );
}


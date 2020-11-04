//-----------------------------------------------------------------------
// File and Version Information:
//    PacEmcAbsClusterProperty
//      Modified from Babar's EmcAbsClusterProperty
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/18)
//
//----------------------------------------------------------------------
#include "BaBar/BaBar.hh"

#include "PacEmc/PacEmcAbsClusterProperty.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsDetIndex.hh"
#include "PacEmc/PacEmcCluster.hh"

//----------------
// Constructors --
//----------------
PacEmcAbsClusterProperty::PacEmcAbsClusterProperty( const PacEmcCluster& toUse )
  : _myCluster( toUse )
{
}

PacEmcAbsClusterProperty::PacEmcAbsClusterProperty( const PacEmcAbsClusterProperty& t )
  : _myCluster( t._myCluster )
{
}

//--------------
// Destructor --
//--------------
PacEmcAbsClusterProperty::~PacEmcAbsClusterProperty()
{
}

const PacEmcCluster&
PacEmcAbsClusterProperty::myCluster() const
{
  return _myCluster;
}


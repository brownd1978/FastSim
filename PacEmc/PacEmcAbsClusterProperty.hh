//-----------------------------------------------------------------------
// File and Version Information:
//    PacEmcAbsClusterProperty
//      Modified from Babar's EmcAbsClusterProperty
//
// Author List:
//     Chih-hsiang Cheng       Caltech   (initial version 2008/06/18)
//
//----------------------------------------------------------------------
#ifndef PACEMCABSCLUSTERPROPERTY_HH
#define PACEMCABSCLUSTERPROPERTY_HH


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class PacEmcCluster;
class PacEmcDigi;
class TwoCoordIndex;
class AbsDetIndex;

class PacEmcAbsClusterProperty {

public:

  // Constructors
  PacEmcAbsClusterProperty( const PacEmcCluster& );
  PacEmcAbsClusterProperty( const PacEmcAbsClusterProperty& );

  // Destructor
  virtual ~PacEmcAbsClusterProperty();

protected:

  // Helper functions
  const PacEmcCluster& myCluster() const;

private:

  PacEmcAbsClusterProperty &operator=(const PacEmcAbsClusterProperty&);
  // Friends

  // Data members
  const PacEmcCluster& _myCluster;

};

#endif


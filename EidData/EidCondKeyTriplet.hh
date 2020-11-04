#ifndef EIDCONDKEYTRIPLET_HH
#define EIDCONDKEYTRIPLET_HH

//--------------------------------------------------------------------------
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1999   The University of Pennsylvania and 
//                           California Institute of Technology
//
//------------------------------------------------------------------------
//
// History:
//    Ed Frank                 01 Jun 99  Creation of first version
//    Gregory Dubois-Felsmann  99.12.10   Inserted EidPartitionDoublet base
//
// Bugs:
//
//------------------------------------------------------------------------

//----------------------
// Base Class Headers --
//----------------------
#include "EidData/EidPartitionDoublet.hh"

//-----------------
// C/C++ Headers --
//-----------------
#include <iosfwd>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BaBar/BaBarODMGTypes.h"
#include "BdbTime/BdbTime.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


/**
 *  This class represents a fully unique conditions database key.
 *  This is a triplet of platform number, partition mask, and a BdbTime
 *  (UTC wall clock) time stamp.
 *
 *  The three components are all required because, in principle, it is
 *  possible for data to be collected at the same time in more than
 *  platform, or in more than one partition within a platform.
 *
 *  The triplet, therefore, exists to support so-called "partition-specific
 *  containers" in the conditions database - a December 1999 upgrade to the
 *  conditions database API.
 *
 *  In practice many conditions database interval histories (containers)
 *  are connected to a non-partitionable part of the detector and do not
 *  need partition-specific treatment.  For these, only the time component
 *  of the triplet is relevant.
 *
 *  For others, such as the record of run numbers of runs taken, partition-
 *  specific containers are required, and the conditions database container
 *  names are intended to be constructed from primary component with a
 *  partition-specific suffix, provided by the containerNameSuffix member
 *  function of this class.  See EidPartitionDoublet for more information
 *  about its implementation.
 *
 *  The platform and partition fields of the triplet may be wildcarded
 *  by setting them equal to zero.
 *
 *  The platform and partition are handled by the EidPartitionDoublet 
 *  base class.
 *
 *  An official ASCII form for an EidCondKeyTriplet (ECKT) has yet to be
 *  defined.  A temporary substitute is provided for output only at this time.
 *
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 1999   The University of Pennsylvania, 
 *                       California Institute of Technology
 *
 *  @see odfTime
 *  @see EidInfo
 *
 *  @version $Id: EidCondKeyTriplet.hh 451 2010-01-14 13:03:45Z stroili $
 *
 *  @author Ed Frank, efrank@slac.stanford.edu  (originator)
 *  @author Gregory Dubois-Felsmann, gpdf@hep.caltech.edu (KDOC & I/O) 
*/

class EidCondKeyTriplet : public EidPartitionDoublet {
public:

  /**
   *  Constructor from specified values of the triplet's components. 
   *  Normal consumers of BaBar data should never have to use it -
   *  all EidCondKeyTriplets they use will be obtained from other sources.
   *
   *  @param platform        Platform number of partition (0 = wildcard)
   *  @param partitionMask   Crate mask of partition (0 = w/c)
   *  @param time            BdbTime of conditions data (no wildcarding)
   */
  EidCondKeyTriplet( d_ULong platform, d_ULong partitionMask,
		     BdbTime time );

  /**
   *  Default constructor producing {0, 0, 0}, an impossible value for
   *  the ECKT for any real data.  (Platform and partition have their
   *  wildcard values.)
   *
   *  Still, it's potentially dangerous.  It exists so that one can create
   *  a ECKT lvalue for use in assignments or with (a future) operator>>.
   */
  EidCondKeyTriplet( );

  /** Copy constructor. */
  EidCondKeyTriplet( const EidCondKeyTriplet& rhs );

  /** Assignment operator */
  EidCondKeyTriplet& operator=( const EidCondKeyTriplet& rhs );

  virtual ~EidCondKeyTriplet();

  /**
   *  This method returns the actual Conditions database key, a BdbTime.
   */
  inline BdbTime key( ) const;

  /**
   *  Equality comparison.  Respects wildcards for platform and partition.
   */
  bool operator==( const EidCondKeyTriplet& other ) const;

  /**
   *  Inequality comparison.  Respects wildcards for platform and partition.
   *  Implemented in terms of ==.
   */
  inline bool operator!=( const EidCondKeyTriplet& other ) const;


  // global operators with friendship (for stream insertion, extraction) ==

  friend std::ostream& operator<<( std::ostream&, const EidCondKeyTriplet& );
  // friend istream& operator>>( istream&, EidCondKeyTriplet& );

private:

  BdbTime _condKey;   // NB: this may not always be == to the ctor arg

};


/**
 *  Stream insertion (output) operator.
 */
std::ostream& operator<<( std::ostream&, const EidCondKeyTriplet& );

//****************************************************************************

// Inline implementations

inline BdbTime 
EidCondKeyTriplet::key( ) const  
{ 
  return _condKey; 
}

inline bool 
EidCondKeyTriplet::operator!=( const EidCondKeyTriplet& other ) const
{ 
  return !( *this == other ); 
}

#endif /* EIDCONDKEYTRIPLET_HH */

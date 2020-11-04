#ifndef EIDEVENTTRIPLETSET_HH
#define EIDEVENTTRIPLETSET_HH

//--------------------------------------------------------------------------
//
// $Id: EidEventTripletSet.hh 451 2010-01-14 13:03:45Z stroili $
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 2003   California Institute of Technology
//
//------------------------------------------------------------------------

//-----------------
// C/C++ Headers --
//-----------------
#include <set>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "EidData/EidEventTriplet.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------


/**
 *  Simple class for maintaining a wildcard-sensitive set of event triplets,
 *  providing insertion and matching functions.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2003   California Institute of Technology
 *
 *  @version $Id: EidEventTripletSet.hh 451 2010-01-14 13:03:45Z stroili $
 *
 *  @author Gregory Dubois-Felsmann  (originator)
 */

class EidEventTripletSet {

//--------------------
// Declarations     --
//--------------------

    // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  EidEventTripletSet() 
    : _tripletSet() { }

  // Destructor
  virtual ~EidEventTripletSet( );

  // Operators

  // Selectors (const)

  // Modifiers

  /** Add a triplet to the set registered. */
  void add( const EidEventTriplet& t );

  /** Remove all registered triplets. */
  void clear();

  /** 
   *  Check to see if a triplet is registered, as such or under a wild card.
   *  If requested, remove every triplet from the set that matches the
   *  specified one.
   */
  bool find( const EidEventTriplet& t, bool remove );


protected:

  // Helper functions

private:

  // Friends
  
  // We use a multiset and WildCardOrdering here because a) we allow the
  // use of wildcards, and, in theory, because it is possible that users
  // might specify multiple triplets with the same time.
  typedef std::multiset< EidEventTriplet,
                         EidEventTriplet::WildCardOrdering> EetSet;

  // Data members

  EetSet _tripletSet;
  
  // Disabled copy constructor and assignment operator.
  EidEventTripletSet( const EidEventTripletSet& );
  EidEventTripletSet& operator=( const EidEventTripletSet& );

};

#endif // EIDEVENTTRIPLETSET_HH

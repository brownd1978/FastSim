#ifndef EIDEVENTTRIPLET_HH
#define EIDEVENTTRIPLET_HH

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
//    Gregory Dubois-Felsmann  1999.10.31   I/O operators & KDOC added
//    Gregory Dubois-Felsmann  1999.12.10   Inserted EidPartitionDoublet base
//    Gregory Dubois-Felsmann  2003.01.04   Provided comparison functor
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
#include "OdfCommon/odfTime.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


/**
 *  This class represents _the_ unique event identifier for BaBar.
 *  This identifier is a triplet of platform number, partition mask,
 *  and DataFlow time stamp (odfTime).
 *
 *  The three components are all required because, in principle, it is
 *  possible for events with identical time stamps to be collected in
 *  more than platform at a time, or in more than one partition within
 *  a platform.
 *
 *  Given an EidEventTriplet obtained from a recorded (simulated or
 *  real) event it should be possible to retrieve that event from any
 *  supported data repository.  
 *
 *  Not all possible bit patterns of EidEventTriplets (EETs) correspond 
 *  to extant events, of course.  It is expected that that a utility will
 *  be created to enable asking whether a particular EET does map to an
 *  event in a specified data collection.
 *
 *  Note that there is no mention of the run number here, nor of an
 *  "event number".  BaBar has runs and run numbers (unique within a
 *  platform), but no event numbers (for reasons beyond the scope of 
 *  header file comments).  The run number is redundant with the EET,
 *  however.  There should also be a tool which permits looking up a
 *  run number based on an EET.  There is sufficient information in the
 *  conditions database to permit this.
 *
 *  The platform and partition are handled by the EidPartitionDoublet 
 *  base class.
 *
 *  The platform and partition fields of the triplet may be wildcarded
 *  by setting them equal to zero.
 *
 *  The ASCII form for an EET is "PP:pppppp:uuuuuu/llllllll:S" where
 *  the first four fields are written in lowercase hexadecimal digits,
 *  'P' for platform, 'p' for partition, 'u' and 'l' for the upper and
 *  lower halves of the 56 bit odfTime, and the last field, 'S', is a
 *  checkdigit drawn from the character class [GHJ-NP-Y].  Wildcard 
 *  values for platform and partition are both printed as "*".
 *
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
 *  @version $Id: EidEventTriplet.hh 451 2010-01-14 13:03:45Z stroili $
 *
 *  @author Ed Frank, efrank@slac.stanford.edu  (originator)
 *  @author Gregory Dubois-Felsmann, gpdf@hep.caltech.edu (KDOC & I/O) */

class EidEventTriplet : public EidPartitionDoublet {
public:

  /**
   *  Constructor from specified values of the triplet's components. 
   *  Normal consumers of BaBar data should never have to use it -
   *  all EidEventTriplets they use will be obtained from other sources.
   *
   *  @param platform        Platform number sourcing event (0 = wildcard)
   *  @param partitionMask   Crate mask of partition sourcing event (0 = w/c)
   *  @param timeStamp       DataFlow time stamp of event (no wildcarding)
   */
  EidEventTriplet( d_ULong platform, d_ULong partitionMask,
		   odfTime timeStamp );

  /**
   *  Default constructor returning "0:0:0/0", unlikely to match the EET for
   *  any real event.  (Impossible for real data, as 0/0 = 1997.01.01,
   *  before any DataFlow hardware functioned, and administratively 
   *  prohibited for simulated data.)  Platform and partition have their
   *  wildcard values.
   *
   *  Still, it's potentially dangerous.  It exists so that one can create
   *  a EET lvalue for use in assignments or with operator>>.
   */
  EidEventTriplet( );

  /** Copy constructor. */
  EidEventTriplet( const EidEventTriplet& rhs );

  /** Assignment operator */
  EidEventTriplet& operator=( const EidEventTriplet& rhs );

  virtual ~EidEventTriplet();

  /** 
   *  Accessor for event time stamp in 56-bit odfTime format of
   *  59.5 MHz clock ticks since 1997.01.01 00:00.
   */
  inline odfTime timeStamp(void) const;

  /**
   *  Equality comparison.  Respects wildcards for platform and partition.
   */
  bool operator==( const EidEventTriplet& other ) const;

  /**
   *  Inequality comparison.  Respects wildcards for platform and partition.
   *  Implemented in terms of ==.
   */
  inline bool operator!=( const EidEventTriplet& other ) const;


  // global operators with friendship (for stream insertion, extraction) ==

  friend std::ostream& operator<<( std::ostream&, const EidEventTriplet& );
  friend std::istream& operator>>( std::istream&, EidEventTriplet& );


  /**
   *  Comparison operator defining a strict weak ordering; usable for
   *  Standard Library sorted associative containers in a way that
   *  respects wildcards.  
   *
   *  This ordering does not take the EidPartitionDoublet into account --
   *  it respects only the odfTime.  Therefore, in the STL sense, all
   *  triplets with the same odfTime are "equivalent", in the sense
   *  that a!<b and b!<a.
   *
   *  When using this to look for a triplet in an associative container,
   *  the intent is that you use a container that permits multiple values
   *  (e.g., std::multiset or std::multimap), and use its equal_range
   *  operation to extract the equivalence class for the triplet you are
   *  looking up.  You must then search linearly through the equivalence
   *  class to see if any of the results satisfy operator== with your
   *  search target.
   */

  class WildCardOrdering {
  public:
    bool operator()( const EidEventTriplet& t1, const EidEventTriplet& t2 ) const;
  };

  // WildCardOrdering should have access to the representation.
  friend class WildCardOrdering;


  /**
   *  Comparison operator defining a strict weak ordering; usable for
   *  Standard Library sorted associative containers in a way that
   *  ignores wildcards, treating them as distinct from all other values.
   *
   *  This can be useful if it is desired to detect true duplicates.
   *
   *  This ordering respects the actual numerical values of odfTime,
   *  platform, and partition mask.  It is a "dictionary order" relation
   *  in which the odfTime has priority, followed by the platform and
   *  the partition mask.  Giving the odfTime a higher priority than the
   *  partition mask is appropriate because the "usual" partition mask
   *  changes over the history of data-taking, so that it seems more 
   *  natural to sort events in time order even across changes in the
   *  definition of the usual partition mask.  The choice of priority
   *  for the platform is less compelling; a good case could also be made
   *  for putting it highest, but in practice it should matter little.
   *
   *  In this ordering, only triplets that are identical bit-for-bit
   *  should be "equivalent" in the Standard Library sense.
   */

  class FullOrdering {
  public:
    bool operator()( const EidEventTriplet& t1, const EidEventTriplet& t2 ) const;
  };

  // FullOrdering should have access to the representation.
  friend class FullOrdering;


private:

  // internal helper functions: ===========================================

  /** Compute the checksum character for an EidEventTriplet */
  char computeChecksum() const;


private:

  odfTime _timeStamp;

};

/**
 *  Stream insertion (output) operator.
 */
std::ostream& operator<<( std::ostream&, const EidEventTriplet& );

/**
 *  Stream extraction (input) operator.
 *
 *  On input, the platform and partition mask may be omitted and will
 *  be treated as wildcards, according to the following scheme:
 *       [plat:[part]:]odfTime:checkSum
 *  A single '*' character may also be used to indicate a wildcard in
 *  each of those fields.  All input digits are treated as hexadecimal,
 *  with uppercase permitted.  No "0x" is permitted.  The platform
 *  may not exceed four hex digits, while the other three fields may
 *  not exceed eight.
 *
 *  Leading whitespace is consumed if the skipws state is set on the istream.
 *  No embedded whitespace is permitted, and trailing whitespace is not
 *  consumed.
 *
 *  When an invalid character is detected in the input stream, the
 *  response depends on whether it is an excess hex digit or a
 *  non-digit or EOF.  If it is an excess digit, any further
 *  consecutive digits will be read from the stream, the stream will
 *  be left positioned at the first non-digit or EOF, and the stream's
 *  fail() state will be set.  If it is an EOF or non-digit, the input
 *  stream will be left positioned there, and the fail() state will be
 *  set if and only if not enough characters have been read from the
 *  stream to form a valid EET.  In either case, if the fail() state
 *  is set, the input variable is not touched.
*/
std::istream& operator>>( std::istream&, EidEventTriplet& );



//***************************************************************************

inline bool 
EidEventTriplet::WildCardOrdering::operator()( const EidEventTriplet& t1, 
					       const EidEventTriplet& t2 ) const
{ 
  return t1._timeStamp < t2._timeStamp; 
}


inline bool 
EidEventTriplet::FullOrdering::operator()( const EidEventTriplet& t1, 
					   const EidEventTriplet& t2 ) const
{ 
  return t1._timeStamp <  t2._timeStamp 
    || ( t1._timeStamp == t2._timeStamp 
	 && ( t1.platform() < t2.platform() 
	      || ( t1.platform() == t2.platform() 
		   && t1.partitionMask() < t2.partitionMask() ) ) ); 
}


inline odfTime 
EidEventTriplet::timeStamp(void) const
{ return _timeStamp; }


inline bool    
EidEventTriplet::operator!=( const EidEventTriplet& other ) const
{ return !( *this == other ); }

#endif /* EIDEVENTTRIPLET_HH */

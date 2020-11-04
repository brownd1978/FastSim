#ifndef EIDPARTITIONDOUBLET_HH
#define EIDPARTITIONDOUBLET_HH

//--------------------------------------------------------------------------
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1999    California Institute of Technology
//
//------------------------------------------------------------------------
//
// History:
//    Gregory Dubois-Felsmann  99.12.04   Created
//
// Bugs:
//
//------------------------------------------------------------------------

//-----------------
// C/C++ Headers --
//-----------------
#include <iosfwd>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BaBar/BaBarODMGTypes.h"

/**
 *  This class represents a data acquisition partition in BaBar (at
 *  any given time).  This identifier is a doublet of platform number
 *  and partition crate mask.  It represents a potential partition in
 *  that platform.
 *
 *  This class is shared by the event ID triplet and the conditions
 *  database key triplet, and provides services especially relevant for 
 *  the latter, in respect of partition-specific containers in the
 *  database.
 *
 *  The ASCII form for this class is "[PP]PP:[pp]pppppp" (P=platform,
 *  p=partition mask, both in lowercase hex, with the unbracketed
 *  field widths as the default).  Wildcard values for platform and 
 *  partition are each printed as "*" instead.
 *
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 1999   California Institute of Technology
 *
 *  @version $Id: EidPartitionDoublet.hh 451 2010-01-14 13:03:45Z stroili $
 *
 *  @author Gregory Dubois-Felsmann, gpdf@hep.caltech.edu (creator) 
*/

class EidPartitionDoublet {
public:

  typedef d_ULong EidPlatform_t;
  typedef d_ULong EidPartition_t;

  /**
   *  Constructor from specified values of the doublet's components. 
   *  Normal consumers of BaBar data should never have to use it -
   *  all EidPartitionDoublets they use will be obtained from other sources.
   *
   *  @param platform        Platform number sourcing event (0 = wildcard)
   *  @param partitionMask   Crate mask of partition sourcing event (0 = w/c)
   */
  EidPartitionDoublet( EidPlatform_t platform, EidPartition_t partitionMask );

  /**
   *  Default constructor returning "0:0", the wildcard value.
   *
   *  It's potentially dangerous.  It exists so that one can create
   *  a EPD lvalue for use in assignments or with operator>>.
   */
  EidPartitionDoublet( );

  /** Copy constructor. */
  EidPartitionDoublet( const EidPartitionDoublet& rhs );

  virtual ~EidPartitionDoublet();

  /** Assignment operator */
  EidPartitionDoublet& operator =( const EidPartitionDoublet& rhs );

  /**
   *  Equality comparison.  Respects wildcards for platform and partition.
   */
  bool operator==( const EidPartitionDoublet& other ) const;

  /**
   *  Inequality comparison.  Respects wildcards for platform and partition.
   *  Implemented in terms of ==.
   */
  inline bool operator!=( const EidPartitionDoublet& other ) const;

  /** Accessor for platform number (0 = wildcard). */
  inline EidPlatform_t platform(void) const;

  /** 
   *  Accessor for partition's crate mask, a 32-bit bitlist of the
   *  crates participating in a partition (0 = wildcard).
   */
  inline EidPartition_t partitionMask(void) const;

  /** 
   *  Accessor for the number of the lowest-numbered crate included in
   *  the partition's crate mask.  (-1 if the crate mask is zero.)
   *  This is effectively a hash function that can be used to construct
   *  a number which is small ([-1,..,31]) but unique for all partitions 
   *  in existence within a platform at a given time.
   *
   *  Unfortunately the pattern of platform numbers doesn't allow an
   *  easy computation of a compact, unique hash for the whole doublet 
   *  (at least, we haven't thought of one yet!).  Concatenating the 
   *  platform number (7 bits) and the partitionLowCrate() value (5 bits
   *  plus the need to represent the -1 no-crates-included value) 
   *  yields 128*33=4224 possible values for the pair.  Probably only
   *  50-90 of these are realistically possible, though.  DataFlow could
   *  help with this by agreeing to set aside a very limited range of
   *  platform numbers as possible multi-crate platforms, but right now
   *  there's no way to extract the necessary information programmatically
   *  from the platform number.                                    // FIXME
   *
   *  Used to construct partition-specific database container names.
   */
  d_Long partitionLowCrate(void) const;


  // global operators with friendship (for stream insertion, extraction) ==

  friend std::ostream& operator<<( std::ostream&, const EidPartitionDoublet& );
  friend std::istream& operator>>( std::istream&, EidPartitionDoublet& );

private:
  EidPlatform_t  _platform;
  EidPartition_t _partitionMask;
};

/**
 *  Stream insertion (output) operator.
 */
std::ostream& operator<<( std::ostream&, const EidPartitionDoublet& );

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
std::istream& operator>>( std::istream&, EidPartitionDoublet& );



//***************************************************************************

EidPartitionDoublet::EidPlatform_t 
EidPartitionDoublet::platform(void) const
  { return _platform; }

EidPartitionDoublet::EidPartition_t 
EidPartitionDoublet::partitionMask(void) const   
  { return _partitionMask; }

bool  
EidPartitionDoublet::operator!=( const EidPartitionDoublet& other ) const
  { return !( *this == other ); }

#endif /* EIDPARTITIONDOUBLET_HH */

//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: EidCondKeyTriplet.cc 451 2010-01-14 13:03:45Z stroili $
//
// Description:
//    See .hh for details.
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      01 Jun 99         Creation of first version
//               
// Bugs:
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "EidData/EidCondKeyTriplet.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include <iostream>


EidCondKeyTriplet::EidCondKeyTriplet( d_ULong platform,
				      d_ULong partitionMask,
				      BdbTime time )
  : EidPartitionDoublet( platform, partitionMask )
  , _condKey( time )
{
  // WARNING:  some day _condKey will not be settable trivially as is
  // done now.
}


EidCondKeyTriplet::EidCondKeyTriplet()
  : EidPartitionDoublet()              // wildcard
  , _condKey( BdbTime(0,0) )
{ }


EidCondKeyTriplet::EidCondKeyTriplet( const EidCondKeyTriplet& rhs ) 
  : EidPartitionDoublet( rhs )
  , _condKey( rhs.key() )
{ }

EidCondKeyTriplet&
EidCondKeyTriplet::operator=( const EidCondKeyTriplet& rhs ) {
  if ( this != &rhs ) {
    EidPartitionDoublet::operator=( rhs );
    _condKey = rhs.key( );
  }

  return *this;
}


EidCondKeyTriplet::~EidCondKeyTriplet()
{ }


bool
EidCondKeyTriplet::operator==( const EidCondKeyTriplet& other ) const
{
  // Implement equality test, including symmetric wildcarding from
  // EidPartitionDoublet's operator== for platform and partition.
  // Test the time stamp first, as it's the most likely to be different.

  return (    _condKey == other.key()
	   && EidPartitionDoublet::operator==( other ) );
}


std::ostream&
operator << ( std::ostream& o, const EidCondKeyTriplet& eid ) {
  o << "[ " << eid.platform() 
    << ", " << eid.partitionMask()
    << ", ( " << eid.key().getGmtSec()
    << ", "   << eid.key().getGmtNsec()
    << " ) ]";

  return o;
}

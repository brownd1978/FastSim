//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: EidPartitionDoublet.cc 622 2010-09-13 09:48:16Z stroili $
//
// Description:
//    See .hh for details.
//
// Author List:
//    Gregory Dubois-Felsmann         California Institute of Technology
//
// History:
//    Gregory Dubois-Felsmann  99.12.04  Created
//               
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "EidData/EidPartitionDoublet.hh"

//-----------------
// C/C++ Headers --
//-----------------
#include <cstdio>
#include <ctype.h>
#include <assert.h>
#include <string.h>

#include <iostream>
#include <sstream>
#include <iomanip>
using std::ends;
using std::hex;
using std::ios;
using std::ostringstream;
using std::setfill;
using std::setw;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

EidPartitionDoublet::EidPartitionDoublet( EidPlatform_t platform,
					  EidPartition_t partitionMask )
  : _platform( platform )
  , _partitionMask( partitionMask )
{}


EidPartitionDoublet::EidPartitionDoublet()
  : _platform( 0 )              // wildcard
  , _partitionMask( 0 )         // wildcard
{ }


EidPartitionDoublet::EidPartitionDoublet( const EidPartitionDoublet& rhs) 
  : _platform( rhs.platform() )
  , _partitionMask( rhs.partitionMask() )
{ }


EidPartitionDoublet&
EidPartitionDoublet::operator =( const EidPartitionDoublet& rhs ) {
  if ( this != &rhs ) {
    _platform      = rhs.platform();
    _partitionMask = rhs.partitionMask();
  }

  return *this;
}


EidPartitionDoublet::~EidPartitionDoublet()
{}


bool
EidPartitionDoublet::operator==( const EidPartitionDoublet& other ) const
{
  // Implement equality test, including symmetric wildcards for platform
  // and partition.

  return (    ( _platform == 0 
	        || other.platform() == 0 
	        || _platform == other.platform() )
	   && ( _partitionMask == 0 
	        || other.partitionMask() == 0 
	        || _partitionMask == other.partitionMask() ) );
}


d_Long
EidPartitionDoublet::partitionLowCrate() const
{
  // Find the lowest-order bit that's set in the partition mask.
  // Return the bit number, or -1 if no bits were set.
  d_Long lowCrate = -1;
  for ( int i=0; i<8*sizeof(EidPartition_t); i++ ) {
    if ( ( _partitionMask & (1<<i) ) != 0 ) {
      lowCrate = i;
      break;
    }
  }

  return lowCrate;
}


std::ostream&
operator<<( std::ostream& o, const EidPartitionDoublet& eid ) 
{
  // Write to a temporary stringstream first.  Keep the memory local for
  // thread safety.
  // Reserve more than the maximum number of bytes one might reasonably
  // need (2+1+8+nullTerm).
//  char buf[32];
//  ostrstream s( buf, 31 );
  ostringstream s;

  // We marshal output like this so that we can properly align the whole
  // resulting string in the output field width currently set on the
  // stream "o".  If we wrote directly to o the field width would be 
  // applied only to the first insertion, the platform number.

  // Write the output, replacing wildcards with '*'s.
  
  s << hex << setfill( '0' );
  if ( eid.platform() != 0 )       s << setw(2) << eid.platform();
  else                             s << '*';
  if ( eid.partitionMask() != 0 )  s << ':' << setw(6) << eid.partitionMask();
  else                             s << ":*";
  s << ends;

  // Copy temporary to output, respecting the field width in place.
  o << s.str();

  return o;
}



std::istream&
operator>>( std::istream& is, EidPartitionDoublet& doublet ) 
{
  // Possible lexical states of the scanner:
  enum State 
       { Plat, Part, Done };

  // Scratch space for numerical components (found in the first two states),
  // with extra room left for safety's sake for the "Done" state
  // in which, however, these arrays should never be used.
  d_ULong in[3] = { 0, 0, 0 };

  // Maximum number of hex digits allowed in each field:
  static const int Max[3] = { 2, 8, 0 };  


  // We start in a state where the first hex digits read must belong
  // to the platform number.
  State cur = Plat;

  // Create a "sentry" object to perform the input prefix and suffix
  // operations via resource-allocation-is-initialization.  When the
  // sentry goes out of scope the suffix is performed.  If the initialization
  // fails, we do nothing.
  //
  // The prefix skips leading whitespace if the stream is so configured 
  // (with "skipws").
  // 
  // For gcc 2.x (2.95.3 in particular), we retain the old ipfx/isfx way of            FIXME
  // doing things.  NB: this is not the same ifdef condition as used 
  // in the iostreams migration of dual-use packages.

#if !(defined(__GNUC__) && (__GNUC__ < 3))                          // BABAR_IOSTREAMS_MIGRATION
  std::istream::sentry isentry( is );
  if ( !isentry ) return is;  // initialization failed, do nothing
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  if ( is.ipfx(0) == 0 ) {
    // The stream is already bad.  Give up without changing the odfTime.
    is.isfx();  // Call the "input suffix operator".
    return is;
  }
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

  // Loop over characters in the stream until an invalid one is found.
  int c = '\0';
  int found = 0;           // how many digits have we found?
  bool overflow = false;   // have exceeded the max # of digits in a field
  bool wild = false;       // have found a wildcard, must find a separator

  // Loop until we terminate due to error.
  while ( is.good() && ( c = is.get() ) != EOF ) {

    bool term = false;     // Set when we find a terminator

    if ( isxdigit(c) && !wild )  { 
      // Shift and add in the new digit.
      // Digits are not valid after a '*'.
      int ic;
      if      ( c >= '0' && c <= '9' )  ic = c - '0';
      else if ( c >= 'A' && c <= 'F' )  ic = c - 'A' + 10;
      else if ( c >= 'a' && c <= 'f' )  ic = c - 'a' + 10;
      else { ic = 0; assert( isxdigit(c) && 0 ); }
      ( in[cur] *= 16 ) += ic;
      found++;
      overflow = ( found > Max[cur] );
    }
    else if ( overflow ) {
      // No non-digit characters are valid once a field has overflowed,
      // but we do consume all available consecutive digits before
      // stopping input.
      term = true;
    }
    else {
      switch ( cur ) {
      case Plat:  //==========================================================
	// This is the case when we start reading.  We allow either a hex
	// number or a '*' for this field.  It may not be omitted.    
	if ( c == '*' && found == 0 ) {
	  // Wildcard is valid only when no digits have been found,
	  // and blocks further reading of digits in the current field.
	  in[cur] = 0;
	  wild = true;
	}
	else if ( c == ':' && ( wild || found > 0 ) ) {
	  // ':' tells us that we are done reading the platform number.
	  // We can continue only if we didn't already read too many digits
	  // and trip the "overflow" test above.
	  cur = Part;  // now look for partition
	  found = 0;
	  wild = false;
	}
	else { term = true; }    // invalid character
	break;

      case Part:  //==========================================================
	// We've been open to reading digits into the partition number.
	if ( c == '*' && found == 0 ) {
	  // Wildcard is valid only when no digits have been found,
	  // and blocks further reading of digits in the current field.
	  in[cur] = 0;
	  wild = true;
	}
	else { term = true; }
	break;

      default:  //============================================================
	assert( /* internal state error */ 0 );
	term = true;
	break;
      }

    }

    if ( term ) {
      // Push back the last character read; we found a terminator.
      is.putback(c);
      break;  // done looping
    }
  }  // end while

  // We can get here in four ways: 
  //   o The stream goes !good()
  //   o We get an end-of-file
  //   o We get a terminator and break out of the loop.
  //
  // Only if we got to reading the partition number (cur == Part) and we
  // found either a wildcard (wild == true) or a number that's not too
  // wide (found > 0 && !overflow) did we get a valid EidPartitionDoublet.
  // If so, copy it into the target variable, otherwise declare an error 
  // on the stream.

  if ( cur == Part && ( wild || ( found > 0 && !overflow ) ) ) {
    doublet = EidPartitionDoublet( in[Plat], in[Part] );
  }
  else {
    // We didn't complete a valid EidPartitionDoublet.
    // Set stream error flag so that user can check for this
    is.setstate( ios::failbit );

    // Do not touch the input variable.
  }

  // The "input suffix operator" will be called here when the 'isentry'
  // object is destructed.  In the ipfx/isfx model, we have to do it by hand.

#if !(defined(__GNUC__) && (__GNUC__ < 3))                          // BABAR_IOSTREAMS_MIGRATION
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  is.isfx();  // Call the "input suffix operator".
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

  return is;
}

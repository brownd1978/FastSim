//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: EidEventTriplet.cc 622 2010-09-13 09:48:16Z stroili $
//
// Description:
//    See .hh for details.
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//    Gregory Dubois-Felsmann         California Institute of Technology
//
// History:
//    Ed Frank                 01 Jun 99  Creation of first version
//    Gregory Dubois-Felsmann  99.10.31   Added I/O operators
//               
// Bugs:
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "EidData/EidEventTriplet.hh"

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

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BaBar/BaBarODMGTypes.h"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

using std::ends;
using std::ios;
using std::istream;
using std::ostream;
using std::ostringstream;
using std::setfill;
using std::setw;


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

EidEventTriplet::EidEventTriplet( d_ULong platform,
				  d_ULong partitionMask,
				  odfTime timeStamp )
  : EidPartitionDoublet( platform, partitionMask )
  , _timeStamp( timeStamp )
{}


EidEventTriplet::EidEventTriplet()
  : EidPartitionDoublet()              // wildcard
  , _timeStamp( odfTime(0,0) )
{ }


EidEventTriplet::EidEventTriplet( const EidEventTriplet& rhs ) 
  : EidPartitionDoublet( rhs )
  , _timeStamp( rhs.timeStamp() )
{ }


EidEventTriplet&
EidEventTriplet::operator =( const EidEventTriplet& rhs ) {
  if ( this != &rhs ) {
    EidPartitionDoublet::operator=( rhs );
    _timeStamp     = rhs.timeStamp();
  }

  return *this;
}


EidEventTriplet::~EidEventTriplet()
{ }


bool
EidEventTriplet::operator==( const EidEventTriplet& other ) const
{
  // Implement equality test, including symmetric wildcarding from
  // EidPartitionDoublet's operator== for platform and partition.
  // Test the time stamp first, as it's the most likely to be different.

  return ( _timeStamp == other.timeStamp()
	   && EidPartitionDoublet::operator==( other ) );
}


std::ostream&
operator<<( std::ostream& o, const EidEventTriplet& eid ) 
{
  // Write to a temporary stringstream first.  Keep the memory local for
  // thread safety.
  // Reserve more than the maximum number of bytes one might reasonably
  // need (2+1+8+1+8+1+8+1+1+nullTerm).  
//  char buf[64];
//  ostrstream s( buf, 64 );
  ostringstream s;

  // We marshal output like this so that we can properly align the whole
  // resulting string in the output field width currently set on the
  // stream "o".  If we wrote directly to o the field width would be 
  // applied only to the first insertion, the platform number.

  // Write the output, replacing wildcards with '*'s.
  
  s << static_cast<const EidPartitionDoublet&>( eid ) 
    << ':' << setw(15) << setfill( '0' ) << eid.timeStamp() 
    << ':' << eid.computeChecksum() << ends;  

  // Copy temporary to output, respecting the field width in place.
  o << s.str();

  return o;
}



std::istream&
operator>>( std::istream& is, EidEventTriplet& eid ) 
{
  // Possible lexical states of the scanner:
  enum State 
       { Indeterminate, Plat, Part, Upper, Lower, Checksum, Done };

  // Scratch space for numerical components (found in the first five states),
  // with extra room left for safety's sake for the other "Checksum" states,
  // in which, however, these arrays should never be used.
  d_ULong in[7] = { 0, 0, 0, 0, 0, 0, 0 };

  // Maximum number of hex digits allowed in each field:
  static const int Max[7] = { 8, 2, 8, 8, 8, 0, 0 };  


  // We start in a state where the first hex digits read could belong
  // either to the platform number or to the odfTime.  We won't know
  // which until we see the first non-digit.
  State cur = Indeterminate;

  
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

  // Loop until we enter the Done state or we terminate due to error.
  while ( cur != Done && is.good() && ( c = is.get() ) != EOF ) {

    bool term = false;     // Set when we find a terminator

    if ( isxdigit(c) && !wild && cur != Checksum )  { 
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
      case Indeterminate:  //=================================================
	// This is the case when we start reading.  It's resolved when
	// we read the first non-digit.  If it is a '*' or a ':', then
	// the first field must be Platform.  If it is a '/', then it
	// must be the Upper part of the odfTime.
	if ( c == '*' && found == 0 ) {
	  // Wildcard is valid only when no digits have been found,
	  // and blocks further reading of digits in the current field.
	  cur = Plat;
	  in[cur] = 0;
	  wild = true;
	}
	else if ( c == ':' && found > 0 && found <= Max[Plat] ) {
	  // ':' tells us that we were reading the platform number.
	  // We can continue only if we didn't already read too many digits.
	  in[Plat] = in[cur];
	  cur = Part;  // now look for partition
	  found = 0;
	  wild = false;
	}
	else if ( c == '/' && found > 0 && found <= Max[Upper] ) {
	  // '/' tells us that we were reading the upper part of the
	  // odfTime and that we wildcarded the platform and partition.
	  in[Plat] = 0;
	  in[Part] = 0;
	  in[Upper] = in[cur];
	  cur = Lower;
	  found = 0;
	  wild = false;
	}
	else { term = true; }
	break;

      case Plat:  //==========================================================
	// This is the case when we know we are reading a platform
	// number.  This can only arise if the platform number was
	// wildcarded as '*'; otherwise, a ':' or '/' will have advanced
	// us straight from Indeterminate to Part or Lower.
	if ( c == ':' && wild ) {
	  cur = Part;  // now look for partition
	  found = 0;
	  wild = false;
	}
	else { term = true; }
	break;

      case Part:  //==========================================================
	// We've been open to reading digits into the partition number.
	if ( c == '*' && found == 0 ) {
	  // Wildcard is valid only when no digits have been found,
	  // and blocks further reading of digits in the current field.
	  in[cur] = 0;
	  wild = true;
	}
	else if ( c == ':' ) {
	  // Found the end of the field.
	  if ( found == 0 && !wild ) {
	    // If nothing at all has been read in this field yet, the
	    // field may be elided and is treated as a wildcard.  This is
	    // true for the partition field only.
	    in[cur] = 0;
	  }
	  cur = Upper;  // now look for upper part of time
	  found = 0;
	  wild = false;
	}
	else { term = true; }
	break;

      case Upper:  //=========================================================
	// We've been trying to read the upper part of the odfTime.
	if ( c == '/' && found > 0 ) {
	  // The only legitimate next step is to the lower part, after a
	  // slash.  At least one digit must have been supplied for the upper.
	  cur = Lower;
	  found = 0;
	  wild = false;
	}
	else { term = true; }
	break;

      case Lower:  //=========================================================
	// We've been trying to read the lower part of the odfTime.
	if ( c == ':' && found > 0 ) {
	  // The only legitimate next step is to the checksum, after a
	  // colon.  At least one digit must have been supplied for the upper.
	  cur = Checksum;
	  found = 0;
	  wild = false;
	}
	// Anything else aborts the input.
	else { term = true; }
	break;

      case Checksum:  //======================================================
	// We are trying to read the checksum character.  We absorb exactly
	// one character, test it, and then exit the read loop.
	{  // make a context for the local variable:
	  EidEventTriplet temp( in[Plat], in[Part], 
				odfTime( in[Upper], in[Lower] ) );
	  if ( toupper(c) == temp.computeChecksum() ) {
	    // We found a good EET.  Copy it to the input variable.
	    eid = temp;
	    cur = Done;
	  }
	  // If the character doesn't match what we need, we exit,
	  // backing up to leave the stream pointing to it.  This is
	  // true regardless of whether the character was in the valid
	  // set of checksum characters or not.
	  else { term = true; }
	}
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
  //   o We found a valid EET -- cur == Done
  //   o The stream goes !good()
  //   o We get an end-of-file
  //   o We get a terminator and break out of the loop.
  //
  // Only if cur == Done did we get a valid EidEventTriplet, and if so,
  // it's already been copied into the target variable, so all we have
  // left to do is declare an error on the stream if we found an invalid
  // triplet.

  if ( cur != Done ) {
    // We didn't complete a valid EidEventTriplet.
    // Set stream error flag so that user can check for this.
    is.setstate( ios::failbit );

    // Do not touch the input variable ("eid").
  }

  // The "input suffix operator" will be called here when the 'isentry'
  // object is destructed.  In the ipfx/isfx model, we have to do it by hand.

#if !(defined(__GNUC__) && (__GNUC__ < 3))                          // BABAR_IOSTREAMS_MIGRATION
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  is.isfx();  // Call the "input suffix operator".
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

  return is;
}



char
EidEventTriplet::computeChecksum() const
{
  // This is a very primitive checksum algorithm, but probably good
  // enough for catching typos.

  // NB: It _must_ match the COMIS code used to print EETs.  There is
  // no good way to enforce this.  Perhaps a regression test could be
  // written to do a build-time test?

  // The algorithm is:
  //     add up the four integer components of the EET (platform,
  //     partition, upper odfTime, lower odfTime) as _signed_ integers
  //     (why signed? no unsigned arithmetic in COMIS - sigh);
  //     take the absolute value; take mod 17 of that; pick the
  //     corresponding character from the seventeen character list
  //     "GHJKLMNPQRSTUVWXY".  (Why those?  Hex digits and I and O were
  //     eliminated.)

  static const char CheckChar[] = "GHJKLMNPQRSTUVWXY";
  static const int CheckMod = strlen( CheckChar );
  assert( CheckMod == 17 );

  d_Long sum  = (d_Long) platform();
         sum += (d_Long) partitionMask();
  odfBinTime b( _timeStamp.binary() );
         sum += (d_Long) b.upper;
         sum += (d_Long) b.lower;

  sum = ( sum > 0 ? sum : -sum );

  return CheckChar[ sum % CheckMod ];
}

//--------------------------------------------------------------------------
//
// odfTime I/O function implementations
//
//      Not in odfTime.cc so that these are in a different object module
//      from the "core" functions and don't get pulled in in DataFlow
//      when they are not wanted.
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1999      California Institute of Technology
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//----------------------
// This class' header --
//----------------------
#include "OdfCommon/odfTime.hh"


//------------------
// C, C++ headers --
//------------------
#include <assert.h>
#include <ctype.h>
#include <cstdio>
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
#include <iostream>
#include <iomanip>
#else                                                               // BABAR_IOSTREAMS_MIGRATION
#include <iostream.h>
#include <iomanip.h>
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
using std::dec;
using std::hex;
using std::ios;
using std::istream;
using std::oct;
using std::ostream;
using std::setfill;
using std::setw;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION


//-----------------------------------
// Member function implementations --
//-----------------------------------

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
std::ostream&
odfTime::print( std::ostream& o ) const
#else                                                               // BABAR_IOSTREAMS_MIGRATION
ostream&
odfTime::print( ostream& o ) const
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
{
  // The main problem here is that, in order to define a truly standard
  // form for the output operator, we must create a well-defined state
  // for the output stream inside this function, without affecting
  // whatever state may have been established by the user.  This all
  // makes this a rather longer piece of code than one might imagine.

  // Save the old ios flags.
  long oldflags = o.flags();       // 'long' is the standard type for this

  // Clear modes we don't want, that would change the format of the
  // output from the odfTime standard, and set the mode (hex) we do.
  long newflags = ( oldflags
		    & (~( ios::dec | ios::oct
			 | ios::showbase           // no '0x'
			 | ios::uppercase          // use a-f, not A-F
			 | ios::showpos )          // no plus sign
		    | ios::hex) );

  // Set the new flags.
  o.flags( newflags );

  // Save the old fill character.  We will change in midway through output.
  int oldfill = o.fill();          // here, 'int' is the standard type

  // Whatever field width is in force will apply to the _entire_ odfTime.
  // Since this actually will play itself out in the low-level insertions
  // to the stream, we have to juggle this some.  Get the width:
  int theWidth = o.width();

  // The fixed-length part of the odfTime ('/' + lower) will take up nine
  // characters.  The remainder is the width we use for upper.
  theWidth -= 9;

  // Apply any positive remaining width, otherwise reset.
  o.width( ( theWidth > 0 ? theWidth : 0 ) );

  // Write the output.  The 'lower' field is padded to eight digits.
  o << btime.upper << '/' << setw(8) << setfill( '0' ) << btime.lower;

  // Reset the flags and the fill character.
  // Width specifiers are not sticky and do not need to be reset.
  o.flags( oldflags );
  o.fill( oldfill );

  return o;
}



#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
std::istream&
odfTime::read( std::istream& is )
#else                                                               // BABAR_IOSTREAMS_MIGRATION
istream&
odfTime::read( istream& is )
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
{
  // Scratch space for numerical components:
  enum Comp { Upper, Lower };
  static const int Max[2] = { 8, 8 };   // max. number of hex digits
  d_ULong in[2] = { 0, 0 };

  // Starting point: look for the upper half:
  Comp cur = Upper;

  // Create a "sentry" object to perform the input prefix and suffix
  // operations via resource-allocation-is-initialization.  When the
  // sentry goes out of scope the suffix is performed.  If the initialization
  // fails, we do nothing.
  //
  // The prefix skips leading whitespace if the stream is so configured
  // (with "skipws").
  //
  // For gcc 2.x (2.95.3 in particular), we retain the old ipfx/isfx way of            FIXME
  // doing things.  NB: this is not the same ifdef condition as above!!

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
  int overflow = 0;        // have exceeded the max # of digits in a field

  while ( is.good() && ( c = is.get() ) != EOF ) {
    int term = 0;    ;     // found a terminator
    if ( isxdigit(c) )  {
      // Shift and add in the new digit.
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
      // No other characters are valid once a field has overflowed,
      // but we do consume all available consecutive digits before
      // stopping input.
      term = 1;
    }
    else {
      switch ( cur ) {

      case Upper:  //=========================================================
	// We've been trying to read the upper part of the odfTime.
	if ( c == '/' && found > 0 ) {
	  // The only legitimate next step is to the lower part, after a
	  // slash.  At least one digit must have been supplied for the upper.
	  cur = Lower;
	  found = 0;
	}
	else { term = 1; }
	break;

      case Lower:  //=========================================================
	// We've been trying to read the lower part of the odfTime.
	// Any non-digit character ends the input.
	term = 1;
	// We'll check outside the loop for whether we got any digits.
	break;

      default:  //============================================================
	assert( /* internal state error */ 0 );
	term = 1;
	break;
      }

    }

    if ( term ) {
      // Push back the last character read; we found a terminator.
      is.putback(c);
      break;  // done looping
    }
  }  // end while

  // We can get here in two ways: the stream goes !good() (e.g., EOF)
  // or a terminator character is found, causing a loop break.

  // If we have gotten to reading the lower time and found a digit for
  // it, but not too many, then we got a valid odfTime.
  if ( cur == Lower && found > 0 && !overflow ) {
    btime.upper = in[Upper];
    btime.lower = in[Lower];
  }
  else {
    // We didn't complete a valid odfTime.
    // Set stream error flag so that user can check for this
    is.clear( ios::failbit | is.rdstate() );

    // Do not touch the current odfTime object.
  }

  // If we are using the Standard iostreams "sentry" model, it will go
  // out of scope here.  In the ipfx/isfx model, we have to do it by hand.

#if !(defined(__GNUC__) && (__GNUC__ < 3))                          // BABAR_IOSTREAMS_MIGRATION
#else                                                               // BABAR_IOSTREAMS_MIGRATION
  is.isfx();  // Call the "input suffix operator".
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

  return is;
}



//-----------------------------------
// Global operator implementations --
//-----------------------------------

#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
std::ostream&
operator<<( std::ostream& os, const odfTime& t )
#else                                                               // BABAR_IOSTREAMS_MIGRATION
ostream&
operator<<( ostream& os, const odfTime& t )
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
{
  return t.print( os );
}


#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
std::istream&
operator>>( std::istream& is, odfTime& t )
#else                                                               // BABAR_IOSTREAMS_MIGRATION
istream&
operator>>( istream& is, odfTime& t )
#endif                                                              // BABAR_IOSTREAMS_MIGRATION
{
  return t.read( is );
}

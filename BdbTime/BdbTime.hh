#ifndef BDBTIME_HH
#define BDBTIME_HH

//-----------------------------------------------------------------------------
//
// File and Version Information:
//      $Id: BdbTime.hh 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//      Class BdbTime.  
//      This is a persistent time class.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      J. Ohnemus                      Original Author
//      Gregory Dubois-Felsmann         Rogue Wave migration, 2002/2003
//
// Copyright Information:
//      Copyright (C) 1994, 1995        Lawrence Berkeley Laboratory
//      Copyright (c) 2002, 2003        California Institute of Technology
//
//-----------------------------------------------------------------------------

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBarODMGTypes.h"

//-----------------
// C/C++ Headers --
//-----------------
#include <time.h>
#include <iostream>
#include <string>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BdbTime/BdbDuration.hh"
#include "BdbTime/BdbTimeConst.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//              ---------------------
//              -- Class Interface --
//              ---------------------

class BdbTime {

public:

  enum Zone { Local, UTC };

  // Constructors

  /**
   *  (Deprecated)
   *  Constructs with the current time, to the nearest second.  This
   *  constructor is deprecated and may be removed in a future release.
   *  The static function BdbTime::now() should be used in preference
   *  to this in all new code, if the current time is really required.
   *
   *  For some reason, the original implementation did not set the _gmtNsec
   *  member, perhaps because it was not easy to get it synchronized with
   *  the Rogue Wave "now()" function's return value.  Now that we use
   *  clock_gettime() directly, this could be fixed.
   *
   *  However, we have decided not to do this at this time, in order not
   *  to change the behavior of existing programs.
   *
   *  BdbTime::now() preserves the full significance available from
   *  clock_gettime().
   *
   *  Please note that this constructor involves a system call and is
   *  expensive!  Do not default-construct BdbTimes unless you really need
   *  the current time value.  If you are just declaring a time variable
   *  to fill in later, BdbTime(0) is a more performant choice.
   */
  BdbTime( );

  /** Copy constructor. */
  BdbTime( const BdbTime& t );

  /** 
   *  Constructs a time from an unsigned number of seconds since the
   *  BdbTime epoch of 1901.  NB: this is not the Unix epoch of 1970!
   */
  explicit BdbTime( d_ULong sec_since_1901, d_ULong nsec = 0 );

  /**
   *  Constructs a time from a broken-down list of components of a date
   *  and time.
   *
   *  This uses definitions inherited from the old RogueWave implementation
   *  of BdbTime.  Thus "year" is the calendar year C.E. (e.g., 2003), and
   *  "month" is the month in the range {1..12}.  Note that these differ 
   *  from the POSIX broken-down time (struct tm) definitions, where
   *  2003 C.E. is represented as 103, and the month is in the range {0..11}.
   */
  BdbTime( d_ULong year,
	   d_ULong month,
	   d_ULong day,
	   d_ULong hour,
	   d_ULong minute,
	   d_ULong second,
	   d_ULong nanosecond = 0,
	   Zone zone = UTC );

  /**
   *  (Deprecated - use parseTime())
   *  Constructs a BdbTime from a string date and a string time.
   *
   *  The use of this constructor is strongly discouraged, as it does not
   *  have a means of reliably reporting parsing errors (BaBar does not
   *  use C++ exceptions).  Use BdbTime::parseTime() instead (which this
   *  is implemented over).
   */
  BdbTime( const std::string& date, const std::string& time, Zone zone = UTC );

  /**
   *  (Deprecated - use parseTime())
   *  Constructs a BdbTime from a single date-time string, in which the
   *  date and time must be given separated by whitespace.  Otherwise
   *  identical to the above constructor.
   *
   *  The use of this constructor is strongly discouraged, as it does not
   *  have a means of reliably reporting parsing errors (BaBar does not
   *  use C++ exceptions).  Use BdbTime::parseTime() instead (which this
   *  is implemented over).
   */
  explicit BdbTime( const std::string&, Zone zone = UTC );

  /** 
   *  Constructs from POSIX high-resolution time, as might be
   *  obtained from clock_gettime.
   */
  BdbTime( const struct timespec& ts );

  /**
   *  Constructs from POSIX "broken-down time".
   *  @param stm Cannot be const because it is internally provided to
   *             POSIX mktime(), which normalizes it -- see man mktime.
   */
  BdbTime( struct tm& stm, Zone zone = UTC );

  /**
   *  The destructor is non-virtual in order to keep this representational
   *  class small and suitable for processing with value semantics.
   *  Classes with non-empty destructors should not be constructed with
   *  non-private inheritance from BdbTime.
   */
  ~BdbTime( ) { }


  // Assignment operator
  BdbTime&    operator=( const BdbTime& t );
    
  // Calculational operators
  
  /**
   *   Calculates the absolute value of the difference between two times.
   *   NB: BdbDuration is an inherently unsigned quantity!  This is
   *   in essence because reasonably foreseeable time differences are
   *   larger in seconds than 2^31 and so can't be represented as a
   *   signed 32 bit integer.
   */
  BdbDuration operator-(  const BdbTime& t ) const;               

  BdbTime&    operator+=( const BdbDuration& d );
  BdbTime     operator+(  const BdbDuration& d ) const;

  BdbTime&    operator-=( const BdbDuration& d );
  BdbTime     operator-(  const BdbDuration& d ) const;

  // Comparison operators
  bool operator==( const BdbTime& t ) const
    { 
      return ( _gmtSec  == t._gmtSec && _gmtNsec == t._gmtNsec );
    }

  bool operator!=( const BdbTime& t ) const
    { 
      return !( *this == t ); 
    }

  bool operator<(  const BdbTime& t ) const
    { 
      return ( _gmtSec  < t._gmtSec ) ||	
	     ( _gmtSec == t._gmtSec   && _gmtNsec < t._gmtNsec );
    }

  bool operator<=( const BdbTime& t ) const
    { 
      return ( _gmtSec  < t._gmtSec )  ||
	     ( _gmtSec == t._gmtSec    && _gmtNsec <= t._gmtNsec );
    }
    
  bool operator>(  const BdbTime& t ) const
    { 
      return !( *this <= t ); 
    }

  bool operator>=( const BdbTime& t ) const
    { 
      return !( *this <  t ); 
    }
  
  // Selectors
  d_ULong getGmtSec( )  const { return _gmtSec;  }
  d_ULong getGmtNsec( ) const { return _gmtNsec; }

  /**
   *  Extracts the value of the BdbTime as a POSIX.1b "struct timespec".
   *  Returns 0 on success in analogy with POSIX.1b clock_gettime().
   *  WARNING: Must and will fail for valid BdbTime values that are more
   *  than 2^31-1 seconds before the beginning of the POSIX 1970 epoch,
   *  i.e., before around 1901.12.13 20:45:53 UTC.
   *
   *  There are such times in the conditions database from early 
   *  SP production, before all times were renormalized into 1997+ space.
   */
  int timeSpec( struct timespec* ts ) const;

  /**
   *  Extracts the value of the BdbTime as a POSIX "struct tm" broken-down
   *  time.  This requires the use of a time zone.  In analogy with the
   *  POSIX.1b gmtime_r() function, a pointer to a "struct tm" to receive
   *  the data must be supplied -- so this function is thread-safe(*).
   *  Following this analogy, the function returns the supplied pointer
   *  on success, and 0 on failure.
   *
   *  This function should work for all times in the BdbTime range.
   */
  struct tm* tm( struct tm* stm, Zone zone ) const;

  /**
   *  Creates a string from the value of the BdbTime, based on a 
   *  specified format specification, as for POSIX strftime(), and on
   *  a time zone.  Note that this function does not provide a way 
   *  to embed the "nanoseconds" part of the BdbTime, since this is
   *  not possible to express in a form recognized by strftime.
   *
   *  The "%N" format specified does not appear to be used in the POSIX
   *  definition of strftime.  It's possible it could be hijacked in a
   *  future upgrade.                                                   FIXME
   *
   *  This function should work for all times in the BdbTime range.
   */
  std::string asString( const char* fmt, Zone zone ) const;

  // Friends
  friend BdbTime  operator+( const BdbDuration& d, const BdbTime& t );
  friend std::ostream& operator<<( std::ostream& os, const BdbTime& t );

private:

  void renormalizeNanoseconds( );

  // Data members
  d_ULong _gmtSec;   // number of seconds since 00:00:00 Jan. 1, 1901 UTC
  d_ULong _gmtNsec;  // number of nanoseconds

public:
  // static interfaces:

  /**
   *  Constructs and returns a BdbTime representing the current time.
   *  This interface, unlike the deprecated default constructor, returns
   *  a BdbTime set to the full resolution available from clock_gettime().
   *  Note that this may vary between platforms and even operating
   *  system versions.
   */
  static BdbTime now();

  /**
   *  Determines whether a string representing a date and a string
   *  representing a time can be converted successfully to a date/time
   *  in BdbTime format, i.e., in the unsigned 1901 epoch.
   *
   *  Note that 1901.01.01 00:00:00 UTC is not a valid time; 00:00:01
   *  is the first valid time.
   *
   *  Date is parsed using strptime formats "%D" and "%d%b%Y", controlled
   *  by BdbTimeInput, with the first one that succeeds taking precedence.
   *  Time is parsed with formats "%T" and "%R".  These are quite a
   *  restricted set compared to those originally accepted by the
   *  Rogue Wave implementation first used here.
   *
   *  By default times are interpreted as UTC (a logical alternative might 
   *  be the local time zone.  If an invalid date or time string is supplied,
   *  the BdbTime time will be set to -Infinity.  If the date string is set to 
   *  "+Infinity" or "-Infinity", the time string will be ignored and the
   *  BdbTime will be set to the corresponding value.
   *
   *  @param time Returned time value; modified only if parsing is successful.
   *  @return Flag indicating whether parsing was successful.
   */
  static bool parseTime( const std::string& sdate, const std::string& stime, 
			 Zone zone,
			 BdbTime& time );

  /**
   *  Determines whether a string representing a date and time can be
   *  converted successfully to a date/time in BdbTime format, i.e., in
   *  the unsigned 1901 epoch.
   *
   *  Equivalent to a call to parseTime( const std::string& sdate, 
   *    const std::string& stime, Zone zone, BdbTime& time )
   *  with the date and time set from the first and second whitespace-
   *  delimited tokens in the sdatetime string and subsequent text ignored.
   *
   *  This is a less precise parsing method than the above.
   *
   *  @param time Returned time value; modified only if parsing is successful.
   *  @return Flag indicating whether parsing was successful.
   */
  static bool parseTime( const std::string& sdatetime, 
			 Zone zone,
			 BdbTime& time );

  // Static members
  static const BdbTime minusInfinity;
  static const BdbTime plusInfinity;

};


inline void
BdbTime::renormalizeNanoseconds( )
{
  if ( _gmtNsec >= BdbTimeConst::nsecInASec ) {
    // carry nanoseconds over into seconds
    d_ULong extraSec   = _gmtNsec / BdbTimeConst::nsecInASec;
    d_ULong remainNsec = _gmtNsec % BdbTimeConst::nsecInASec;
    _gmtSec            += extraSec;
    _gmtNsec           =  remainNsec;
  } 
}

#endif

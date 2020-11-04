//-----------------------------------------------------------------------------
//
// File and Version Information:
//      $Id: BdbTime.cc 496 2010-01-13 17:10:44Z stroili $
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
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BdbTime/BdbTime.hh"

//-----------------
// C/C++ Headers --
//-----------------
#include <assert.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <iomanip>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BdbTime/BdbTimeInput.hh"
#include "BbrStdUtils/Tokenize.hh"
using std::ostream;

//---------------------
// static declarations
//---------------------
const BdbTime BdbTime::minusInfinity = BdbTime( BdbTimeConst::minusInfinity );
const BdbTime BdbTime::plusInfinity  = BdbTime( BdbTimeConst::plusInfinity  );

//------------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions, and Forward Declarations --
//------------------------------------------------------------------------

// This function is intended to handle the renormalization of the POSIX
// 1970 epoch to the BdbTime (formerly Rogue Wave) epoch of 1901.  Since the
// conversion constant is an unsigned quantity too large to be represented
// in a signed 32-bit number like an ILP32 time_t, this should be done
// carefully to avoid overflows, sign extensions, etc.
//
// Note that we don't attempt to deal with the problem that there are
// some 11.5 months' worth of 32-bit POSIX time_t's in 2037-2038 that
// would require more than 32 bits to represent in the 1901 epoch.

static inline d_ULong POSIXto1901( time_t t )
{
  return ( t >= 0 
	   // After 1970, no problems.
	   ? ( BdbTimeConst::seconds_1901_to_1970 + d_ULong(t) )
	   // Before 1970.  Flip sign first.
	   : ( BdbTimeConst::seconds_1901_to_1970 - d_ULong(-t) ) );
}

// Need the number of seconds from 1901.01.01 to 1907.01.01.  This 
// includes one leap year.
static const d_ULong fix1901_offset = ( 6*365 + 1 ) * 24 * 60 * 60;

// This function, used in several places, takes us from a POSIX struct
// tm to a 1901-epoch time.  The return value tells us whether there
// was a problem with the conversion.  t1901 is not modified if there is
// a problem.
//
// It has a really nasty problem to deal with.  POSIX mktime() is considered
// "non-portable" for times before 1970.  It _can_ work for such times if
// time_t is a 32-bit signed integer, as it is on Linux and Solaris.  But
// it can under no circumstances work for times earlier than 2^31 seconds
// before the POSIX epoch -- and that time happens to be in December 1901.
// Hence, there is an 11+ month period at the beginning of the 1901 epoch
// that mktime() can't handle.
// So, we have to kludge this.  Please don't watch.
static bool TMto1901( struct tm& stm, BdbTime::Zone zone, d_ULong& t1901 )
{
  const bool fix1901 = ( stm.tm_year == 1 ); // i.e., 1900+1 == 1901
  if ( fix1901 ) {
    stm.tm_year = 7;       // 1907 has the same weekday pattern as 1901
                           // and is also not a leap year.
  }

  // Build a POSIX time_t from the broken-down time.  mktime() uses the
  // current time zone, so we have to take that into account.
  //
  // mktime() can modify its struct tm argument, so it can't be const.
  //
  // POSIX specifies that mktime() should return (time_t)-1 if the struct tm
  // cannot be represented as a time_t, with errno set to EOVERFLOW.  If the
  // time conversion is successful, mktime() is required not to change errno.
  // Because -1 is also a legal value for the "extended" time range
  // supported on platforms (all of ours) where time_t is signed, we test only
  // on errno.
  errno = 0;
  time_t t = mktime( &stm );
  // This assertion is here to catch problems during development.  This
  // function should not assert() in production and this line should be
  // removed.                                                          FIXME
  assert( errno == 0 );
  if ( errno != 0 ) {
    return false;
  }

  // The Mac OSX doesn't have the timezone variable needed below,  so
  // we'll need to use its non-standard timegm() method to work this out.
  // We do this now to benefit from the fix1901
#ifdef __APPLE_CC__
    d_Long timezone=0;
    errno = 0;
    time_t otherT = timegm( &stm );
    if ( errno != 0 ) return false;
    timezone = t - otherT;
#endif


  // Offset the time to the 1901 epoch.  Do this carefully to avoid 
  // overflows and sign-extension headaches.
  d_ULong ut = POSIXto1901( t );

  // Now, if we relocated the time to 1907 above, in order to get around
  // the limitations of POSIX time_t, we have to put it back:
  if ( fix1901 ) {
    assert( ut >= fix1901_offset );
    ut -= fix1901_offset;
    stm.tm_year = 1;
  }

  // If the user actually wanted to work in UTC, we need to correct the
  // resulting time for the timezone offset.
  if ( BdbTime::UTC == zone ) {
    // mktime() is defined by POSIX to have called tzset(), so we can
    // use the external variable 'timezone'.

    // Subtract the number of seconds between UTC and the current timezone.
    // Justification for the sign: POSIX defines 'timezone' as positive for
    // locations west of the meridian.  A given broken-down time, interpreted
    // as local time west of the meridian, is a later absolute time than that
    // broken-down time interpreted as UTC.  Thus, to get from mktime()'s
    // local time interpretation to a UTC interpretation, we must subtract
    // the value of 'timezone'.
    ut -= timezone;

    // Correct for daylight savings time.  POSIX appears to assume that
    // the DST offset is always exactly one hour, so that's all we can do.
    // Justification for the sign: a time, say, "noon" interpreted as
    // DST produces the same time_t value as "11am" non-DST, 60*60 seconds
    // less than the value produced for "noon" non-DST -- so we have to add
    // that number back.

    // We have to decide on DST/non-DST _for the time in question_, not
    // for "now", so we need to use the decision that mktime() made.  Make
    // sure it made it, and changed the negative value of tm_isdst that we
    // originally set.
    // This assertion merely validates a POSIX-guaranteed invariant, so it
    // can stay in place in production.
    assert( 0 <= stm.tm_isdst );
    if ( 0 < stm.tm_isdst )  ut += ( 60*60 );
    else if ( 0 > stm.tm_isdst ) {
      // If assert() has been compiled away, make the time value invalid.
      return false;
    }
  }

  t1901 = ut;
  return true;
}


//              ----------------------------------------
//              -- Public Function Member Definitions --
//              ----------------------------------------

//----------------
// Constructors --
//----------------

BdbTime::BdbTime( )                         // default: construct present time
  : _gmtSec(0), _gmtNsec(0)
{
  struct timespec ts;
  int gettimeStatus = clock_gettime( CLOCK_REALTIME, &ts );
  // This should never fail on a normal general-purpose Unix computer.
  assert( 0 == gettimeStatus );
  if ( 0 == gettimeStatus ) {
    _gmtSec  = POSIXto1901( ts.tv_sec );
    // NB: The original Rogue Wave implementation of BdbTime() failed to
    // set the _gmtNsec to reflect the current time.  This was a bug, but
    // in order not to change the behavior of existing programs, this
    // behavior is being retained here.  The following line is therefore
    // commented out.  This could be reconsidered.                    // FIXME
    // _gmtNsec = ts.tv_nsec;
  }
}

BdbTime::BdbTime( const BdbTime& t )
  : _gmtSec(t._gmtSec), _gmtNsec(t._gmtNsec)
{
}

BdbTime::BdbTime( d_ULong sec, d_ULong nsec  )
  : _gmtSec(sec), _gmtNsec(nsec)
{
  renormalizeNanoseconds();
}

BdbTime::BdbTime( d_ULong year,
		  d_ULong month,
		  d_ULong day,
		  d_ULong hour,
		  d_ULong minute,
		  d_ULong second,
		  d_ULong nanosecond,
		  BdbTime::Zone zone )
  : _gmtSec(0), _gmtNsec(nanosecond)
{
  // Construct a POSIX broken-down time from the components supplied.
  // The definitions of the constructor arguments' meaning are inherited
  // from Rogue Wave's specifications, so they need to be renormalized
  // for POSIX.
  struct tm stm;
  stm.tm_year  = year - 1900;
  stm.tm_mon   = month - 1;   // Rogue Wave/BdbTime: [1,12], POSIX: [0,11]
  stm.tm_mday  = day; 
  stm.tm_hour  = hour;
  stm.tm_min   = minute;
  stm.tm_sec   = second;

  stm.tm_isdst = -1;          // Let mktime figure out whether DST is in force

  d_ULong ut = 0;
  bool ok = TMto1901( stm, zone, ut );

  if ( ok ) {
    _gmtSec = ut;
    renormalizeNanoseconds();
  }
  else {
    // Invalidate the time -- (0,0) is a reserved value.
    _gmtNsec = 0;
  }
}


BdbTime::BdbTime( const std::string& sdate, const std::string& stime,
		  Zone zone ) :
  _gmtSec( 0 ), _gmtNsec( 0 ) 
{
  // This is ugly because no error checking can be performed.

  BdbTime t( 0, 0 );

  bool status = parseTime( sdate, stime, zone, t );

  if ( status ) {
    _gmtSec  = t.getGmtSec();
    _gmtNsec = t.getGmtNsec();
  }
  // else leave it at -Infinity = (0,0).
}


BdbTime::BdbTime( const std::string& sdatetime, Zone bzone ) :
  _gmtSec( 0 ), _gmtNsec( 0 ) 
{
  // This is ugly because no error checking can be performed.

  BdbTime t( 0, 0 );

  bool status = parseTime( sdatetime, bzone, t );

  if ( status ) {
    _gmtSec  = t.getGmtSec();
    _gmtNsec = t.getGmtNsec();
  }
  // else leave it at -Infinity = (0,0).
}

BdbTime::BdbTime( struct tm& stm, Zone zone )
  : _gmtSec(0), _gmtNsec(0)
{
  d_ULong ut = 0;
  bool ok = TMto1901( stm, zone, ut );

  if ( ok ) {
    _gmtSec = ut;
  }
  // else time is already invalidated -- (0,0) is a reserved value.
}


BdbTime::BdbTime( const struct timespec& ts )
  // shift from POSIX 1970 epoch to BdbTime 1901 epoch
  : _gmtSec(POSIXto1901(ts.tv_sec)), _gmtNsec(ts.tv_nsec)
{
  renormalizeNanoseconds();
}

int
BdbTime::timeSpec( struct timespec* ts ) const 
{
  // See if the time is earlier than the minimum representable signed
  // 32-bit POSIX time: (1970.01.01 00:00:00 UTC) - (2^31 - 1) seconds
  const d_ULong maxSigned = 0x7fffffff;  // maximum signed number of seconds
  assert( BdbTimeConst::seconds_1901_to_1970 > maxSigned );
  const d_ULong minPOSIX = BdbTimeConst::seconds_1901_to_1970 - maxSigned;

  if ( _gmtSec < minPOSIX ) {
    return -1;
  }

  // Be absolutely sure about sign extension.  Both the _gmtSec and
  // BdbTimeConst::seconds_1901_to_1970 are usually going to be greater
  // than the maximum signed 32-bit integer.
  if ( _gmtSec < BdbTimeConst::seconds_1901_to_1970 ) {
    d_ULong diff = BdbTimeConst::seconds_1901_to_1970 - _gmtSec;
    ts->tv_sec = -( (time_t) diff );
  }
  else {
    ts->tv_sec = (time_t) ( _gmtSec - BdbTimeConst::seconds_1901_to_1970 );
  }
  ts->tv_nsec = _gmtNsec;

  return 0;
}


struct tm*
BdbTime::tm( struct tm* stm, Zone zone ) const
{
  if ( stm == 0 ) return 0;

  // See if the time is earlier than the minimum representable signed
  // 32-bit POSIX time: (1970.01.01 00:00:00 UTC) - (2^31 - 1) seconds
  const d_ULong maxSigned = 0x7fffffff;  // maximum signed number of seconds
  assert( BdbTimeConst::seconds_1901_to_1970 > maxSigned );
  const d_ULong minPOSIX = BdbTimeConst::seconds_1901_to_1970 - maxSigned;

  const bool fix1901 = ( _gmtSec < minPOSIX );

  d_ULong fixedSec = _gmtSec;
  // Shift to the similar year 1907 if the time is in the pre-signed-POSIX
  // range where we can't trust gmtime_r().
  if ( fix1901 ) fixedSec += fix1901_offset;

  // Shift to POSIX epoch without triggering signedness conversions:
  time_t posixt = ( fixedSec >= BdbTimeConst::seconds_1901_to_1970 
		    ? ( fixedSec - BdbTimeConst::seconds_1901_to_1970 )
		    : -(time_t)( BdbTimeConst::seconds_1901_to_1970 - fixedSec ) );

  struct tm* stmout = 0;
  if ( zone == UTC ) {
    stmout = gmtime_r( &posixt, stm );
  }
  else { // Local
    stmout = localtime_r( &posixt, stm );
  }

  // Post-condition for gm/localtime_r: either success or failure.
  assert( stmout == stm || stmout == 0 );

  if ( fix1901 && stmout != 0 ) {
    // Go back from 1907 to 1901.
    stmout->tm_year -= 6;
    // Validate that it really is 1901.  It is also possible that for
    // very small BdbTime values (shortly after midnight UTC on 1901.1.1),
    // the broken-down time might be late in the last day of 1900 if a
    // local time zone is being used.  So allow for that (NB: Dec = 11).
    assert( stmout->tm_year == 1 
	    || ( zone != UTC 
		 && stmout->tm_year == 0 
		 && stmout->tm_mon == 11 && stmout->tm_mday == 31 ) );  
  }

  return stmout;
}


std::string
BdbTime::asString( const char* fmt, Zone zone ) const
{
  // In order to use strftime(), we must have a "struct tm" containing
  // the time.
  struct tm stm;
  struct tm* pstm = tm( &stm, zone );

  if ( pstm != &stm ) {
    return std::string( "(time conversion failed)" );
  }

  // Reserve an output buffer for the conversion, and do it.
  // Note that the size argument includes room for the terminating
  // null byte, but the return value does not account for it.
  char buffer[1024];
  size_t nstr = strftime( buffer, sizeof(buffer), fmt, &stm );
  
  if ( nstr >= sizeof(buffer) || ( nstr == 0 && strlen(fmt) != 0 ) ) {
    return std::string( "(time conversion failed)" );
  }

  return std::string( buffer );
}

//-------------
// Operators --
//-------------

// t2 = t1
BdbTime& BdbTime::operator=( const BdbTime& t1 )
{
    if ( this == &t1 )  return *this;

    _gmtSec  = t1._gmtSec;
    _gmtNsec = t1._gmtNsec;

    return *this;
}

// d = |t2 - t1|
BdbDuration BdbTime::operator-( const BdbTime& t1 ) const
{
    // This code forms |t2-t1| without having to use signed intergers.

    d_ULong t2Sec;
    d_ULong t2Nsec;
    d_ULong t1Sec;
    d_ULong t1Nsec;

    if ( *this > t1 )
	{
	    t2Sec  = _gmtSec;
	    t2Nsec = _gmtNsec;
	    t1Sec  = t1._gmtSec;
	    t1Nsec = t1._gmtNsec;
	}
    else
	{
	    t2Sec  = t1._gmtSec;
	    t2Nsec = t1._gmtNsec;
	    t1Sec  = _gmtSec;
	    t1Nsec = _gmtNsec;
	}

    if ( t2Nsec < t1Nsec )
	{
	    // borrow a second from t2Sec
	    t2Nsec += BdbTimeConst::nsecInASec;
	    t2Sec--;
	}

    d_ULong sec  = t2Sec  - t1Sec;
    d_ULong nsec = t2Nsec - t1Nsec;

    BdbDuration diff( sec, nsec );

    return diff;
}

// t = t1 - d
BdbTime BdbTime::operator-( const BdbDuration& d ) const
{
  return BdbTime(*this) -= d;
}

// t -= d
BdbTime& BdbTime::operator-=( const BdbDuration& d )
{
  // if t1 - d < 0 then return t = 0
  if ( ( _gmtSec  < d.getSec() ) ||
       ( _gmtSec == d.getSec()   && _gmtNsec < d.getNsec() ) ) {
    _gmtSec  = 0;
    _gmtNsec = 0;
  }
  else {
    d_ULong tempSec  = _gmtSec;
    d_ULong tempNsec = _gmtNsec;
    
    if ( tempNsec < d.getNsec() ) {
      // if t1._gmtNsec < d._nsec borrow a second from t1._gmtSec
      tempNsec += BdbTimeConst::nsecInASec;
      tempSec--;
    }
    
    _gmtSec  = tempSec  - d.getSec();
    _gmtNsec = tempNsec - d.getNsec();
  }
  
  return *this;
}

// t = t1 + d
BdbTime BdbTime::operator+( const BdbDuration& d ) const
{
  return BdbTime(*this) += d;
}

// t += d
BdbTime& BdbTime::operator+=( const BdbDuration& d )
{
  d_ULong totalSec  = _gmtSec  + d.getSec();
  d_ULong totalNsec = _gmtNsec + d.getNsec();
  
  if ( totalNsec >= BdbTimeConst::nsecInASec ) {
    // carry nanoseconds over into seconds
    d_ULong extraSec   = totalNsec / BdbTimeConst::nsecInASec;
    d_ULong remainNsec = totalNsec % BdbTimeConst::nsecInASec;
    totalSec          += extraSec;
    totalNsec          = remainNsec;
  }
  
  _gmtSec  = totalSec;
  _gmtNsec = totalNsec;

  return *this;
}

//
//  Static functions
//

BdbTime
BdbTime::now( )  // construct present time
{
  struct timespec ts;
  int gettimeStatus = clock_gettime( CLOCK_REALTIME, &ts );
  // This should never fail on a normal general-purpose Unix computer.
  assert( 0 == gettimeStatus );
  if ( 0 != gettimeStatus ) {
    return BdbTime(0,0);    // NB: there is no "invalid" value
  }

  // Renormalize to BdbTime 1901 epoch.
  return BdbTime( POSIXto1901( ts.tv_sec ), ts.tv_nsec );
}


bool 
BdbTime::parseTime( const std::string& sdate, const std::string& stime, 
		    Zone zone,
		    BdbTime& time )
{
  bool status = false;

  if ( strcasecmp( sdate.c_str(), "-Infinity" ) == 0 ) {
    time = minusInfinity;
    status = true;
  }
  else if ( strcasecmp( sdate.c_str(), "+Infinity" ) == 0 ) {
    time = plusInfinity;
    status = true;
  }
  else {
    struct tm stm;
    memset( &stm, 0, sizeof(stm) );
    stm.tm_isdst = -1;                     // Flag DST state as unknown

    BdbTimeInput::Status s;
    s = BdbTimeInput::parseDate( sdate.c_str(), stm );
    if ( s == BdbTimeInput::Success ) {
      s = BdbTimeInput::parseTime( stime.c_str(), stm );
      if ( s == BdbTimeInput::Success ) {
	d_ULong ut = 0;
	bool ok = TMto1901( stm, zone, ut );
	if ( ok ) {
	  status = true;
	  time = BdbTime( ut, 0 );
	}
      }
    }
  }

  return status;
}

bool 
BdbTime::parseTime( const std::string& sdatetime,
		    Zone zone,
		    BdbTime& time )
{
  bool status = false;

  if ( strcasecmp( sdatetime.c_str(), "-Infinity" ) == 0 ) {
    time = minusInfinity;
    status = true;
  }
  else if ( strcasecmp( sdatetime.c_str(), "+Infinity" ) == 0 ) {
    time = plusInfinity;
    status = true;
  }
  else {
    babar::String::Tokenize tokens( sdatetime );

    std::string sdate = tokens();
    std::string stime = tokens();

    status = parseTime( sdate, stime, zone, time );
  }

  return status;
}



//--------------------
// Friend functions --
//--------------------

// t = d + t1
BdbTime operator+( const BdbDuration& d, const BdbTime& t1 )
{
    return t1 + d;
}

ostream & operator <<( ostream& os, const BdbTime& t ) 
{
  if ( t == BdbTime::minusInfinity )
    os << "-Infinity";
  else if ( t == BdbTime::plusInfinity )
    os << "+Infinity";
  else 
    {
      os << t.asString( "%c", BdbTime::Local ) << " (local time) " 
	 << t.getGmtNsec( ) << " ns";
    }
  return os;
}

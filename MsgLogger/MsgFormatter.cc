//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgFormatter.cc 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgFormatter
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Andy Salnikov
//
// Copyright Information:
//      Copyright (C) 2005 SLAC
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "MsgLogger/MsgFormatter.hh"

#include <algorithm>

//-------------
// C Headers --
//-------------
extern "C" {
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "MsgLogger/MsgLogRecord.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

namespace {

  // get current time and format it
  void formattedTime ( std::string fmt, std::ostream& out ) ;

}


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

// Constructor
MsgFormatter::MsgFormatter( const std::string& fmt, const std::string& timefmt )
  : _fmt(fmt)
  , _timefmt(timefmt)
  , _fmtMap()
{
  if ( _fmt.empty() ) {
    if ( const char* env = getenv ( "MSGLOGFMT" ) ) {
      _fmt = env ;
    } else {
      _fmt = "%(time) [%(logger)/%(level)] %(file):%(line) - %(message)" ;
    }
  }
  if ( _timefmt.empty() ) {
    if ( const char* env = getenv ( "MSGLOGTIMEFMT" ) ) {
      _timefmt = env ;
    } else {
      _timefmt = "%m/%d %H:%M:%S.%f" ;
    }
  }
}

// Destructor
MsgFormatter::~MsgFormatter()
{
}

// add level-specific format
void
MsgFormatter::addFormat ( MsgLogLevel level, const std::string& fmt )
{
  _fmtMap[level] = fmt ;
}

// format message to the output stream
void
MsgFormatter::format ( const MsgLogRecord& rec, std::ostream& out )
{
  FormatMap::const_iterator it = _fmtMap.find( rec.level() ) ;
  const std::string& fmt = it != _fmtMap.end() ? it->second : _fmt ;

  // read format and fill the stream
  for ( std::string::const_iterator i = fmt.begin() ; i != fmt.end() ; ++ i ) {

    if ( *i != '%' ) {
      out.put( *i ) ;
      continue ;
    }

    std::string::const_iterator j = i ;
    if ( ++j == fmt.end() ) {
      out.put( *i ) ;
      continue ;
    }

    // escaped percent
    if ( *j == '%' ) {
      out.put( '%' ) ;
      i = j ;
      continue ;
    }

    // should be opening paren after percent
    if ( *j != '(' ) {
      out.put( *i ) ;
      continue ;
    }

    // find closing paren
    j = std::find ( j, fmt.end(), ')' ) ;
    if ( j == fmt.end() ) {
      out.put( *i ) ;
      continue ;
    }

    // get the name between parens
    std::string name ( i+2, j ) ;
    bool known = true ;
    if ( name == "logger" ) {
      if ( rec.logger().empty() ) {
        out << "/root/" ;
      } else {
        out << rec.logger() ;
      }
    } else if ( name == "level" ) {
      out << rec.level() ;
    } else if ( name == "L" ) {
      out << rec.level().levelLetter() ;
    } else if ( name == "message" ) {
      // hack - reset buffer state
      rec.msgbuf()->pubseekoff ( 0, std::ios_base::beg, std::ios_base::in ) ;
      out << rec.msgbuf() ;
    } else if ( name == "path" ) {
      const char* path = rec.fileName() ;
      out << ( path ? path : "<empty>" ) ;
    } else if ( name == "file" ) {
      const char* path = rec.fileName() ;
      if ( path ) {
	const char* p = strrchr ( path, '/' ) ;
	if ( ! p ) p = path ;
	out << p+1 ;
      } else {
	out << "<empty>" ;
      }
    } else if ( name == "line" ) {
      out << rec.lineNum() ;
    } else if ( name == "time" ) {
      formattedTime ( _timefmt, out ) ;
    } else if ( name == "pid" ) {
      out << (unsigned long)getpid() ;
    } else {
      known = false ;
    }

    // advance
    if ( known ) {
      i = j ;
    }

  }


}


namespace {

  // get current time and format it
  void formattedTime ( std::string fmt, std::ostream& out )
  {
    // get seconds/nanoseconds
    struct timespec ts;
    clock_gettime( CLOCK_REALTIME, &ts );

    // convert to break-down time
    struct tm tms ;
    localtime_r( &ts.tv_sec, &tms );

    // replace %f in the format string with miliseconds
    std::string::size_type n = fmt.find("%f") ;
    if ( n != std::string::npos ) {
      char subs[4] ;
      snprintf ( subs, 4, "%03d", int(ts.tv_nsec/1000000) ) ;
      while ( n != std::string::npos ) {
	fmt.replace ( n, 2, subs ) ;
	n = fmt.find("%f") ;
      }
    }

    char buf[1024] ;
    strftime(buf, 1024, fmt.c_str(), &tms );
    out << buf ;

  }

}

#ifndef MSGLOGLEVEL_HH
#define MSGLOGLEVEL_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgLogLevel.hh 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgLogLevel.
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Author List:
//      Andy Salnikov
//
// Copyright Information:
//      Copyright (C) 2005 SLAC
//
//------------------------------------------------------------------------

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  This class defines message logging levels, their names and ordering.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2005 SLAC
 *
 *  @see MsgLogLevelMsgLogLevel
 *
 *  @version $Id: MsgLogLevel.hh 518 2010-01-15 08:25:10Z stroili $ 
 *
 *  @author Andy Salnikov
 */

class MsgLogLevel {

public:

  /**
   *  Message Level levels.
   */
  enum Level { debug,
	       trace,
	       info,
	       nolog      // There should be no messages with this level, it's only for loggers
             };  

  // Construct root logger
  MsgLogLevel( Level code ) : _level(code) {}

  // default copy ctor is OK
  //MsgLogLevel( const MsgLogLevel& );

  // Destructor
  ~MsgLogLevel() {}

  // default assignment is OK
  //MsgLogLevel& operator= ( const MsgLogLevel& );
  MsgLogLevel& operator= ( const Level code ) { _level = code ; return *this ;}

  // comparison operators
  bool operator == ( MsgLogLevel other ) const { return _level == other._level ; }
  bool operator != ( MsgLogLevel other ) const { return _level != other._level ; }
  bool operator < ( MsgLogLevel other ) const { return int(_level) < int(other._level) ; }
  bool operator <= ( MsgLogLevel other ) const { return int(_level) <= int(other._level) ; }
  bool operator > ( MsgLogLevel other ) const { return int(_level) > int(other._level) ; }
  bool operator >= ( MsgLogLevel other ) const { return int(_level) >= int(other._level) ; }

  // get full printable name of Level level
  const char* levelName () const ;

  // get one-char Level code
  char levelLetter () const ;

protected:

  // Helper functions

private:

  // Data
  Level _level ;

//------------------
// Static Members --
//------------------

public:

  // default level for loggers
  static MsgLogLevel defaultLevel () { return MsgLogLevel(nolog) ; }

};

inline std::ostream& 
operator<< ( std::ostream& o, MsgLogLevel sev ) {
  return o << sev.levelName() ;
}

#endif // MSGLOGLEVEL_HH

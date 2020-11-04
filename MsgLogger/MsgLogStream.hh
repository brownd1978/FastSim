#ifndef MSGLOGSTREAM_HH
#define MSGLOGSTREAM_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgLogStream.hh 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgLogStream.
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
#include <string>
#include <sstream>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "MsgLogger/MsgLogLevel.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  Macros(grrr!) for user's convenience
 */
#ifdef MsgLog
#undef MsgLog
#endif
#define MsgLog(logger,sev) MsgLogStream( logger, MsgLogLevel(MsgLogLevel::sev), __FILE__, __LINE__ ).ostream_hack()

#ifdef MsgLogRoot
#undef MsgLogRoot
#endif
#define MsgLogRoot(sev) MsgLogStream( MsgLogLevel::sev, __FILE__, __LINE__ ).ostream_hack()

#ifdef MsgLogDecl
#undef MsgLogDecl
#endif
#define MsgLogDecl(str,logger,sev) MsgLogStream str( logger, MsgLogLevel(MsgLogLevel::sev), __FILE__ , __LINE__ )

#ifdef MsgLogRootDecl
#undef MsgLogRootDecl
#endif
#define MsgLogRootDecl(str,sev) MsgLogStream str( MsgLogLevel::sev, __FILE__ , __LINE__ )


/**
 *  Special stream class (subclass of standard stream class) which collects
 *  the message source and forwards complete message to the logger class
 *  when encounters a special manipulator object.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2005 SLAC
 *
 *  @see MsgLogger
 *
 *  @version $Id: MsgLogStream.hh 518 2010-01-15 08:25:10Z stroili $ 
 *
 *  @author Andy Salnikov
 */

class MsgLogStream : public std::stringstream {

public:

  /**
   *  Constructors. 'where' argument is usually a filename:line string constructed from 
   *  __FILE__, __LINE__ macros. It is char* type instead of std::string for optimization 
   *  reasons (crappy C++ has no compile-time constructors for classes.) The pointer is stored 
   *  in an object, content is not copied, so make sure that you don't overwrite the string you pass.
   */
  explicit MsgLogStream ( MsgLogLevel sev, const char* file = 0, int line = -1 ) 
    : std::stringstream(), _logger(), _sev(sev), _file(file), _lineNum(line) {}
  MsgLogStream ( const std::string& loggerName, MsgLogLevel sev, const char* file = 0, int line = -1 )
    : std::stringstream(), _logger(loggerName), _sev(sev), _file(file), _lineNum(line) {}

  // Destructor
  virtual ~MsgLogStream() ;

  // g++ somehow fails to recognize temporary MsgLogStream() as a good stream,
  // had to add this "cast" operation
  std::ostream& ostream_hack() { return *this ; }

  // send my content to logger
  void emit() ;

protected:

private:

  // Data members
  std::string _logger ;
  MsgLogLevel _sev ;
  const char* _file ;
  int _lineNum ;


  // Note: if your class needs a copy constructor or an assignment operator, 
  //  make one of the following public and implement it.
  MsgLogStream( const MsgLogStream& );                // Copy Constructor
  MsgLogStream& operator= ( const MsgLogStream& );    // Assignment op

};

/// manipulator which sends the message
std::ostream& endlog ( std::ostream& o ) ;

#endif // MSGLOGSTREAM_HH

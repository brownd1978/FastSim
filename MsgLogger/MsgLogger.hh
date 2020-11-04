#ifndef MSGLOGGER_HH
#define MSGLOGGER_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgLogger.hh 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgLogger.
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
#include <string>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "MsgLogger/MsgLogStream.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class MsgHandler ;
class MsgLogRecord ;
class MsgLoggerImpl ;

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  Macros(grrr!) for user's convenience
 */
#ifdef MsgLogging
#undef MsgLogging
#endif
#define MsgLogging(logger,sev) MsgLogger(logger).logging( MsgLogLevel(MsgLogLevel::sev) )

#ifdef MsgLoggingRoot
#undef MsgLoggingRoot
#endif
#define MsgLoggingRoot(sev) MsgLogger().logging( MsgLogLevel(MsgLogLevel::sev) )

/**
 *  The class for the first-level processing of the messages. The streams
 *  send messages to this class, and this class forwards them to handlers
 *  (if it decides that message is indeed to be logged.) The loggers are
 *  organized into hierarchical structure, every logger has a parent, except 
 *  one root logger which does not have any parents. Every message is also sent
 *  by logger to its parent logger, unless propagate flag is clear.
 *
 *  Note that this class is just a kind of smart poiner  class for the 
 *  real objects of the class MsgLoggerImpl which do real job.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2005 SLAC
 *
 *  @see MsgLoggerImpl
 *
 *  @version $Id: MsgLogger.hh 518 2010-01-15 08:25:10Z stroili $ 
 *
 *  @author Andy Salnikov
 */

class MsgLogger {

public:

  // Construct named logger, or root logger for empty name
  MsgLogger( const std::string& name = "" ) ;

  // default copy ctor is OK
  //MsgLogger( const MsgLogger& );

  // Destructor
  ~MsgLogger() {}

  // default assignment is OK
  //MsgLogger& operator= ( const MsgLogger& );

  /// set the logger level, messages with the level below this won't be logged
  void setLevel ( MsgLogLevel level ) ;

  /// define wheter or not we need messages propagated to ancestors
  void propagate ( bool flag ) ;

  /// add a handler for the messages, takes ownership of the object 
  void addHandler ( MsgHandler* handler ) ;

  /// check if the specified level will log any message
  bool logging ( MsgLogLevel sev ) const ;

  /// get the stream for the specified log level
  bool log ( const MsgLogRecord& record ) const ;

protected:

  // Helper functions

private:

  // Friends

  // Data members
  MsgLoggerImpl* const _impl ;   // ponter to the real implementation

};

#endif // MSGLOGGER_HH

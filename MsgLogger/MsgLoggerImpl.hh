#ifndef MSGLOGGERIMPL_HH
#define MSGLOGGERIMPL_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgLoggerImpl.hh 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgLoggerImpl.
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
#include <vector>

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
class MsgHandler ;
class MsgLogRecord ;

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  Implmentation of the message logger. This class is actually an implementation 
 *  detail used by the MsgLogger class. It should not be visible to clients in any way.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2005 SLAC
 *
 *  @see MsgLogger
 *  @see MsgHandler
 *
 *  @version $Id: MsgLoggerImpl.hh 518 2010-01-15 08:25:10Z stroili $ 
 *
 *  @author Andy Salnikov
 */

class MsgLoggerImpl {

public:

  // Construct named logger
  MsgLoggerImpl( const std::string& name ) ;

  // default copy ctor is OK
  //MsgLoggerImpl( const MsgLoggerImpl& );

  // Destructor
  ~MsgLoggerImpl();

  // default assignment is OK
  //MsgLoggerImpl& operator= ( const MsgLoggerImpl& );

  /// set the logger level, messages with the level below this won't be logged
  void setLevel ( MsgLogLevel level ) { _level = level ; }

  /// define wheter or not we need messages propagated to ancestors, root logger never propagates
  void propagate ( bool flag ) { if ( ! _name.empty() ) _propagate = flag ; }

  /// add a handler for the messages, takes ownership of the object 
  void addHandler ( MsgHandler* handler ) ;

  /// name of the logger
  const std::string name() const { return _name ; }

  /// check if the specified level will log any message
  bool logging ( MsgLogLevel sev ) const ;

  /// get the stream for the specified log level
  bool log ( const MsgLogRecord& record ) const ;

  /// send the message to the handles and parents
  void handle ( MsgLogRecord& record ) const ;

protected:

private:

  // Types
  typedef std::vector<MsgHandler*> HandlerList ;

  // Data members
  std::string _name ;
  MsgLogLevel _level ;
  mutable bool _propagate ;
  mutable MsgLoggerImpl* _parent ;
  HandlerList _handlers ;

//------------------
// Static Members --
//------------------

public:

  static MsgLoggerImpl* getLogger( const std::string& name ) ;

};

#endif // MSGLOGGERIMPL_HH

#ifndef MSGHANDLERSTDSTREAMS_HH
#define MSGHANDLERSTDSTREAMS_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgHandlerStdStreams.hh 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgHandlerStdStreams.
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
#include "MsgLogger/MsgHandler.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class MsgLogLevel ;

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  Implementation of the message handler which logs formatted messages to 
 *  the stanfard C++ streams cout and cerr (well I have removed "error" 
 *  levels so it's only logs to cout now.)
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2005 SLAC
 *
 *  @see MsgHandlerStdStreamsMsgHandlerStdStreams
 *
 *  @version $Id: MsgHandlerStdStreams.hh 518 2010-01-15 08:25:10Z stroili $ 
 *
 *  @author Andy Salnikov
 */

class MsgHandlerStdStreams : public MsgHandler {

public:

  // Constructor
  MsgHandlerStdStreams() ;

  // Destructor
  virtual ~MsgHandlerStdStreams() ;

  /// get the stream for the specified log level
  virtual bool log ( const MsgLogRecord& record ) const ;

protected:

private:

  // Disable copy
  MsgHandlerStdStreams( const MsgHandlerStdStreams& );
  MsgHandlerStdStreams& operator= ( const MsgHandlerStdStreams& );

};

#endif // MSGHANDLERSTDSTREAMS_HH

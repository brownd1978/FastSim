#ifndef MSGFORMATTER_HH
#define MSGFORMATTER_HH

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: MsgFormatter.hh 518 2010-01-15 08:25:10Z stroili $
//
// Description:
//	Class MsgFormatter.
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
#include <map>

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
class MsgLogRecord ;

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 *  The class which does message formating to show the message in  the 
 *  human-readable format. In principle you can inherit this class and 
 *  override some methods, but this implementation is already sufficiently 
 *  generic and can be controlled completely via the format strings.
 *
 *  This software was developed for the BaBar collaboration.  If you
 *  use all or part of it, please give an appropriate acknowledgement.
 *
 *  Copyright (C) 2005 SLAC
 *
 *  @see MsgHandler
 *
 *  @version $Id: MsgFormatter.hh 518 2010-01-15 08:25:10Z stroili $ 
 *
 *  @author Andy Salnikov
 */

class MsgFormatter {

public:

  // Constructor
  MsgFormatter( const std::string& fmt = "", const std::string& timefmt = "" ) ;

  // Destructor
  virtual ~MsgFormatter() ;

  // add level-specific format
  virtual void addFormat ( MsgLogLevel level, const std::string& fmt ) ;

  // format message to the output stream
  virtual void format ( const MsgLogRecord& rec, std::ostream& out ) ;

protected:

private:

  typedef std::map< MsgLogLevel, std::string > FormatMap ;

  // Data members
  std::string _fmt ;
  std::string _timefmt ;
  FormatMap _fmtMap ;
  
  // Disable copy
  MsgFormatter( const MsgFormatter& );
  MsgFormatter& operator= ( const MsgFormatter& );

};

#endif // MSGFORMATTER_HH

#ifndef APPERRLOG_HH
#define APPERRLOG_HH

//--------------------------------------------------------------------------
// Description:
//     Interface description for the reconstruction logging interface
//
//     Available constructs are a logging call:
//
//            ErrMsg(severity) << "Foo" << endl;
//     or
//            ErrLog::msg(severity, facility, code) << "Foo" << endl;
//
//     and a test to see if a particular logging call will do anything:
//
//            if (ErrLogging(severity)) ...
//            if (ErrLog::logging(severity, facility, code)) ...
//
//     Note that the semantics of these are _not_ defined.  In particular, 
//     the user should not depend on their being cpp macros, ctors, or
//     global fn calls.  Just use the syntax shown above!
//
//     "severity" is a member of an enum; see the ErrLog::Severity enum.
//     "facility" names the source of the message
//     "code"     distinguishes the individual errors possible
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Bob Jacobsen
//
// $Id: AppErrLog.hh 507 2010-01-14 15:17:33Z stroili $
//
// Copyright Information:
//	Copyright (C) 1998
//
//------------------------------------------------------------------------

//-----------------
// C/C++ Headers --
//-----------------
#include <vector>

//----------------------
// Base Class Headers --
//----------------------
#include "ErrLogger/ErrLog.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrStreamWrapper.hh"


//		---------------------
// 		-- Class Interface --
//		---------------------

class AppErrLog : public ErrLog {

  protected:
    // implement the underlying behavior
    virtual ErrStream& doMsg( ErrLog::Severity severity, 
                              const char* facility, 
			      int code );

    virtual void doEndmsg( const char* text, ErrStream& stream );

    virtual bool doLogging( ErrLog::Severity severity, 
                            const char* facility, 
			    int code );

    // Parameters defining the range of severities to be accounted for in
    // the vector of message counts.
    // - The number of defined severities:
    enum { numSeverity    = ErrLog::MaxSeverity+1-ErrLog::MinSeverity };
    // - The offset to add to the lowest severity to get a range starting at 0:
    enum { offsetSeverity = -ErrLog::MinSeverity };

  public:

    // ctor and dtor just register and unregister
    AppErrLog();
    virtual ~AppErrLog();

  private:
    ErrStreamWrapper _myDevnull;
    ErrStreamWrapper _myCout;
    ErrStreamWrapper _myCerr;

  std::vector<int>  _msgCount;
  void dumpJobReport();

};

#endif // APPERRLOG_HH

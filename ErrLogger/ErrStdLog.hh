#ifndef ERRSTDLOG_HH
#define ERRSTDLOG_HH

//--------------------------------------------------------------------------
// Description:
//	Class ErrStdLog :
//	Standard implementation of ErrLog.  This implementation
//	allows a user to set the severity of messages sent to the
//	output streams, but the streams are cout and cerr.
//
// Author List:
//	Scott Metzler	(addition of ErrStream)
//
//      $Id: ErrStdLog.hh 503 2010-01-14 13:12:26Z stroili $
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1998, 2002   California Institute of Technology
//
//------------------------------------------------------------------------

#include "ErrLogger/ErrLog.hh"

class ErrStream; // declare

class ErrStdLog : public ErrLog {
  public:
    // constructor
    ErrStdLog( Severity theSeverity=warning );

    // constructor 
    ErrStdLog( std::ostream & theStream, Severity theSeverity=warning );
    
    // destructor
    virtual ~ErrStdLog();

  protected:
    // data members for the default implementation
    ErrStream * _myStreamCout;
    ErrStream * _myStreamCerr;
    ErrStream * _myDevnull;

    virtual ErrStream& doMsg( ErrLog::Severity severity, 
			      const char* facility, 
			      int code );

    virtual void doEndmsg( const char* text, ErrStream& stream );


  private:
    // dis-allow copy construction and assignment
    ErrStdLog( const ErrStdLog& );
    ErrStdLog& operator=( const ErrStdLog& );
};

#endif

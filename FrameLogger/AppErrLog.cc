//--------------------------------------------------------------------------
//
// Description:
//
//  Error handling implementation for use with reco Framework jobs
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen
//      $Id: AppErrLog.cc 507 2010-01-14 15:17:33Z stroili $
//
// Copyright Information:
//	Copyright (C) 1998	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "FrameLogger/AppErrLog.hh"

#include <assert.h>
#include <sstream>
#include <iostream>
#include <stdio.h>

#include "Framework/AppModule.hh"
#include "ErrLogger/ErrStream.hh"
#include "FrameLogger/AppActionErrLogger.hh"
#include "FrameLogger/AppErrLogModule.hh"
#include "CLHEP/String/Strings.h"
#include "CommonUtils/ComTimeStamp.hh"
#include "Framework/AppFramework.hh"
#include "FrameJobReport/FjrFwkJobReport.hh"


using std::cerr;
using std::cout;
using std::endl;
using std::ios;
using std::ostream;

//		--------------------------------
// 		-- Local Variable Definitions --
//		--------------------------------

static std::ostringstream devnull;

//		---------------------------------
// 		-- Global Variable Definitions --
//		---------------------------------

class AppModule;
AppModule* ErrLogCurMod=0;

//		---------------------------------
// 		-- Global Function Definitions --
//		---------------------------------

// ctor registers the object into the static of the base class
AppErrLog::AppErrLog() 
  : ErrLog( )
  , _myDevnull(devnull,this)
  , _myCout(cout,this)
  , _myCerr(cerr,this)
  , _msgCount(numSeverity, 0)
{
  // There can be only one.  Make sure this test happens even if the 
  // assertion in ErrLog is compiled away.
  assert( _implementation() == 0 );

  // Establish this as the active implementation.
  ErrLog::_setImplementation( this );
}

AppErrLog::~AppErrLog() 
{
  // There can be only one, and it should be this one.
  assert( ErrLog::_implementation() == this );

  // Remove this one and leave the default implementation in place
  // (in case subsequently called destructors issue ErrMsgs).
  ErrLog::_setImplementation(0);

  // Flush out the job report.
  dumpJobReport();
}

void
AppErrLog::dumpJobReport() {

  FjrFwkJobReport* theReport = FjrFwkJobReport::getFwkJobReport();
  if (theReport->isEnabled()) {

    std::ostringstream theBuff;

    theBuff << numSeverity;
    theReport->report(FjrAbsJobReport::Bbr, "ErrLog",
		      "numberSeverityTypes", theBuff.str());
    theBuff.str("");
    
    // dump information in the job report
    for (int ctr = 0; ctr < numSeverity; ctr++) {

      theBuff << _msgCount[ctr];
      
      std::string hierarchy = "ErrLog::";
      switch (ctr-offsetSeverity) {
      case ErrLog::debugging:
	hierarchy += "Debugging";
	break;
      case ErrLog::trace:
	hierarchy += "Trace";
	break;
      case ErrLog::routine:
	hierarchy += "Routine";
	break;
      case ErrLog::warning:
	hierarchy += "Warning";
	break;
      case ErrLog::error:
	hierarchy += "Error";
	break;
      case ErrLog::severe:
        hierarchy += "Severe";
        break;
      case ErrLog::fatal:
	hierarchy += "Fatal";
	break; 
      default:
	// unknown type !
	hierarchy += "Unknown";
	break;
      }
      theReport->report(FjrAbsJobReport::Bbr, hierarchy,
			"numberMessages", theBuff.str());      
      theBuff.str("");
    } 
    
  }
    
}

// member function to handle the actual logging request   
ErrStream& 
AppErrLog::doMsg( ErrLog::Severity severity, 
		  const char* facility, 
		  int code )
{
    // count types of messages
    _msgCount[severity+offsetSeverity] += 1;
    

    // first, check is logging.  If not, ignore by routing to local character
    // string after resetting to beginning
    if (!ErrLog::logging(severity, facility, code)) {
	devnull.rdbuf()->pubseekoff(0, ios::beg, ios::out);
	return _myDevnull;
    }
    // temporary to refer to output stream
    ErrStream* logTo = 0;
    
    // OK, check severity.  Policy here is for warning, error, severe, and
    // fatal to produce output to cerr, the rest to cout. This is intended
    // to be the same as the default in ErrLogger itself.
    if (severity>=warning) logTo = &_myCerr;
    else logTo = &_myCout;
        
    assert(logTo!=0); // sanity check
    
    // Prepare an actual ostream to use in writing out any message prefix.
    ostream& myOstream( *logTo );

    // prepend time stamp if production is set in framework
    if (0 !=  AppActionErrLogger::currentExec() ) {
      if (AppActionErrLogger::currentExec()->framework()->production() ) {
	myOstream << ComTimeStamp() << " " ;
      }
    }

    // Prepend standard header if required.
    // The logic is this: 
    //   1) if the severity is routine or higher, or if the "code" is
    //      non-zero, we print the name of the currently executing
    //      Framework module (or other AppExecutable);
    //   2) if the severity is higher than routine, we add the "facility"
    //      name, which by default for ErrMsg()-based messages will be the
    //      filename and line number; and
    //   3) if the "code" is non-zero, we print the code in square brackets.


    if ( severity >= routine || code != 0 ) {
      if (AppActionErrLogger::currentExec() != 0 )  {
	myOstream << AppActionErrLogger::currentExec()->name();	
      } else {  
	myOstream << "<unknown>";	
      }
      
      if (severity > routine ) {
	// facility is typically the filename, which has to have the long
	// pathname removed from it
	HepString filename(facility);
	HepString shortname(filename(filename.lIndex('/')+1, 100));
	if (shortname.length() == 0) shortname = filename;
	myOstream << "::"<< (const char*) shortname;
      }

      if ( code != 0 ) {
	myOstream << " [" << code << "]";
      }

      // and turn it back to the user for the rest of the message
      myOstream << ":";
    }

    return (*logTo);
}

void 
AppErrLog::doEndmsg( const char* text, ErrStream& stream )
{
  // Since the streams defined by this class are all ErrStreamWrapper,
  // they do output as-you-go and should never pass text back to here.
  assert( text == 0 );

  // But we can handle it if assertions are compiled away...
  if ( text != 0 ) {
    static_cast<ostream&>(stream) << text;
  }

  // Terminate all messages with endl (at least for now; eventually we
  // might decide to -- optionally? -- terminate low-severity messages
  // with just '\n' for improved performance).
  static_cast<ostream&>(stream) << endl;

  // Determine whether termination will occur; if so, we need to write out
  // the message severity summary to the job report first!
  if ( ErrLog::_getDefaultTermination( stream.severity(), stream.code() )
       != 0 ) {
    // Yes, this will terminate.
    dumpJobReport();

    // Actually apply the termination policy.
    ErrLog::_defaultTermination( stream.severity(), stream.code() );
  }
}

bool 
AppErrLog::doLogging( ErrLog::Severity severity, 
		      const char*,                // unused
		      int )                       // unused
{
    // The implementation of this relies on AppActionErrLog to record the
    // Framework's "current module", whose verbose and production
    // parameters will be used to check what should be printed.
    // If that action is not in use, messages at/above routine will be logged
    if (AppActionErrLogger::currentModule() == 0 )
	if (severity >= routine ) return true;
	else return false;

    // always pass warnings and above
    if (severity >= warning ) return true;
    
    // verbose passes everything
    if (AppActionErrLogger::currentModule()->verbose()) return true;
    
    // production passes warnings, !production passes routine
    if (!(AppActionErrLogger::currentModule()->production()) 
	&& severity>=ErrLog::routine) 
	   return true;
    
    // nobody claims this one
    return false;
}

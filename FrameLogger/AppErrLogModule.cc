// $Id: AppErrLogModule.cc 507 2010-01-14 15:17:33Z stroili $

#include "BaBar/BaBar.hh"
#include "FrameLogger/AppErrLogModule.hh"

#include <iostream>

using std::cerr;
using std::endl;

#include "GenEnv/GenEnv.hh"
#include "AbsEnv/AbsEnv.hh"


AppErrLogModule* AppErrLogModule::myInstance = 0;

AppErrLogModule::AppErrLogModule( const char* const theName, 
    const char* const theDescription )
    : AppModule( theName, theDescription )
    , _makeHisto("makeHisto", this, false )
    , _nAtSeverityN(0)
    , _beginJobDone(false)
{
  commands( )->append( &_makeHisto );
  
  myInstance = this;
}

AppErrLogModule::~AppErrLogModule()
{
  myInstance = 0;
}

AppResult AppErrLogModule::beginJob( AbsEvent* anEvent )
{
    _beginJobDone = true;
    initHisto();

    return AppResult::OK;
}

void AppErrLogModule::initHisto()
{
    if (_makeHisto.value() && _nAtSeverityN == 0)
    {
	HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
	if (manager == 0)
	{
	    // Don't recurse -- use cerr.
	    cerr << "HepTupleManager not found in AppErrLogMon." << endl;
    	}
	else
	{
	    const int numSeverity = ErrLog::MaxSeverity+1-ErrLog::MinSeverity;
	    _nAtSeverityN = manager->histogram("ErrLogger Messages By Severity",
	    	    	    	    	       numSeverity,
					       (float) ErrLog::MinSeverity,
					       (float) ErrLog::MaxSeverity,
					       5000);
	    if (_nAtSeverityN == 0)
	    {
    	    	cerr << "Cannot create histogram in AppErrLogMon." << endl;
	    }
	}
    }
}

AppResult AppErrLogModule::endJob( AbsEvent* anEvent )
{
    _nAtSeverityN = 0;
    _beginJobDone = false;

    return AppResult::OK;
}

HepHistogram* AppErrLogModule::getLogHistogram()
{
    if (myInstance == 0 || ! myInstance->_beginJobDone || ! myInstance->_makeHisto.value())
      return 0;
    
    return myInstance->_nAtSeverityN;
}

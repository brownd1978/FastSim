//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRandomControl.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRandomControl
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A.Salnikov
//
// Copyright Information:
//	Copyright (C) 1997	<Institution>
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "RandControl/RacRandomControl.hh"

//-------------
// C Headers --
//-------------
extern "C" {
#include <assert.h>
}

//---------------
// C++ Headers --
//---------------
#include <fstream>
#include <algorithm>
#include <functional>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BbrStdUtils/BbrStdDeleteObject.hh"
#include "AbsEnv/AbsEnv.hh"
#include "AbsEvent/AbsEvent.hh"
#include "AbsEvent/AbsEventID.hh"
#include "AbsParm/AbsParmIfdStrKey.hh"
#include "CLHEP/String/Strings.h"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandGauss.h"
#include "ErrLogger/ErrLog.hh"
#include "EidData/EidEventTriplet.hh"
#include "Framework/AbsParmBool.hh"
#include "Framework/AbsParmString.hh"
#include "G3Data/GEvent.hh"
#include "GenEnv/GenEnv.hh"
#include "HepTuple/TupleManager.h"
#include "HepTuple/Tuple.h"
#include "OdfCommon/odfTime.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "RandControl/RacRanecuEngine.hh"
#include "RandControl/RacRanecuEnginePtr.hh"
#include "RandControl/RacEidConfig.hh"
#include "RandControl/RacRunConfig.hh"
using std::endl;
using std::fstream;
using std::ofstream;
using std::ostream;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

const long RacRandomControl::_ranecuSeeds[2] = { 1234567, 8901234 } ;

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
RacRandomControl::RacRandomControl ( const char* theName, 
				       const char* theDescription ) :
  AppModule ( theName, theDescription ),
  _myGevEngines(),
  _myEidEngines(),
  _oldEngine(0),
  _runConfig( new RacRunConfig ),
  _eidConfig( new RacEidConfig ),
  _maxEventsPerRun("maxEventsPerRun", this, _runConfig->maxEventsPerRun()),
  _maxRandomsPerEvent("maxRandomsPerEvent", this, _runConfig->maxRandomsPerEvent()),
  _randomsPercentPrimary("randomsPercentPrimary", this, _runConfig->randomsPercent(0)),
  _randomsPercentSimulation("randomsPercentSimulation", this, _runConfig->randomsPercent(1)),
  _geventList ( new AbsParmIfdStrKey( "gEventList", this, "Default" ) ),
  _eventIdKey( new AbsParmIfdStrKey( "eventIdKey", this, "AbsEventID" ) ),
  _engineList ( new AbsParmIfdStrKey( "engineList", this, "RandomControl" ) ),
  _computeEventSeeds ( new AbsParmBool( "computeEventSeeds", this, true ) ),
  _hist ( new AbsParmBool( "hist", this, false ) ),
  _abortOnPrimaryOverConsumption( new AbsParmBool("abortOnPrimaryOverConsumption", this, true) ),
  _reportOverConsumption( new AbsParmBool("reportOverConsumption", this, true) ),
  _reportNumSeedsUsed( new AbsParmBool("reportNumSeedsUsed", this, false) ),
  _reportFileName ( new AbsParmString("reportFile", this, "seed-overuse.txt") ),
  _reportFile(0),
  _runNumber(0),
  _eventNumber(0),
  _ntuple(0)
{
  commands() -> append ( &_maxEventsPerRun ) ;
  commands() -> append ( &_maxRandomsPerEvent ) ;
  commands() -> append ( &_randomsPercentPrimary ) ;
  commands() -> append ( &_randomsPercentSimulation ) ;
  commands() -> append ( _geventList ) ;
  commands() -> append ( _eventIdKey ) ;
  commands() -> append ( _engineList ) ;
  commands() -> append ( _computeEventSeeds ) ;
  commands() -> append ( _hist ) ;
  commands() -> append ( _abortOnPrimaryOverConsumption );
  commands() -> append ( _reportOverConsumption );
  commands() -> append ( _reportNumSeedsUsed );
  commands() -> append ( _reportFileName );
}

//--------------
// Destructor --
//--------------
RacRandomControl::~RacRandomControl ()
{
  // restore saved engine
  HepRandom::setTheEngine ( _oldEngine ) ;

  // delete all other engines
  std::for_each ( _myGevEngines.begin(), _myGevEngines.end(), BbrStdDeleteObject() ) ;
  _myGevEngines.clear() ;
  std::for_each ( _myEidEngines.begin(), _myEidEngines.end(), BbrStdDeleteObject() ) ;
  _myEidEngines.clear() ;

  // close & delete _reportFile:
  delete _reportFile;

}

//-------------
// Methods   --
//-------------
AppResult
RacRandomControl::beginJob ( AbsEvent* ) 
{
  if ( ErrLogging(routine) ) {
    ErrMsg(routine) << name() << " -- starting job" << endmsg;
  }

  // Set the run class parameters:
  _runConfig->setMaxEventsPerRun(_maxEventsPerRun.value());
  _runConfig->setMaxRandomsPerEvent(_maxRandomsPerEvent.value());
  _runConfig->setRandomsPercent(0, _randomsPercentPrimary.value());
  _runConfig->setRandomsPercent(1, _randomsPercentSimulation.value());

  int randomPercentDigis = 100
    - _runConfig->randomsPercent(0) 
    - _runConfig->randomsPercent(1);
  
  if (randomPercentDigis < 0) {
    ErrMsg(fatal) << name() << ": Given randomsPercentPrimary = "
		  <<  _randomsPercentPrimary.value()
		  << " and randomsPercentSimulation = "
		  << _randomsPercentSimulation.value() << endl
		  << "  I get randomsPercentDigis = "
		  << _runConfig->randomsPercent(2) << " < 0. Aborting."
		  << endmsg;
  }
  
  _runConfig->setRandomsPercent(2, randomPercentDigis);

  if ( ErrLogging(routine) ) {
    ErrMsg(routine) 
      << name() << ": Run configuration: " << endl
      << "  firstRunNumber = " << _runConfig->firstRunNumber() << endl
      << "  lastRunNumber = " << _runConfig->lastRunNumber() << endl
      << "  maxEventsPerRun = " << _runConfig->maxEventsPerRun() << endl
      << "  maxRandomsPerEvent = " << _runConfig->maxRandomsPerEvent() << endl
      << "  Fraction of events used for Primary = " 
      << _runConfig->randomsPercent(0) << "%" << endl
      << "  Fraction of events used for Simulation = " 
      << _runConfig->randomsPercent(1) << "%" << endl
      << "  Fraction of events used for Digis = " 
      << _runConfig->randomsPercent(2) << "%" << endmsg;
  }

  // !!! generally speaking run number should be known in the beginRun()
  // but not here. But this is how our Framework works now - it even does not
  // call beginRun()
  _runNumber = 0 ;      // do not know where to get the run number !!!
  if ( ErrLogging(trace) ) {
    ErrMsg(trace) << name() << " -- run number will be " << _runNumber << '\n' 
		    << name() << " -- this run can have max " 
		    << _runConfig->maxEventsPerRun() - 2
		    << " events" << endmsg ;
  }

  // reset my own event counter
  _eventNumber = 0 ;

  // ====== make all engines ======
  for ( int is = 0 ; is < _runConfig->nStages() ; is ++ ) {

    // create new ranecu engine
    bool countCalls = _hist->value() || _reportOverConsumption->value();
    RacRanecuEngine* engine = new RacRanecuEngine( 0, countCalls ) ;

    // skip to the event number 0
    engine -> setSeeds ( _ranecuSeeds ) ;
    _runConfig->skip ( *engine, is, _runNumber, 0 ) ;
    
    _myGevEngines.push_back( engine ) ;
    
    if (ErrLogging(trace)){
      ErrMsg(trace) << name() << " -- engine \"" << _runConfig->stageName(is)
		      << "\" consumes " 
		      << _runConfig->randomsPerStagePerEvent(is) 
		      << " randoms max per event" << endmsg ;
    }
    
  }

  for ( int ise = 0 ; ise < _eidConfig->nStages() ; ise ++ ) {

    // create new ranecu engine
    bool countCalls = _hist->value() || _reportOverConsumption->value();
    RacRanecuEngine* engine = new RacRanecuEngine( 0, countCalls ) ;

    // skip to the event number 0
    engine -> setSeeds ( _ranecuSeeds ) ;
    
    _myEidEngines.push_back( engine ) ;
    
    if (ErrLogging(trace)){
      ErrMsg(trace) << name() << " -- engine \"" << _eidConfig->stageName(ise)
		      << "\" consumes " 
		      << _eidConfig->randomsPerStagePerEvent(ise) 
		      << " randoms max per event" << endmsg ;
    }
    
  }

  // ====== store current engine ======
  _oldEngine = HepRandom::getTheEngine ( ) ;

  // ====== create ntuple ======
  if ( _hist->value() ) {
    HepTupleManager* manager = gblEnv->getGen()->ntupleManager();
    _ntuple = manager->ntuple ( "Random Control" ) ;
  }

  // ======= open report file ===========
  if (_reportOverConsumption->value()){
    _reportFile = new ofstream(_reportFileName->value().c_str());
    if ( !*_reportFile ) {
      ErrMsg(warning) << name() << ": WARNING: Unable to create reportFile " 
		      << _reportFileName->value() << '\n'
		      << "       Will not report seed over consumption." << endmsg;
      delete _reportFile ;
      _reportFile = 0 ;
    }
  }

  return AppResult::OK;
}

//
// the next event
//
AppResult
RacRandomControl::event ( AbsEvent* anEvent )
{
  // ====== find the current run and event number ======
  const GEvent* gevent = 0 ;
  const HepAList<GEvent>* evlist = 
    Ifd< HepAList<GEvent> >::get ( anEvent, _geventList->value() ) ;

  if ( evlist && evlist->length() > 0 ) {
    gevent = evlist->first() ;
    _runNumber = gevent->idrun() ;
    _eventNumber = gevent->idevt() ;
  } else {
    // increment my own event counter # and complain
    _eventNumber ++ ;
    //ErrMsg(warning) << name() << " -- cannot find event info to get the event number" << endmsg ;
  }

  // ====== Get also odfTime form the event ======
  const AbsEventID* eid = Ifd< AbsEventID >::get( anEvent, _eventIdKey->value() ) ;

  if ( ErrLogging(debugging) ) {
    ostream& log = ErrMsg(debugging) ;
    log << name() << " -- starting run/event number " << _runNumber 
	 << "/" << _eventNumber ;
    if ( eid ) {
      log << "\n" << name() << " -- event time: " << eid->eventIdTriplet().timeStamp() ;
    }
    log << endmsg ;
  }

  // ====== save some data in ntuple ======
  if ( _ntuple && _hist->value() ) {
    fillNtuple() ;
  }

  // ====== report seed consumption ======
  if (true == _reportNumSeedsUsed->value()) {
    reportSeeds() ;
  }

  // ====== check seed consumption and make reports ======
  checkGevConsumption() ;
  checkEidConsumption() ;

  // ===== following histogramming or seed report, reset counters ====
  std::for_each ( _myGevEngines.begin(), 
                  _myGevEngines.end(), 
                  std::mem_fun(&RacRanecuEngine::resetCounter) ) ;
  std::for_each ( _myEidEngines.begin(), 
                  _myEidEngines.end(), 
                  std::mem_fun(&RacRanecuEngine::resetCounter) ) ;

  // temporary workaround for the abcense of the run number in the begin()
  // compute and seet the seeds for the very first event
  static bool firstEvent = true ;
  bool needToComputeSeeds = false ;
  if ( firstEvent ) {
    needToComputeSeeds = true ;
    firstEvent = false ;
  }

  // ====== set seeds for engines, and store them in AbsEvent ======
  HepAList<RacAbsEnginePtr>* list = new HepAList<RacAbsEnginePtr> ;

  // Add GEvent-engines only if GEvent was present in event
  if ( gevent ) {

    for ( int is = 0 ; is < _myGevEngines.size() ; is ++ ) {
      
      RacRanecuEngine *engine = _myGevEngines[is] ;
      
      if ( _computeEventSeeds->value() || needToComputeSeeds ) {
	// skip to the run/event/stage
	engine -> setSeeds ( _ranecuSeeds ) ;
	_runConfig->skip ( *engine, is, _runNumber, _eventNumber ) ;
	checkEventNumber();
      }
      
      // store (pointer to) engine in the AbsEvent with the key equal to the name
      const char* name = _runConfig->stageName(is) ;
      list->append( new RacRanecuEnginePtr( name, engine, false ) ) ;
      
    }
  }

  // Add EID-engines only if event ID was present in event
  if ( eid ) {

    for ( int is = 0 ; is < _myEidEngines.size() ; is ++ ) {
      
      RacRanecuEngine *engine = _myEidEngines[is] ;
      
      if ( _computeEventSeeds->value() || needToComputeSeeds ) {
	// skip to the stage
	engine -> setSeeds ( _ranecuSeeds ) ;
	_eidConfig->skip ( *engine, eid, is ) ;
      }
      
      // store (pointer to) engine in the AbsEvent with the key equal to the name
      const char* name = _eidConfig->stageName(is) ;
      list->append( new RacRanecuEnginePtr( name, engine, false ) ) ;
      
    }
  }


  // ====== store list in the AbsEvent ======
  Ifd< HepAList<RacAbsEnginePtr> >::put 
    ( anEvent, 
      new IfdHepAListProxy<RacAbsEnginePtr>(list),
      _engineList->value() ) ;

  //
  // Clear the RandGauss cache, to remove any data associated
  // with a previous event
  //
  RandGauss::setFlag(false);
  
  return AppResult::OK;
}

AppResult
RacRandomControl::endJob ( AbsEvent* )
{
  if ( ErrLogging(routine) ) {
    ErrMsg(routine) << name() << " -- end of job" << endmsg;
  }
  return AppResult::OK;
}

//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------

void 
RacRandomControl::fillNtuple() const
{

  // ====== save some data in ntuple ======
  _ntuple -> column ( "run", _runNumber, 0, "EVENT" ) ;
  _ntuple -> column ( "event", _eventNumber, 0, "EVENT" ) ;
  for ( int is = 0 ; is < _myGevEngines.size() ; is ++ ) {
    RacRanecuEngine *engine = _myGevEngines[is] ;
    HepString name = HepString("GeV") + _runConfig->stageName(is);
    _ntuple -> column ( name, engine->getCounter(), 0, "CALLS" ) ;
  }
  for ( int ise = 0 ; ise < _myEidEngines.size() ; ise ++ ) {
    RacRanecuEngine *engine = _myEidEngines[ise] ;
    HepString name = HepString("Eid") + _runConfig->stageName(ise);
    _ntuple -> column ( name, engine->getCounter(), 0, "CALLS" ) ;
  }
  _ntuple->dumpData() ;

}

void 
RacRandomControl::reportSeeds() const 
{

  if ( ! ErrLogging(routine) ) return ;

  ostream& log = ErrMsg(routine) ;

  // Print header for numSeedsUsed report below
  log << name() << ": Run " << _runNumber << ", event " << _eventNumber << "." ;

  for ( int istage = 0 ; istage < _myGevEngines.size() ; istage ++ ) {
    const int numSeedsUsed = _myGevEngines[istage]->getCounter();
    if ( numSeedsUsed > 0 ) {
      log << "\n" << name() << ": " 
	  << _runConfig->stageName(istage) << " consumed " 
	  << numSeedsUsed << " seeds" ;
    }
  }

  for ( int istage1 = 0 ; istage1 < _myEidEngines.size() ; istage1 ++ ) {
    const int numSeedsUsed = _myEidEngines[istage1]->getCounter();
    if ( numSeedsUsed > 0 ) {
      log << "\n" << name() << ": " 
	  << _eidConfig->stageName(istage1) << " consumed " 
	  << numSeedsUsed << " seeds" ;
    }
  }
  log << endmsg ;
}

void RacRandomControl::checkEventNumber() const {
  if (_eventNumber > _runConfig->maxEventsPerRun()) {
    if (true == _reportOverConsumption->value()) {
      if (_reportFile){
	*_reportFile << name() << ": Event number = "
		     << _eventNumber 
		     << " when maxEventsPerRun = " 
		     << _runConfig->maxEventsPerRun()
		     << endmsg;
      }
      ErrMsg(error) << name() << ": Event number = "
		    << _eventNumber 
		    << " when maxEventsPerRun = " 
		    << _runConfig->maxEventsPerRun()
		    << endmsg;
    }
    
    if (true == _abortOnPrimaryOverConsumption->value()) {
      if (_reportFile){
	*_reportFile << name() << ": Event number = "
		     << _eventNumber 
		     << " when maxEventsPerRun = " 
		     << _runConfig->maxEventsPerRun()
		     << ". Aborting" << endmsg;
      }
      ErrMsg(fatal) << name() << ": Event number = "
		    << _eventNumber 
		    << " when maxEventsPerRun = " 
		    << _runConfig->maxEventsPerRun()
		    << ". Aborting" << endmsg;
    }
  }
}

void 
RacRandomControl::checkGevConsumption() const 
{
  // ====== check seed consumption and make reports ======
  for ( int istage = 0 ; istage < _myGevEngines.size() ; istage ++ ) {
    
    const int numSeedsUsed = _myGevEngines[istage]->getCounter();
    
    const int numAllowedSeedsPerEvent = 
      _runConfig->maxRandomsPerEvent();
      
    const int numAllowedSeedsPerStage = 
      _runConfig->randomsPerStagePerEvent(istage);
      
    // Report that more seeds were used than allowed for this stage:
    if (numAllowedSeedsPerStage < numSeedsUsed){
      if (_reportOverConsumption->value()) {
	if (_reportFile){
	  *_reportFile << name() << ": "
		       << _runConfig->stageName(istage) << " consumed " 
		       << numSeedsUsed << " seeds with only "
		       << numAllowedSeedsPerStage 
		       << " allowd for this stage. run "
		       << _runNumber << ", event " << _eventNumber - 1<< "."
		       << endl;
	}
 	ErrMsg(error) << name() << ": "
		      << _runConfig->stageName(istage) << " consumed " 
		      << numSeedsUsed << " seeds with only "
		      << numAllowedSeedsPerStage 
		      << " allowd for this stage. run "
		      << _runNumber << ", event " << _eventNumber - 1 << "."
		      << endmsg;
      }
    }

    // If more seeds were used than allowed for the whole event, it's
    // more serious:
    if (numAllowedSeedsPerEvent < numSeedsUsed){
      // First, report it:
      if ( _reportFile ) {
 	*_reportFile << name() << ": "
		     << _runConfig->stageName(istage) << " consumed " 
		     << numSeedsUsed 
		     << " seeds -- more than allowed per event ("
		     << numAllowedSeedsPerEvent << "). run "
		     << _runNumber << ", event " << _eventNumber - 1 << "."
		     << endl;
      }      

      // If this is the primary stage and the tcl parameter is true, abort:
      if ( _abortOnPrimaryOverConsumption->value() && 0 == istage) {
	ErrMsg(fatal) <<  name() << ": "
		      << _runConfig->stageName(istage) << " consumed " 
		      << numSeedsUsed
		      << " seeds -- more than allowed per event ("
		      << numAllowedSeedsPerEvent << "). run "
		      << _runNumber << ", event " << _eventNumber - 1 << "." 
		      << '\n'
		      << " /*   will abort job immediately   */"
		      << endmsg ;
      }
      else {
	ErrMsg(error) << name() << ": "
		      << _runConfig->stageName(istage) << " consumed " 
		      << numSeedsUsed 
		      << " seeds -- more than allowed per event ("
		      << numAllowedSeedsPerEvent << "). run "
		      << _runNumber << ", event " << _eventNumber - 1 << "."
		      << endmsg;
      }      
    }
  }
}


void 
RacRandomControl::checkEidConsumption() const 
{
  // ====== check seed consumption and make reports ======
  for ( int istage = 0 ; istage < _myEidEngines.size() ; istage ++ ) {
    
    const int numSeedsUsed = _myEidEngines[istage]->getCounter();
    
    const int numAllowedSeedsPerStage = 
      _eidConfig->randomsPerStagePerEvent(istage);
      
    // Report that more seeds were used than allowed for this stage:
    if (numAllowedSeedsPerStage < numSeedsUsed){
      if (_reportOverConsumption->value()) {
	if (_reportFile){
	  *_reportFile << name() << ": "
		       << _eidConfig->stageName(istage) << " consumed " 
		       << numSeedsUsed << " seeds with only "
		       << numAllowedSeedsPerStage 
		       << " allowd for this stage."
		       << endl;
	}
 	ErrMsg(error) << name() << ": "
		      << _eidConfig->stageName(istage) << " consumed " 
		      << numSeedsUsed << " seeds with only "
		      << numAllowedSeedsPerStage 
		      << " allowd for this stage."
		      << endmsg;
      }
    }
  }
}

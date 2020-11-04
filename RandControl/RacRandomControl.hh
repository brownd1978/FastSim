//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRandomControl.hh 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRandomControl.
//	  Framework module which will produce a set of the random generators
//	and store them in the AbsEvent. These generators will be retrieved 
//	later by other modules and installed as the static CLHEP generators.
//	For the "names" of the generators and their configuration this module
//	will go to the RacRunCofig class.
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

#ifndef RACRANDOMCONTROL_HH
#define RACRANDOMCONTROL_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <vector>

//----------------------
// Base Class Headers --
//----------------------
#include "Framework/AppModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Framework/AbsParmGeneral.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
class RacRunConfig ;
class RacEidConfig ;
class RacRanecuEngine ;
class AbsParmBool ;
class AbsParmString ;
class AbsParmIfdStrKey ;
class HepTuple ;
#include "CLHEP/Random/RandomEngine.h"
#include <iosfwd>


//		---------------------
// 		-- Class Interface --
//		---------------------

class RacRandomControl : public AppModule {


public:

  // Constructors
  RacRandomControl ( const char* theName = "RandomControl",
		      const char* theDescription = "Control of the random numbers" ) ;
    
  // Destructor
  virtual ~RacRandomControl( );

  // Operators
    
  // Selectors (const)

  // Modifiers
  virtual AppResult  beginJob ( AbsEvent* anEvent );
  virtual AppResult  event    ( AbsEvent* anEvent );
  virtual AppResult  endJob   ( AbsEvent* anEvent );

protected:

  // Helper functions

private:

  // Friends

  // Data members

  std::vector<RacRanecuEngine*> _myGevEngines ; /** engines controlled by Gevent's run 
						    and event number */

  std::vector<RacRanecuEngine*> _myEidEngines ; /** engines gontrolled by EventID's 
						   odfTime value */
  HepRandomEngine* _oldEngine ;                 /** old static enegine */
  RacRunConfig* _runConfig ;                    /** run cofig object which knows everything 
						    about runs, events, stages, etc. */
  RacEidConfig* _eidConfig ;                    /** run cofig object which knows everything 
						    about runs, events, stages, etc. */

  AbsParmGeneral<int> _maxEventsPerRun;         /** set run config parameter */
  AbsParmGeneral<int> _maxRandomsPerEvent;      /** set run config parameter */
  AbsParmGeneral<int>  _randomsPercentPrimary;  /** set run config parameter */
  AbsParmGeneral<int> _randomsPercentSimulation;  /** set run config param */

  AbsParmIfdStrKey* _geventList ;               /** key to get GEvent */
  AbsParmIfdStrKey* _eventIdKey ;               /** key for the event ID */
  AbsParmIfdStrKey* _engineList ;               /** key for the produced engines */
  AbsParmBool* _computeEventSeeds ;             /** when true - calculate seeds on every event,
						    otherwise only on the first event */
  AbsParmBool* _hist ;                          /** produce ntuple when this is true */
  AbsParmBool* _abortOnPrimaryOverConsumption ; /** when true - abort if the comsumption at 
						    the first stage is grater than no. of seeds
						    for the whole event */
  AbsParmBool* _reportOverConsumption ;         /** when true - print message if the consumption 
						    was more that allowed for the stage */
  AbsParmBool* _reportNumSeedsUsed;             /** if true - print seed consumption by each
						    engine */
  AbsParmString* _reportFileName;               /** file name to store report */

  std::ostream * _reportFile;                        /** output file for report */

  size_t _runNumber ;
  size_t _eventNumber ;

  HepTuple* _ntuple ;

  // Note: if your class needs a copy constructor or an assignment
  // operator, make one of the following public and implement it.
  RacRandomControl( const RacRandomControl& );       // Copy Constructor
  RacRandomControl&       operator= ( const RacRandomControl& );  // Assignment op


  // implementation
  void fillNtuple() const ;

  void reportSeeds() const ;

  void checkGevConsumption() const ;
  void checkEidConsumption() const ;
  void checkEventNumber() const;

  //------------------
  // Static Members --
  //------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members
  static const long _ranecuSeeds[2] ;

};

#endif // RACRANDOMCONTROL_HH

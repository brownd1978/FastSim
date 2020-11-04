//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RacRandomPicker.cc 449 2010-01-15 11:42:59Z stroili $
//
// Description:
//	Class RacRandomPicker
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
#include "RandControl/RacRandomPicker.hh"

//-------------
// C Headers --
//-------------
extern "C" {
#include <stdlib.h>
#include <assert.h>
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEvent/AbsEvent.hh"
#include "CLHEP/Alist/AList.h"
#include "CLHEP/Random/Random.h"
#include "ErrLogger/ErrLog.hh"
#include "ProxyDict/Ifd.hh"
#include "ProxyDict/IfdHepAListProxy.hh"
#include "RandControl/RacAbsEnginePtr.hh"
#include "RandControl/RacRunConfig.hh"
#include "RandControl/RacEidConfig.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
RacRandomPicker::RacRandomPicker ( const char* theName, 
				   const char* theDescription ) :
  AppModule ( (std::string("RandomPicker")+std::string(theName)).c_str(), theDescription ),
  _name(theName),
  _inputList ( "inputList", this, "Input" ),
  _randomControlList ( "randomControlList", this, "RandomControl" ),
  _outputList ( "outputList", this, "Output" )
{
  // check that the name is good one
  bool good = false ;

  RacRunConfig runConfig ;
  for ( int i = 0 ; i < runConfig.nStages() ; i ++ ) {
    if ( strcmp(theName,runConfig.stageName(i)) == 0 ) {
      good = true ;
      break ;
    }
  }
  if ( ! good ) {
    RacEidConfig eidConfig ;
    for ( int i = 0 ; i < eidConfig.nStages() ; i ++ ) {
      if ( strcmp(theName,eidConfig.stageName(i)) == 0 ) {
	good = true ;
	break ;
      }
    }
  }
  if ( ! good ) {
    ErrMsg(fatal) << "RacRandomPicker -- bad module name: " << theName << endmsg ;
  }

}

//--------------
// Destructor --
//--------------
RacRandomPicker::~RacRandomPicker ()
{
}

//-------------
// Methods   --
//-------------
AppResult
RacRandomPicker::beginJob ( AbsEvent* ) 
{
  if ( ErrLogging(routine) ) {
    ErrMsg(routine) << name() << " -- starting job" << endmsg;
  }
  return AppResult::OK;
}

//
// the next event
//
AppResult
RacRandomPicker::event ( AbsEvent* anEvent )
{
  HepAList<RacAbsEnginePtr>* list ;
  const RacAbsEnginePtr* engPtr = 0 ;
  bool foundInInputList = false ;
  bool foundInControlList = false ;

  // first look for the engine in the "input" list
  list = Ifd< HepAList<RacAbsEnginePtr> >::get( anEvent, _inputList.value() ) ;
  if ( list ) {
    for ( int i = 0 ; i < list->length() ; i ++ ) {
      const RacAbsEnginePtr* ptr = (*list)[i] ;
      if ( ptr->getName() == _name ) {
	engPtr = ptr ;
	foundInInputList = true ;
	break ;
      }
    }
  }

  if ( ! foundInInputList ) {
    // then look in the "RandomControl" list
    list = Ifd< HepAList<RacAbsEnginePtr> >::get( anEvent, _randomControlList.value() );
    if ( list ) {
      for ( int i = 0 ; i < list->length() ; i ++ ) {
	const RacAbsEnginePtr* ptr = (*list)[i] ;
	if ( ptr->getName() == _name ) {
	  engPtr = ptr ;
	  foundInControlList = true ;
	  break ;
	}
      }
    }
  }

  if ( ! foundInInputList && ! foundInControlList ) {
    // no engine with such name was found
    ErrMsg(error) << name() << " -- cannot find random engine in the AbsEvent" << endmsg ;
  } else {

    // set it as the static CLHEP engine
    HepRandom::setTheEngine ( engPtr->getEngine() ) ;
    if ( ErrLogging(debugging) ) {
      ErrMsg(debugging) << name() << " -- setting engine " << (*engPtr) 
			<< " found in "
			<< ( foundInInputList ? _inputList.value() : _randomControlList.value() )
			<< " list" 
			<< endmsg ;
    }

    // save it in the output list in the case it was found in the "RandomControl" list
    // note that engine must be saved together with its state (means that only seeds
    // need to be saved) so that later calls to the engine do not change the
    // state of the engine in the output list
    if ( foundInControlList ) {

      // first look for the "output" list
      HepAList<RacAbsEnginePtr>* outlist = 
	Ifd< HepAList<RacAbsEnginePtr> >::get( anEvent, _outputList.value() ) ;
      if ( ! outlist ) {
	// it does not exist yet - need to create one
	HepAList<RacAbsEnginePtr>* newlist = new HepAList<RacAbsEnginePtr> ;
	Ifd< HepAList<RacAbsEnginePtr> >::put 
	  ( anEvent, 
	    new IfdHepAListProxy<RacAbsEnginePtr>(newlist),
	    _outputList.value() ) ;
	outlist = newlist ;
      }

      // check that no such engine in the list yet
      bool alreadyExist = false ;
      for ( int i = 0 ; i < outlist->length() ; i ++ ) {
	const RacAbsEnginePtr* ptr = (*outlist)[i] ;
	if ( ptr->getName() == engPtr->getName() ) {
	  alreadyExist = true ;
	  break ;
	}
      }

      if ( alreadyExist ) {
	ErrMsg(error) << name() << " -- engine with the name " << engPtr->getName() 
		      << " is already in the output list" << endmsg ;
      } else {
	// make a new engine and put it in the output list
	RacAbsEnginePtr* neweng = engPtr->clone() ;
	outlist->append ( neweng ) ;
	if ( ErrLogging(debugging) ) {
	  ErrMsg(debugging) << name() << " -- putting engine " << (*neweng) 
			    << " in the " << _outputList.value() << " list" << endmsg ;
	}
      }
      
    }
  }

  return AppResult::OK;
}

AppResult
RacRandomPicker::endJob ( AbsEvent* )
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

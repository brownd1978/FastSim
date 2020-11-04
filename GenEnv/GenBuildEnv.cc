//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: GenBuildEnv.cc 510 2010-01-14 15:28:09Z stroili $
//
// Description:
//	Class GenBuildEnv. 
//      This is the class which builds the Gen environment GenEnv.  It has 
//      memory managment repsonsiblities for GenEnv and ntupleManager. 
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//       Phil Strother                  Imperial College
//
// Copyright Information:
//	Copyright (C) 1994		Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "GenEnv/GenBuildEnv.hh"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <iomanip>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "GenEnv/GenEnv.hh"
#include "ErrLogger/ErrLog.hh"
#include "BdbTime/BdbTime.hh"
using std::cout;
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

struct GenBuildEnvData
{
  GenEnv* myEnv;
  BdbTime _time; // cache of initialization time
};

//----------------
// Constructors --
//----------------

GenBuildEnv::GenBuildEnv( const char* const theName, 
			  const char* const theDescription )
  : AppModule( theName, theDescription ),
    _privateData( new GenBuildEnvData )
{ 
  _privateData->myEnv = 0;
}

//--------------
// Destructor --
//--------------
GenBuildEnv::~GenBuildEnv()
{
  if ( _privateData->myEnv != 0 )
    {
      delete _privateData->myEnv;
      _privateData->myEnv=0;
    }
  delete _privateData;
  _privateData=0;
  gblEnv->resetGen();
}

//-------------
// Methods   --
//-------------
    
AppResult
GenBuildEnv::beginJob( AbsEvent* )
{
  if ( _verbose.value() ) ErrMsg( routine ) << name( ) << " begin Job"
					    << endmsg;

  // Create the GenEnv object
  _privateData->myEnv = new GenEnv;

  // initialize the environment time
  _privateData->myEnv->setEventTime(&(_privateData->_time));

  // Now put the finished GenEnv in the environment
  gblEnv->setGen( _privateData->myEnv );

  return AppResult::OK;
}

AppResult
GenBuildEnv::event( AbsEvent* )
{
  return AppResult::OK;
}

AppResult
GenBuildEnv::other( AbsEvent* )
{
  if ( _verbose.value() ) ErrMsg( routine ) << name( ) << " other" << endmsg;
    return AppResult::OK;
}

AppResult
GenBuildEnv::endJob( AbsEvent* )
{
  if ( _verbose.value() ) ErrMsg( routine ) << name( ) << " end Job"
					    << endmsg;
    return AppResult::OK;
}

AppResult
GenBuildEnv::abortJob( AbsEvent* )
{
  if ( _verbose.value() ) ErrMsg( routine ) << name( ) << " abort Job"
					    << endmsg;
    return AppResult::OK;
}

void
GenBuildEnv::help( int argc, char** argv )
{
  cout << "This module is to construct the general environment." << endl;

  AppModule::help(argc, argv) ;
}

void
GenBuildEnv::help()
{
  cout << "This module is to construct the general environment." << endl;

  AppModule::help() ;
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














//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: GenEnv.cc 510 2010-01-14 15:28:09Z stroili $
//
// Description:
//	Class GenEnv
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		        Original Author
//      Stephen J. Gowdy                University Of Edinburgh
//      Dave Brown                      LBL
//
// Copyright Information:
//	Copyright (C) 1995, 1996	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "GenEnv/GenEnv.hh"

//-------------
// C Headers --
//-------------
#include <stdlib.h>

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "AbsEnv/AbsEnv.hh"
#include "EidData/EidCondKeyTriplet.hh"
#include "ErrLogger/ErrLog.hh"
#include "GenEnv/GenEnvData.hh"
#include "GenEnv/MaterialInfo.hh"
#include "ProxyDict/IfdSimpleProxyDict.hh"
using std::endl;

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------


//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
GenEnv::GenEnv()
  : _privateData( new GenEnvData ), _matinfo( 0 ),
    _empty( new std::vector< std::string > )
{
}

GenEnv::GenEnv( const GenEnv& )
{
  ::abort();
}

//--------------
// Destructor --
//--------------
GenEnv::~GenEnv()
{
  delete _privateData;
  _privateData=0;
  delete _empty;
  _empty=0;
}

//-------------
// Methods   --
//-------------

//-------------
// Operators --
//-------------
GenEnv&
GenEnv::operator=( const GenEnv&  )
{
  if ( this != 0 ) ::abort();
  return *this;
}

//-------------
// Selectors --
//-------------

const DetMaterial*
GenEnv::findDetMaterial( const std::string& matName ) const
{
  if(_matinfo != 0)
    return _matinfo->findDetMaterial(matName);
  else {
    ErrMsg( warning ) << "GenEnv has no materials defined " << endl
		      << " Please add MatBuildEnv to your AppUserBuild -AFTER- GenBuildEnv " << endmsg;
    return 0;
  }
}

const std::vector< std::string >&
GenEnv::materialNames() const {
  if(_matinfo != 0)
    return _matinfo->materialNames();
  else {
    ErrMsg( warning ) << "GenEnv has no materials defined " << endl
		      << " Please add MatBuildEnv to your AppUserBuild -AFTER- GenBuildEnv " << endmsg;
    return *_empty;
  }
}
  

MaterialInfo*&
GenEnv::materialInfo() {
  return _matinfo;
}

const BdbTime*
GenEnv::eventTime() const
{
  return _privateData->_eventTime;
}

const EidCondKeyTriplet*
GenEnv::mixedBkgCondKey() const
{
  return _privateData->_bkgKey;
}

const EidCondKeyTriplet*
GenEnv::primaryCondKey() const
{
  return _privateData->_primaryKey;
}

//-------------
// Modifiers --
//-------------
HepTupleManager*&
GenEnv::ntupleManager(const std::string& managername)
{
  return _privateData->_ntupleManager[managername];
}

HepTupleManager*&
GenEnv::ntupleManager()
{
// default value is RooTupleEnv
  static std::string defname("RooTupleEnv");
  return ntupleManager(defname);
}

HepScalerManager*&
GenEnv::scalerManager()
{
  return _privateData->_scalerManager;
}

void
GenEnv::setEventTime( const BdbTime* newtime )
{
  // set the time in GenEnv, and force all proxies to verify their caches
  _privateData->_eventTime = newtime;
  // we need to have at least a primary CondKey, so create a wildcarded one
  static EidCondKeyTriplet ourPrimaryKey;
  ourPrimaryKey = EidCondKeyTriplet(0,0,*newtime);
  _privateData->_primaryKey = &ourPrimaryKey;

  _privateData->_bkgKey = 0;
  gblPEnv->testProxyCaches();
}

void
GenEnv::setConditionsKeys( const BdbTime* time,
			  const EidCondKeyTriplet* primary, 
			  const EidCondKeyTriplet* bkg)
{
  // set the conditions keys in GenEnv, and force all proxies to verify their caches
  _privateData->_primaryKey = primary;
  _privateData->_bkgKey = bkg;
  _privateData->_eventTime = time;
  gblPEnv->testProxyCaches();
}


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


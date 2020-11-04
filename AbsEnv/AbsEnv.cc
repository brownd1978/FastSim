//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsEnv.cc 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//	Class AbsEnv
//
//      See header for more info
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Bob Jacobsen		Original Author
//      Stephen J. Gowdy        Revised
//
// Copyright Information:
//	Copyright (C) 1995, 1996	Lawrence Berkeley Laboratory
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsEnv/AbsEnv.hh"

//-------------
// C Headers --
//-------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ProxyDict/IfdSimpleProxyDict.hh"
#include "ErrLogger/ErrLog.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

static AbsEnv initialEnv;
AbsEnv* gblEnv = &initialEnv;
static IfdSimpleProxyDict initialProxy;
IfdProxyDict* gblPEnv = &initialProxy;

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

AbsEnv::AbsEnv()
  : _genEnv( 0 ),
    _pepEnv( 0 ),
    _emcEnv( 0 ),
    _dchEnv( 0 ),
    _svtEnv( 0 ),
    _ifrEnv( 0 ),
    _drcEnv( 0 ),
    _trkEnv( 0 ),
    _l1DctEnv( 0 ),
    _l1EmtEnv( 0 ),
    _l1GltEnv( 0 ),
    _l3tEnv( 0 ),
    _oprEnv( 0 ),
    _cfgEnv( 0 ),
    _dhpMonEnv( 0 ),
    _filtEnv( 0 ),
    _dnaEnv( 0 )
{
}

//--------------
// Destructor --
//--------------
AbsEnv::~AbsEnv()
{
}

//-------------------------------
// members
//-------------------------------
GenEnv*
AbsEnv::getGen()
{
  return _genEnv;
}

PepEnv*
AbsEnv::getPep()
{
  return _pepEnv;
}

EmcEnv*
AbsEnv::getEmc()
{
  return _emcEnv;
}

DchEnv*
AbsEnv::getDch()
{
  return _dchEnv;
}

SvtEnv*
AbsEnv::getSvt()
{
  return _svtEnv;
}

IfrEnv*
AbsEnv::getIfr()
{
  return _ifrEnv;
}

DrcEnv*
AbsEnv::getDrc()
{
  return _drcEnv;
}

TrkEnv*
AbsEnv::getTrk()
{
  return _trkEnv;
}

L1DctEnv*
AbsEnv::getL1Dct()
{
  return _l1DctEnv;
}

L1EmtEnv*
AbsEnv::getL1Emt()
{
  return _l1EmtEnv;
}

L1GltEnv*
AbsEnv::getL1Glt()
{
  return _l1GltEnv;
}

L3TEnv*
AbsEnv::getL3T()
{
  return _l3tEnv;
}

OprEnv*
AbsEnv::getOpr()
{
  return _oprEnv;
}

CfgEnv*
AbsEnv::getCfg()
{
  return _cfgEnv;
}

BdbConfigEnv*
AbsEnv::getBdbConfig()
{
  return _bdbConfigEnv;
}

DhpMonEnv*
AbsEnv::getDhpMon()
{
  return _dhpMonEnv;
}

BtaEnv*
AbsEnv::getBta()
{
  return _btaEnv;
}

FiltEnv*
AbsEnv::getFilt()
{
  return _filtEnv;
}

DnaEnv*
AbsEnv::getDna()
{
    return _dnaEnv;
}

void
AbsEnv::setGen( GenEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding GenEnv object in global environment." 
		    << endmsg;
  }
  _genEnv=envPointer;
  first = false;
}

void
AbsEnv::setPep( PepEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding PepEnv object in global environment." 
		    << endmsg;
  }
  _pepEnv=envPointer;
  first = false;
}

void
AbsEnv::setEmc( EmcEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding EmcEnv object in global environment." 
		    << endmsg;
  }
  _emcEnv=envPointer;
  first = false;
}

void
AbsEnv::setDch( DchEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding DchEnv object in global environment." 
		    << endmsg;
  }
  _dchEnv=envPointer;
  first = false;
}

void
AbsEnv::setSvt( SvtEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding SvtEnv object in global environment." 
		    << endmsg;
  }
  _svtEnv=envPointer;
  first = false;
}

void
AbsEnv::setIfr( IfrEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding IfrEnv object in global environment." 
		    << endmsg;
  }
  _ifrEnv=envPointer;
  first = false;
}

void
AbsEnv::setDrc( DrcEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding DrcEnv object in global environment." 
		    << endmsg;
  }
  _drcEnv=envPointer;
  first = false;
}

void
AbsEnv::setTrk( TrkEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding TrkEnv object in global environment." 
		    << endmsg;
  }
  _trkEnv=envPointer;
  first = false;
}

void
AbsEnv::setL1Dct( L1DctEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding L1DctEnv object in global environment." 
		    << endmsg;
  }
  _l1DctEnv=envPointer;
  first = false;
}

void
AbsEnv::setL1Emt( L1EmtEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding L1EmtEnv object in global environment." 
		    << endmsg;
  }
  _l1EmtEnv=envPointer;
  first = false;
}

void
AbsEnv::setL1Glt( L1GltEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding L1GltEnv object in global environment." 
		    << endmsg;
  }
  _l1GltEnv=envPointer;
  first = false;
}

void
AbsEnv::setL3T( L3TEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding L3TEnv object in global environment." 
		    << endmsg;
  }
  _l3tEnv=envPointer;
  first = false;
}

void
AbsEnv::setOpr( OprEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding OprEnv object in global environment." 
		    << endmsg;
  }
  _oprEnv=envPointer;
  first = false;
}

void
AbsEnv::setCfg( CfgEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding CfgEnv object in global environment." 
		    << endmsg;
  }
  _cfgEnv=envPointer;
  first = false;
}

void
AbsEnv::setBdbConfig( BdbConfigEnv* envPointer )
{
  ErrMsg(warning) << "AbsEnv::setBdbConfig -- deprecated." << endmsg;

  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding BdbConfigEnv object in global environment." 
		    << endmsg;
  }
  _bdbConfigEnv=envPointer;
  first = false;
}

void
AbsEnv::setDhpMon( DhpMonEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding DhpMonEnv object in global environment." 
		    << endmsg;
  }
  _dhpMonEnv=envPointer;
  first = false;
}

void
AbsEnv::setBta( BtaEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding BtaEnv object in global environment." 
		    << endmsg;
  }
  // Beta allows two different levels of environment
  // Setting of second is legitimate since the
  // second is designed to encompass the first
  _btaEnv=envPointer;
  first = false;
}


void
AbsEnv::setFilt( FiltEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding FiltEnv object in global environment." 
		    << endmsg;
  }
  _filtEnv=envPointer;
  first = false;
}

void
AbsEnv::setDna( DnaEnv* envPointer )
{
  static bool first = true;
  if ( !first ) {
    ErrMsg(warning) << "Overriding DnaEnv object in global environment." 
		    << endmsg;
  }
  _dnaEnv=envPointer;
  first = false;
}





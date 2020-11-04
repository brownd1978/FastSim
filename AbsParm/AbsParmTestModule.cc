//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsParmTestModule.cc 489 2010-01-13 16:45:39Z stroili $
//
// Description:
//      Implementation.  See .hh for commments.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank                   University of Pennsylvania
//
// Copyright Information:
//	Copyright (C) 1994
//
// History:
//      16Dec98  Ed Frank          First version
//      13Jul99  Michael Kelsey    Add testing of AbsParmNamedObject
//
// BUGS:
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsParm/AbsParmTestModule.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "ErrLogger/ErrLog.hh"
using std::ostream;

//-----------------
// Constructors ---
//-----------------

AbsParmTestModule::AbsParmTestModule(const char* const theName, 
				     const char* const theDescription) 
  : AppModule(theName, theDescription)
  , _doubleParmList( "doubleParmList", this )
  , _floatParmList(  "floatParmList",  this )
  , _longParmList(   "longParmList",   this )
  , _intParmList(    "intParmList",    this )
  , _stringParmList( "stringParmList", this )
  , _boolParmList(    "boolParmList",    this )
  , _enumParm(       "enumParm",       this )
  , _symIntParmList( "symbolicIntList", this)
  , _pairList("pairList", this)
{ 
  commands()->append( &_doubleParmList );
  commands()->append( &_floatParmList );
  commands()->append( &_longParmList );
  commands()->append( &_intParmList );
  commands()->append( &_boolParmList );
  commands()->append( &_stringParmList );

  commands()->append( &_enumParm );		// Test actual function
  _enumParm.addItem("APEVal2",APEVal2);		// First value is default
  _enumParm.addItem("APEVal1",APEVal1);
  _enumParm.addItem("APEVal3",APEVal3);

  commands()->append (&_symIntParmList);
  _symIntParmList.addItem("Power0", 0);
  _symIntParmList.addItem("Power1", 10);
  _symIntParmList.addItem("Power2", 100);
  _symIntParmList.addItem("Power3", 1000);

  commands()->append(&_pairList);
}

AbsParmTestModule::AbsParmTestModule( const char* const newName,
                                      const char* const theDescription, 
		   		      const AbsParmTestModule& other )
  : AppModule( newName, theDescription )
  , _doubleParmList( other._doubleParmList, this )
  , _floatParmList(  other._floatParmList, this )
  , _longParmList(   other._longParmList, this )
  , _intParmList(    other._intParmList, this )
  , _stringParmList( other._stringParmList, this )
  , _boolParmList(    other._boolParmList, this )
  , _enumParm(       other._enumParm, this )
  , _symIntParmList( other._symIntParmList, this)
  , _pairList( other._pairList, this)
{ 
  commands()->append( &_doubleParmList );
  commands()->append( &_floatParmList );
  commands()->append( &_longParmList );
  commands()->append( &_intParmList );
  commands()->append( &_boolParmList );
  commands()->append( &_stringParmList );
  commands()->append( &_enumParm );	
  commands()->append( &_symIntParmList);
  commands()->append(&_pairList);
}

AbsParmTestModule::~AbsParmTestModule() {
}

AppResult
AbsParmTestModule::beginJob( AbsEvent* anEvent )
{
  ErrMsg(routine) << "Double list has " << _doubleParmList.value().size()
		  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _doubleParmList.print(o);
    o << endmsg;
  }

  ErrMsg(routine) << "Float list has :" << _floatParmList.value().size()
		  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _floatParmList.print(o);
    o << endmsg;
  }

  ErrMsg(routine) << "Long list has:" <<  _longParmList.value().size()
		  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _longParmList.print(o);
    o << endmsg;
  }

  ErrMsg(routine) << "Int list:" <<  _intParmList.value().size()
		  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _intParmList.print(o);
    o << endmsg;
  }

  ErrMsg(routine) << "String list:" << _stringParmList.value().size()
		  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _stringParmList.print(o);
    o << endmsg;
  }

  ErrMsg(routine) << "Bool list:" <<  _boolParmList.value().size()
		  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _boolParmList.print(o);
    o << endmsg;
  }

  ErrMsg(routine) << "Symbolic long list:" << _symIntParmList.value().size()
                  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _symIntParmList.print(o);
    o << endmsg;
  }

  ErrMsg(routine) << "Symbol parameter:" << _enumParm << endmsg;

  ErrMsg(routine) << "Pair list:" <<  _pairList.value()->size()
		  << " entries: " <<endmsg;
  {
    ostream& o = ErrMsg(routine);
    _pairList.print(o);
    o << endmsg;
  }

  return AppResult::OK;
}

AppResult
AbsParmTestModule::event( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
AbsParmTestModule::endJob  ( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppResult
AbsParmTestModule::abortJob( AbsEvent* anEvent )
{
  return AppResult::OK;
}

AppModule* AbsParmTestModule::clone(const char* cloneName) {
  return new AbsParmTestModule( cloneName, "Cloned module", *this);
}







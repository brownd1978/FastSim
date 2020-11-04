//-----------------------------------------------------------------------------
//
// File and Version Information:
// 	$Id: BdbIntervalBase.cc 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//	Class BdbIntervalBase
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Dave Brown, 11-12-97
//
// Copyright Information:
//	Copyright (C) 1997	Lawrence Berkeley Laboratory
//
//-----------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
//-----------------------
// This Class's Header --
//-----------------------
#include "BdbTime/BdbIntervalBase.hh"
using std::ostream;

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
 
//----------------
// Constructors --
//----------------
 
BdbIntervalBase::BdbIntervalBase( const BdbTime& begin, const BdbTime& end ) :
  _beginTime( begin ), _endTime( end )
{
}

BdbIntervalBase::BdbIntervalBase(const BdbIntervalBase& other ) :
  _beginTime( other._beginTime ), _endTime( other._endTime )
{
}

// overloaded stream-insertion operator <<
ostream & operator << ( ostream& os, const BdbIntervalBase& interval )
{
  os << "Start time = "  << interval.beginTime( )
     << " , End time = " << interval.endTime( );
  return os;
}





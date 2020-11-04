
//-----------------------------------------------------------------------------
//
// File and Version Information:
// 	$Id: BdbIntervalBase.hh 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//	Class BdbIntervalBase. Purely transient class describing a time interval,
//      base for the BdbInterval class used in database lookups.
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

#ifndef BDBINTERVALBASE_HH
#define BDBINTERVALBASE_HH

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BdbTime/BdbTime.hh"
//		---------------------
// 		-- Class Interface --
//		---------------------

class BdbIntervalBase {

public:

  // Constructors
  BdbIntervalBase( const BdbTime& startTime = BdbTime::minusInfinity,
		   const BdbTime& endTime   = BdbTime::plusInfinity   );

  BdbIntervalBase( const BdbIntervalBase& other );

  // Destructor
  virtual ~BdbIntervalBase( )
    { 
    }

  // Operators
  BdbIntervalBase & operator = ( const BdbIntervalBase& other )
    {
      if( &other != this ) 
	{
	  _beginTime = other._beginTime;
	  _endTime   = other._endTime;
	}
      return *this;
    }

  // Selectors (const)
  BdbTime getBeginTime( ) const 
    {
      return _beginTime; 
    }

  BdbTime getEndTime( ) const 
    {
      return _endTime; 
    }

  const BdbTime& beginTime( ) const 
    { 
      return _beginTime;
    }

  const BdbTime& endTime( ) const 
    { 
      return _endTime; 
    }

  // Modifiers
  void setBeginTime( const BdbTime& theTime ) 
    {
      _beginTime = theTime; 
    }

  void setEndTime( const BdbTime& theTime ) 
    {
      _endTime = theTime; 
    }

  // Member Functions
  bool inInterval( const BdbTime& time ) const 
    {
      return ( time >= _beginTime && time < _endTime ); 
    }

  bool inInterval( const BdbIntervalBase& other ) const 
    {
      return ( other._beginTime >= _beginTime &&
	       other._endTime   <= _endTime      ); 
    }

  bool operator == ( const BdbIntervalBase & o   ) const     
    { 
      return (    _beginTime  == o._beginTime 
	       && _endTime    == o._endTime     );
    }


  // overloaded stream-insertion operator <<
  friend std::ostream & operator << ( std::ostream& os, const BdbIntervalBase& t );   


protected:

  // Data members
  BdbTime            _beginTime;
  BdbTime            _endTime;

};
#endif













































































































































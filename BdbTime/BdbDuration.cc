//-----------------------------------------------------------------------------
//
// File and Version Information:
//      $Id: BdbDuration.cc 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//      Class BdbDuration.  
//      This is a time duration class.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      J. Ohnemus                      Original Author
//
// Copyright Information:
//      Copyright (C) 1994, 1995        Lawrence Berkeley Laboratory
//
//-----------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "BdbTime/BdbDuration.hh"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iostream>
#include <iomanip>
using std::cout;
using std::ostream;
using std::setw;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//              ----------------------------------------
//              -- Public Member Function Definitions --
//              ----------------------------------------
 
//----------------
// Constructors --
//----------------

BdbDuration::BdbDuration( ) 
: _sec( 0 ), _nsec( 0 )
{
}

BdbDuration::BdbDuration( const BdbDuration & d ) 
: _sec( d._sec ), _nsec( d._nsec )
{
}

BdbDuration::BdbDuration( d_ULong sec, d_ULong nsec  ) 
{
    if ( nsec >= BdbTimeConst::nsecInASec )
	{
	    // carry nanoseconds over into seconds
	    d_ULong extraSec   = nsec / BdbTimeConst::nsecInASec;
	    d_ULong remainNsec = nsec % BdbTimeConst::nsecInASec;
	    sec               += extraSec;
	    nsec               = remainNsec;
	}

    _sec  = sec;
    _nsec = nsec;
}


//-------------
// Operators --
//-------------

// d = |d2 - d1|
BdbDuration BdbDuration::operator - ( const BdbDuration & d1 ) const
{
    // This code forms |d2-d1| without having to use signed intergers.

    d_ULong d2Sec;
    d_ULong d2Nsec;
    d_ULong d1Sec;
    d_ULong d1Nsec;
   
    if ( *this > d1 )
	{
	    d2Sec  = _sec;
	    d2Nsec = _nsec;
	    d1Sec  = d1._sec;
	    d1Nsec = d1._nsec;
	}
    else
	{
	    d2Sec  = d1._sec;
	    d2Nsec = d1._nsec;
	    d1Sec  = _sec;
	    d1Nsec = _nsec;
	}

    if ( d2Nsec < d1Nsec )
	{
	    // borrow a second from d2Sec
	    d2Nsec += BdbTimeConst::nsecInASec;
	    d2Sec--;
	}

    d_ULong sec  = d2Sec  - d1Sec;
    d_ULong nsec = d2Nsec - d1Nsec;

    BdbDuration diff( sec, nsec );
    
    return diff;
}

// d = d2 + d1
BdbDuration BdbDuration::operator + ( const BdbDuration & d1 ) const
{
    d_ULong totalSec  = _sec  + d1._sec;
    d_ULong totalNsec = _nsec + d1._nsec;

    if ( totalNsec >= BdbTimeConst::nsecInASec )
	{
	    // carry nanoseconds over into seconds
	    d_ULong extraSec   = totalNsec / BdbTimeConst::nsecInASec;
	    d_ULong remainNsec = totalNsec % BdbTimeConst::nsecInASec;
	    totalSec          += extraSec;
	    totalNsec          = remainNsec;
	}

    BdbDuration sum( totalSec, totalNsec );
    
    return sum;
}

// d2 = d1
BdbDuration & BdbDuration::operator = ( const BdbDuration & d1 ) 
{
    if ( this == &d1 )
	{
	    return *this;
	}

    _sec  = d1._sec;
    _nsec = d1._nsec;

    return *this;
}

// d2 += d1
BdbDuration & BdbDuration::operator += ( const BdbDuration & d1 ) 
{
    d_ULong totalSec  = _sec  + d1._sec;
    d_ULong totalNsec = _nsec + d1._nsec;

    if ( totalNsec >= BdbTimeConst::nsecInASec )
	{
	    // carry nanoseconds over into seconds
	    d_ULong extraSec   = totalNsec / BdbTimeConst::nsecInASec;
	    d_ULong remainNsec = totalNsec % BdbTimeConst::nsecInASec;
	    totalSec          += extraSec;
	    totalNsec          = remainNsec;
	}

    _sec  = totalSec;
    _nsec = totalNsec;
    
    return *this;
}

//-----------
// Friends --
//-----------

// overloaded stream-insertion operator <<
ostream & operator << ( ostream & os, const BdbDuration & d )
{
    if ( d._nsec == 0 )
	{
	    os << d._sec << " sec";
	    return os;
	}
    else
	{
	    cout.fill( '0' );
	    os << d._sec << "." << setw(9) << d._nsec << " sec";
	    cout.fill( ' ' );
	    return os;
	}
}










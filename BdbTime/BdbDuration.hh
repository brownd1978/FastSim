#ifndef BDBDURATION_HH
#define BDBDURATION_HH

//-----------------------------------------------------------------------------
//
// File and Version Information:
//      $Id: BdbDuration.hh 496 2010-01-13 17:10:44Z stroili $
//
// Description:
//      Class BdbDuration.  
//      This is a time duration class.  
//      Note that a time duration is positive definite, thus 
//      d = abs( d1 - d2 ), 
//      and
//      d = abs( t1 - t2 ).
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
 
//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBarODMGTypes.h"

//-------------
// C Headers --
//-------------
extern "C" {
}
 
//---------------
// C++ Headers --
//---------------
#include <iostream>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "BdbTime/BdbTimeConst.hh"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
 
//              ---------------------
//              -- Class Interface --
//              ---------------------

class BdbDuration {

//--------------------
// Instance Members --
//--------------------

public:
    
    // Constructors
    BdbDuration( );                               // default
    
    BdbDuration( const BdbDuration & d );         // copy

    BdbDuration( d_ULong sec, d_ULong nsec = 0 );


    // Destructor
    ~BdbDuration( ) { }
    

    // Operators
    BdbDuration   operator  - ( const BdbDuration & d1 ) const;            

    BdbDuration   operator  + ( const BdbDuration & d1 ) const;            

    BdbDuration & operator  = ( const BdbDuration & d1 );            

    BdbDuration & operator += ( const BdbDuration & d1 );            

    bool operator == ( const BdbDuration & d ) const    
	{ 
	    return ( _sec  == d._sec && _nsec == d._nsec );
	}

    bool operator != ( const BdbDuration & d ) const 
	{ 
	    return !( *this == d ); 
	}

    bool operator <  ( const BdbDuration & d ) const  
	{ 
	    return ( _sec < d._sec ) || ( _sec == d._sec && _nsec < d._nsec );
	}

    bool operator <= ( const BdbDuration & d ) const
	{ 
	    return ( _sec < d._sec ) || ( _sec == d._sec && _nsec <= d._nsec );
	}

    bool operator >  ( const BdbDuration & d ) const 
	{ 
	    return !( *this <= d );
	}

    bool operator >= ( const BdbDuration & d ) const 
	{ 
	    return !( *this <  d ); 
	}

    // Selectors
    d_ULong getSec ( ) const { return _sec;  }
    d_ULong getNsec( ) const { return _nsec; }

    // Friends
    friend std::ostream & operator << ( std::ostream & os, const BdbDuration & d );   

private:

    // Data members
    d_ULong _sec;         // number of seconds
    d_ULong _nsec;        // number of nano seconds

};

#endif

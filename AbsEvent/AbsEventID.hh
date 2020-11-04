//------------------------------------------------------------------------------
// File:
//      AbsEventID.hh
//
// Description:
//      The BaBar abstract event ID header file. 
//  
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      David R. Quarrie
//
// Copyright Information:
//      Copyright (C) 1998      Lawrence Berkeley National Laboratory
//
//------------------------------------------------------------------------------

#ifndef ABSEVENTID_HH
#define ABSEVENTID_HH

//-----------------
// BaBar Headers --
//-----------------
#include "BaBar/BaBar.hh"
#include "BaBar/BaBarODMGTypes.h"

class EidEventTriplet;
class EidCondKeyTriplet;
#include <iosfwd>

//      ----------------------
//      -- Class Interfaces --
//      ----------------------
 
class AbsEventID  {

//--------------------
// Instance Members --
//--------------------

public:

    // Destructor
    virtual ~AbsEventID( );

    // Selectors
    //   ** majorID() and minorID() are obsolete.  They are maintained only
    //   ** to maintain backwards compatabillity w/ old data.  They
    //   ** may be removed.  If present, they represent the two halves
    //   ** of the BdbTime from the conditions database Key.

    virtual long majorID( ) const = 0;
    virtual long minorID( ) const = 0;

    // Modifiers
    //   ** setID is obsolete.  See comments for majorID and minorID.
    virtual void setID( long major, long minor=0 ) = 0;

    // Accessors

    virtual const EidEventTriplet    eventIdTriplet( void )  const = 0;
    virtual const EidCondKeyTriplet  condKeyTriplet ( void ) const = 0;

    virtual d_ULong                  run( void )             const = 0;
    virtual d_ULong                  configKey( void )       const = 0;
  
    // Confabulators

    friend std::ostream& operator << (std::ostream& o, const AbsEventID& id);

       // There are no guarantees at the present time that the
       // detailed format of this printout will be maintained over time
       // nor that it will be invertible.  We will provide those capabilities
       // later.

protected:

    // Constructors
    AbsEventID( );

};

#endif /* ABSEVENTID_HH */




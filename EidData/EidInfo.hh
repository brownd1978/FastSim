#ifndef EIDINFO_HH
#define EIDINFO_HH
//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: EidInfo.hh 451 2010-01-14 13:03:45Z stroili $
//
// Description:
//    Concrete AbsEventID.
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      01 Jun 99         Creation of first version
//               
// Bugs:
//
//------------------------------------------------------------------------

#include "AbsEvent/AbsEventID.hh"
#include "BaBar/BaBarODMGTypes.h"

#include "EidData/EidEventTriplet.hh"
#include "EidData/EidCondKeyTriplet.hh"

class EidInfo : public AbsEventID {
public:
    virtual ~EidInfo();

    // Warning:  EidInfo must be completely constructable from the
    //           information in the event header, BdbEvent.  Thus we
    //           do not give access to EidEventContext.

    EidInfo( const EidEventTriplet&   eidTriplet,
	     const EidCondKeyTriplet& eidCondTriplet,
	     d_ULong                  configKey,
	     d_ULong                  run
	   );

    EidInfo( const EidInfo& rhs);
    EidInfo& operator = (const EidInfo& rhs);

    // Selectors
    //   ** majorID() and minorID() are obsolete.  They are maintained only
    //   ** to maintain backwards compatabillity w/ old data.  They
    //   ** may be removed.  If present, they represent the two halves
    //   ** of the BdbTime from the conditions database Key.

    virtual long majorID( ) const;
    virtual long minorID( ) const;

    // Modifiers
    //   ** setID is obsolete.  See comments for majorID and minorID.

    virtual void setID( long major, long minor=0 );

    // Accessors

    virtual const EidEventTriplet    eventIdTriplet( void ) const;
    virtual const EidCondKeyTriplet  condKeyTriplet ( void ) const;

    virtual d_ULong                  run( void ) const;
    virtual d_ULong                  configKey( void ) const;

private:
   EidEventTriplet     _eidTriplet;
   EidCondKeyTriplet   _eidCondTriplet;
   d_ULong             _configKey;
   d_ULong             _run;
};


#endif /* EIDINFO_HH */

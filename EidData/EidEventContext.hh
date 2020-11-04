#ifndef EIDEVENTCONTEXT_HH
#define EIDEVENTCONTEXT_HH
//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: EidEventContext.hh 451 2010-01-14 13:03:45Z stroili $
//
// Description:
//    Holds info related to per event identification/manipulation.  The
//   other info is in the EidEventTriplet and EidcondKeyTriplet classes.
//   Why don't we put the context info into the other classes?  One
//   reason is so that we can makea persistent analoge of this class
//   and store it in the data _separate_ from the event header, even if
//   we do put it in the header too.  This redundancy may help us fix
//   corrupt data.
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

#include  "AbsEvent/AbsEvtObj.hh"
#include "BaBar/BaBarODMGTypes.h"
#include <time.h>
#include <netinet/in.h>

class EidEventContext : public AbsEvtObj{
public:
  EidEventContext( d_ULong configKey,
		   d_Long run,
		   const timespec& oepReceiveTime,
		   const in_addr&  oepNodeIp
		 );

  EidEventContext();

  EidEventContext( const EidEventContext& rhs );
  EidEventContext& operator = ( const EidEventContext& rhs );

  virtual ~EidEventContext();

  inline d_ULong           configKey(void) const;
  inline d_ULong           run( void ) const;
  inline const timespec&   oepReceiveTime( void ) const;
  inline const in_addr&    oepNodeIp( void ) const;

private:
  d_ULong     _configKey;
  d_ULong     _run;
  timespec    _oepReceiveTime;
  in_addr     _oepNodeIp;
};

//*****************************************************************************
d_ULong
EidEventContext::configKey(void)       const { return _configKey; }

d_ULong
EidEventContext::run( void )           const { return _run; }

const timespec&
EidEventContext::oepReceiveTime( void) const {return _oepReceiveTime;}

const in_addr&
EidEventContext::oepNodeIp( void )const { return _oepNodeIp; }


#endif /* EIDEVENTCONTEXT_HH */

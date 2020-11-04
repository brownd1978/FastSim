//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: EidEventContext.cc 451 2010-01-14 13:03:45Z stroili $
//
// Description:
//    See .hh for details.
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


#include "BaBar/BaBar.hh"
#include "EidData/EidEventContext.hh"

EidEventContext::EidEventContext(
				 d_ULong configKey,
				 d_Long run,
				 const timespec& oepReceiveTime,
				 const in_addr&  oepNodeIp 
				)
  : _configKey( configKey )
  , _run( run )
{
  _oepReceiveTime =  oepReceiveTime;
  _oepNodeIp =  oepNodeIp;
}

EidEventContext::EidEventContext()
  : _configKey( 0 )
  , _run( 0 )
{
  _oepReceiveTime.tv_sec = 0;
  _oepReceiveTime.tv_nsec = 0;
  _oepNodeIp.s_addr =  0;
}

EidEventContext::EidEventContext( const EidEventContext& rhs )
  : _oepReceiveTime( rhs.oepReceiveTime() )
  , _oepNodeIp(      rhs.oepNodeIp() )
{

  if (this == &rhs ) return;

  _configKey      = rhs.configKey();
  _run            = rhs.run();

  return;
}

EidEventContext&
EidEventContext::operator = ( const EidEventContext& rhs ) {
  if (this != &rhs ) {
    _configKey      = rhs.configKey();
    _run            = rhs.run();
    _oepReceiveTime = rhs.oepReceiveTime();
    _oepNodeIp      = rhs.oepNodeIp();
  }

  return *this;
}


EidEventContext::~EidEventContext()
{}


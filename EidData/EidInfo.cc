//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: EidInfo.cc 451 2010-01-14 13:03:45Z stroili $
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

//-----------------------
// This Class's Header --
//-----------------------
#include "EidData/EidInfo.hh"

//-----------------
// C/C++ Headers --
//-----------------
#include <iostream>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "EidData/EidEventTriplet.hh"
#include "EidData/EidCondKeyTriplet.hh"
#include "ErrLogger/ErrLog.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
using std::endl;
using std::ostream;

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
EidInfo::EidInfo( const EidEventTriplet&   eidTriplet,
		  const EidCondKeyTriplet& eidCondTriplet,
		  d_ULong                  configKey,
		  d_ULong                  run
		)
   : _eidTriplet( eidTriplet )
   , _eidCondTriplet( eidCondTriplet )
   , _configKey( configKey )
   , _run( run )
{}


EidInfo::EidInfo( const EidInfo& rhs )
 :  _eidTriplet(      rhs.eventIdTriplet( ) )
 ,  _eidCondTriplet(  rhs.condKeyTriplet( ) )
 ,  _configKey(       rhs.configKey( ) )
 ,  _run(             rhs.run( ) )
{}


EidInfo&
EidInfo::operator=( const EidInfo& rhs )
{
   if ( &rhs != this ) {
     _eidTriplet     = rhs.eventIdTriplet( );
     _eidCondTriplet = rhs.condKeyTriplet( );
     _configKey      = rhs.configKey( );
     _run            = rhs.run( );
   }

   return *this;
}


//--------------
// Destructor --
//--------------

EidInfo::~EidInfo() 
{
}


//   *********************************************************************
//   ** majorID() and minorID() are obsolete.  They are maintained only **
//   ** to maintain backwards compatabillity w/ old data.  They         **
//   ** may be removed.  If present, they represent the two halves      **
//   ** of the BdbTime from the conditions database Key.                **
//   *********************************************************************


long
EidInfo::majorID( ) const 
{
  return (long) condKeyTriplet().key().getGmtSec( );
}


long
EidInfo::minorID( ) const 
{
  return (long) condKeyTriplet().key().getGmtNsec( );
}



void
EidInfo::setID( long major, long minor )
{
//   ** setID is obsolete.  See comments for majorID and minorID.
//****************************************************************************
  ErrMsg(error) << "EidInfo::setID(long,long) attempted.  Not supported: "
		<< "ignored."
		<< endmsg;
}


//-------------
// Accessors --
//-------------

const EidEventTriplet
EidInfo::eventIdTriplet( void ) const 
{
   return _eidTriplet;
}


const EidCondKeyTriplet
EidInfo::condKeyTriplet ( void ) const 
{
  return _eidCondTriplet;
}


d_ULong
EidInfo::configKey( void ) const 
{
  return _configKey;
}


d_ULong
EidInfo::run( void ) const 
{
  return _run;
}



// ******** Very peculiar thing: implementation of a function for the
// ******** AbsEventID class (base of EidInfo) is here, not in AbsEventID.cc.


std::ostream&
operator<<( std::ostream& o, const AbsEventID& id ) 
{
  // This is awful.  Putting this in AbsEventID.cc, where it is more
  // natural, causes a link loop between AbsEvent and EidData.  Clean
  // it later, but dump it here for now.  The weak argument is that, if
  // you refer to AbsEventID.hh, you _don't_ have to depend on EidData,
  // but if you _use_ eventIdTriplet() or condKeyTriplet() in AbsEventID,
  // then you do depend on EidData.  We thus say that calling operator <<
  // is tantamount to using these (see below). So, if you use it, you, the
  // client must #include something from it.  You thus will get this in your 
  // link.  Nah nah nah.      -Ed

  o << "AbsEventID:  EventIdTriplet " << id.eventIdTriplet() << '\n'
    << "AbsEventID:  CondKeyTriplet " << id.condKeyTriplet() << '\n'
    << "AbsEventID:  run= " << id.run() << " configKey = " << id.configKey() 
    << '\n';

  return o;
}

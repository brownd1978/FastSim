//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdKey.cc 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      17 Nov 96         Creation of first version
//
// Bugs:
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include "ProxyDict/IfdKey.hh"
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
using std::ostream;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION


IfdKey::IfdKey( keyKind kind )
  : _hashVal( 0 )
  , _myKeyKind( kind ) 
  , _myCardinality( 0 )
{}

IfdKey::~IfdKey()
{}


void
IfdKey::add( const IfdKey& ) {
  // only makes sense for the composite class.

  assert( 0 );
}

void
IfdKey::make_vtab( void ) const {
  // bogus virtual non-inline routine to get vtab emitted.
}

ostream& operator<<( ostream &o, const IfdKey & k ) {
    k.print(o);
    return o;
}

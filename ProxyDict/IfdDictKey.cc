//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdDictKey.cc 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//    Implementation of IfdDictKey.  See .hh file for details.
//
// Author List:
//    Ed Frank                        University of Pennsylvania
//
// History:
//    Ed Frank      07 Apr 99         Creation of first version
//
// Bugs:
//
//------------------------------------------------------------------------



#include "BaBar/BaBar.hh"
#include "ProxyDict/IfdDictKey.hh"
#if !(defined(__GNUC__) && (__GNUC__ < 3) && (__GNUC_MINOR__ < 95)) // BABAR_IOSTREAMS_MIGRATION
using std::cerr;
using std::endl;
#endif                                                              // BABAR_IOSTREAMS_MIGRATION

IfdDictKey::IfdDictKey( const IfdTypeKeyIFace& k) { 
   // This ctor (1) requires that the IfdDictKey start with a TypeKey
   // (2) actually CompositeKey::add()'s the TypeKey in.

   add( k );

   // A DictKey always starts with an IfdTypeKeyIFace.  Whe we do a
   // reset() our goal is to get to the state of having just a type
   // key as fast as possible.  So we cache the hashValue from the
   // TypeKey given to us in our ctor to save the time of fishing around
   // in the kids[] for the first key and then in there for the hashVal.

   _hashValCache = k.hash();
}


IfdDictKey::~IfdDictKey() {
   // More evil (see add()).   We set our cardinality to zero.
   // This makes the base class, IfdCompositeKey, think that it
   // has no kids and thus keeps it from deleting them.

    _myCardinality = 0;
    return;
}

void
IfdDictKey::deleteKeys( void ) {
    int i;
    for( i=0; i < _myCardinality; i++ ) {
	delete const_cast<IfdKey*>( kid[i] );
    }
    _myCardinality=0;
    _hashVal=0;
    return;
}

void
IfdDictKey::add( const IfdKey& k ) {
   // Much time searching ProxyDict's goes into doing new/delete on the
   // DictKeys made in the Front End.  To speed this up we modify the add()
   // method so that DictKeys do not clone keys add()'d to them and thus
   // do not own them.  This break's Liskoff's rule but by adding a reset()
   // method, the two together mean we do not need to make a new key every
   // time IF the LIFETIME management of DictKeys is done correctly.  What
   // that means is that the lookup in the dictionary must be atomic with
   // respect to the DictKey lifetime:  we must complete that much of the
   // lookup that requires the DictKey before there is ever an opportunity
   // for any other attempt to use the front-end and thus create another
   // DictKey.  I think this is ok.  If it proves to be a problem, we can
   // get trickier and provide a stack that allows reentrency to some depth
   // without having to make/delete new keys every time.

   // If we have room left to take another child...

   if ( _myCardinality > maxChildren ) {
     cerr << "IfdDictKey::add() no more room.  Go shoot Ed." << endl;
     ::exit( 1 );
   }

   // Keep a pointer to the key but do not clone it.  This is where we
   // differ from CompositeKeys.

   kid[ _myCardinality ] = &k;

   // Update the hash value.

   _hashVal = ( (_hashVal << 8 ) + k._hashVal ) % _nHashBuckets;

   _myCardinality++;
}

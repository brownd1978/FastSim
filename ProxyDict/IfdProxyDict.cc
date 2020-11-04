//--------------------------------------------------------------------------
// File and Version Information:
//    $Id: IfdProxyDict.cc 520 2010-01-15 11:40:48Z stroili $
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
#include "ProxyDict/IfdProxyDict.hh"
#include "ProxyDict/IfdDataProxyIFace.hh"
#include "ProxyDict/IfdDictKey.hh"



IfdProxyDict::~IfdProxyDict() {
  // Get the vtab out.
}

void*
IfdProxyDict::get( const IfdDictKey &k, AbsArg& a )  {
  // Derivatives will implement an actual dictionary
  // mechanism, and provide a find() for searching it.
  // Here we implement how that dictionary is used.
  // This method is not const because a get could cascade to
  // a put in its implementation.

  IfdDataProxyIFace *p = find( k );

  if ( p ) {
    return p->get( this, k, a);
  } else {
    return 0;
  }
}

bool
IfdProxyDict::put( const IfdDictKey& k, IfdDataProxyIFace *p ) {
  // Put a proxy into the dictionary according to the Key k and enforce
  // the rule that k is only in the dictionary once.  Note that we do
  // not attempt (yet) to require only one instance of a IfdDataProxyIFace.
  // (You could have one in more than once if passed in w/ differnt
  // keys).  We really ought to enforce that, because the destructor
  // is just going to come along and delete everthing.  If we check here,
  // its a 2N problem, but if we check in the dtor, it will be N**2.
  //

  IfdDataProxyIFace* pTest=find( k );

  if ( pTest ) {
    return false;		// Require uniqueness of proxy in dictionary
  } else {
    add( k, p );		// Put the proxy in the dictionary.
    return true;
  }
}

void
IfdProxyDict::storeItem( const IfdDictKey& k, AbsArg& a) {

  IfdDataProxyIFace* pTest=find( k );

  if ( pTest ) {
    pTest->store( this, k, a );
  }
}


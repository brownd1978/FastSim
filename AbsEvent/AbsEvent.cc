//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsEvent.cc 487 2010-01-13 16:40:34Z stroili $
//
// Description:
//	Class AbsEvent
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	<Author1>		<originator/contributor etc.>
//	<Author2>		<originator/contributor etc.>
//
// Copyright Information:
//	Copyright (C) 1996	<Institution>
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"
#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------
#include "AbsEvent/AbsEvent.hh"

//-------------
// C Headers --
//-------------

//---------------
// C++ Headers --
//---------------
#include "ProxyDict/IfdDictKey.hh"
using std::cerr;
using std::endl;

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------
AbsEvent::AbsEvent() {}

//--------------
// Destructor --
//--------------
AbsEvent::~AbsEvent() {}

//-------------
// Methods   --
//-------------
    
//-------------
// Operators --
//-------------
    
//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------

bool
AbsEvent::put( const IfdDictKey& k, IfdDataProxyIFace* p ) {
  //
  // We were finding that people were not checking the return status
  // of Ifd<T>::put().  This means that someone might think they have
  // installed a proxy of some kind, but in fact end up using a previously
  // installed proxy.  It is possible for this to cause data corruption,
  // depending upon what exactly the proxies are.  Thus, since this is
  // a configuration error, we decided to override the behavior of
  // put in IfdProxyDict and make a failure be a hard error.
  //
  // Users who wish to test for the existence of a proxy before doing
  // a put may use  bool keyUsed(IfdProxyDict *d ) or 
  // bool keyUsed(IfdProxyDict *d, const IfdKey& k )
  //

  IfdDataProxyIFace* pTest=find( k );
  if ( pTest ) {
    cerr << "AbsEvent::put():  attempt to add a proxy with a key already in"
         << " use." << endl;
    cerr << "AbsEvent::put():  key has value ";
    k.print( cerr );
    cerr << endl;
    cerr << "AbsEvent::put():  aborting." << endl;
    // This is always true, just remove warning
    return false;		// Require uniqueness of proxy in dictionary
  } else {
    add( k, p );		// Put the proxy in the dictionary.
    return true;
  }
}

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------




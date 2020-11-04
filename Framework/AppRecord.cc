//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppRecord.cc 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppRecord
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
//
// Author List:
//	Marc Turcotte		Original Author (Clone of AbsEvent) 3/26/98
//      Marc Turcotte           Updated to clone of AbsEvent V00-01-01 4/24/98
//
// Copyright Information:
//	Copyright (C) 1998	University of Texas at Dallas
//
//------------------------------------------------------------------------
#include "Experiment/Experiment.hh"

#include <stdlib.h>

//-----------------------
// This Class's Header --
//-----------------------
#include "Framework/AppRecord.hh"

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
AppRecord::AppRecord() {}

//--------------
// Destructor --
//--------------
AppRecord::~AppRecord() {}

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
AppRecord::put( const IfdDictKey& k, IfdDataProxyIFace* p ) {
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
  // a put may user Ifd<T>::proxyExits() and Ifd<T>::proxyExists(IfdKey&).
  //

  IfdDataProxyIFace* pTest=find( k );
  if ( pTest ) {
    cerr << "AppRecord::put():  attempt to add a proxy with a key already in"
         << " use." << endl;
    cerr << "AppRecord::put():  key has value ";
    k.print( cerr );
    cerr << endl;
    cerr << "AppRecord::put():  aborting." << endl;
    ::abort();
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



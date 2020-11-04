//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IfdProxyDictFrontEnd.cc 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//	Implementation of IfdProxyDictFrontend.  See .hh file for comments.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank		University of Pennsylvania
//
// Copyright Information:
//	Copyright (C) 1997
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "ProxyDict/IfdProxyDictFrontEnd.hh"


//---------------
// C++ Headers --
//---------------

#include "ProxyDict/IfdTypeKeyIFace.hh"
#include "ProxyDict/IfdDictKey.hh"

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
IfdProxyDictFrontEnd::IfdProxyDictFrontEnd()
{}

//--------------
// Destructor --
//--------------

IfdProxyDictFrontEnd::~IfdProxyDictFrontEnd() {
}



//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------


IfdDictKey*
IfdProxyDictFrontEnd::newDictKey( const IfdTypeKeyIFace& k ) {

  // NOTE: The caller owns the returned IfdDictKey and must delete it.

  IfdDictKey* dk = new IfdDictKey( k );
  return dk;
}


IfdDictKey*
IfdProxyDictFrontEnd::newDictKey( const IfdTypeKeyIFace& k, const IfdKey& k2 )
{

  // NOTE: The caller owns the returned IfdDictKey and must delete it.

  IfdDictKey* dk = new IfdDictKey( k );
  dk->add( k2 );
  return dk;
}


//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

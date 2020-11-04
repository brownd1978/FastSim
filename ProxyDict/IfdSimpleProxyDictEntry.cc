//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: IfdSimpleProxyDictEntry.cc 520 2010-01-15 11:40:48Z stroili $
//
// Description:
//    See .hh file
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//
// Author List:
//	Ed Frank		University of Pennsylvania
//
// Copyright Information:
//	Copyright (C) 1997
//
//
//------------------------------------------------------------------------
#include "BaBar/BaBar.hh"

//-----------------------
// This Class's Header --
//-----------------------
#include "ProxyDict/IfdSimpleProxyDictEntry.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "ProxyDict/IfdDictKey.hh"
#include "ProxyDict/IfdDataProxyIFace.hh"
#include "ProxyDict/AbsArg.hh"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------



//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

IfdSimpleProxyDictEntry::IfdSimpleProxyDictEntry(const IfdDictKey& k,
						 IfdDataProxyIFace* p )
    : key( k.cloneDictKey() ), proxy( p ), storeFlag( false), arg(0)
     ,next(0)
     ,nextDelete(0)
{}
  
  
//--------------
// Destructor --
//--------------
IfdSimpleProxyDictEntry::~IfdSimpleProxyDictEntry() {
  key->deleteKeys();
  delete key;
  delete proxy;
  delete arg; 
}

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

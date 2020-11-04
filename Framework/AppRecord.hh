//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AppRecord.hh 509 2010-01-14 15:18:55Z stroili $
//
// Description:
//	Class AppRecord.
//
// Environment:
//
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
//      This software was developed in cooperation with CLEO.
//
// Author List:
//	Ed Frank		University of Pennsylvania
//	Bob Jacobsen		LBL
//      Marc Turcotte           Adapted from AbsEvent developed by
//                              above people. 3/26/98
//      Marc Turcotte           Updated to clone of AbsEvent V00-01-01 4/24/98
//
// Copyright Information:
//	Copyright (C) 1997	University of Pennsylvania
//      Copyright (C) 1998      University of Texas at Dallas
//
//------------------------------------------------------------------------

#ifndef APPRECORD_HH
#define APPRECORD_HH
//---------------
// C Headers --
//---------------
#include <stdlib.h>

//----------------------
// Base Class Headers --
//----------------------
#include "ProxyDict/IfdSimpleProxyDict.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "CLHEP/Alist/AList.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class AppRecord : public IfdSimpleProxyDict
{

  //--------------------
  // Instance Members --
  //--------------------
  
public:
  
  // Constructors
  AppRecord();
  
  // Destructor
  virtual ~AppRecord( );

  // Operators
  
  // Selectors (const)
  
  // Modifiers

  //    Override the definition of put() from IfdProxyDict
  //    so that a failed put() causes an abort.
  virtual bool put( const IfdDictKey& k, IfdDataProxyIFace* p );

protected:
  
  // Helper functions
  
private:
  
  // Friends
  
  // Data members
  
  // Copy Constructor & Assignment operator:  not allowed.
  //    Deep copies of this structure is not well defined.
  //    Since AppRecord owns its data, and since we do not
  //    do any kind of reference counting, we must forbid
  //    copies.
  
  AppRecord( const AppRecord& ) { ::abort(); }
  AppRecord& operator= ( const AppRecord& ) {::abort(); return *this;}
  
};

#endif




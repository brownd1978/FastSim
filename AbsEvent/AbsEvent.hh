//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AbsEvent.hh 487 2010-01-13 16:40:34Z stroili $
//
// Description:
//	Class AbsEvent.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Ed Frank		University of Pennsylvania
//	Bob Jacobsen		LBL
//
// Copyright Information:
//	Copyright (C) 1997	University of Pennsylvania
//
//------------------------------------------------------------------------

#ifndef ABSEVENT_HH
#define ABSEVENT_HH

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

class AbsEvent : public IfdSimpleProxyDict {

//--------------------
// Instance Members --
//--------------------

public:

    // Constructors
    AbsEvent();

    // Destructor
    virtual ~AbsEvent( );

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
    //    Since AbsEvent owns its data, and since we do not
    //    do any kind of reference counting, we must forbid
    //    copies.

    AbsEvent( const AbsEvent& ) { ::abort(); }
    AbsEvent& operator= ( const AbsEvent& ) { return *this;}

};

#endif

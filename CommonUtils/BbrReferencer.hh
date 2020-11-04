//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BbrReferencer.hh 443 2010-01-14 12:24:42Z stroili $
//
// Description:
//	Class BbrReferencer
//	
//      Reference counting 
//  
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Gautier Hamel de Monchenault,
//         from Bob Jacobsen, original author of BtaCandidate & BtaCandBase,
//         and discussions with Steve Schaffner, Massimo Marino,
//                              Yury Kolomensky, Ed Franck
//
// Copyright Information:
//	Copyright (C) 1998		CEN Saclay
//
//------------------------------------------------------------------------

#ifndef BbrReferencer_hh
#define BbrReferencer_hh

#include "BaBar/BaBar.hh"

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating class Headers --
//-------------------------------
#include <assert.h>

//		---------------------
// 		-- Class Interface --
//		---------------------

template <class Ref>
// 1) class Ref has to inherit from BbrRefCounted. 
// 2) class Ref has to define a clone function.
class BbrReferencer 
{

  //--------------------
  // Instance Members --
  //--------------------

  // the Reference
  Ref* _ref;

public:

  //
  // Constructors
  //

  //   default (type conversion)
  BbrReferencer( Ref* ref=0 ) : _ref(ref) 
  {
    increment();
  }

  //   copy
  BbrReferencer( const BbrReferencer<Ref>& o )
  {
    _ref = o._ref;
    increment();
  }

  //
  // Destructor
  //

  virtual ~BbrReferencer() 
  { 
    dropRef(); 
  }

  //
  // Operators
  //

  //  access
  Ref* operator->()             { return  _ref; }
  Ref& operator*()              { return *_ref; }
  Ref* operator()()             { return  _ref; }

  // const-access
  const Ref* operator->() const { return  _ref; }
  const Ref& operator*()  const { return *_ref; }
  const Ref* operator()() const { return  _ref; }

  //  logical operators
  bool operator==( const BbrReferencer<Ref>& o ) const 
  { 
    return _ref==o._ref; 
  }

  bool operator!=( const BbrReferencer<Ref>& o ) const 
  { 
    return _ref!=o._ref; 
  }

  bool operator<( const BbrReferencer<Ref>& o ) const 
  { 
    return _ref < o._ref; 
  }

  //  assignment
  BbrReferencer<Ref>& operator=( const BbrReferencer<Ref>& o ) 
  {
    if( this != &o && _ref!=o._ref ) 
      {
	dropRef();
	_ref = o._ref;
	increment();
      }
    return *this;
  } 

  //
  //  Accessors
  // 

  // number of counted referencers to the referenced object (including this)
  int number() const
  {
    if( _ref ) return _ref->nLinks();
    return 0;
  }

  //
  // Exclusive access 
  //
  Ref* ownRef()
  {
    // make sure the reference exists
    assert( _ref!=0 );
    
    // make sure the referenced object has links (at least this one)
    assert( number() >= 1 );

    if ( number() != 1 ) 
      {
	//  no exclusive access to the referenced object
      
	// decrement the referenced object counter
	decrement();
	assert( number() > 0 );

	// clone the referenced object
	_ref = _ref->clone();

	// increment the new referenced object counter
	increment();
	assert( number()==1 );
      }

    // return the reference
    return _ref;
  }

  //
  // Modifiers
  //

  //  increment the counter of the referenced object
  int increment() 
  {
    if( _ref ) return _ref->addLink();
    return 0;
  }

  //  decrement the counter of the referenced object
  int decrement() 
  {
    if( _ref ) {
      return _ref->dropLink();
    }
    return 0;
  }

  // drop the reference to the referenced object
  void dropRef()
  {
    if( _ref && decrement()==0 ) delete _ref;
    _ref=(Ref*)0;  
  }

  // set a new reference (after having droped a possible existing reference)
  void setRef( Ref* ref ) 
  {
    if( _ref!=ref ) 
      { 
	dropRef(); 
	_ref=ref; 
	increment();
      }
  }

};

#endif





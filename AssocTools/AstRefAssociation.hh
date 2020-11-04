//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstRefAssociation.hh 436 2010-01-13 16:51:56Z stroili $
//
// Description:
//	Class AstRefAssociation
//      Very simple class to provide an association between two
//      pointer types which don't obviously belong together in a class,
//      but need to go together for some reason.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Phil Strother           Original author
//
// Copyright Information:
//	Copyright (C) 1997      Imperial College
//
//------------------------------------------------------------------------

#ifndef ASTREFASSOCIATION_HH
#define ASTREFASSOCIATION_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//----------------------
// Base Class Headers --
//----------------------
#include "AbsEvent/AbsEvtObj.hh"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

template <class T1, class T2> class AstRefAssociation;

//		---------------------
// 		-- Class Interface --
//              ---------------------

template <class T1, class T2>
class AstRefAssociation : public AbsEvtObj {

//--------------------
// Declarations     --
//--------------------

    // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AstRefAssociation(const T1 &, const T2 &);

  // Copy Constructor
  AstRefAssociation( const AstRefAssociation<T1, T2> &);

  // Destructor
  virtual ~AstRefAssociation( );

  // Operators

  AstRefAssociation<T1, T2> &operator= ( const AstRefAssociation<T1, T2> &);

  virtual bool operator==( const AstRefAssociation<T1, T2> & ) const;

  bool operator!=( const AstRefAssociation<T1, T2> & ) const;

  // Selectors (const)

  const T1 &firstArgument() const {return _itsT1;}
  const T2 &secondArgument() const {return _itsT2;}

  // Modifiers

protected:

    // Helper functions

private:

    // Friends

    // Data members

  const T1 &_itsT1;
  const T2 &_itsT2;

  AstRefAssociation();
  
//------------------
// Static Members --
//------------------

public:

    // Selectors (const)

    // Modifiers

private:

    // Data members

};

#ifdef    BABAR_COMP_INST
#include  "AssocTools/AstRefAssociation.icc"
#endif // BABAR_COMP_INST

#endif








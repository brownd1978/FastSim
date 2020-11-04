//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstSortableAssoc.hh 436 2010-01-13 16:51:56Z stroili $
//
// Description:
//	Class AstSortableAssoc
//      Association between two pointers which provides enough info
//      to be sorted.
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Phil Strother           Original author
//
// Copyright Information:
//	Copyright (C) 1999      LBNL
//
//------------------------------------------------------------------------

#ifndef ASTSORTABLEASSOC_HH
#define ASTSORTABLEASSOC_HH

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

template <class T, class Q> class AstSortableAssoc;

//		---------------------
// 		-- Class Interface --
//              ---------------------

template <class T, class Q>
class AstSortableAssoc : public AbsEvtObj {

//--------------------
// Declarations     --
//--------------------

    // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  AstSortableAssoc(T *, Q *, bool (*lessThan)(const Q &, const Q&));

  // Copy Constructor
  AstSortableAssoc( const AstSortableAssoc<T, Q> &);

  // Destructor
  virtual ~AstSortableAssoc( );

  // Operators

  AstSortableAssoc<T, Q> &operator= ( const AstSortableAssoc<T, Q> &);

  virtual bool operator==( const AstSortableAssoc<T, Q> & ) const;
  virtual bool operator<(const AstSortableAssoc<T, Q> &) const;

  bool operator!=( const AstSortableAssoc<T, Q> & ) const;

  // Selectors (const)

  const T &key() const {return *_myT;}
  const Q &quality() const {return *_myQ;}

  // Modifiers

  void deleteMembers();
  // Normally one woulkd expect this just to be a container class,
  // but occaisionally it may be that we want to take responsibility
  // for the contained pointers.  This function sets the pointers
  // to zero after deleting them, so should be safe to call twice

protected:

    // Helper functions

private:

  // Friends
  
  // Data members

  bool (*_lessThanFunction)(const Q & q1, const Q& q2);

  T *_myT;
  Q *_myQ;

  AstSortableAssoc();
  
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
#include  "AssocTools/AstSortableAssoc.icc"
#endif // BABAR_COMP_INST

#endif








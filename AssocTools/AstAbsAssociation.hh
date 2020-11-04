//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstAbsAssociation.hh 436 2010-01-13 16:51:56Z stroili $
//
// Description:
//	Class AstAbsAssociation
//      Abstract interface for simple one to one associations
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Phil Strother           LBNL
//
//------------------------------------------------------------------------

#ifndef ASTABSASSOCIATION_HH
#define ASTABSASSOCIATION_HH

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

template <class T1, class T2> class AstAbsAssociation;

//		---------------------
// 		-- Class Interface --
//              ---------------------

template <class T1, class T2>
class AstAbsAssociation: public AbsEvtObj{

//--------------------
// Declarations     --
//--------------------

    // Typedefs, consts, and enums

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  // Should be protected?
  AstAbsAssociation();

  // Copy Constructor

  // Destructor
  virtual ~AstAbsAssociation( );

  // Operators
  
  bool operator==( const AstAbsAssociation<T1, T2> & ) const;

  bool operator!=( const AstAbsAssociation<T1, T2> & ) const;

  // Selectors (const)

  virtual const T1 *firstArgument() const=0;
  virtual const T2 *secondArgument() const=0;

private:

  AstAbsAssociation( const AstAbsAssociation<T1, T2> &);

};

#ifdef    BABAR_COMP_INST
#include  "AssocTools/AstAbsAssociation.icc"
#endif // BABAR_COMP_INST

#endif








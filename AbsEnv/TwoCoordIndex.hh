//======================================================================
// File and Version Information:
//      $Id: TwoCoordIndex.hh 483 2010-01-13 14:03:08Z stroili $
//
// Class TwoCoordIndex
//
//    Implementation of AbsDetIndex for system in which two co-ordinates 
//    specifies a channel uniquely.
//
//  Direct dependencies:
//      Inherits from AbsDetIndex.
//
//  Implementation dependencies:
//      iostream
//
//  Author List:
//
//         Phil Strother                         Imperial College
//         Stephen J. Gowdy                      University of Edinburgh
// 
//  Bugs, questions, etc. to:    pd.strother@ic.ac.uk, S.Gowdy@ed.ac.uk
// 
// 
//======================================================================

#ifndef TWOCOORDINDEX_HH
#define TWOCOORDINDEX_HH

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <stdlib.h>

//----------------------
// Base Class Headers --
//----------------------
#include "AbsEnv/AbsDetIndex.hh"
#include "AbsEnv/AbsDetCoord.hh"

//-------------------------------
// Collaborating class headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//              ---------------------
//              -- Class Interface --
//              ---------------------
 
class TwoCoordIndex : public AbsDetIndex
{

  enum {numberOfCoords=2};

public:

  // Constructors
  
  // Private null - TCI is just an information holder - no 
  // sense in having no information in it. 
  TwoCoordIndex( long theXcoord, long theYcoord, long theIndex );
  TwoCoordIndex( long theIndex, AbsDetCoord* theCoords );
  TwoCoordIndex( const TwoCoordIndex& otherIndex );

  // Destructors

  // destructor in base class
  virtual ~TwoCoordIndex();

  // Assignment operator 

  // Operators
  bool operator==( const TwoCoordIndex& c ) const { return index==c.index; }
  bool operator==( const AbsDetIndex& c ) const { return this->AbsDetIndex::operator==( c ); }
  bool operator!=( const TwoCoordIndex& c ) const { return index!=c.index; }
  bool operator!=( const AbsDetIndex& c ) const { return this->AbsDetIndex::operator!=( c ); }

  // Selectors (const)
  virtual unsigned hash() const;

  static unsigned hashFun(const TwoCoordIndex &);
  // Provided such that TCI's may be used in hashed collections

  inline long itsCardinality() const {return numberOfCoords;}
  inline long itsXCoord() const      {return (*itsCoords)[0];}

  inline long itsYCoord() const      {return (*itsCoords)[1];}


  // Inherited from AbsEnvObj.....
  // 'print(cout)', used by and equivalent to <<, occupies exactly one line
  virtual void print(std::ostream& o) const;

  // 'printAll(cout)', is allowed to take multiple lines and should
  // dump entire contents
  virtual void printAll(std::ostream& o) const;

  // Modifiers

private:
  TwoCoordIndex();

};

#endif // TWOCOORDINDEX_HH




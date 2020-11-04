//======================================================================
//
// Class ThreeCoordIndex
//
//    Implementation of AbsDetIndex for system in which three co-ordinates 
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

#ifndef THREECOORDINDEX_HH
#define THREECOORDINDEX_HH

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
#include "AbsEnv/AbsEnvTypes.hh"
#include "AbsEnv/AbsDetIndex.hh"

//-------------------------------
// Collaborating class headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//              ---------------------
//              -- Class Interface --
//              ---------------------

class ThreeCoordIndex : public AbsDetIndex
{

  //--------------------
  // Declarations     --
  //--------------------

  // Typedefs, consts, and enums


  enum {numberOfCoords=3};

  //--------------------
  // Instance Members --
  //--------------------

public:

  // Constructors
  
  // Private null - TCI is just an information holder - no 
  // sense in having no information in it. 

  ThreeCoordIndex( long theXcoord,
		   long theYcoord,
		   long theZcoord,
		   long theIndex );
  ThreeCoordIndex(long, AbsDetCoord *);
  
  ThreeCoordIndex(const ThreeCoordIndex &);

  // Destructors

  // Destructor in base class
  virtual ~ThreeCoordIndex();

  // Assignment operator 
  bool operator==( const ThreeCoordIndex& toCompare ) const { return index==toCompare.index; }
  bool operator==( const AbsDetIndex& toCompare ) const { return this->AbsDetIndex::operator==( toCompare ); }
  bool operator!=( const ThreeCoordIndex& toCompare ) const { return index!=toCompare.index; }
  bool operator!=( const AbsDetIndex& toCompare ) const { return this->AbsDetIndex::operator!=( toCompare );  }

  // Accessors

  // Selectors (const)
  virtual unsigned hash() const;

  static unsigned hash(const ThreeCoordIndex &);
  // Provided such that TCI's may be used in hashed collections

  inline long itsCardinality() const {return numberOfCoords;}
  inline long itsXCoord() const      {return (*itsCoords)[0];}
  inline long itsYCoord() const      {return (*itsCoords)[1];}
  inline long itsZCoord() const      {return (*itsCoords)[2];}

  // Inherited from AbsEnvObj.....
  // 'print(cout)', used by and equivalent to <<, occupies exactly one line
  virtual void print(std::ostream& o) const;

  // 'printAll(cout)', is allowed to take multiple lines and should
  // dump entire contents
  virtual void printAll(std::ostream& o) const;

private:

  ThreeCoordIndex();
};

#endif // THREECOORDINDEX_HH

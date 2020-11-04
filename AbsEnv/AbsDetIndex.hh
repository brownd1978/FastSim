//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsDetIndex.hh 659 2010-09-03 07:39:54Z stroili $
//
// Description:
//      Class AbsDetIndex.  Abstract class to hold co-ordinate and
//      index information. A given concrete class derived from
//      AbsDetIndex may need to redefined one or all of the member
//      functions
//
//           virtual int compareTo(const AbsDetIndex *) const;
//           virtual bool isEqual(const AbsDetIndex *) const;
//           virtual unsigned hash() const {return 0;}
//
//      For details, see below.  N.B. If the derived class is to be
//      stored in a hash table, or equivalent then the hash() function
//      MUST be redefined.  The actual co-ordinates are held as long
//      ints.
//
//  Direct dependencies:
//      Inherits from AbsEnvObj.
//
//  Author List:
//
//         Phil Strother                         Imperial College
//         Stephen J. Gowdy                      University of Edinburgh
// 
//  Bugs, questions, etc. to:    pd.strother@ic.ac.uk, S.Gowdy@ed.ac.uk
// 
//
//------------------------------------------------------------------------
#ifndef ABSDETINDEX_HH
#define ABSDETINDEX_HH

//-------------
// C Headers --
//-------------

//---------------
// C++ Headers --
//---------------

#include "AbsEnv/AbsDetCoord.hh"
#include "AbsEnv/NeighbourStore.hh"

//----------------------
// Base Class Headers --
//----------------------
#include "AbsEnv/AbsEnvObj.hh"


//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------

class AbsDetIndex : public AbsEnvObj
{

  //--------------------
  // Declarations     --
  //--------------------
  
  // Typedefs, consts, and enums

  // A set does not allow for duplicates

  //--------------------
  // Instance Members --
  //--------------------


public:

  // Constructor
  AbsDetIndex();

  AbsDetIndex(long, AbsDetCoord *);

  AbsDetIndex(const AbsDetIndex &);

  // Destructors

  virtual  ~AbsDetIndex();

  // Operators

  AbsDetIndex &operator=(const AbsDetIndex &);
  virtual bool operator==( const AbsDetIndex& ) const;
  virtual bool operator!=( const AbsDetIndex& ) const;
  virtual bool operator<( const AbsDetIndex& ) const;

  // Selectors

  inline virtual long itsIndex() const; 

  virtual const NeighbourStore *itsNeighbours() const;
  bool isNeighbour(const AbsDetIndex*) const;
  const AbsDetIndex *getPreviousIndex() const {return previousIndex;}
  const AbsDetIndex *getNextIndex() const {return nextIndex;}


  // Depending on the type of collection you choose, you may want to
  // redefine some of the functions below, particularly the hash
  // function.

  virtual int compareTo(const AbsDetIndex *) const;
  
  // Yes, int.

  // Returns 0 if the passed object is equal in some way to *this,
  // -1 for "less than" and +1 for "greater than".  For now, compare
  // the member variable "index."
  
  virtual bool isEqual(const AbsDetIndex *) const;

  // Returns true if the passed object is equal (presumably in the same
  // same sense as above) and false if not.
  
  virtual unsigned hash() const; 

  // Put the AbsEnvObj inherited print methods in here, to remind
  // derived classes that they must be redefined. 

  // 'print(cout)', used by and equivalent to <<, occupies exactly one line
  virtual void print(std::ostream& o) const;

  // 'printAll(cout)', is allowed to take multiple lines and should
  // dump entire contents
  virtual void printAll(std::ostream& o) const;
  
  // Modifiers

  virtual void setPreviousIndex(AbsDetIndex *);
  virtual void setNextIndex(AbsDetIndex *); 
  
  virtual void addToNeighbourList(const AbsDetIndex *);
  
protected:

  // Helper functions

  long index;
  AbsDetCoord *itsCoords;

  AbsDetIndex *previousIndex;
  AbsDetIndex *nextIndex;

  NeighbourStore *theNeighbours;

private:

  // Friends

  //  friend class AbsDetStructure;
  //  friend class AbsDetIndexFactory;
  
  // Data members

  
  // No information in the index makes no sense
  
  //------------------
  // Static Members --
  //------------------
  
public:
  
  // Selectors (const)
  
  // Modifiers
  
private:
  
  // Data members
  
};



inline
long
AbsDetIndex::itsIndex() const
{
  return index;
}


#endif








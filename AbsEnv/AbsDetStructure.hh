//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: AbsDetStructure.hh 483 2010-01-13 14:03:08Z stroili $
//
// Description:
//      Class AbsDetStructure header file
//
//      Abstract class to define the structure of a non-determined 
//      detector system in terms of AbsDetIndex classes which hold 
//      co-ordinate and index information. Each detector system will 
//      define its own structure (there may be more than one if 
//      there is more than one representation of the detector). 
//      Collections of AbsDetIndeces are implemented as std::vector.
//
//      A particular system's Structure, implemented as derived classes
//      from this one, will be posessed (via an inherited data member)
//      by that system's derived version of AbsDetEnv.
//      See EmcEnv/EmcEnv.cc/hh for an example.
//    
// Direct dependencies:
//      Inherits from AbsEnvObj.
//
// Implementation dependencies:
//      General:
//            AbsDetIndex
//
//  Author List:
//
//         Phil Strother                         Imperial College
//         Stephen J. Gowdy                      University of Edinburgh
// 
//  Bugs, questions, etc. to:    pd.strother@ic.ac.uk, S.Gowdy@ed.ac.uk
//
//------------------------------------------------------------------------
#ifndef ABSDETSTRUCTURE_HH
#define ABSDETSTRUCTURE_HH

//----------------------
// Base Class Headers --
//----------------------

#include "AbsEnv/AbsEnvTypes.hh"
#include "AbsEnv/AbsDetIndex.hh"

//-------------------------------
// Collaborating class headers --
//-------------------------------

//-------------------------------------
// Collaborating class declarations  --
//-------------------------------------

class AbsDetIndexFactory;

//---------------------
//-- Class Interface --
//---------------------

class AbsDetStructure:public AbsDetIndex{

  //--------------------
  // Declarations     --
  //--------------------


    // Typedefs, consts, and enums

public:

  //--------------------
  // Instance Members --
  //--------------------
  
  // 
  // Constructors
  //
  
  AbsDetStructure();
  AbsDetStructure(long, AbsDetCoord *);
  
  //
  // Destructors
  //
  
  virtual ~AbsDetStructure();

  // Operators

  bool operator==( const AbsDetStructure& ) const;
  bool operator!=( const AbsDetStructure& ) const;
  bool operator==( const AbsDetIndex& c ) const 
                      { return this->AbsDetIndex::operator==( c ); }
  bool operator!=( const AbsDetIndex& c ) const 
                      { return this->AbsDetIndex::operator!=( c ); }

  // Selectors (const)

  virtual long itsCardinality() const=0;
  virtual const AbsDetIndex *find(const AbsDetCoord &) const=0;
  // Find a given ADI from a list of coordinates

  virtual unsigned hash() const; 
  // needed for hash collections

  // Put the AbsEnvObj inherited print methods in here, to remind
  // derived classes that they must be redefined. 

  virtual void print(std::ostream& o) const;

  virtual void printAll(std::ostream& o) const;

  // Modifiers

  inline void setFactory(AbsDetIndexFactory *factory){theFactory=factory;}
  // Give tha factory to make the sub-components

  virtual void build(const AbsDetCoord &, AbsIndexStore *collectionToAddTo=0)=0;
  // Build the structure.  

  virtual void makeDirectNeighbours();
  // Inherited from AbsDetStructure. Makes "touching" neighbours.

  virtual void makeDirectNeighbours(const AbsDetStructure *const);
  // Make the direct (touching )neighbours for the channels in this structure.
  // This may or may not need another AbsDetStructure to accomplish the task. 
  // to this end, these are defined as (effectively) empty, not pure virtual 

  // 
  // Accessors
  //  


protected:

  long cardinality;

  AbsIndexStore *itsChannels;

  // Choose your favourite collection.  

  AbsDetIndexFactory *theFactory;

private:
  // Copy or assignment not allowed
  AbsDetStructure(const AbsDetStructure &);
  AbsDetStructure& operator=(const AbsDetStructure &);

  // Friends
  friend class AbsDetStructureIterator;

};

#endif


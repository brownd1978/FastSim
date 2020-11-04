#ifndef COMTEMPLATEDBITMAP_HH
#define COMTEMPLATEDBITMAP_HH

//--------------------------------------------------------------------------
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1998      <Institution>
//
//------------------------------------------------------------------------

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <vector>
#include <map>

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "BaBar/BaBarODMGTypes.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------


//		---------------------
// 		-- Class Interface --
//		---------------------


/**
 *  ComTemplatedBitMap.
 *  
 *  Class to allow the setting of bits in a 32 bit word 
 *  according to string types. 
 * 
 *  Copyright (C) 2000 LBNL
 *
 *  @see ComTemplatedBitMap
 *
 *  @version $Id: ComTemplatedBitMap.hh 443 2010-01-14 12:24:42Z stroili $ 
 *
 *  @author (Phil Strother)		(originator);
 */

template <class T> 
class ComTemplatedBitMap {


public:


  // Constructors
  // The vector of Ts supplied dictates the relation to the 
  // bits in the word.  Specifically, the nth accessor in the 
  // vector describes the (n+1)th bit.
  // i.e. given a vector of type Foo, if vector[2] is a particular
  // object fooObject, then setBit(fooObject) will set the 3rd bit.
  ComTemplatedBitMap(const std::vector<T*> &);

  // As above but with hash function defined for a T*
  ComTemplatedBitMap(const std::vector<T*> &,
		     unsigned (*hashFunction)(const T* const &));

  // Copy Constructor
  ComTemplatedBitMap( const ComTemplatedBitMap<T>& );                
  
  
  // Destructor
  virtual ~ComTemplatedBitMap( );
  
  // Operators
  ComTemplatedBitMap<T>& operator= ( const ComTemplatedBitMap<T>& );    // Assignment op
  
  virtual bool operator==( const ComTemplatedBitMap<T>& ) const;
  bool operator!=( const ComTemplatedBitMap<T>& ) const;
  
  // Selectors (const)
  
  // Returns the value of the bit relating to the object of type T
  bool getBit(const T &) const;
  
  // Returns the underlying representation of the bits
  d_ULong wordRepLong() const;
  
  // Two ints, whose rightmost 16 bits are the left and rightmost 16 bits
  // of the 32 bit underlying rep respectively.
  int leftHandRep() const;
  int rightHandRep() const;
  
  
  // Modifiers
  // Returns true if bit has not already been set. 
  // Returns false if the bit has been set, but leaves it set.
  bool setBit(const T &);

  // resets all bits to zero
  void resetAllBits();

protected:
  
  // Helper functions
  

private:

  static unsigned hashFunction(const T* const&);


  std::map<const T*, int> *_dictionaryIndex;
  std::vector<T*> *_rawIndex;
  unsigned (*_hashFunction)(const T* const&);
  d_ULong _wordRep;
};


#ifdef    BABAR_COMP_INST
#include  "CommonUtils/ComTemplatedBitMap.icc"
#endif  // BABAR_COMP_INST

#endif // COMTEMPLATEDBITMAP_HH

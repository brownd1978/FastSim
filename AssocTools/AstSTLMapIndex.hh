//-------------------------------------------------------------------
// File and Version Information:
// 	$Id: AstSTLMapIndex.hh 436 2010-01-13 16:51:56Z stroili $
//
// Description:
//      Given a HepAList or std::vector, provides a fast
//      map between the objects in the vector and the 
//      index within the vector.
//   
// Author List:
//      Matthias Steinke
//
//-------------------------------------------------------------------
#ifndef ASTSTLMAPINDEX_HH
#define ASTSTLMAPINDEX_HH

//-------------
// C Headers --
//-------------

//---------------
// C++ Headers --
//---------------
#include <vector>
#include <map>
#include "BbrStdUtils/BbrCollectionUtils.hh"

//----------------------
// Base Class Headers --
//----------------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

template <class T> class AstIndexedObject;
template <class T> class HepAList;

//              ---------------------
//              -- Class Interface --
//              ---------------------


template<class T> 
class AstSTLMapIndex {
  //--------------------
  // Declarations     --
  //--------------------

  // Typedefs, consts, and enums

  //--------------------
  // Instance Members --
  //--------------------

public:
  // Constructors
  AstSTLMapIndex(const HepAList<T> &, 
	      unsigned (*hFun)(const T&)=AstSTLMapIndex<T>::hashFunction );
  AstSTLMapIndex(const std::vector<T*> &,
	      unsigned (*)(const T&)=AstSTLMapIndex<T>::hashFunction);

  // Detsructor
  virtual ~AstSTLMapIndex();

  // Selectors (const)

  // Contains the given object
  bool contains(const T*) const;

  // Fills given index with position in original
  // vector.  Returns true if T* can be found
  bool index(const T*, int &index) const;

  int members() const;

  
  // Modifiers
  // Append a T* to the end of the list
  virtual bool append(T*); 

  // Remove an entry from the map
  // *Expensive* and will result in indices being recalculated!!!
  virtual T* remove(const T*);

  // Clear map of entries.  Does not delete contents.
  virtual void clear();

  // Clear map of entries and delete the objects
  virtual void clearAndDestroy();

protected:

private:

  AstSTLMapIndex();

  std::map<T*, AstIndexedObject<T>*, babar::Collection::PtrLess> *_map;

  unsigned int (*_hashFun)(const T&);

  // default hashFunctions (based on pointer value)
  static unsigned hashFunction(const T& p);

};

#ifdef    BABAR_COMP_INST
#include  "AssocTools/AstSTLMapIndex.icc"
#endif // BABAR_COMP_INST


#endif // ASTMAPINDEX_HH






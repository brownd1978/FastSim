//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: APPListIterator.hh 508 2010-01-14 15:18:18Z stroili $
//
// Description:
//	Class APPListIterator. This is an addapter class for the iterator of
//      the Sandard Library List class for use with Framework.
//
// Author List:
//	Liz Sexton-Kennedy		Original Author
//      A. De Silva                     Modified for RogueWave
//
//------------------------------------------------------------------------

#ifndef APPLISTITERATOR_HH
#define APPLISTITERATOR_HH

//-----------
// Headers --
//-----------
#include "FrameUtil/APPList.hh"
#include <list>

//		---------------------
// 		-- Class Interface --
//		---------------------

template<class Item> 
class APPListIterator {
//--------------------
// Instance Members --
//--------------------

public:

  // Constructors taking a corresponding list as argument. Starting at the 
  // first element.
  APPListIterator( const APPList<Item> & aList );
  
  // Copy constructor.
  APPListIterator( const APPListIterator<Item> &);
  
  // Destructor
  ~APPListIterator( );
  
  // Selectors (const)
  // Returns a pointer to the current object in the associated list,
  // without incrementing the index. 
  Item * current() const;
  
  // Returns a pointer to the current object in the associated list, moving
  // forward to the next. Returns 0 if all objects are done.
  Item * next();
  
  // Left over from Alist next = function of operator
  Item * operator () () {return next();}
  
  // Moves backward one step in the list and returns the object found
  // there.  If current object is the first in the list, no stepping is
  // done and 0 is returned.
  Item * prev();
  
  // Rewinds the iterator to the first element of the list.
  void rewind();
  
  // Skips iterator to just past the last element of the list.
  void skipAll();
  
private:
  
  // keep a pointer to the list so we can do rewind
  std::list<Item>* _theList;
  typename std::list<Item>::iterator _listIter;
  bool _isValid;
};

#include  "FrameUtil/APPListIterator.icc" 

#endif


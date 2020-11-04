//--------------------------------------------------------------------------
// File and Version Information:
//
// Description:
//	Class APPList. This is an addapter class for List class for use
//      with Framework.  It uses the Rogue Wave class for its implementation.
//
//  Warning: Use this class only as a container of pointers.
//           (That is, define APPList<T*>).
//
// Author List:
//	Liz Sexton-Kennedy		Original Author
//      A. De Silva                     Adapted from Liz's STL classes to RW.
//
//------------------------------------------------------------------------

#ifndef APPLIST_HH
#define APPLIST_HH

//-----------
// Headers --
//-----------

#include <list>

//		---------------------
// 		-- Class Interface --
//		---------------------

template<class Item> 
class APPList {

//--------------------
// Instance Members --
//--------------------

public:

  // Constructors
  APPList( );
  APPList( const APPList<Item> &aList );

  // Asignment
  APPList<Item>& operator =( const APPList<Item>& );

  // Destructor
  ~APPList( );
  
  // Selectors (const)
  // Returns a pointer to the first and last object in the list.
  Item * first() const;
  Item * last() const;
  bool   isEmpty() const {return _list->empty();}
  
  // Modifiers
  // Inserts the object theItem at the position pos in the list. If pos is
  // outside the list, the object will be appended.
  void insert( const Item *theItem, unsigned int pos) {
    insert(*theItem, pos);
  }
  void insert( const Item &theItem, unsigned pos);

  // Inserts an object first in the list.
  void insert( const Item * theItem) {_list->push_front(*theItem);}
  void insert( const Item & theItem) {_list->push_front(theItem);}
  
  // Inserts the object theItem just before the first occurence of
  // object beforeItem in the list.     
  void insert( const Item *theItem, const Item *beforeItem);
  void insert( const Item &theItem, const Item &beforeItem);
  
  
  // Appends an object in the end of the list
  void append( const Item *theItem) {_list->push_back(*theItem);}
  void append( const Item &theItem) {_list->push_back(theItem);}
  
  // Appends the object theItem just after the last occurrence of object
  // afterItem in the list
  void append( const Item *theItem, const Item *afterItem);
  void append( const Item &theItem, const Item &afterItem);
  
  // Appends all objects of list addedList to the end of this list.
  void append(const APPList<Item> &addedList);
  
  // Remove all occurencies of the object from the list.
  void remove( const Item *theItem) {_list->remove(*theItem );}
  void remove( const Item &theItem) {_list->remove(theItem);}
  
  // Remove all occurencies of the objects in list removedList from this 
  // list.
  void remove(const APPList<Item> &removedList);
  
  // Remove an object from the list.
  void remove(unsigned pos);
  
  // Accessor for the one data item, 
  // (this should really only be used by the iterator class)
  std::list<Item>* getList( ) {return _list;} 

//************************************************************************
// The following methods are not used by Framework.
// They were in HepAList and were used by other packages.
// They are here now to maintain backward compatibility during the Framework
// migration.

  // Returns the i:th object in the list
  // Note: the objects are numbered 0 to n-1
  // This is extremely slow and you should avoid using this.
  Item* operator [] ( unsigned i) const; // cdf did not have this

  // Returns the number of objects in the list 
  // Implemented in CLHEP's AListBase class
  int length() const {return _list->size();}

  // Remove all objects from the list
  void removeAll() { _list->clear();}

//************************************************************************
  
private:
  std::list<Item>* _list;
};

template <class Item>
void APPListDeleteAll( APPList<Item> & alist);
// This is nothing like CLHEP's DeleteAll, and should only be used by
// Framework.  The framework lists are all lists of pointers to things
// that have been allocated with new.  In other words Item is always a
// pointer.  The following is therefore OK.  Under any other circumstances
// this code would not be OK.

#include  "FrameUtil/APPList.icc" 

#endif



